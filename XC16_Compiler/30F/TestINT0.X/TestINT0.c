/*
 * File:   TestINT0.c
 * Author: ricar
 *
 * Created on 24 de abril de 2022, 07:00 AM
 */


#include "main.h"

void __attribute__((__interrupt__, no_auto_psv)) _INT0Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _INT2Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt(void);

void Enable_INT0(char edge);
void Enable_INT2(char edge);
void Enable_INT1(char edge);

int main(void) {
    //Set PORTB pines
    ADPCFG = 0x1FFF;    //All pins are digital
    TRISBbits.TRISB9 = 0;
    LATBbits.LATB9 = 0;
    
    //Set PORTA INT0-RA11
    //TRISAbits.TRISA11 = 1;
    
    //Set PORTD INT2-RD9
    //TRISDbits.TRISD9 = 1;
    
    //Set PORTD INT1-RD8
    TRISDbits.TRISD8 = 1;
    
    //Enable_INT0(0);
    //Enable_INT2(0);
    Enable_INT1(0);
    
    while(1);
    
    return 0;
}


// Interrupt INT0
void Enable_INT0(char edge){
    
    if(edge == 1){
        //Rising edge
        INTCON2bits.INT0EP = 0;
    }else{
        //Falling_edge
        INTCON2bits.INT0EP = 1;
    }
    IFS0bits.INT0IF = 0;        //Clear Flag bit
    IPC0bits.INT0IP = 7;        //INT0 Max. priority
    IEC0bits.INT0IE = 1;        //Interrupt INT0 enabled 
    return;
}

void __attribute__((__interrupt__, no_auto_psv)) _INT0Interrupt(void){
    
    IFS0bits.INT0IF = 0;        //Clear Flag bit
    __delay_ms(50);
    if(PORTAbits.RA11 == 0){
        LATBbits.LATB9 ^= 1;        //Tootgle bit
    }
    
    return;
}

// Interrupt INT2
void Enable_INT2(char edge){
    
    if(edge == 1){
        //Rising edge
        INTCON2bits.INT2EP = 0;
    }else{
        //Falling_edge
        INTCON2bits.INT2EP = 1;
    }
    IFS1bits.INT2IF = 0;        //Clear Flag bit
    IPC5bits.INT2IP = 6;        //INT2 level 6 priority
    IEC1bits.INT2IE = 1;        //Interrupt INT2 enabled 
    return;
}

void __attribute__((__interrupt__, no_auto_psv)) _INT2Interrupt(void){
    
    IFS1bits.INT2IF = 0;        //Clear Flag bit
    __delay_ms(50);
    if(PORTDbits.RD9 == 0){
        LATBbits.LATB9 ^= 1;    //Tootgle bit
    }
    
    return;
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
    __delay_ms(50);
    if(PORTDbits.RD8 == 0){
        LATBbits.LATB9 ^= 1;    //Tootgle bit
    }
    
    return;
}