/*
 * File:   Timig_AD7177.c
 * Author: ricar
 *
 * Created on 27 de septiembre de 2022, 01:59 PM
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
#define LED_RED LATAbits.LATA1
#define LED_GREEN LATAbits.LATA2

#define PIN_DIN PORTCbits.RC4

//Definiciones para seleccion de canal de ADC: MAX337
/* PINES       SW_A    SW_B    A1   A0
 * CHANNEL      RA5     RA4    RC0  RC1
 *    X          0       1      0    0
 *    Y          0       1      1    0
 *    Z          1       0      0    0
 *   TC          1       0      1    0
 *   TS          1       0      1    1
 * AUX0          0       1      0    1
 * AUX1          0       1      1    1
 * AUX2          1       0      0    1
 *  GND          0       0      X    X
 */
#define PIN_SWA LATAbits.LA5
#define PIN_SWB LATAbits.LA4
#define PIN_A1 LATCbits.LC0
#define PIN_A0 LATCbits.LC1

//Definicon de numero de muestras a promediar
#define Nsamples 10

//Global variables

#include <xc.h>
#include "LIB18F_USART.h"
#include "LIB18F_SPI.h"
#include "LIBGPS_38400.h"

//Global variables
unsigned char conta1;

//Definitions of functions
signed long ReadADC32(void);

void __interrupt() Interrups(void){
    
    if((PIE1bits.TMR1IE == 1) && (PIR1bits.TMR1IF == 1)){
        PIR1bits.TMR1IF = 0;
        conta1++;
    }
    else if((INTCONbits.INT0IE == 1) && (INTCONbits.INT0IF == 1)){
        INTCONbits.INT0IF = 0;
        //--------------------
        //Config to use TIMER0
        TMR0H = 0;
        TMR0L = 0;
        T0CON = 0b10000011; //Timer0: Start, 16bits, prescaler x16
        //--------------------
        GPS_TIMEDATE();
        //--------------------
        T0CONbits.TMR0ON = 0;   //Timer0: Stop
        BufferDAT[0] = TMR0L;
        BufferDAT[1] = TMR0H;
        TransmitDAT_USART('8');
        __delay_ms(500);
        print_string(DateTimeGPS);
        INTCONbits.INT0IE = 0;  //Disable INT0 interrupt
    }
    return;
}


