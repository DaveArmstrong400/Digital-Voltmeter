// Joshua Rowe and David Armstrong

//**************************************************************** configuration
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config CP = OFF
#include <xc.h>
// Include the LCD library
#include "lcd.h"
// Include ADC library
#include "adc.h"

//**************************************************************** storage for integers
  //Initialisation of global variables

unsigned int Universal=0;                                                         //The universal integer is used for various different variables therefore reducing memory
int StoredRemainder=0;                                                            //This variable is the remainder level that has been stored in memory 
int StoredVoltage=0;                                                              //This variable is the voltage level that has been stored in memory

//**************************************************************** Start and initialisation of pins

//Starts the main loop 
void main(void) 
{
    
//sets both port A and port B pins to either inputs or outputs;
    TRISB= 0b10000000;
    TRISA= 0b00011010;

// Set the R/W LCD pin to Write
    RW = 0;

// Set CS high and CLK low for ADC
    CS = 1;
    CLK = 0;

 //**************************************************************** initialising LCD and welcome message   
   //a welcome message will be displayed that flashes on and off for 
   // three repetitions. welcome message highlights voltmeters range
    
// Initialises and clears the LCD
    Lcd_Init();
    
    Lcd_Clear();

//flashing welcome message
    while (Universal<3)
    {
            //Producing the message on the LCD screen for 500 milliseconds
            Lcd_Set_Cursor(1,1);              
            Lcd_Write_String("Voltage");
            
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String(" 0-5V");
            
            __delay_ms(500);
            
            //clears the LCD and Increments the variable controlling the while loop
            Lcd_Clear();
            
            Universal=Universal+1;                                                //This variable increments the loop
            __delay_ms(500);
    }
    
 //**************************************************************** reading ADC and converting value to decimal on LCD
   // converts ADC value in to a voltage value to three significant figures
   //the voltage value is split into an integer value and a decimal integer value
   //this is then displayed on the LCD with the voltage unit symbol using functions
   //created in lcd.c library
    
    int VoltageHigh=0;                                                            //Highest voltage reading stored in memory 
    int RemainderHigh=0;                                                          //Highest remainder reading stored in memory 
    int Timer=0;                                                                  //Sets threshold timer to zero 

    while(1)
    {
        Universal=0;
        Universal= readADC();                                                     //The universal variable is now being used for reading the Bit Count after being reset
        unsigned int Voltage=Universal/204;                                       //This produces the current voltage reading from the Bit Count
        unsigned int Remainder=((Universal%204)*100)/204;                         //This produces the current remainder reading from the Bit Count 
    
        Lcd_Clear();
        
        Lcd_Display_1(Voltage, 1);
        Lcd_Display_if(Remainder, 1);
        
        Lcd_Set_Cursor(1,6);
        Lcd_Write_String("V");
        
        __delay_ms(250);

//**************************************************************** pause and restart button      
  //when button is pushed the universal variable will be set to zero 
  //a delay loop will repeat until the same button is pressed for a second time
  //the universal variable will be set to one and will cause the programme to exit delay loop
        
        if (RA3==1 && RA4==0)
        {
            __delay_ms(100);
            
            Universal=0;                                                          
            
            while (Universal==0)                                                  //Continuous loop which can only be exited by the change of the universal variable 
            {
                __delay_ms(100);
            
                if (RA3=1)
                {  
                     Universal=1;                                                 //The change in the universal variable to exit the loop 
                }
            }
        }
 
//**************************************************************** highest peak voltage           
  //if the current voltage is higher than the previous highest voltage the if 
  //statement is entered. This then resets the high voltage variables, the else if
  //statement makes sure that the higher voltage is still stored when only the 
  //remainder is higher
        
        if (Voltage>VoltageHigh)
        { 
            VoltageHigh=Voltage;                                                  //VoltageHigh overwritten by current Voltage
            RemainderHigh=Remainder;                                              //RemainderHigh overwritten by current Remainder
        }

        else if (Voltage==VoltageHigh && Remainder>RemainderHigh)   
        {
            VoltageHigh=Voltage;
            RemainderHigh=Remainder;                                                       
        }
    
//**************************************************************** displaying peak voltage and current voltage
  //When push button is pressed the LCD displays the current voltage alongside 
  //the highest voltage thus far. The main screen returns after one second
        
        if (RA4==1 && RA3==0)
        {
            Lcd_Clear();
            Lcd_Display_1(Voltage, 1);                                            //using created function display Voltage on cursor bit block 1
            
            Lcd_Display_if(Remainder, 1);                                         //using created function display Remainder on cursor bit block 1
            
            Lcd_Set_Cursor(1,6);
            Lcd_Write_String("V");

            Lcd_Display_1(VoltageHigh, 2);
            
            Lcd_Display_if(RemainderHigh, 2);
            
            Lcd_Set_Cursor(2,6);
            Lcd_Write_String("V");
            
            __delay_ms(1000);
        }
       

//**************************************************************** display Saved Value  
  //When the push button is pressed a buzzer will sound notifying it has been 
  //pressed and the LCD will display 'Saved:' alongside the saved voltage 
  //value. after a delay the screen will resume to the current voltage level
        
        if (RB7==1)
        {
            Lcd_Clear();
            
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Saved:");

            Lcd_Display_1(StoredVoltage, 2);
            
            Lcd_Display_if(StoredRemainder,2);

             __delay_ms(500);
        }
        
//**************************************************************** threshold buzzer        
  // when the voltage level goes below or above one or four volts respectively,
  // a buzzer will sound to alert the user of this. Port B pin 7 is configured 
  // to an output for the buzzer        
        
        if (Voltage<1 || Voltage>3)
        {
            TRISB= 0x0;                                                           //Sets all pins within port B to outputs
            
            PORTBbits.RB7 = 1;                                                    //sets pin seven within port B high which sounds the buzzer
            
            __delay_ms(100);
            
            PORTBbits.RB7 = 0;

//**************************************************************** threshold timer  
  // as above when the voltage is in the threshold range a built in timer 
  // will begin to count in seconds. once out of the threshold range the timer
  // will reset. port B pin 7 is reconfigured to inputs for switch, 
  //after being switched to outputs for buzzer
            
            Lcd_Clear();
            
            Lcd_Display_1(Voltage, 1);
            
            Lcd_Display_if(Remainder, 1);
            
             Lcd_Set_Cursor(1,6);
            Lcd_Write_String("V");

            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("Time=");

            Lcd_Set_Cursor(2,6);
            Lcd_Write_Int(Timer);

             Lcd_Set_Cursor(2,8);
            Lcd_Write_String("s");

            __delay_ms(800);
            
            Timer= Timer + 1;                                                     //Increments the timer when in the threshold voltage
        }
        
        else
        {
            Timer=0;                                                              //Resets the timer when not in the threshold range
        }
        
        TRISB= 0b10000000;                                                        //Resets port B pin 7 to an input
        
//**************************************************************** save current voltage  
  // when push button is pressed the current voltage is saved to a variable.
  // the LCD is will display 'Saving..' to notify the user it is being saved.
  // this message will shift across the screen whilst the voltage is being saved      
        
        if ( RA4==1 && RA3==1)
        { 
            Lcd_Clear();
            
            Universal=0;                                            
            
            while(Universal<8)                                                    //sets eight as the limit for the universal variable
            {
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("Saving..");
                
                __delay_ms(300);

                 Lcd_Set_Cursor(2,1);
                Lcd_Write_String("Saving..");
                
                __delay_ms(300);

                Lcd_Shift_Right();
                
                Universal=Universal+1;                                            //This variable increments the loop
                
                __delay_ms(100);
            }
            
            StoredRemainder=Remainder;                                            //Saves the current voltage to the stored voltage variable
            StoredVoltage=Voltage;                                                //Saves the current remainder to the stored remainder variable
        }
}
return;
} 