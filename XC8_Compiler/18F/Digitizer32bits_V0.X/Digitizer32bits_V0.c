/*
 * File:   Digitizer32bits_V0.c
 * Author: ricar
 *
 * Created on 18 de octubre de 2022, 02:07 PM
 */


#include "main.h"

//Number of Samples per second
#define Ssamples 10

//Number of Samples per channel X,Y,Z
#define Nsamples 10


//Global Variables:
unsigned char ContaSample, ModeSample;

signed long Data_X, Data_Y, Data_Z, Data_TC, Data_TS;
signed long long SumData_X, SumData_Y, SumData_Z, SumData_TC, SumData_TS;

//Definitions of functions
signed long ReadADC32(void);

//Interrupt Function
void __interrupt() Interrups(void){
    char ID;
    unsigned char conta;
    
    if((PIE1bits.RC1IE == 1) && (PIR1bits.RC1IF == 1)){
        ID = ReceiveCMD_USART();
        switch(ID){
            case '0':
                //--------------------------
                // Config Registers
                ResetAD7177();
                __delay_ms(5);
                BufferSPI[0] = 0x01;
                BufferSPI[1] = 0x00;
                BufferSPI[2] = 0x2C;
                WriteRegister_2bytesAD7177();     //ADCMODE: Standby
                __delay_ms(5);
                BufferSPI[0] = 0x20;
                BufferSPI[1] = 0x1F;
                BufferSPI[2] = 0x00;
                WriteRegister_2bytesAD7177();     //SETUPCON0: All buffers ON, VREF External
                __delay_ms(5);
                BufferSPI[0] = 0x02;
                BufferSPI[1] = 0x00;
                BufferSPI[2] = 0x02;
                WriteRegister_2bytesAD7177();     //IFMODE: 32 bits
                __delay_ms(5);
                BufferSPI[0] = 0x28;
                BufferSPI[1] = 0x05;
                BufferSPI[2] = 0x07;
                WriteRegister_2bytesAD7177();     //FILTCON0: 10000Sps
                //--------------------------
                TransmitCMD_USART('0');
                break;
            case '1':   // Read Latitud GPS
                TransmitCMD_USART('1');
                //--------------------
                //--------------------
                break;
            case '2':   // Read Longitude GPS
                TransmitCMD_USART('2');
                //--------------------
                //--------------------
                break;
            case '3':   //Config ADC
                TransmitCMD_USART('3');
                //--------------------------
                //Read registers AD7177-2:
                ReadRegister_2bytesAD7177(0x50);    //CH0
                BufferDAT[0] = BufferSPI[0];
                BufferDAT[1] = BufferSPI[1];
                __delay_ms(1);
                ReadRegister_2bytesAD7177(0x60);    //SETUP0
                BufferDAT[2] = BufferSPI[0];
                BufferDAT[3] = BufferSPI[1];
                __delay_ms(1);
                ReadRegister_2bytesAD7177(0x68);    //FILTER0
                BufferDAT[4] = BufferSPI[0];
                BufferDAT[5] = BufferSPI[1];
                __delay_ms(1);
                ReadRegister_2bytesAD7177(0x42);    //INTERFACE MODE
                BufferDAT[6] = BufferSPI[0];
                BufferDAT[7] = BufferSPI[1];
                __delay_ms(1);
                ReadRegister_2bytesAD7177(0x47);    //ID
                BufferDAT[8] = BufferSPI[0];
                BufferDAT[9] = BufferSPI[1];
                __delay_ms(1);
                ReadRegister_1bytesAD7177(0x40);    //STATUS
                BufferDAT[10] = BufferSPI[0];
                //--------------------------
                // Config ADC in Mode Continous Conversion
                BufferSPI[0] = 0x01;
                BufferSPI[1] = 0x00;
                BufferSPI[2] = 0x0C;
                WriteRegister_2bytesAD7177();     //ADCMODE: Continous conversion
                //--------------------------
                TransmitDAT_USART('3');
                break;
            case '4':
                TransmitCMD_USART('4');
                //--------------------------
                //--------------------------
                break;
            case '5':
                ModeSample = 0;
                TransmitCMD_USART('5');
                break;
            case '6':
                TransmitCMD_USART('6');
                break;
            case '7':
                TransmitCMD_USART('7');
                break;
            case '8':   // Sync Date/Time GPS
                TransmitCMD_USART('8');
                //--------------------
                //--------------------
                break;
            case '9'://Read simple adquisition
                LED_RED = 1;
                //-----------------------------
                //Config TMR0 to 100ms:
                TMR0H = 0x10;
                TMR0L = 0x3D;
                T0CON = 0b10000011;     //TMR0: Start,16bits,16 prescaler, INTCLK
                //-----------------------------
                INTCONbits.TMR0IF = 0;  //Clearing flag bit
                INTCONbits.TMR0IE = 1;  //Enable TMR0 INT
                INTCONbits.INT0IE = 0;  //Disable EXT INT0
                //-----------------------------
                // init values:
                ContaSample = 0;
                ModeSample = 0;
                TransmitCMD_USART('9');
                //Digitalizar y enviar datos:
                //-----------------------------
                print_string("G-JRODATEXXXXXXXXXXXX\n\r");
                SumData_X = 0;
                SumData_Y = 0;
                SumData_Z = 0;
                SumData_TC = 0;
                SumData_TS = 0;        
                //------------------------------------------------------
                PIN_CS = 0;     //BUS SPI Active to ADC in CC
                LED_GREEN = 1;
                //Read channel:X
                SelectChannelX();
                __delay_ms(5);
                Data_X = ReadADC32();   //CleanBufferADC
                for(conta=0; conta<Nsamples; conta++){  
                    Data_X = ReadADC32();
                    SumData_X = (signed long long) SumData_X + Data_X;
                }
                //------------------------------------------------------
                //Read channel:Y
                SelectChannelY();
                __delay_ms(5);
                Data_Y = ReadADC32();   //CleanBufferADC
                for(conta=0; conta<Nsamples; conta++){  
                    Data_Y = ReadADC32();
                    SumData_Y = (signed long long) SumData_Y + Data_Y;
                }
                //------------------------------------------------------
                //Read channel:Z
                SelectChannelZ();
                __delay_ms(5);
                Data_Z = ReadADC32();   //CleanBufferADC
                for(conta=0; conta<Nsamples; conta++){  
                    Data_Z = ReadADC32();
                    SumData_Z = (signed long long) SumData_Z + Data_Z;
                }
                //------------------------------------------------------
                //Read channel:TC
                SelectChannelC();
                __delay_ms(5);
                Data_TC = ReadADC32();   //CleanBufferADC
                for(conta=0; conta<Nsamples; conta++){  
                    Data_TC = ReadADC32();
                    SumData_TC = (signed long long) SumData_TC + Data_TC;
                }
                //------------------------------------------------------
                //Read channel:TS
                SelectChannelS();
                __delay_ms(5);
                Data_TS = ReadADC32();   //CleanBufferADC
                for(conta=0; conta<Nsamples; conta++){  
                    Data_TS = ReadADC32();
                    SumData_TS = (signed long long) SumData_TS + Data_TS;
                } 
                //------------------------------------------------------
                //Averaging datos
                Data_X = (signed long long) SumData_X/Nsamples;
                Data_Y = (signed long long) SumData_Y/Nsamples;
                Data_Z = (signed long long) SumData_Z/Nsamples;
                Data_TC = (signed long long) SumData_TC/Nsamples;
                Data_TS = (signed long long) SumData_TS/Nsamples;
                
                // Transmit JROADQ:
                print_string("JROADQ,");
                WriteByte_USART(ContaSample);
                print_value(Data_X);
                print_value(Data_Y);
                print_value(Data_Z);
                print_value(Data_TC);
                print_value(Data_TS);
                print_string(",\n\r");
                LED_GREEN = 0;
                break;
            default:
                TransmitCMD_USART('X');
                RCSTAbits.CREN = 1;     //Re-enable continuos receive
        }
    }else if((INTCONbits.TMR0IE == 1) && (INTCONbits.TMR0IF == 1)){
        INTCONbits.TMR0IF = 0;      //Clean Flag TMR0 INT
        TMR0H = 0x10;
        TMR0L = 0x3D;
        ContaSample++;
        //-----------------------------------------------
        //Digitalizar y enviar datos:
        SumData_X = 0;
        SumData_Y = 0;
        SumData_Z = 0;
        SumData_TC = 0;
        SumData_TS = 0;
        //------------------------------------------------------
        LED_GREEN = 1;
        //------------------------------------------------------
        //Read channel:X
        SelectChannelX();
        __delay_ms(5);
        Data_X = ReadADC32();   //CleanBufferADC
        for(conta=0; conta<Nsamples; conta++){  
            Data_X = ReadADC32();
            SumData_X = (signed long long) SumData_X + Data_X;
        }
        //------------------------------------------------------
        //Read channel:Y
        SelectChannelY();
        __delay_ms(5);
        Data_Y = ReadADC32();   //CleanBufferADC
        for(conta=0; conta<Nsamples; conta++){  
            Data_Y = ReadADC32();
            SumData_Y = (signed long long) SumData_Y + Data_Y;
        }
        //------------------------------------------------------
        //Read channel:Z
        SelectChannelZ();
        __delay_ms(5);
        Data_Z = ReadADC32();   //CleanBufferADC
        for(conta=0; conta<Nsamples; conta++){  
            Data_Z = ReadADC32();
            SumData_Z = (signed long long) SumData_Z + Data_Z;
        }
        //------------------------------------------------------
        //Read channel:TC
        SelectChannelC();
        __delay_ms(5);
        Data_TC = ReadADC32();   //CleanBufferADC
        for(conta=0; conta<Nsamples; conta++){  
            Data_TC = ReadADC32();
            SumData_TC = (signed long long) SumData_TC + Data_TC;
        }
        //------------------------------------------------------
        //Read channel:TS
        SelectChannelS();
        __delay_ms(5);
        Data_TS = ReadADC32();   //CleanBufferADC
        for(conta=0; conta<Nsamples; conta++){  
            Data_TS = ReadADC32();
            SumData_TS = (signed long long) SumData_TS + Data_TS;
        }
        //------------------------------------------------------
        //Averaging datos
        Data_X = (signed long long) SumData_X/Nsamples;
        Data_Y = (signed long long) SumData_Y/Nsamples;
        Data_Z = (signed long long) SumData_Z/Nsamples;
        Data_TC = (signed long long) SumData_TC/Nsamples;
        Data_TS = (signed long long) SumData_TS/Nsamples;
        //-----------------------------------------------
        // Transmit JROADQ:
        print_string("JROADQ,");
        WriteByte_USART(ContaSample);
        print_value(Data_X);
        print_value(Data_Y);
        print_value(Data_Z);
        print_value(Data_TC);
        print_value(Data_TS);
        print_string(",\n\r");
        LED_GREEN = 0;
        //-----------------------------------------------
        if(ContaSample > (Ssamples - 2)){
            INTCONbits.TMR0IE = 0;  //INT TMR0 Disabled
            if(ModeSample == 1){
                INTCONbits.INT0IF = 0;  //Clean Flag EXT INT0
                INTCONbits.INT0IE = 1;  //Enable interrupt EXT INT0
            }else{
                PIN_CS = 1;     //BUS SPI disactive to ADC in CC
            }
            LED_RED = 0;
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
    //CONFIG PIN
    TRISBbits.TRISB0 = 1;   //Input tu GPS
    LED_RED = 0;
    LED_GREEN = 0;
    //-----------------------------------------------
    //Init Global Variables
    ContaSample = 0;
    ModeSample = 0;
    
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

//---------------------------------------------------
//Function to read DATA from AD7177-2:
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
