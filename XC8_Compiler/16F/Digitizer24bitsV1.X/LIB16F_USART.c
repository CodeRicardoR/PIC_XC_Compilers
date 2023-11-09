
#include "main.h"

void ConfigPort_USART(void){
    
    TRISCbits.TRISC6 = 0;   //Output to TX
    TRISCbits.TRISC7 = 1;   //Input to RX
    //Config Serial Port: 38400,8,1,N: FOSC = 9.8304 MHz
    TXSTA = 0x24;   //Transmisor enabled, High Speed, mode async.
    RCSTA = 0x10;   //Continuos reception enabled.
    SPBRG = 0x0F;   //(_XTAL_FREQ/(16*baud) - 1):baud = 38400
    //Config TIMER2: Timeout: 26.67 ms
    T2CON = 0x7A;
    PR2 = 0xFF;
    PIR1bits.TMR2IF = 0;
    //Enable USART Receiver Interrupt:
    PIE1bits.RCIE = 1;
    return;
}
//--------------------------------------------------------------------
void OpenPort_USART(void){
    
    RCSTAbits.SPEN = 1; // USART enabled 
    return;
}
//--------------------------------------------------------------------
void ClosePort_USART(void){
    
    RCSTAbits.SPEN = 0; // USART disabled
    return;
}
//--------------------------------------------------------------------
unsigned char ReadByte_USART(void){
    
    TMR2 = 0x00;
    T2CONbits.TMR2ON = 1;   //TMR2 ON
    while((!PIR1bits.RCIF) && (!PIR1bits.TMR2IF));
    //while(!PIR1bits.RCIF);
    T2CONbits.TMR2ON = 0;
    PIR1bits.TMR2IF = 0;
    return RCREG;
}
//--------------------------------------------------------------------
void WriteByte_USART(unsigned char byte){
    
    TXREG = byte;
    NOP();
    while(!PIR1bits.TXIF);
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
unsigned char ReceiveCMD_USART(void){
    unsigned char conta_byte;
    unsigned char data_byte;
    
    for(conta_byte=0; conta_byte<10; conta_byte++){
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
//unsigned char ReceiveDAT_USART(void){
//    unsigned char conta_byte;
//    unsigned char data_byte;
//    
//    for(conta_byte=0; conta_byte<10; conta_byte++){
//        data_byte = ReadByte_USART();
//        if(data_byte == 'J'){
//            break;
//        }
//    }
//    if(conta_byte < 10){
//        data_byte = ReadByte_USART();
//        if(data_byte == 'R'){
//            data_byte = ReadByte_USART();
//            if(data_byte == 'O'){
//                data_byte = ReadByte_USART();
//                if(data_byte == 'D'){
//                    data_byte = ReadByte_USART();   //A
//                    data_byte = ReadByte_USART();   //T
//                    BufferDAT[12] = ReadByte_USART();   //ID
//                    for(conta_byte=0; conta_byte<12;conta_byte++){
//                        BufferDAT[conta_byte] = ReadByte_USART();
//                    }
//                    data_byte = ReadByte_USART();   //13
//                    data_byte = ReadByte_USART();   //10
//                    return BufferDAT[12];
//                }
//            }
//        }
//    }
//    RCSTAbits.CREN = 0; //Disable continuos receive.
//    return 'X';
//}
//--------------------------------------------------------------------
void TransmitCMD_USART(unsigned char CMD_ID){
    
    print_string("NNNNJROCMD");
    WriteByte_USART(CMD_ID);
    print_string("\n\r");
    return;
}
//--------------------------------------------------------------------
void TransmitDAT_USART(unsigned char DAT_ID){
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
    //[Byte_2|Byte_1|Byte_0]
    char Bytes[3];
    
    //Byte3:
    Bytes[2] = (char) ((value&0x00FF0000)>>16);
    //Byte2:
    Bytes[1] = (char) ((value&0x0000FF00)>>8);
    //Byte1:
    Bytes[0] = (char) ((value&0x000000FF)>>0);
    
    
    WriteByte_USART(Bytes[0]);
    WriteByte_USART(Bytes[1]);
    WriteByte_USART(Bytes[2]);
    
    return;
}