void main(void) {
    unsigned char ID,conta;
    
    signed long Data_X, Data_Y, Data_Z, Data_TC, Data_TS;
    signed long long SumData_X, SumData_Y, SumData_Z, SumData_TC, SumData_TS;
    
    
    //-----------------------------------------------
    //Config IO PORTS
    ADCON1bits.PCFG = 0x07;     //Config PORTA DIGITAL
    LATA = 0;
    LATB = 0;
    LATC = 0;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISBbits.TRISB0 = 1;       //RB0 to GPS-TX input
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
    //-----------------------------------------------
    //Config Interrupts
    RCONbits.IPEN = 0;      //Interrupt in mode 16F compatibility
    INTCON = 0x00;          //Disable others interrupta and clear Flags
    //Interrupts
    INTCONbits.PEIE = 1;    //Enable interrupt peripherical
    INTCONbits.GIE = 1;     //Enable Global Interrupt
    
    //----------------------------------------------
    LED_GREEN = 0;
    LED_RED = 0;

    while(1){
        ID = ReceiveCMD_USART();
        switch(ID){
            case '0'://Reset ADC y Config init
                //-----------------------------------------
                ResetAD7177();
                WriteRegister_2bytesAD7177(0x01, 0x00, 0x2C);     //ADCMODE: Standby
                __delay_ms(5);
                WriteRegister_2bytesAD7177(0x20, 0x1F, 0x00);     //SETUPCON0: All buffers ON, VREF External
                __delay_ms(5);
                WriteRegister_2bytesAD7177(0x02, 0x00, 0x02);     //IFMODE: 32 bits
                //-----------------------------------------
                TransmitCMD_USART('0');
                break;
            case '1':// Write register specific:
                TransmitCMD_USART('1');
                //-----------------------------------------
                ID = ReceiveDAT_USART();
                if(ID == 'X'){
                    RCSTAbits.CREN = 1; //Re-enable continuos receive.
                }else if(ID == '1'){
                    WriteRegister_2bytesAD7177(BufferDAT[0], BufferDAT[1], BufferDAT[2]);
                }
                //-----------------------------------------
                break;
            case '2':// Read register specific:
                TransmitCMD_USART('2');
                //-----------------------------------------
                ID = ReceiveDAT_USART();
                if(ID == 'X'){
                    RCSTAbits.CREN = 1; //Re-enable continuos receive.
                }else if(ID == '2'){
                    ReadRegister_2bytesAD7177(BufferDAT[0]);
                    TransmitDAT_USART(ID);
                }
                //-----------------------------------------
                break;
            case '3':// Select channel
                TransmitCMD_USART('3');
                //-----------------------------------------
                ID = ReceiveDAT_USART();
                if(ID == 'X'){
                     RCSTAbits.CREN = 1; //Enable continuos receive.
                }else if(ID == '3'){
                    switch(BufferDAT[0]){
                        case 'X':
                            PIN_SWA = 0;
                            PIN_SWB = 1;
                            PIN_A1 = 0;
                            PIN_A0 = 0;
                            break;
                        case 'Y':
                            PIN_SWA = 0;
                            PIN_SWB = 1;
                            PIN_A1 = 1;
                            PIN_A0 = 0;
                            break;
                        case 'Z':
                            PIN_SWA = 1;
                            PIN_SWB = 0;
                            PIN_A1 = 0;
                            PIN_A0 = 0;
                            break;
                        case 'C':
                            PIN_SWA = 1;
                            PIN_SWB = 0;
                            PIN_A1 = 1;
                            PIN_A0 = 0;
                            break;
                        case 'S':
                            PIN_SWA = 1;
                            PIN_SWB = 0;
                            PIN_A1 = 1;
                            PIN_A0 = 1;
                            break;
                        case '1':
                            PIN_SWA = 0;
                            PIN_SWB = 1;
                            PIN_A1 = 0;
                            PIN_A0 = 1;
                            break;
                        case '2':
                            PIN_SWA = 0;
                            PIN_SWB = 1;
                            PIN_A1 = 1;
                            PIN_A0 = 1;
                            break;
                        case '3':
                            PIN_SWA = 1;
                            PIN_SWB = 0;
                            PIN_A1 = 0;
                            PIN_A0 = 1;
                            break;
                        case 'G':
                            PIN_SWA = 1;
                            PIN_SWB = 1;
                            break;
                        default:
                            NOP();
                    }
                    TransmitDAT_USART(ID);
                }
                //-----------------------------------------
                break;
            case '4':// Simple conversion
                TransmitCMD_USART('4');
                //-----------------------------------------
                PIN_CS = 0;
                TransferByte_SPI(0x01); //Write ADC Mode Register
                TransferByte_SPI(0x00); //Simple Conversión
                TransferByte_SPI(0x1C); //REF_EN desahbilitado, Cristal externo
                //-----------------------------------------
                //Enabling TMR1
                conta1 = 0;
                TMR1H = 0;
                TMR1L = 0;
                T1CON = 0xB1;           //TIMER1 mode 16bits, prescaler x8, Start
                PIR1bits.TMR1IF = 0;    //Clearing flag
                PIE1bits.TMR1IE = 1;    //Enable Interrupt TMR1
                //-----------------------------------------
                Data_X = ReadADC32();
                PIN_CS = 1;
                T1CONbits.TMR1ON = 0;   //TIMER1 Stoped
                PIE1bits.TMR1IE = 0;    //Disable Interrupt TMR1
                BufferDAT[0] = TMR1L;
                BufferDAT[1] = TMR1H;
                BufferDAT[2] = conta1;
                TransmitDAT_USART('4');
                __delay_ms(500);
                //printf("Valor:%+011ld",Data_X);
                //-----------------------
                print_value(Data_X);
                //-----------------------
                break;
            case '5':// Continuos conversion:
                TransmitCMD_USART('5');
                SumData_X = 0;
                conta1 = 0;
                TMR1H = 0;
                TMR1L = 0;
                T1CON = 0xB1;           //TIMER1 mode 16bits, prescaler x8, Start
                PIR1bits.TMR1IF = 0;    //Clearing flag
                PIE1bits.TMR1IE = 1;    //Enable Interrupt TMR1
                PIN_CS = 0;
                for(conta=0; conta<10; conta++){  
                    Data_X = ReadADC32();
                    SumData_X = (signed long long) SumData_X + Data_X;
                }
                PIN_CS = 1;
                Data_X = (signed long long) SumData_X/10;
                T1CONbits.TMR1ON = 0;   //TIMER1 Stoped
                PIE1bits.TMR1IE = 0;    //Disable Interrupt TMR1
                BufferDAT[0] = TMR1L;
                BufferDAT[1] = TMR1H;
                BufferDAT[2] = conta1;
                TransmitDAT_USART('5');
                __delay_ms(500);
                //printf("Valor:%+011ld",Data_X);
                //-----------------------
                print_value(Data_X);
                //-----------------------
                break;
            case '6':// Read Status Register
                TransmitCMD_USART('6');
                ReadRegister_1bytesAD7177(0x40);
                TransmitDAT_USART('6');
                break;
            case '7':// Read 5 canales:
                TransmitCMD_USART('7');
                conta1 = 0;
                TMR1H = 0;
                TMR1L = 0;
                T1CON = 0xB1;           //TIMER1 mode 16bits, prescaler x8, Start
                PIR1bits.TMR1IF = 0;    //Clearing flag
                PIE1bits.TMR1IE = 1;    //Enable Interrupt TMR1
                //------------------------------------> Start 
                LED_RED = 1;
                SumData_X = 0;
                SumData_Y = 0;
                SumData_Z = 0;
                SumData_TC = 0;
                SumData_TS = 0;
                PIN_CS = 0;
                //------------------------------------------------------
                //Lectura canalX:
                PIN_SWA = 0;
                PIN_SWB = 1;
                PIN_A1 = 0;
                PIN_A0 = 0;
                __delay_ms(5);
                LED_GREEN = 1;
                Data_X = ReadADC32();   //CleanBufferADC
                for(conta=0; conta<Nsamples; conta++){  
                    Data_X = ReadADC32();
                    SumData_X = (signed long long) SumData_X + Data_X;
                }
                LED_GREEN = 0;
                //------------------------------------------------------
                //Lectura canalY:
                PIN_SWA = 0;
                PIN_SWB = 1;
                PIN_A1 = 1;
                PIN_A0 = 0;
                __delay_ms(5);
                LED_GREEN = 1;
                Data_Y = ReadADC32();   //CleanBufferADC
                for(conta=0; conta<Nsamples; conta++){  
                    Data_Y = ReadADC32();
                    SumData_Y = (signed long long) SumData_Y + Data_Y;
                }
                LED_GREEN = 0;
                //------------------------------------------------------
                //Lectura canalZ:
                PIN_SWA = 1;
                PIN_SWB = 0;
                PIN_A1 = 0;
                PIN_A0 = 0;
                __delay_ms(5);
                LED_GREEN = 1;
                Data_Z = ReadADC32();   //CleanBufferADC
                for(conta=0; conta<Nsamples; conta++){  
                    Data_Z = ReadADC32();
                    SumData_Z = (signed long long) SumData_Z + Data_Z;
                }
                LED_GREEN = 0;
                //------------------------------------------------------
                //Lectura canalTC:
                PIN_SWA = 1;
                PIN_SWB = 0;
                PIN_A1 = 1;
                PIN_A0 = 0;
                __delay_ms(5);
                LED_GREEN = 1;
                Data_TC = ReadADC32();   //CleanBufferADC
                for(conta=0; conta<Nsamples; conta++){  
                    Data_TC = ReadADC32();
                    SumData_TC = (signed long long) SumData_TC + Data_TC;
                }
                LED_GREEN = 0;
                //------------------------------------------------------
                //Lectura canalTS:
                PIN_SWA = 1;
                PIN_SWB = 0;
                PIN_A1 = 1;
                PIN_A0 = 1;
                __delay_ms(5);
                LED_GREEN = 1;
                Data_TS = ReadADC32();   //CleanBufferADC
                for(conta=0; conta<Nsamples; conta++){  
                    Data_TS = ReadADC32();
                    SumData_TS = (signed long long) SumData_TS + Data_TS;
                }
                LED_GREEN = 0;
                //------------------------------------------------------
                PIN_CS = 1;
                //------------------------------------------------------
                //Averaging datos
                LED_GREEN = 1;
                Data_X = (signed long long) SumData_X/Nsamples;
                Data_Y = (signed long long) SumData_Y/Nsamples;
                Data_Z = (signed long long) SumData_Z/Nsamples;
                Data_TC = (signed long long) SumData_TC/Nsamples;
                Data_TS = (signed long long) SumData_TS/Nsamples;
                LED_GREEN = 0;
                //-----------------------
                //printf("JROADQ09,%+011ld,%+011ld,%+011ld,%+011ld,%+011ld\n\r",Data_X, Data_Y, Data_Z, Data_TC, Data_TS);
                //-----------------------
                print_string("JROADQ09,");
                print_value(Data_X);
                print_value(Data_Y);
                print_value(Data_Z);
                print_value(Data_TC);
                print_value(Data_TS);
                print_string(",\n\r");
                LED_RED = 0;
                //-----------------------
                T1CONbits.TMR1ON = 0;   //TIMER1 Stoped
                PIE1bits.TMR1IE = 0;    //Disable Interrupt TMR1
                BufferDAT[0] = TMR1L;
                BufferDAT[1] = TMR1H;
                BufferDAT[2] = conta1;
                __delay_ms(500);
                TransmitDAT_USART('5');
                break;
            case '8':// Read Time from GPS:
                TransmitCMD_USART('8');
                //--------------------
                // Enabling EXT INT
                INTCON2bits.INTEDG0 = 0;        //Falling Edge INT0
                INTCONbits.INT0IF = 0;          //Clear lag EXTINT0
                INTCONbits.INT0IE = 1;          //Enable EXTINT0
                //--------------------
                PIE1bits.TMR1IE = 0;            //Disable Interrupt TMR1
            default:
                NOP();
        }
    }
    CloseSerialPort();
    ClosePort_SPI();
    return;
}

signed long ReadADC32(void){
    unsigned char byte3, byte2, byte1, byte0;
    unsigned long data_ini;
    signed long DataADC;
    
    while(PIN_DIN);     //Wait to '0'
    TransferByte_SPI(0x44); //Write ADC Mode Register
    byte3 = TransferByte_SPI(0x00);   //Bytes
    byte2 = TransferByte_SPI(0x00);   //Bytes
    byte1 = TransferByte_SPI(0x00);   //Bytes
    byte0 = TransferByte_SPI(0x00);   //Bytes
    
    //Convert data:
    DataADC = 0;
    data_ini = 0;
    data_ini = data_ini | (unsigned long)byte3<<24;
    data_ini = data_ini | (unsigned long)byte2<<16;
    data_ini = data_ini | (unsigned long)byte1<<8;
    data_ini = data_ini | (unsigned long)byte0;
    data_ini = data_ini ^ 0x80000000;
    DataADC = (signed long) data_ini;
    
    return DataADC;
}
