
#include "adclib2450.h"

void ConfigADC(void){
    
    ADCON1 = 0b00001011;
    ADCON2 = 0b10110110; //Right justified, 16TAD, Tacq=1.3us
    ADCON0bits.ADON = 1; //A/D converter enabled
}

unsigned int ReadChannelADC(char channel){
    unsigned int valueADC;
    
    ADCON0bits.CHS = channel & 0x03; //CH0,CH1,CH2 o CH3
    ADCON0bits.GO = 1;  // Start ADC
    while(ADCON0bits.GO); //wait A/D done
    valueADC = 256*ADRESH + ADRESL;
    return valueADC;
}