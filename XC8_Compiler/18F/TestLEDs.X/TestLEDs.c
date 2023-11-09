/*
 * File:   TestLEDs.c
 * Author: ricar
 *
 * Created on 23 de septiembre de 2022, 09:15 PM
 */

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


#define _XTAL_FREQ 39321600UL

//Definiciones de pines
#define LED_RED LATAbits.LATA1
#define LED_GREEN LATAbits.LATA2

#include <xc.h>

void __interrupt() Interrups(void){
    
    if((INTCONbits.TMR0IE == 1) && (INTCONbits.TMR0IF == 1)){
        INTCONbits.TMR0IF = 0;
        TMR0H = 0x10;
        TMR0L = 0x3D;
        LED_GREEN = PORTAbits.RA2 ^ 1;
    }
    
    return;
}
void main(void) {
    
    //-----------------------------------------------
    //Config IO PORTS
    ADCON1bits.PCFG = 0x07;     //Config PORTA DIGITAL
    LATA = 0;
    TRISA = 0;

    //-----------------------------------------------
    //Config TMR0 to 100ms:
    INTCONbits.TMR0IF = 0;
    TMR0H = 0x10;
    TMR0L = 0x3D;
    T0CON = 0b10000011;     //TMR0: Start,16bits,16 prescaler, INTCLK 
    //-----------------------------------------------
    //Config Interrupts
    RCONbits.IPEN = 0;      //Interrupt in mode 16F compatibility
    INTCON = 0x00;          //Disable others interrupta and clear Flags
    //Interrupts
    INTCONbits.TMR0IE = 1;  //Enable TMR0 INT
    //INTCONbits.PEIE = 1;    //Enable interrupt peripherical
    INTCONbits.GIE = 1;     //Enable Global Interrupt
    
    while(1);    
    return;
}

