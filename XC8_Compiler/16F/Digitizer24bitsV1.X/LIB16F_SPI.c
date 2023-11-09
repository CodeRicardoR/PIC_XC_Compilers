
#include "main.h"

void ConfigPort_SPI(void){
    
    TRISCbits.TRISC3 = 0;       //SCK output
    TRISCbits.TRISC4 = 1;       //SDI input
    TRISCbits.TRISC5 = 0;       //SDO output
    
    SSPSTATbits.SMP = 0;        //SMP:0
    SSPSTATbits.CKE = 1;        //CKE:1
    SSPCONbits.CKP = 0;         //LOW is idle for CLK
    SSPCONbits.SSPM = 0x1;      //Freq = FOSC/16 = 614.4 Kbits  
    return;
}

void OpenPort_SPI(void){
    
    SSPCONbits.SSPEN = 1;      //SPI Enabled
    return;
}

void ClosePort_SPI(void){

    SSPCONbits.SSPEN = 0;      //SPI Disabled
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
unsigned char ResetCS5532(void){
    unsigned char conta;
    
    //Reset PORT:
    for(conta=0;conta<15;conta++){
        TransferByte_SPI(0xFF);
    }
    TransferByte_SPI(0xFE);
    __delay_ms(2);
    //Reset ADC
    ReadRegister_4bytesCS5532(0x0B);    //Read Configuration register
    BufferSPI[0] |= 0x20;               // Bit 5:RS = '1'
    WriteRegister_4bytesCS5532(0x03);   //Write Configuration Register, bit RS = '1'
    __delay_us(25);
    BufferSPI[0] &= 0xDF;               // Bit 5:RS = '0'
    WriteRegister_4bytesCS5532(0x03);   //Write Configuration Register, bit RS = '0'
    __delay_us(25);
    ReadRegister_4bytesCS5532(0x0B);    //Read Configuration register, check bit RV.
    return BufferSPI[0];                //BufferSPI[0] = 0x10
}
//-----------------------------------------------
void InitCS5532(void){
    
    //Set range of reference voltage:
    ReadRegister_4bytesCS5532(0x0B);    //Read Configuration register
    BufferSPI[0] |= 0x02;               // Bit 1: VRS = '1'
    WriteRegister_4bytesCS5532(0x03);   //Write Configuration Register, bit VRS = '1'
    //Set Word rate
    BufferSPI[0] = 0x05;                // Word rate = 960Sps
    BufferSPI[1] = 0x00;
    BufferSPI[2] = 0x05;
    BufferSPI[3] = 0x00;
    WriteRegister_4bytesCS5532(0x05);   //Write in Channel Setup Register
    return;
}
//-----------------------------------------------
void ReadRegister_4bytesCS5532(unsigned char command){

    TransferByte_SPI(command);
    BufferSPI[0] = TransferByte_SPI(0x00);  //Upper byte
    BufferSPI[1] = TransferByte_SPI(0x00);  //High byte
    BufferSPI[2] = TransferByte_SPI(0x00);  //Low byte
    BufferSPI[3] = TransferByte_SPI(0x00);  //Lower byte
    return;
}
//-----------------------------------------------
void WriteRegister_4bytesCS5532(unsigned char command){
    
    TransferByte_SPI(command);
    TransferByte_SPI(BufferSPI[0]);         //Upper byte
    TransferByte_SPI(BufferSPI[1]);         //High byte
    TransferByte_SPI(BufferSPI[2]);         //Low byte
    TransferByte_SPI(BufferSPI[3]);         //Lower byte

    return;
}
//---------------------------------------------------
//Function to read DATA from CS5532:
signed long ReadAD24(unsigned char flag_convert, unsigned char cmd_convert){
    signed long DataADC;
   
    
    while(PIN_SDI);
    ReadRegister_4bytesCS5532(cmd_convert);
    
    DataADC = 0;
    if(flag_convert){
        DataADC = DataADC | (signed long) BufferSPI[0]<<24;         //ByteUpper
        DataADC = DataADC | (signed long) BufferSPI[1]<<16;         //ByteHigh
        DataADC = DataADC | (signed long) BufferSPI[2]<<8;          //ByteLow
        DataADC = DataADC | (signed long) 0x00<<0;                  //ByteLower
        DataADC = DataADC/256;
    }
    
    return DataADC;
}
//-----------------------------------------------
void CleanHolding(void){
    
    PORTB = 0X0C;
    __delay_us(100);
    ReadAD24(0, 0x00);
    
    return;
}
//-----------------------------------------------