/*
 * File:   MagnetDigi24.c
 * Author: ricar
 *
 * Created on 14 de abril de 2023, 01:52 PM
 */

#define _XTAL_FREQ 39321600UL
#include <xc.h>
#include "BitsConfig.h"
#include "Lib258_serial.h"
#include "Lib258_spi.h"


//-------------------------------------------------
//To select channel ADC
//------------------------------------------------
//|(NON)|(NON)|(A0) |(A1) |(SWB)|(SWA)|(NON)|(GPS)|
//| RB7 | RB6 | RB5 | RB4 | RB3 | RB2 | RB1 | RB0 |
//-------------------------------------------------
#define ChannelX    0x38
#define ChannelY    0x18
#define ChannelZ    0x28
#define ChannelTC   0x08
#define ChannelTS   0x04
#define ChannelAUX0 0x34
#define ChannelAUX1 0x14
#define ChannelAUX2 0x24
#define ChannelGND  0x0C

//Numbers
//-------------------------------------------------
#define delay_channel   7       //Delay to adquisition
#define n_sample        5       //Samples to average
//-------------------------------------------------

//Pines:
//-------------------------------------------------
#define PIN_LED         LATAbits.LA2        //output
#define PIN_TEST        LATBbits.LB7        //output
//-------------------------------------------------


//-------------------------------------------------
//Global variables:
unsigned char CmdGPS = 0;
unsigned char ControlSample = 0;
unsigned char ContaSample = 0;
unsigned long SumData_X, SumData_Y, SumData_Z, SumData_TC, SumData_TS, SumData_AUX0, SumData_AUX1, SumData_AUX2, SumData_GND;
unsigned long Data_X, Data_Y, Data_Z, Data_TC, Data_TS, Data_G, Data_AUX0, Data_AUX1, Data_AUX2, Data_GND;
//-------------------------------------------------


