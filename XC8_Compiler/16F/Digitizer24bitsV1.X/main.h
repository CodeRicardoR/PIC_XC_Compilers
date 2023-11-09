
#define _XTAL_FREQ 9830400UL

#include <xc.h>
//#include "LIB16F_GPS38400.h"
#include "LIB16F_SPI.h"
#include "LIB16F_USART.h"

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

//-------------------------------------------------
//To select channel ADC
//------------------------------------------------
//|(NON)|(NON)|(A0) |(A1) |(SWB)|(SWA)|(NON)|(GPS)|
//| RB7 | RB6 | RB5 | RB4 | RB3 | RB2 | RB1 | RB0 |
//-------------------------------------------------
#define ChannelX    0x38
#define ChannelY    0x18
#define ChannelZ    0x28
#define ChannelTC   0x08
#define ChannelTS   0x04
#define ChannelAUX0 0x34
#define ChannelAUX1 0x14
#define ChannelAUX2 0x24
#define ChannelGND  0x0C
//-------------------------------------------------
#define delay_channel   7       //Delay to adquisition
#define n_sample        5       //Samples to average
//-------------------------------------------------
//Pines:
//-------------------------------------------------
#define PIN_LED     PORTAbits.RA2        //output
//-------------------------------------------------
//Pines Test:
#define PIN_TEST1   PORTBbits.RB7
#define PIN_TEST2   PORTBbits.RB6
//-------------------------------------------------
//Global variables:
unsigned char CmdGPS = 0;
unsigned char ControlSample = 0;
unsigned char ContaSample = 0;

signed long SumData_X, SumData_Y, SumData_Z, SumData_TC, SumData_TS, SumData_GND;
signed long Data_X, Data_Y, Data_Z, Data_TC, Data_TS, Data_GND;
//-------------------------------------------------
signed long SumData_AUX0, SumData_AUX1, SumData_AUX2;
signed long Data_AUX0, Data_AUX1, Data_AUX2;
//-------------------------------------------------

char BufferDAT[13];
