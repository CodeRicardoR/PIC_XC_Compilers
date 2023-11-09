/*
 
 
 */

#include <xc.h>
#include "LIB18F_USART.h"

void ConfigSerialPort(void){
    
    TRISCbits.TRISC6 = 0;   //Output
    TRISCbits.TRISC7 = 1;   //Input

    TXSTA = 0b00100100;     //8bits, HIGH speed, 
    RCSTA = 0b00010000;     //Continous rx enabled, Serial Port Disabled
    // Fosc = 9.8304 MHz (PLL x 4) => Baud rate = 38400 bps
    SPBRG = 63;
    // Setting interrupt RX USART
    PIE1bits.RCIE = 1;
    //Config TMR2 to receive data
    T2CON = 0b01111011;     //TMR2 Off, Prescaler:16, Postcaler:16
    PR2 = 0xFF;
    return;
}
void OpenSerialPort(void){
    
    RCSTAbits.SPEN = 1;     //Enabled Serial port
    return;
}
void CloseSerialPort(void){
    
    RCSTAbits.SPEN = 0;     //Disabled Serial port
    return;
}
char ReadByte_USART(void){
    unsigned char conta;
    unsigned char Ready;
    char data;
    
    conta = 0;
    Ready = 255;
    TMR2 = 0x00;
    T2CONbits.TMR2ON = 1;
    //while((!PIR1bits.RCIF) && (!PIR1bits.TMR2IF));
    while(Ready){
        while((!PIR1bits.RCIF) && (!PIR1bits.TMR2IF));
        PIR1bits.TMR2IF = 0;
        conta++;
        if((PIR1bits.RCIF == 1) || (conta>3)){
            Ready = 0;
        }
    }
    PIR1bits.TMR2IF = 0;
    T2CONbits.TMR2ON = 0;
    data = (char) RCREG;
    return data;
}
void WriteByte_USART(char byte){
    
    TXREG = byte;
    NOP();
    while(!TXSTAbits.TRMT);
    return;
}

//--------------------------------------------------------------------
void print_string(char *str){
    while(*str != '\0'){
        WriteByte_USART(*str);
        str++;
    }
}

//--------------------------------------------------------------------
char ReceiveCMD_USART(void){
    unsigned char conta_byte;
    char data_byte;
    
    for(conta_byte=0; conta_byte<10;conta_byte++){
        data_byte = ReadByte_USART();
        if(data_byte == 'J'){
            break;
        }
    }
    if(conta_byte < 10){
        data_byte = ReadByte_USART();
        if(data_byte == 'R'){
            data_byte = ReadByte_USART();
            if(data_byte == 'O'){
                data_byte = ReadByte_USART();
                if(data_byte == 'C'){
                    data_byte = ReadByte_USART();   //M
                    data_byte = ReadByte_USART();   //D
                    BufferDAT[12] = ReadByte_USART(); //ID
                    data_byte = ReadByte_USART();   //13
                    data_byte = ReadByte_USART();   //10
                    return BufferDAT[12];
                }
            }
        }
    }
    RCSTAbits.CREN = 0; //Disable continuos receive.
    return 'X';
}
//--------------------------------------------------------------------
char ReceiveDAT_USART(void){
    unsigned char conta_byte;
    char data_byte;
    
    for(conta_byte=0; conta_byte<10;conta_byte++){
        data_byte = ReadByte_USART();
        if(data_byte == 'J'){
            break;
        }
    }
    if(conta_byte < 10){
        data_byte = ReadByte_USART();
        if(data_byte == 'R'){
            data_byte = ReadByte_USART();
            if(data_byte == 'O'){
                data_byte = ReadByte_USART();
                if(data_byte == 'D'){
                    data_byte = ReadByte_USART();   //A
                    data_byte = ReadByte_USART();   //T
                    BufferDAT[12] = ReadByte_USART();   //ID
                    for(conta_byte=0; conta_byte<12;conta_byte++){
                        BufferDAT[conta_byte] = ReadByte_USART();
                    }
                    data_byte = ReadByte_USART();   //13
                    data_byte = ReadByte_USART();   //10
                    return BufferDAT[12];
                }
            }
        }
    }
    RCSTAbits.CREN = 0; //Disable continuos receive.
    return 'X';
}
//--------------------------------------------------------------------
void TransmitCMD_USART(char CMD_ID){
    
    print_string("NNNNJROCMD");
    WriteByte_USART(CMD_ID);
    print_string("\n\r");
    return;
}
//--------------------------------------------------------------------
void TransmitDAT_USART(char DAT_ID){
    unsigned char conta;
    
    print_string("NNNNJRODAT");
    WriteByte_USART(DAT_ID);
    for(conta=0; conta<12; conta++){
        WriteByte_USART(BufferDAT[conta]);
    }
    print_string("\n\r");
    return;
}

//--------------------------------------------------------------------
void print_value(signed long value){
    //[Byte_3|Byte_2|Byte_1|Byte_0]
    char Bytes[5]={0,0,0,0};
    signed long RegTemp;
    unsigned char conta;
    
    //Byte3:
    RegTemp = value>>24;
    Bytes[3] = (char) RegTemp;
    
    RegTemp = value>>16;
    Bytes[2] = (char) RegTemp;
    
    RegTemp =  value>>8;
    Bytes[1] = (char) RegTemp;
    
    RegTemp = value;
    Bytes[0] = (char) RegTemp;
    
    for(conta=0; conta<4; conta++){
        WriteByte_USART(Bytes[conta]);
    }
    
    return;
}
