// Joshua Rowe and David Armstrong

#include "lcd.h"
#include <xc.h>
#include <stdlib.h>

// Set the bits on the LCD port
void Lcd_Port(char a)
{
	if(a & 1)
		D4 = 1;
	else
		D4 = 0;

	if(a & 2)
		D5 = 1;
	else
		D5 = 0;

	if(a & 4)
		D6 = 1;
	else
		D6 = 0;

	if(a & 8)
		D7 = 1;
	else
		D7 = 0;
}

// Send a command to the LCD
void Lcd_Cmd(char a)
{
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
        __delay_ms(4);
        EN  = 0;             // => E = 0
}

// Clear the LCD
void Lcd_Clear()
{
	Lcd_Cmd(0x00);
	Lcd_Cmd(0x01);
    __delay_ms(2);
}

// Set the cursor to a particular place and line
void Lcd_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
	  temp = (char) (0x80 + b - 1);
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
}

// Bring up the LCD
void Lcd_Init()
{
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x03);
	__delay_ms(5);
  Lcd_Cmd(0x03);
	__delay_ms(11);
  Lcd_Cmd(0x03);
  /////////////////////////////////////////////////////
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x08);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x0C);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x06);
}

// Write a single character
void Lcd_Write_Char(char a)
{
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1;             // => RS = 1
   Lcd_Port(y>>4);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   Lcd_Port(temp);
   EN = 1;
   __delay_us(40);
   EN = 0;
}

// Write an entire string
void Lcd_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
    {
        Lcd_Write_Char(a[i]);
    }
}

// Write an int
void Lcd_Write_Int(unsigned int a)
{
    char temp[16];
    utoa((char*)temp, a, 10);
    Lcd_Write_String(temp);
}

void Lcd_Shift_Right()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

//*********************************************************** created functions
  
  //char a, sets which integer value you want to display.
  //char b, sets the cursor position for block 1 or 2 of 8 bits
  //the second cursor coordinate will alway be the same positioning for each loop

void Lcd_Display_if( char a, char b)                                              //this line must be included in header library
 {                
        if (a<10)                                                                 //if the integer value is less than ten insert a '0' in front of integer in order to make two decimal places.
        { Lcd_Set_Cursor(b,3);                                                    //order to make two decimal places
            Lcd_Write_Int(0);
            Lcd_Set_Cursor(b,4);
            Lcd_Write_Int(a);
        }
        
        else                                                                      //if greater than ten insert without any '0' additions as it will contain two digits already
        {
            Lcd_Set_Cursor(b,3);      
            Lcd_Write_Int(a);
        }
 }

  //displays Voltage integer and decimal point
  //char a, sets which integer value you want to display.
  //char b, sets the cursor position for block 1 or 2 of 8 bits
  //the second cursor coordinate will alway be the same positioning

void Lcd_Display_1( char a, char b)                                              //this line must be included in header library
 {
        Lcd_Set_Cursor(b,1);
        Lcd_Write_Int(a);
        Lcd_Set_Cursor(b,2);
        Lcd_Write_String(".");
 }