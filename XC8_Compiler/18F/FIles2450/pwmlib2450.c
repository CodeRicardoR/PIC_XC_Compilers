
#include "pwmlib2450.h"

void ConfigPWM(void){
    
    TRISCbits.RC2 = 0;  //PIN output
    LATCbits.LATC2 = 0;
    PR2 = 99;           //PWM = 7.5KHz
    T2CONbits.T2CKPS = 3; // Prescaler x16
    return;
}
void ConfigDutyCycle(unsigned char value){
    
    if(value == 0){
        T2CONbits.TMR2ON = 0;
        CCP1CONbits.CCP1M = 0; //Module Disabled
    }else{
        CCPR1L = 99 - value;
        CCP1CONbits.CCP1M = 0b1111; //Module PWM enabled
        TMR2 = 0;
        T2CONbits.TMR2ON = 1;
    }
    return;
        
}
