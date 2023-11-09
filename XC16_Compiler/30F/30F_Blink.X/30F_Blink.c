/*
 * File:   30F_Blink.c
 * Author: ricar
 *
 * Created on 1 de abril de 2022, 05:40 PM
 */

#include "main.h"

int main(void){
    
    ADPCFG = 0x1FFF;    //All pins are digital
    TRISBbits.TRISB9 = 0;
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB11 = 0;
    
    //Config to board:
    TRISFbits.TRISF4 = 1;
    TRISFbits.TRISF5 = 1;
    
    while(1){
        LATBbits.LATB9 ^= 1;
        LATBbits.LATB10 ^= 1;
        LATBbits.LATB11 ^= 1;
        __delay_ms(100);
    }
    return 0;
}
