
#define _XTAL_FREQ 9830400UL

#include <xc.h>
#include "LIB258_SPI.h"
#include "LIB258_USART.h"

// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator with PLL enabled/Clock frequency = (4 x FOSC))
#pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = ON         // Brown-out Reset Enable bit (Brown-out Reset enabled)
#pragma config BORV = 25        // Brown-out Reset Voltage bits (VBOR set to 2.5V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

// CONFIG4L
#pragma config STVR = OFF       // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will not cause Reset)
#pragma config LVP = OFF        // Low-Voltage ICSP Enable bit (Low-Voltage ICSP disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

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

//Numbers
//-------------------------------------------------
#define delay_channel   7       //Delay to adquisition
#define n_sample        5       //Samples to average
//-------------------------------------------------

//Pines:
//-------------------------------------------------
#define PIN_LED         LATAbits.LA2        //output
#define PIN_TEST        LATBbits.LB7        //output
//-------------------------------------------------

//-------------------------------------------------
//Global variables:
unsigned char CmdGPS = 0;
unsigned char ControlSample = 0;
unsigned char ContaSample = 0;

signed long SumData_X, SumData_Y, SumData_Z, SumData_TC, SumData_TS, SumData_AUX0, SumData_AUX1, SumData_AUX2, SumData_GND;
signed long Data_X, Data_Y, Data_Z, Data_TC, Data_TS, Data_G, Data_AUX0, Data_AUX1, Data_AUX2, Data_GND;
//-------------------------------------------------

char BufferDAT[13];