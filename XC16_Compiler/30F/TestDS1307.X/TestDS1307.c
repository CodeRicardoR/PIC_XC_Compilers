/*
 * File:   TestDS1307.c
 * Author: ricar
 *
 * Created on 24 de abril de 2022, 04:05 PM
 */

#include "main.h"

#define UNI 0x0F
#define DEC 0x70

void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt(void);

void Enable_INT1(char edge);
void UART1_Config(void);
void UART1_Open(void);
void UART1_Close(void);
void UART1_Transmit(char data);
char UART1_receive(void);
char UART1_receive(void);
void print_string_U1(char *str);


int main(void) {
    
    //Set PORTD INT1-RD8
    TRISDbits.TRISD8 = 1;
    
    UART1_Open();
    UART1_Config();

    Enable_INT1(1);     //Rising edge
    
    while(1);
    UART1_Close();
    return 0;
}


//USART UART1
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
    return (char)U1RXREG;
}

void print_string_U1(char *str){
    
    while(*str != '\0'){
        UART1_Transmit(*str);
        str++;
    }
}

// Interrupt INT1
void Enable_INT1(char edge){
    
    if(edge == 1){
        //Rising edge
        INTCON2bits.INT1EP = 0;
    }else{
        //Falling_edge
        INTCON2bits.INT1EP = 1;
    }
    IFS1bits.INT1IF = 0;        //Clear Flag bit
    IPC4bits.INT1IP = 6;        //INT2 level 6 priority
    IEC1bits.INT1IE = 1;        //Interrupt INT2 enabled 
    return;
}

void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt(void){
    
    IFS1bits.INT1IF = 0;        //Clear Flag bit
    
    DS1307_ReadTime();
    //Time
    sec_uni = DataTime[0] & UNI;
    sec_dec = DataTime[0] & DEC;
    sec_dec >>= 4;
    Second = sec_dec*10 + sec_uni;
    
    min_uni = DataTime[1] & UNI;
    min_dec = DataTime[1] & DEC;
    min_dec >>= 4;
    Minute = min_dec*10 + min_uni;
    
    hou_uni = DataTime[2] & UNI;
    hou_dec = DataTime[2] & 0x30;
    hou_dec >>= 4;
    Hour = hou_dec*10 + hou_uni;
    
    //Date
    dat_uni = DataTime[3] & UNI;
    dat_dec = DataTime[3] & DEC;
    dat_dec >>= 4;
    Date = dat_dec*10 + dat_uni;
    
    mon_uni = DataTime[4] & UNI;
    mon_dec = DataTime[4] & DEC;
    mon_dec >>= 4;
    Month = mon_dec*10 + mon_uni;
    
    yea_uni = DataTime[5] & UNI;
    yea_dec = DataTime[5] & DEC;
    yea_dec >>= 4;
    Year = yea_dec*10 + yea_uni;
    
    sprintf(DATE,"%02d%02d%02d%02d%02d%02d",Hour,Minute,Second,Date,Month,Year);
    DATE[13] = '\0';
    print_string_U1("DATE:");
    print_string_U1(DATE);
    print_string_U1("\n\r");

    return;
}