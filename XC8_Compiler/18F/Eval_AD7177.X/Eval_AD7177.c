/*
 * File:   Eval_AD7177.c
 * Author: Usuario
 *
 * Created on 6 de enero de 2022, 11:54 AM
 */

// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator with PLL enabled/Clock frequency = (4 x FOSC))
#pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = ON         // Brown-out Reset Enable bit (Brown-out Reset enabled)
#pragma config BORV = 25        // Brown-out Reset Voltage bits (VBOR set to 2.5V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

// CONFIG4L
#pragma config STVR = OFF       // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will not cause Reset)
#pragma config LVP = OFF        // Low-Voltage ICSP Enable bit (Low-Voltage ICSP disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 39321600UL

//Definiciones de pines
#define LED_LOGGER LATAbits.LATA3
#define PIN_DIN PORTCbits.RC4
#define LED PORTAbits.RA3

//Channels to select
#define ChannelX    0x40
#define ChannelY    0x60
#define ChannelZ    0x80
#define ChannelTC   0xA0
#define ChannelTS   0xB0
#define ChannelAUX0 0x50
#define ChannelAUX1 0x70
#define ChannelAUX2 0x90
#define ChannelGND  0xC0

#include <xc.h>
#include "LIB18F_USART.h"
#include "LIB18F_SPI.h"

