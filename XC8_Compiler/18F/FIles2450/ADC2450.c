/*
 * File:   ADC2450.c
 * Author: ricar
 *
 * Created on 31 de marzo de 2023, 04:20 PM
 */

#define _XTAL_FREQ 48000000UL

#include <xc.h>
#include "configbits2450.h"
#include "serial2450.h"
#include "adclib2450.h"
#include "pwmlib2450.h"

unsigned char duty;

void __interrupt() Interrups(void){
    char ByteData;
    unsigned int numero;
    
    if((PIE1bits.RC1IE == 1) && (PIR1bits.RC1IF == 1)){
        ByteData = ReadByte_SerialPort();
        switch(ByteData){
            case '1':
                numero = ReadChannelADC(2);
                break;
            case '2':
                numero = ReadChannelADC(3);
                break;
            default:
                numero = 0;
        }
        WriteByte_SerialPort(numero);
        WriteByte_SerialPort(numero>>8);
    }
    
    return;
}

void main(void) {
    
    ADCON1bits.PCFG = 0xF;      //All pins are digital mode
    TRISA = 0x0F;
    TRISB = 0x00;
    TRISC = 0X00;
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    
    //Init Seria Port
    Config_SerialPort();
    Open_SerialPort();
    //Init ADC
    ConfigADC();
    //Config PWM
    ConfigPWM();
    //Config Interrupts
    RCONbits.IPEN = 0;      //Interrupt in mode 16F compatibility
    INTCON = 0x00;          //Disable others interrupta and clear Flags
    //Interrupts
    INTCONbits.PEIE = 1;    //Enable interrupt peripherical
    INTCONbits.GIE = 1;     //Enable Global Interrupt
    //PWM
    duty = 1;
    while(1){
        ConfigDutyCycle(duty);
        duty++;
        __delay_ms(1000);
        if(duty==100)
            duty=1;
    }
    Close_SerialPort();
    return;
}
