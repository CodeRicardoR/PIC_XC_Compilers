/*
 * File:   serial2450.c
 * Author: ricar
 *
 * Created on 31 de marzo de 2023, 04:18 PM
 */

#include "serial2450.h"

void Config_SerialPort(void){
    
    TRISCbits.TRISC6 = 0;       //Pin TX, output
    TRISCbits.TRISC7 = 1;       //Pin RX, input
    
    TXSTA = 0b00100100;         //Asyn mode, 8 bits, High speed, TX enabled
    RCSTA = 0b00010000;         //Continuos receive, 8 bits, Port OFF
    BAUDCON = 0x00;             //SPBRG register 8 bits.
    SPBRG = 25;                 //Baud rate = 115200
    // Setting interrupt RX USART
    PIE1bits.RCIE = 1;
    return;
}
void Open_SerialPort(void){
    
    RCSTAbits.SPEN = 1;         //Serial Port ON
    return;
}
void Close_SerialPort(void){
    
    RCSTAbits.SPEN = 0;         //Serial Port OFF
    return;
}
char ReadByte_SerialPort(void){
    char DataReg;
    
    while(!PIR1bits.RCIF);
    DataReg = RCREG;
    return DataReg;
}
void WriteByte_SerialPort(char data){
    
    TXREG = data;
    while(!TXSTAbits.TRMT);
    return;
}