void __interrupt() Interrups(void){
    unsigned char ID, conta, samples, interval, conta_10;
    if((PIE1bits.RC1IE == 1) && (PIR1bits.RC1IF == 1)){
        ID = ReceiveCMD_USART();
        switch(ID){
            case '0':   //Test Serial Communication
                ResetAD7177();
                TransmitCMD_USART('0');
                break;
            case '1':   //Read Registers 1 byte in AD7177-2:
                TransmitCMD_USART('1');
                ID = ReceiveDAT_USART();
                if(ID == 'X'){
                    RCSTAbits.CREN = 1; //Enable continuos receive.
                }else if(ID == '1'){
                    ReadRegister_1bytesAD7177(BufferDAT[0]);
                }
                TransmitDAT_USART(ID);
                break;
            case '2':   //Read Registers 2 byte in AD7177-2:
                TransmitCMD_USART('2');
                ID = ReceiveDAT_USART();
                if(ID == 'X'){
                    RCSTAbits.CREN = 1; //Enable continuos receive.
                }else if(ID == '2'){
                    ReadRegister_2bytesAD7177(BufferDAT[0]);
                }
                TransmitDAT_USART(ID);
                break;
            case '3':   //Read Registers 3 byte in AD7177-2:
                TransmitCMD_USART('3');
                ID = ReceiveDAT_USART();
                if(ID == 'X'){
                    RCSTAbits.CREN = 1; //Enable continuos receive.
                }else if(ID == '3'){
                    ReadRegister_3bytesAD7177(BufferDAT[0]);
                }
                TransmitDAT_USART(ID);
                break;
            case '4':   //Write Registers 2 bytes in AD7177-2:
                TransmitCMD_USART('4');
                ID = ReceiveDAT_USART();
                if(ID == 'X'){
                    RCSTAbits.CREN = 1; //Enable continuos receive.
                }else if(ID == '4'){
                    WriteRegister_2bytesAD7177(BufferDAT[0], BufferDAT[1], BufferDAT[2]);
                }
                TransmitDAT_USART(ID);
                break;
            case '5':   //Write Registers 3 bytes in AD7177-2:
                TransmitCMD_USART('5');
                ID = ReceiveDAT_USART();
                if(ID == 'X'){
                    RCSTAbits.CREN = 1; //Enable continuos receive.
                }else if(ID == '5'){
                    WriteRegister_3bytesAD7177(BufferDAT[0], BufferDAT[1], BufferDAT[2], BufferDAT[3]);
                }
                TransmitDAT_USART(ID);
                break;
            case '6':   //Start Continuos conversion
                TransmitCMD_USART('6');
                ID = ReceiveDAT_USART();
                if(ID == 'X'){
                    TransmitDAT_USART(ID);
                    RCSTAbits.CREN = 1; //Enable continuos receive.
                }else if(ID == '6'){
                    PIN_CS = 0;
                    samples = BufferDAT[0];
                    interval = BufferDAT[1];
                    for(conta=0; conta<(samples - 1); conta++){
                        for(conta_10=0; conta_10 < 10; conta_10++){
                            while(PIN_DIN);     //Wait to '0'
                            TransferByte_SPI(0x44); //Write ADC Mode Register
                            BufferDAT[0] = TransferByte_SPI(0x00);   //Bytes
                            BufferDAT[1] = TransferByte_SPI(0x00);   //Bytes
                            BufferDAT[2] = TransferByte_SPI(0x00);   //Bytes
                            BufferDAT[3] = TransferByte_SPI(0x00);   //Bytes
                            TransmitDAT_USART('6');  
                        }
                        Interval_sleep(interval);
                    }
                    for(conta_10=0; conta_10 < 10; conta_10++){
                        while(PIN_DIN);     //Wait to '0'
                        TransferByte_SPI(0x44); //Write ADC Mode Register
                        BufferDAT[0] = TransferByte_SPI(0x00);   //Bytes
                        BufferDAT[1] = TransferByte_SPI(0x00);   //Bytes
                        BufferDAT[2] = TransferByte_SPI(0x00);   //Bytes
                        BufferDAT[3] = TransferByte_SPI(0x00);   //Bytes
                        TransmitDAT_USART('6');
                    }
                    PIN_CS = 1;
                }
                break;
            case '7':   //
                TransmitCMD_USART('7');
                ID = ReceiveDAT_USART();
                if(ID == 'X'){
                     RCSTAbits.CREN = 1; //Enable continuos receive.
                }else if(ID == '7'){
                    switch(BufferDAT[0]){
                        case '1':
                            PORTB = ChannelX;
                            break;
                        case '2':
                            PORTB = ChannelY;
                            break;
                        case '3':
                            PORTB = ChannelZ;
                            break;
                        case '4':
                            PORTB = ChannelTC;
                            break;
                        case '5':
                            PORTB = ChannelTS;
                            break;
                        case '6':
                            PORTB = ChannelAUX0;
                            break;
                        case '7':
                            PORTB = ChannelAUX1;
                            break;
                        case '8':
                            PORTB = ChannelAUX2;
                            break;
                        case '9':
                            PORTB = ChannelGND;
                            break;
                        default:
                            NOP();
                    }
                    __delay_ms(50);
                }
                TransmitDAT_USART(ID);
                break;
            case '8':   //Simple conversion
                TransmitCMD_USART('8');
                PIN_CS = 0;
                TransferByte_SPI(0x01); //Write ADC Mode Register
                TransferByte_SPI(0x00); //Simple Conversi?n
                TransferByte_SPI(0x1C); //REF_EN desahbilitado, Cristal externo
                NOP();
                NOP();
                while(PIN_DIN);     //Wait to '0'
                TransferByte_SPI(0x44); //Write ADC Mode Register
                BufferDAT[0] = TransferByte_SPI(0x00);   //Bytes
                BufferDAT[1] = TransferByte_SPI(0x00);   //Bytes
                BufferDAT[2] = TransferByte_SPI(0x00);   //Bytes
                BufferDAT[3] = TransferByte_SPI(0x00);   //Bytes
                PIN_CS = 1;
                TransmitDAT_USART('8');
                break;
            case '9':   //Execute specific task
                TransmitCMD_USART('9');
                ID = ReceiveDAT_USART();
                if(ID == 'X'){
                    RCSTAbits.CREN = 1; //Enable continuos receive.
                }else if(ID == '9'){
                    PIN_CS = 0;
                    TransferByte_SPI(0x01); //Write ADC Mode Register
                    TransferByte_SPI(BufferDAT[0]); // Byte High
                    TransferByte_SPI(BufferDAT[1]); // Byte Low
                    NOP();
                    NOP();
                    while(PIN_DIN);     //Wait to '0'
                    TransferByte_SPI(0x44); //Write ADC Mode Register
                    BufferDAT[0] = TransferByte_SPI(0x00);   //Bytes
                    BufferDAT[1] = TransferByte_SPI(0x00);   //Bytes
                    BufferDAT[2] = TransferByte_SPI(0x00);   //Bytes
                    PIN_CS = 1;    
                }
                TransmitDAT_USART(ID);
                break;
            default:
                TransmitCMD_USART('X');
                RCSTAbits.CREN = 1; //Enable continuos receive.
        }
    }
    return;
}

void main(void) {
    
    //-----------------------------------------------
    //Config IO PORTS
    ADCON1bits.PCFG = 0x07;     //Config PORTA DIGITAL
    LATA = 0;
    LATB = 0;
    LATC = 0;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    //-----------------------------------------------
    //Init Global Variables
    //
    //-----------------------------------------------
    //Config SPI to 2.5 Mbits/s
    PIN_CS = 1;         //CS = '1'
    ConfigPort_SPI();
    OpenPort_SPI();
    //-----------------------------------------------
    //Config USART to 38400 bauds
    ConfigSerialPort();
    OpenSerialPort();
    //-----------------------------------------------
    //
    //-----------------------------------------------
    //Config Interrupts
    RCONbits.IPEN = 0;      //Interrupt in mode 16F compatibility
    INTCON = 0x00;          //Disable others interrupta and clear Flags
    //Interrupts
    INTCONbits.PEIE = 1;    //Enable interrupt peripherical
    INTCONbits.GIE = 1;     //Enable Global Interrupt
    while(1);
    CloseSerialPort();
    ClosePort_SPI();
    return;
}

