/*
 * File:   TestTIMER1.c
 * Author: ricar
 *
 * Created on 23 de abril de 2022, 08:56 PM
 */


#include "main.h"

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void);

void Config_TIMER1(void);
void Enable_TIMER1(void);
void Disable_TIMER1(void);

int main(void) {
    
    //Set PORTB pines
    ADPCFG = 0x1FFF;    //All pins are digital
    TRISBbits.TRISB9 = 0;
    LATBbits.LATB9 = 0;
    
    Config_TIMER1();
    Enable_TIMER1();
    while(1);
    Disable_TIMER1();
    
    return 0;
}

void Config_TIMER1(void){
    
    //T1CON Register
    T1CONbits.TON = 0;      //TIMER1 disabled
    T1CONbits.TGATE = 0;
    T1CONbits.TCKPS = 0b10; //Prescaler x64 => 100ms
    T1CONbits.TCS = 0;      //Fosc/4
    
    //PR1 Register: Periode
    PR1 = 0xB400;           // T = 100.0 ms
    
    return;
}
void Enable_TIMER1(void){
    
    IPC0bits.T1IP = 7;      //Max. Priority
    IFS0bits.T1IF = 0;      //Clear Flag bit
    IEC0bits.T1IE = 1;      //TIMER1 interrupt enabled
    TMR1 = 0;
    T1CONbits.TON = 1;      //TIMER1 Enabled
    return;
}
void Disable_TIMER1(void){
    
    return;
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void){
    
    IFS0bits.T1IF = 0;      //Clear Flag bit
    LATBbits.LATB9 ^= 1;    //Tootgle bit
    return;
}
