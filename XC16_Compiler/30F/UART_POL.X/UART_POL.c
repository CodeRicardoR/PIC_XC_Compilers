/*
 * File:   UART_POL.c
 * Author: ricar
 *
 * Created on 2 de abril de 2022, 09:18 PM
 */


#include "main.h"
void UART1_Config(void);
void UART1_Open(void);
void UART1_Close(void);
void UART1_Transmit(char data);
char UART1_receive(void);

int main(void){
    char uart_data;
    
    ADPCFG = 0x1FFF;    //All pins are digital
    
    //Config to board:
    TRISFbits.TRISF4 = 1;
    TRISFbits.TRISF5 = 1;
    
    UART1_Open();
    UART1_Config();
    UART1_Transmit('R');
    UART1_Transmit(':');
    UART1_Transmit(10);
    UART1_Transmit(13);
    while(1){
        uart_data = UART1_receive();
        UART1_Transmit(uart_data);
    }
    UART1_Close();
    
    return 0;
}

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
