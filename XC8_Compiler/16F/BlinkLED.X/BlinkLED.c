#include <xc.h>

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 9830400UL




void main(void) {
    
    //Config PORTS
    //Config IO PORTS
    ADCON1bits.PCFG = 0x6;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    PORTCbits.RC6 = 1;          //RC6/TX for IDLE in high
    TRISBbits.TRISB0 = 1;       //Input for INT_EXT GPS
    TRISBbits.TRISB4 = 1;       //Input for PIN_READY
    TRISCbits.TRISC3 = 1;       //Input, connect to RB0 for GPS
    TRISCbits.TRISC5 = 1;       //Input for PIN_DATA
    
    while(1){
        PORTAbits.RA0 = 1;
        __delay_ms(500);
        PORTAbits.RA0 = 0;
        __delay_ms(500);
    }
    return;
}
