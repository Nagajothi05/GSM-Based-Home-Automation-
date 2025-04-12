#include<lpc21xx.h>
#include<string.h>
#define LCD 0XF<<20
#define RS 1<<17
#define RW 1<<18
#define EN 1<<19
#define LED 1<<17
#define BUZZ 1<<21
#define MR 1<<4        // ROTATE IN CLOCKWISE
#define ML 1<<5       // ROTATE IN ANTI-CLOCKWISE

void UART0_STR(unsigned char *);
void UART0_TX(unsigned char );
char UART0_RX(void);
void UART0_INIT(void);
void LED_ON(void);
void LED_OFF(void);
void BUZZER_ON(void);
void BUZZER_OFF(void);
void DC_ON(void);
void DC_OFF(void);


void delay(unsigned int sec)
{
	T0PR=15000-1;
	T0TCR=0X01;
	while(T0TC<sec);
	T0TCR=0X03;
	T0TCR=0X00;
}
void LCD_CMD(unsigned char CMD)
{
IOCLR1=LCD;
IOSET1=(CMD&0xf0)<<16;
IOCLR1=RS;
IOSET1=EN;
delay(2);
IOCLR1=EN;
	
IOCLR1=LCD;
IOSET1=(CMD&0x0f)<<20;
IOCLR1=RS;
IOSET1=EN;
delay(2);
IOCLR1=EN;
}

void LCD_INIT(void)
{
IODIR1=LCD|RS|EN|RW;
IOCLR1=RW;
LCD_CMD(0X01);
LCD_CMD(0X02);
LCD_CMD(0X28);
//LCD_CMD(0X84);
LCD_CMD(0X0C);
}

void LCD_DATA(unsigned char DATA)
{
IOCLR1=LCD;
IOSET1=(DATA&0xf0)<<16;
IOSET1=RS;
IOSET1=EN;
delay(2);
IOCLR1=EN;
	
IOCLR1=LCD;
IOSET1=(DATA&0x0f)<<20;
IOSET1=RS;
IOSET1=EN;
delay(2);
IOCLR1=EN;
}
void LCD_STR(unsigned char*s)
{
	while(*s)
	{
		LCD_DATA(*s++);
	}
}
 
unsigned int i;
 char ch;
        char* p;
int main()
{
       char m[100];
   char a[50]="LEDON";
	 char b[50]="LEDOFF";
   char c[50]="BUZZERON";
	 char d[50]="BUZZEROFF";
	 char e[50]="MOTORON";
	 char f[50]="MOTOROFF";
	 PINSEL0|=0x00000005;
	 IODIR0|=LED|BUZZ|MR|ML;
   	IOSET0|=LED|MR|ML;
	
	LCD_INIT();
	UART0_INIT();
	LCD_CMD(0x80);
	LCD_STR("   GSM BASED   ");
	LCD_CMD(0xc0);
	LCD_STR("HOME AUTOMATION");
	delay(2000);
  
	UART0_STR("AT\r\n");
	delay(2000);
	UART0_STR("AT+CMGF=1\r\n");
	delay(2000);
    ab:
	UART0_STR("AT+CMGD=1,4\r\n");
	delay(2000);
	UART0_STR("AT+CMGS= \"+919442087881\" \r\n");
	delay(2000);		
	UART0_STR("WAITING FOR INPUT\r\n");
	UART0_TX(0X1A);

	delay(15000);
	UART0_STR("AT+CMGR=1\r\n");
	for(i=0;i<90;i++)
	{ 
	 m[i]=UART0_RX();		
	}
   m[i]='\0';
         	p = strstr(m,a);
			if(p!='\0')
			LED_ON();
			if(1)
			{
				p = NULL;
				p = strstr(m,b);
				if(p!='\0')	
				LED_OFF();
			}			   
			if(1)
			{
				p = NULL;
				p = strstr(m,c);
				if(p!='\0')
				BUZZER_ON();
			}
			if(1)
			{
				p = NULL;
				p = strstr(m,d);
				if(p!='\0')
		    	BUZZER_OFF();
			}
			if(1)
			{
				p = NULL;
				p = strstr(m,e);
				if(p!='\0') 
				DC_ON();
			}
			if(1)
			{
				p = NULL;
				p = strstr(m,f);
				if(p!='\0')
			    DC_OFF();
			}
	goto ab;
	
}

void UART0_STR(unsigned char *s)
{
	while(*s)
	{
		UART0_TX(*s++);
	}
}
void UART0_TX(unsigned char d)
{
	while(!(U0LSR&(1<<5)));
	U0THR = d;
}
char UART0_RX(void)
{
	while((U0LSR&(1<<0))==0);
	return U0RBR;
	
}
void UART0_INIT(void)
{
	PINSEL0 = 0X00000005;
	U0LCR=0x83;
	U0DLL=97;
	U0LCR=0x03;
}
void LED_ON(void)
{
        LCD_CMD(0x01);
	   	LCD_CMD(0x80);
	    LCD_STR("LED0N");
	IOCLR0=LED;
}
void LED_OFF(void)
{
            LCD_CMD(0x01);
	       	LCD_CMD(0x80);
	       	LCD_STR("LEDOFF");
	IOSET0=LED;
}
void BUZZER_ON(void)
{
          	LCD_CMD(0x01);
	        LCD_CMD(0x80);
	        LCD_STR("BUZZER0N");
	IOSET0=BUZZ;
}
void BUZZER_OFF(void)
{
             LCD_CMD(0x01);
		     LCD_CMD(0x80);
	         LCD_STR("BUZZEROFF");
	IOCLR0=BUZZ;
}
void DC_ON(void)
{
            LCD_CMD(0x01);
	     	LCD_CMD(0x80);
	       	LCD_STR("MOTORON");
	IOSET0=MR;
	IOCLR0=ML;
}
void DC_OFF(void)
{
             LCD_CMD(0x01);
		        LCD_CMD(0x80);
		LCD_STR("MOTOROFF");
	IOCLR0=MR;
	IOCLR0=ML;
}