
/*
 * File:   TestBothUART.c
 * Author: ricar
 *
 * Created on 14 de abril de 2022, 08:41 PM
 */


#include "main.h"

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void){
    char uart_data;
    
    if((IEC0bits.U1RXIE == 1)&&(IFS0bits.U1RXIF == 1)){
        IFS0bits.U1RXIF = 0;            //Clear Flag U1RX
        uart_data = UART1_receive();
        
        switch(uart_data){
            case '1'://Enable interrupt U2RX
                //Enable UART2 interrupt
                Enable_InterruptUART2();
                LATBbits.LATB9 = 1;
                UART1_Transmit('1');
                break;
            case '0':
                //Disable UART2 interrupt
                Disable_InterruptUART2();
                LATBbits.LATB9 = 0;
                LATBbits.LATB10 = 0;
                UART1_Transmit('0');
                break;
            default:
                Nop();
        }
    }
    
    return;
}

void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void){
    
    if((IEC1bits.U2RXIE == 1)&&(IFS1bits.U2RXIF == 1)){
        IFS1bits.U2RXIF = 0;            //Clear Flag U1RX
        UART2_receive();
        UART1_Transmit(RxBuffer);
        LATBbits.LATB10 = 1;
    }

    return;
}

int main(void) {
    
    ADPCFG = 0x1FFF;        //All pins are digital
    TRISBbits.TRISB9 = 0;
    TRISBbits.TRISB10 = 0;
    LATBbits.LATB9 = 0;
    LATBbits.LATB10 = 0;

    //Setting UART2
    UART2_Open();
    UART2_Config();
    
    //Setting UART1
    UART1_Open();
    UART1_Config();
    
    //Enablin UART1 interrupt
    IPC2bits.U1RXIP = 7;        //Level 7 (of 7)) level priority
    IFS0bits.U1RXIF = 0;        //Clear Flag U1RX
    IEC0bits.U1RXIE = 1;        //Enabled Interrupt Rx from UART1
    
    //Enable UART2 interrupt
    IPC6bits.U2RXIP = 6;        //Level 6 (of 7)) level priority
    //IFS1bits.U2RXIF = 0;        //Clear Flag U2RX
    //IEC1bits.U2RXIE = 1;        //Enabled Interrupt Rx from UART2
    
    while(1);
    UART2_Close();
    UART1_Close();
    
    return 0;
}
