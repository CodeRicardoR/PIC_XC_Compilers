

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#include <xc.h>
#include "LIB16F_GPS38400.h"
#include "LIB16F_USART.h"
#include "LIB_ADS7825.h"

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 9830400UL

//-------------------------------------------------
//To select channel ADC
//   RB7  RB6  RB5      RB4       RB3    RB2    RB1    RB0
//   NC   NC   CS->    READY<-    A0->   A1->   A2->   INT0<-
//   '0'  '0'  '0'       '0'       X      X      X      '0'
#define ChannelX    0x2E
#define ChannelY    0x2A
#define ChannelZ    0x2C
#define ChannelTC   0x28
#define ChannelTS   0x20
#define ChannelAUX0 0x24
#define ChannelAUX1 0x22
#define ChannelAUX2 0x26
//-------------------------------------------------

//-------------------------------------------------
#define delay_channel   5       //Delay to adquisition
#define n_sample        5       //Samples to average
//-------------------------------------------------

//-------------------------------------------------
//Definiciones de pines
#define PIN_RC      PORTAbits.RA1        //output
#define PIN_CLK     PORTAbits.RA2        //output
#define PIN_CS      PORTBbits.RB5        //output
#define PIN_LED     PORTAbits.RA0        //output
#define PIN_READY   PORTBbits.RB4       //input
#define PIN_SDI     PORTCbits.RC5       //input
//-------------------------------------------------

//-------------------------------------------------
//Global variables:
unsigned char CmdGPS = 0;
unsigned char ControlSample = 0;
unsigned char ContaSample = 0;

signed long SumData_X, SumData_Y, SumData_Z, SumData_TC, SumData_TS;
signed short Data_X, Data_Y, Data_Z, Data_TC, Data_TS, Data_G;

char BufferDAT[13];
//-------------------------------------------------


