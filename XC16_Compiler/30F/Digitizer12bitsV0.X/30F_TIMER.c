/*
 * File:   30F_TIMER.c
 * Author: ricar
 *
 * Created on 30 de abril de 2022, 09:33 AM
 */


#include "xc.h"
#include "30F_TIMER.h"

void Config_TIMER1(void){
    
    //T1CON Register
    T1CONbits.TON = 0;      //TIMER1 disabled
    T1CONbits.TGATE = 0;
    T1CONbits.TCKPS = 0b10; //Prescaler x64 => 100ms
    T1CONbits.TCS = 0;      //Fosc/4
    
    //PR1 Register: Periode
    //PR1 = 0xB400;           // T = 100.0 ms 
    PR1 = 0xB3F3;               // T = 100.0ms 
    return;
}
void Enable_TIMER1(void){
    
    IFS0bits.T1IF = 0;      //Clear Flag bit
    IEC0bits.T1IE = 1;      //TIMER1 interrupt enabled
    TMR1 = 0;
    T1CONbits.TON = 1;      //TIMER1 ON
    return;
}
void Disable_TIMER1(void){
    
    T1CONbits.TON = 0;      //TIMER1 OFF
    IFS0bits.T1IF = 0;      //Clear Flag bit
    IEC0bits.T1IE = 0;      //TIMER1 interrupt disabled
    return;
}


