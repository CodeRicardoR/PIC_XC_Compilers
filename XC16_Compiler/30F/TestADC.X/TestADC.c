/*
 * File:   TestADC.c
 * Author: ricar
 *
 * Created on 10 de abril de 2022, 05:35 PM
 */

#include "main.h"

void ConfigADC(void);
unsigned short ReadADC(char Channel);

unsigned short DataADC;
char Letras[20];

int main(void) {
    
    //Config UART1
    UART1_Open();
    UART1_Config();
    //Config ADC
    ConfigADC();
    while(1){
        DataADC = ReadADC(1);
        sprintf(Letras,"\n\rValor es: %u",DataADC);
        print_string_U1(Letras);
        __delay_ms(1000);
    }
    
    UART1_Close();
  
    return 0;
}

void ConfigADC(void){
    
    //Setting RB0 and RB1 Analog pins:
    ADPCFGbits.PCFG0 = 0;       //RB0
    ADPCFGbits.PCFG1 = 0;       //RB1
    //VREF+, VREF-
    ADCON2bits.VCFG = 0;        //AVDD and AVSS pins
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
    __delay_ms(10);
    ADCON1bits.SAMP = 0;
    while(!ADCON1bits.DONE);     //Waiting for conversion finished
    return ADCBUF0;
}
