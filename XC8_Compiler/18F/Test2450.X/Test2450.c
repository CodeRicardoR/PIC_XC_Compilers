/*
 * File:   Test2450.c
 * Author: ricar
 *
 * Created on 12 de junio de 2023, 12:10 PM
 */

#define _XTAL_FREQ 48000000UL

#include <xc.h>
#include "configbits2450.h"

void main(void) {
    
    ADCON1bits.PCFG = 0xF;      //All pins are digital mode
    //TRIS Registers
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0X00;
    //LAT Registers
    LATA = 0x01;
    LATB = 0x00;
    LATC = 0x00;
    
    
    //Config Interrupts
    RCONbits.IPEN = 0;      //Interrupt in mode 16F compatibility
    INTCON = 0x00;          //Disable others interrupta and clear Flags
    
    while(1){
        LATAbits.LA0 = PORTAbits.RA0 ^ 1;
        LATAbits.LA1 = PORTAbits.RA1 ^ 1;
        __delay_ms(250);
    }
    
    return;
}