void __interrupt() Interrups(void){
    unsigned char ID, conta;
    
    if((PIE1bits.RC1IE == 1) && (PIR1bits.RC1IF == 1)){
        ID = ReceiveCMD_USART();
        switch(ID){
            case '5':               // Stop sampling ADC CS5532
                TransmitCMD_USART('5');
                ControlSample = 0;
                break;
                
            case '0':               //Transmit Digitizer Version
                TransmitCMD_USART('0');
                BufferDAT[0] = 'V';
                BufferDAT[1] = '2';
                BufferDAT[2] = '0';
                BufferDAT[12] = '\0';
                TransmitDAT_USART('0');
                break;
                
            case '1':           //Trasmit Latitude GPS
                TransmitCMD_USART('1');
                //---------------------
                //No implementtado
                //---------------------
                break;
                
            case '2':           //Trasmit Longitude GPS
                TransmitCMD_USART('2');
                //---------------------
                //No implementtado
                //---------------------
                break;
                
            case '3':           //Config Digitizer
                TransmitCMD_USART('3');
                //---------------------
                //Setting ADC CS5532
                BufferDAT[0] = ResetCS5532();
                InitCS5532();
                
                __delay_ms(1);
                //Read Offset Register
                ReadRegister_4bytesCS5532(0x09);
                BufferDAT[1] = BufferSPI[0];
                BufferDAT[2] = BufferSPI[1];
                BufferDAT[3] = BufferSPI[2];
                BufferDAT[4] = BufferSPI[3];
                
                __delay_ms(1);
                //Read Gain Register
                ReadRegister_4bytesCS5532(0x0A);
                BufferDAT[5] = BufferSPI[0];
                BufferDAT[6] = BufferSPI[1];
                BufferDAT[7] = BufferSPI[2];
                BufferDAT[8] = BufferSPI[3];
                BufferDAT[12] = '\0';
                TransmitDAT_USART('3');
                //---------------------
                break;
                
            case '4':           // Enable continuous acquisition
                TransmitCMD_USART('4');
                //---------------------
                //No implementado
                //---------------------
                break;
                
            case '6':           //Read Data channel FND
                TransmitCMD_USART('6');
                //---------------------
                PORTB = ChannelGND;
                __delay_ms(delay_channel);
                SumData_GND = 0;
                TransferByte_SPI(0x80);     // Start single convertion ADC CS5532
                ReadAD24(0,0x00);   //Ignore this first data
                for(conta=0; conta<n_sample; conta++){
                    TransferByte_SPI(0x80);
                    Data_GND = ReadAD24(1,0x00);    //Reading valid data
                    SumData_GND = SumData_GND + Data_GND;
                }
                Data_GND = SumData_GND/n_sample;
                print_value(Data_GND);
                //---------------------
                break;
                
            case '7':           //No implementado
                TransmitCMD_USART('7');
                break;
                
            case '8':           //Sync Date/Time GPS
                TransmitCMD_USART('8');
                //---------------------
                //No implementado
                //---------------------
                break;
                
            case '9':           //Read simple adquisition
                //----------------------------
                PIN_TEST = 1;   //Solo para pruebas
                //----------------------------
                PIN_LED = 1;
                //----------------------------
                //Config TMR0 to 200ms:
                TMR0H = 0x10;
                TMR0L = 0x00;
                T0CON = 0b10000100;     //TMR0: Start,16bits,32 prescaler, INTCLK
                //-----------------------------
                INTCONbits.TMR0IF = 0;  //Clearing flag bit
                INTCONbits.TMR0IE = 1;  //Enable TMR0 INT
                INTCONbits.INT0IE = 0;  //Disable EXT INT0
                //Initialization values:
                ContaSample = 0;
                ControlSample = 0;
                TransmitCMD_USART('9');
                //----------------------------
                //Sending JRODATE
                print_string("G-JRODATEXXXXXXXXXXXX\n\r");
                //----------------------------
                //Data Conversion and sending
                TransferByte_SPI(0xC0);     // Start sampling ADC CS5532
                //----------------------------
                CleanHolding();
                //----------------------------
                PORTB = ChannelX;
                __delay_ms(delay_channel);
                SumData_X = 0;
                ReadAD24(0,0x00);   //Ignore this first data
                for(conta=0; conta<n_sample; conta++){
                    Data_X = ReadAD24(1,0x00);    //Reading valid data
                    SumData_X = SumData_X + Data_X;
                }
                //----------------------------
                CleanHolding();
                //----------------------------
                PORTB = ChannelY;
                __delay_ms(delay_channel);
                SumData_Y = 0;
                ReadAD24(0,0x00);   //Ignore this first data
                for(conta=0; conta<n_sample; conta++){
                    Data_Y = ReadAD24(1,0x00);    //Reading valid data
                    SumData_Y = SumData_Y + Data_Y;
                }
                //----------------------------
                CleanHolding();
                //----------------------------
                PORTB = ChannelZ;
                __delay_ms(delay_channel);
                SumData_Z = 0;
                ReadAD24(0,0x00);   //Ignore this first data
                for(conta=0; conta<n_sample; conta++){
                    Data_Z = ReadAD24(1,0x00);    //Reading valid data
                    SumData_Z = SumData_Z + Data_Z;
                }
                //----------------------------
                CleanHolding();
                //----------------------------
                PORTB = ChannelTC;
                __delay_ms(delay_channel);
                SumData_TC = 0;
                ReadAD24(0,0x00);   //Ignore this first data
                for(conta=0; conta<n_sample; conta++){
                    Data_TC = ReadAD24(1,0x00);    //Reading valid data
                    SumData_TC = SumData_TC + Data_TC;
                }
                //----------------------------
                CleanHolding();
                //----------------------------
                PORTB = ChannelTS;
                __delay_ms(delay_channel);
                SumData_TS = 0;
                ReadAD24(0,0x00);   //Ignore this first data
                for(conta=0; conta<n_sample; conta++){
                    Data_TS = ReadAD24(1,0x00);    //Reading valid data
                    SumData_TS = SumData_TS + Data_TS;
                }
                //-----------------------------
                CleanHolding();
                //----------------------------
                PORTB = ChannelAUX0;
                __delay_ms(delay_channel);
                SumData_AUX0 = 0;
                ReadAD24(0,0x00);   //Ignore this first data
                for(conta=0; conta<n_sample; conta++){
                    Data_AUX0 = ReadAD24(1,0x00);    //Reading valid data
                    SumData_AUX0 = SumData_AUX0 + Data_AUX0;
                }
                //-----------------------------
                CleanHolding();
                //----------------------------
                PORTB = ChannelAUX1;
                __delay_ms(delay_channel);
                SumData_AUX1 = 0;
                ReadAD24(0,0x00);   //Ignore this first data
                for(conta=0; conta<n_sample; conta++){
                    Data_AUX1 = ReadAD24(1,0x00);    //Reading valid data
                    SumData_AUX1 = SumData_AUX1 + Data_AUX1;
                }
                //-----------------------------
                CleanHolding();
                //----------------------------
                PORTB = ChannelAUX2;
                __delay_ms(delay_channel);
                SumData_AUX2 = 0;
                ReadAD24(0,0x00);   //Ignore this first data
                for(conta=0; conta<n_sample; conta++){
                    Data_AUX2 = ReadAD24(1,0x00);    //Reading valid data
                    SumData_AUX2 = SumData_AUX2 + Data_AUX2;
                }
                //-----------------------------
                //Averaging data
                Data_X = SumData_X/n_sample;
                Data_Y = SumData_Y/n_sample;
                Data_Z = SumData_Z/n_sample;
                Data_TC = SumData_TC/n_sample;
                Data_TS = SumData_TS/n_sample;
                Data_AUX0 = SumData_AUX0/n_sample;
                Data_AUX1 = SumData_AUX1/n_sample;
                Data_AUX2 = SumData_AUX2/n_sample;
                //----------------------------
                //Sending data
                print_string("JROADQ");
                WriteByte_USART(ContaSample);
                print_value(Data_X);
                print_value(Data_Y);
                print_value(Data_Z);
                print_value(Data_TC);
                print_value(Data_TS);
                print_value(Data_AUX0);
                print_value(Data_AUX1);
                print_value(Data_AUX2);
                print_string("\n\r");
                //----------------------------
                PIN_LED = 0;
                break;
            default:
                TransmitCMD_USART('X');
                RCSTAbits.CREN = 1;     //Enable continuous reception.
        }   
    }else if((INTCONbits.TMR0IE == 1) && (INTCONbits.TMR0IF == 1)){
        //----------------------------
        PIN_LED = 1;
        //----------------------------
        INTCONbits.TMR0IF = 0;      //Clean Flag TMR0 INT
        TMR0H = 0x10;
        TMR0L = 0x00;
        ContaSample++;
        //-----------------------------------------------------
        ////Data Conversion and sending
        CleanHolding();
        //----------------------------
        PORTB = ChannelX;
        __delay_ms(delay_channel);
        SumData_X = 0;
        ReadAD24(0,0x00);   //Ignore this first data
        for(conta=0; conta<n_sample; conta++){
            Data_X = ReadAD24(1,0x00);    //Reading valid data
            SumData_X = SumData_X + Data_X;
        }
        //----------------------------
        CleanHolding();
        //----------------------------
        PORTB = ChannelY;
        __delay_ms(delay_channel);
        SumData_Y = 0;
        ReadAD24(0,0x00);   //Ignore this first data
        for(conta=0; conta<n_sample; conta++){
            Data_Y = ReadAD24(1,0x00);    //Reading valid data
            SumData_Y = SumData_Y + Data_Y;
        }
        //----------------------------
        CleanHolding();
        //----------------------------
        PORTB = ChannelZ;
        __delay_ms(delay_channel);
        SumData_Z = 0;
        ReadAD24(0,0x00);   //Ignore this first data
        for(conta=0; conta<n_sample; conta++){
            Data_Z = ReadAD24(1,0x00);    //Reading valid data
            SumData_Z = SumData_Z + Data_Z;
        }
        //----------------------------
        CleanHolding();
        //----------------------------
        PORTB = ChannelTC;
        __delay_ms(delay_channel);
        SumData_TC = 0;
        ReadAD24(0,0x00);   //Ignore this first data
        for(conta=0; conta<n_sample; conta++){
            Data_TC = ReadAD24(1,0x00);    //Reading valid data
            SumData_TC = SumData_TC + Data_TC;
        }
        //----------------------------
        CleanHolding();
        //----------------------------
        PORTB = ChannelTS;
        __delay_ms(delay_channel);
        SumData_TS = 0;
        ReadAD24(0,0x00);   //Ignore this first data
        for(conta=0; conta<n_sample; conta++){
            Data_TS = ReadAD24(1,0x00);    //Reading valid data
            SumData_TS = SumData_TS + Data_TS;
        }
        //----------------------------
        CleanHolding();
        //----------------------------
        PORTB = ChannelAUX0;
        __delay_ms(delay_channel);
        SumData_AUX0 = 0;
        ReadAD24(0,0x00);   //Ignore this first data
        for(conta=0; conta<n_sample; conta++){
            Data_AUX0 = ReadAD24(1,0x00);    //Reading valid data
            SumData_AUX0 = SumData_AUX0 + Data_AUX0;
        }
        //----------------------------
        CleanHolding();
        //----------------------------
        PORTB = ChannelAUX1;
        __delay_ms(delay_channel);
        SumData_AUX1 = 0;
        ReadAD24(0,0x00);   //Ignore this first data
        for(conta=0; conta<n_sample; conta++){
            Data_AUX1 = ReadAD24(1,0x00);    //Reading valid data
            SumData_AUX1 = SumData_AUX1 + Data_AUX1;
        }
        //----------------------------
        CleanHolding();
        //----------------------------
        PORTB = ChannelAUX2;
        __delay_ms(delay_channel);
        SumData_AUX2 = 0;
        ReadAD24(0,0x00);   //Ignore this first data
        for(conta=0; conta<n_sample; conta++){
            Data_AUX2 = ReadAD24(1,0x00);    //Reading valid data
            SumData_AUX2 = SumData_AUX2 + Data_AUX2;
        }
        //----------------------------
        //Averaging data
        Data_X = SumData_X/n_sample;
        Data_Y = SumData_Y/n_sample;
        Data_Z = SumData_Z/n_sample;
        Data_TC = SumData_TC/n_sample;
        Data_TS = SumData_TS/n_sample;
        Data_AUX0 = SumData_AUX0/n_sample;
        Data_AUX1 = SumData_AUX1/n_sample;
        Data_AUX2 = SumData_AUX2/n_sample;
        //----------------------------
        //Sending data
        print_string("JROADQ");
        WriteByte_USART(ContaSample);
        print_value(Data_X);
        print_value(Data_Y);
        print_value(Data_Z);
        print_value(Data_TC);
        print_value(Data_TS);
        print_value(Data_AUX0);
        print_value(Data_AUX1);
        print_value(Data_AUX2);
        print_string("\n\r");
        //----------------------------
        PIN_LED = 0;
        //-----------------------------------------------------
        if(ContaSample == 4){
            INTCONbits.TMR0IE = 0;      //INT TMR0 Disabled
            PIN_TEST = 0;               //Solo para pruebas
            if(ControlSample == 1){
                INTCONbits.INT0IF = 0;  //Clean Flag EXT INT0
                INTCONbits.INT0IE = 1;  //Enable interrupt EXT INT0 
            }else{
                ReadAD24(0,0xFF);       //Stop sampling ADC CS5532
            }
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
    TRISA = 0x01;               // RA0 Input GPS-RX
    TRISB = 0x01;               // RB0 Input GPS-RX edge
    TRISC = 0x00;               // Default All output
    __delay_ms(100);
    
    //-----------------------------------------------
    //Config Serial USART
    ConfigSerialPort();
    OpenSerialPort();
    
    //-----------------------------------------------
    //Config SPI bus
    ConfigPort_SPI();
    OpenPort_SPI();

    //-----------------------------------------------
    //Config Interrupts
    RCONbits.IPEN = 0;      //Interrupt in mode 16F compatibility
    INTCON = 0x00;          //Disable others interrupta and clear Flags
    //-----------------------------------------------
    //Setting Interrupt:
    INTCONbits.PEIE = 1;    //Peripheral interrupts  enabled
    INTCONbits.GIE = 1;     //Global interrupt enabled
    //-----------------------------------------------
    //Initialize global variables:
    PIN_LED = 0;
    PIN_TEST = 0;
    //-----------------------------------------------
    
    while(1);
    CloseSerialPort();
    ClosePort_SPI();
    
    return;
}