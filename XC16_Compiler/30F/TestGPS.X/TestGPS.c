/*
 * File:   TestGPS.c
 * Author: ricar
 *
 * Created on 9 de abril de 2022, 08:56 PM
 */


#include "main.h"

//Global variables
char RxBuffer;
char BufferDAT[14];
char command;

void __attribute__((__interrupt__,auto_psv)) _U2RXInterrupt(void){
    
    IFS1bits.U2RXIF = 0;            //Clear Flag U1RX
    
    switch(command){
        case 0:
            GPS_STATUS();
            if(RxBuffer == 'A'){
                print_string_U1("GPS Ready!!\n\r");
                command = 1;
            }
            break;
            
        case 1:
            GPS_LATITUDE();
            print_string_U1("Latitude: ");
            print_string_U1(BufferDAT);
            print_string_U1("\n\r");
            command = 2;
            break;
            
        case 2:
            GPS_LONGITUDE();
            print_string_U1("Longitude: ");
            print_string_U1(BufferDAT);
            print_string_U1("\n\r");
            command = 3;
            break;
            
        case 3:
            GPS_TIMEDATE();
            print_string_U1("DATE:");
            print_string_U1(BufferDAT);
            print_string_U1("\n\r");
            command = 3;
            break;
    }
    
    IFS1bits.U2RXIF = 0;            //Clear Flag U1RX
    
    return;
}

int main(void) {
    
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
    
    command = 0;
    while(1);
    UART1_Close();
    UART2_Close();
    return 0;
}
