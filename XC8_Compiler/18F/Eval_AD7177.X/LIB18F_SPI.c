/*
 
 
 
 */

#include <xc.h>
#include "LIB18F_USART.h"
#include "LIB18F_SPI.h"

void ConfigPort_SPI(void){
    
    TRISCbits.TRISC3 = 0;       //SCK output
    TRISCbits.TRISC4 = 1;       //SDI input
    TRISCbits.TRISC5 = 0;       //SDO output
    
    SSPSTAT = 0b00000000;       //SMP:0, CKE:0
    SSPCON1bits.CKP = 1;        //HIGH is idle for CLK
    SSPCON1bits.SSPM = 0x01;    //Freq = FOSC/16 = 2.5Mbits
    return;
}

void OpenPort_SPI(void){
    
    SSPCON1bits.SSPEN = 1;      //SPI Enabled
    return;
}

void ClosePort_SPI(void){

    SSPCON1bits.SSPEN = 0;      //SPI Disabled
    return;
}

unsigned char TransferByte_SPI(unsigned char data){
    unsigned char valor;
    
    valor = SSPBUF;
    SSPBUF = data;
    while(!SSPSTATbits.BF);
    return SSPBUF;
}

//-----------------------------------------------
void ResetAD7177(void){
    unsigned char conta;
    
    PIN_CS = 0;
    for(conta=0;conta<9;conta++){
        TransferByte_SPI(0xFF);
    }
    PIN_CS = 1;
    __delay_ms(3);
    return;
}

//-----------------------------------------------
void ReadRegister_1bytesAD7177(unsigned char command){
    
    PIN_CS = 0;
    TransferByte_SPI(command);
    BufferDAT[0] = TransferByte_SPI(0x00);
    PIN_CS = 1;
    return;
}

//-----------------------------------------------
void ReadRegister_2bytesAD7177(unsigned char command){
    
    PIN_CS = 0;
    TransferByte_SPI(command);
    BufferDAT[0] = TransferByte_SPI(0x00);
    BufferDAT[1] = TransferByte_SPI(0x00);
    PIN_CS = 1;
    return;
}
//-----------------------------------------------
void ReadRegister_3bytesAD7177(unsigned char command){
    
    PIN_CS = 0;
    TransferByte_SPI(command);
    BufferDAT[0] = TransferByte_SPI(0x00);
    BufferDAT[1] = TransferByte_SPI(0x00);
    BufferDAT[2] = TransferByte_SPI(0x00);
    PIN_CS = 1;
    return;
}
//-----------------------------------------------
void WriteRegister_2bytesAD7177 (unsigned char command, unsigned char BYTE_HIGH, unsigned char BYTE_LOW){
    
    PIN_CS = 0;
    TransferByte_SPI(command);
    TransferByte_SPI(BYTE_HIGH);
    TransferByte_SPI(BYTE_LOW);
    PIN_CS = 1;
    return;
}
//-----------------------------------------------

void WriteRegister_3bytesAD7177 (unsigned char command, unsigned char BYTE_HIGH, unsigned char BYTE_MEDIUM, unsigned char BYTE_LOW){
    
    PIN_CS = 0;
    TransferByte_SPI(command);
    TransferByte_SPI(BYTE_HIGH);
    TransferByte_SPI(BYTE_MEDIUM);
    TransferByte_SPI(BYTE_LOW);
    PIN_CS = 1;
    return;
}
//-----------------------------------------------
void Interval_sleep(unsigned char times){
    unsigned char i;
    
    for(i=0; i<times; i++){
        __delay_ms(1);
    }
    return;
}