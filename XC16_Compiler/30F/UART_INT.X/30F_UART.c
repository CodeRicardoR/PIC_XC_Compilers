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
char UART2_receive(void){
    
    while(!U2STAbits.URXDA);
    return U2RXREG;
}