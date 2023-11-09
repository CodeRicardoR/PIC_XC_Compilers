/*
 * File:   UART_INT.c
 * Author: ricar
 *
 * Created on 3 de abril de 2022, 03:27 PM
 */

#include "main.h"


void __attribute__((__interrupt__,no_auto_psv)) _U1RXInterrupt(void){
    char uart_data;
    
    IFS0bits.U1RXIF = 0;            //Clear Flag U1RX
    uart_data = UART1_receive();
    UART1_Transmit(uart_data);
    return;
}


//void __attribute__((__interrupt__,no_auto_psv)) _U2RXInterrupt(void){
//    char uart_data;
//    
//    IFS1bits.U2RXIF = 0;            //Clear Flag U1RX
//    uart_data = UART2_receive();
//    UART1_Transmit(uart_data);
//    return;
//}

int main(void){
    
    ADPCFG = 0x1FFF;    //All pins are digital
    
    //Setting UART1
    UART1_Open();
    UART1_Config();
    UART1_Transmit('R');
    UART1_Transmit(':');
    //Enablin UART1 interrupt
    IPC2bits.U1RXIP = 4;        //Level 4 (of 7)) level priority
    IFS0bits.U1RXIF = 0;        //Clear Flag U1RX
    IEC0bits.U1RXIE = 1;        //Enabled Interrupt Rx from UART1
    while(1);
    UART1_Close();
    
    /*
    //Setting UART2
    UART2_Open();
    UART2_Config();
    //Setting UART1
    UART1_Open();
    UART1_Config();
    //Enablin UART2 interrupt
    IPC6bits.U2RXIP = 4;        //Level 4 (of 7)) level priority
    IFS1bits.U2RXIF = 0;        //Clear Flag U2RX
    IEC1bits.U2RXIE = 1;        //Enabled Interrupt Rx from UART2

    while(1);
    UART2_Close();
    UART1_Close();
    */
    
    return 0;
}
