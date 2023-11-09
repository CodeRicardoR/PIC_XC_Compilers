/*
 * File:   30F_ADC.c
 * Author: ricar
 *
 * Created on 30 de abril de 2022, 10:54 AM
 */

#include "xc.h"
#include "30F_ADC.h"
#include <libpic30.h>

void ConfigADC(void){
    
    //Setting RB0 and RB1 Analog pins:
    ADPCFGbits.PCFG0 = 0;       //RB0 <- AN
    ADPCFGbits.PCFG1 = 0;       //RB1 <- AN
    ADPCFGbits.PCFG2 = 0;       //RB2 <- AN
    ADPCFGbits.PCFG3 = 0;       //RB3 <- AN
    ADPCFGbits.PCFG4 = 0;       //RB4 <- AN
    ADPCFGbits.PCFG5 = 0;       //RB5 <- AN
    //VREF+, VREF-
    ADCON2bits.VCFG = 3;        //AVDD and AVSS externals pins
    //Select channel AN
    ADCHSbits.CH0NA = 0;        //VREF- => V- in
    ADCHSbits.CH0SA = 0;        //Default Channel is AN0
    //Select ADC Clock conversion
    ADCON3bits.ADCS = 59;       //(59 + 1)*(Tcy/2) = 30*Tcy = 1017ns > 667ms
    ADCON3bits.ADRC = 0;        // Conversion clock is system clock
    //ADC conversion trigger
    ADCON1bits.SSRC = 0;        //Software trigger selected
    ADCON1bits.FORM = 0;        //Fomart coding out is Interger
    ADCON1bits.ASAM = 0;        //Samplig with SAMP bit
    ADCON1bits.ADON = 1;        //ADC module is ON
    return;
}

unsigned short ReadADC(char Channel){
    
    ADCHSbits.CH0SA = Channel;
    ADCON1bits.SAMP = 1;
    __delay_ms(2);
    ADCON1bits.SAMP = 0;
    while(!ADCON1bits.DONE);     //Waiting for conversion finished
    return ADCBUF0;
}


