/*
 * File:   30F_UART.c
 * Author: ricar
 *
 * Created on 3 de abril de 2022, 03:47 PM
 */


#include <xc.h>   
#include "30F_UART.h"

//*************      UART1      *************
void UART1_Config(void){
    
    TRISFbits.TRISF3 = 0;
    TRISFbits.TRISF2 = 1;
    
    U1MODEbits.PDSEL = 0b00;
    U1MODEbits.STSEL = 0;
    U1BRG = 15;                 //Baud:115200
    U1STAbits.UTXEN = 1;
    return;
}
void UART1_Open(void){
    
    U1MODEbits.UARTEN = 1;
    return;
}
void UART1_Close(void){
    
    U1MODEbits.UARTEN = 0;
    return;
}

void UART1_Transmit(char data){
    
    while (!U1STAbits.TRMT);
    U1TXREG = data;
    return;
}
char UART1_receive(void){
    
    while(!U1STAbits.URXDA);
    return U1RXREG;
}

void print_string_U1(char *str){
    
    while(*str != '\0'){
        UART1_Transmit(*str);
        str++;
    }
}

//--------------------------------------------------------------------
//Program commands
char ReceiveCMD_UART1(void){
    int conta_byte;
    int data_byte;
    
    for(conta_byte=0; conta_byte<10; conta_byte++){
        data_byte = UART1_receive();
        if(data_byte == 'J'){
            break;
        }
    }
    if(conta_byte < 10){
        data_byte = UART1_receive();
        if(data_byte == 'R'){
            data_byte = UART1_receive();
            if(data_byte == 'O'){
                data_byte = UART1_receive();
                if(data_byte == 'C'){
                    data_byte = UART1_receive();        //M
                    data_byte = UART1_receive();        //D
                    BufferDAT[12] = UART1_receive();    //ID
                    data_byte = UART1_receive();        //13
                    data_byte = UART1_receive();        //10
                    return BufferDAT[12];
                }
            }
        }
    }
    //Disable continuos receive.
    return 'X';
}

//--------------------------------------------------------------------
void TransmitCMD_UART1(char CMD_ID){
    
    print_string_U1("NNNNJROCMD");
    UART1_Transmit(CMD_ID);
    print_string_U1("\n\r");
    return;
}

//--------------------------------------------------------------------
void TransmitDAT_USART(char DAT_ID){
    int conta;
    
    print_string_U1("NNNNJRODAT");
    UART1_Transmit(DAT_ID);
    for(conta=0; conta<12; conta++){
        UART1_Transmit(BufferDAT[conta]);
    }
    print_string_U1("\n\r");
    return;
}





//*************      UART2      *************
void UART2_Config(void){
    
    TRISFbits.TRISF4 = 1;
    TRISFbits.TRISF5 = 0;
    
    U2MODEbits.PDSEL = 0b00;
    U2MODEbits.STSEL = 0;
    U2BRG = 191;                 //Baud:9600
    U2STAbits.UTXEN = 1;
    return;
}
void UART2_Open(void){
    
    U2MODEbits.UARTEN = 1;
    return;
}
void UART2_Close(void){
    
    U2MODEbits.UARTEN = 0;
    return;
}

void UART2_Transmit(char data){
    
    while (!U2STAbits.TRMT);
    U2TXREG = data;
    return;
}
void UART2_receive(void){
    
    while(!U2STAbits.URXDA);
    RxBuffer = U2RXREG;
}

void Enable_InterruptUART2(void){
    
    IFS1bits.U2RXIF = 0;        //Clear Flag U1RX
    U2STAbits.OERR = 0;         //Clear Overrun error bit
    IEC1bits.U2RXIE = 1;        //Enabled Interrupt Rx from UART2
    return;
}

void Disable_InterruptUART2(void){
    
    IFS1bits.U2RXIF = 0;        //Clear Flag U2RX
    IEC1bits.U2RXIE = 0;        //Disabled Interrupt Rx from UART2
    return;
}