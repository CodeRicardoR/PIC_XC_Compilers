/*
 * File:   Digitizer16bitsV2.c
 * Author: ricar
 *
 * Codigo fuente para control del digitalizador de 16bits +/-10V con PIC18F258
 * 
 * Created on 6 de julio de 2023, 08:50 AM
 */


#include "main.h"

//Rutine interrupt

void __interrupt() Interrups(void){
    unsigned char ID, conta;
    
    if ((INTCONbits.INT0IF == 1) && (INTCONbits.INT0IE == 1)){
        INTCONbits.INT0IF = 0;          // Clear bit Flag
        INTCONbits.INT0IE = 0;          // INT_EXT Disabled
        
        switch(CmdGPS){
            case 0:
                if(ControlSample == 1){
                    //----------------------------
                    PIN_LED = 1;
                    //----------------------------
                    //Setting TMR0 to T = 200 e-3
                    TMR0H = 0x10;
                    TMR0L = 0x00;
                    T0CON = 0b10000100;     //TMR0: Start,16bits,32 prescaler, INTCLK
                    INTCONbits.TMR0IF = 0;  //Clearing flag bit
                    INTCONbits.TMR0IE = 1;  //Enable TMR0 INT
                    //----------------------------
                    //Initialization values:
                    ContaSample = 0;
                    //----------------------------
                    //Read DATE/TIME from GPS:
                    //GPS_TIMEDATE();
                    //Sending JRODATE
                    print_string("G-JRODATE");
                    print_string(BufferDAT);
                    print_string("\n\r");  
                    //----------------------------
                    //Data Conversion and sending
                    LATB = ChannelX;
                    PIN_TEST1 = 1;
                    __delay_ms(delay_channel);
                    //----------------------------
                    SumData_X = 0;
                    Data_X = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_X = ReadDataADC();
                        SumData_X = SumData_X + Data_X;
                    }
                    PIN_TEST1 = 0;
                    //----------------------------
                    LATB = ChannelY;
                    __delay_ms(delay_channel);
                    //---------------------------
                    SumData_Y = 0;
                    Data_Y = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_Y = ReadDataADC();
                        SumData_Y = SumData_Y + Data_Y;
                    }
                    //---------------------------
                    LATB = ChannelZ;
                    __delay_ms(delay_channel);
                    //---------------------------
                    SumData_Z = 0;
                    Data_Z = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_Z = ReadDataADC();
                        SumData_Z = SumData_Z + Data_Z;
                    }
                    //---------------------------
                    LATB = ChannelTC;
                    __delay_ms(delay_channel);
                    //---------------------------
                    SumData_TC = 0;
                    Data_TC = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_TC = ReadDataADC();
                        SumData_TC = SumData_TC + Data_TC;
                    }
                    //---------------------------
                    LATB = ChannelTS;
                    __delay_ms(delay_channel);
                    //----------------------------
                    SumData_TS = 0;
                    Data_TS = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_TS = ReadDataADC();
                        SumData_TS = SumData_TS + Data_TS;
                    }
                    //---------------------------
                    LATB = ChannelAUX0;
                    __delay_ms(delay_channel);
                    //----------------------------
                    SumData_AUX0 = 0;
                    Data_AUX0 = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_AUX0 = ReadDataADC();
                        SumData_AUX0 = SumData_AUX0 + Data_AUX0;
                    }
                    //---------------------------
                    LATB = ChannelAUX1;
                    __delay_ms(delay_channel);
                    //----------------------------
                    SumData_AUX1 = 0;
                    Data_AUX1 = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_AUX1 = ReadDataADC();
                        SumData_AUX1 = SumData_AUX1 + Data_AUX1;
                    }
                    //---------------------------
                    LATB = ChannelAUX2;
                    PIN_TEST1 = 1;
                    __delay_ms(delay_channel);
                    //----------------------------
                    SumData_AUX2 = 0;
                    Data_AUX2 = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_AUX2 = ReadDataADC();
                        SumData_AUX2 = SumData_AUX2 + Data_AUX2;
                    }
                    PIN_TEST1 = 0;
                    //----------------------------
                    //Averaging data
                    Data_X = (signed short) (SumData_X/n_sample);
                    Data_Y = (signed short) (SumData_Y/n_sample);
                    Data_Z = (signed short) (SumData_Z/n_sample);
                    Data_TC = (signed short) (SumData_TC/n_sample);
                    Data_TS = (signed short) (SumData_TS/n_sample);
                    Data_AUX0 = (signed short) (SumData_AUX0/n_sample);
                    Data_AUX1 = (signed short) (SumData_AUX1/n_sample);
                    Data_AUX2 = (signed short) (SumData_AUX2/n_sample);
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
                    //----------------------------
                }
                break;
            case 1:     // Read Latitude GPS
                //GPS_LATITUDE();
                TransmitDAT_USART('1');
                break;
            case 2:     // Read Longitude GPS
                //GPS_LONGITUDE();
                TransmitDAT_USART('2');
                break;
            case 3:
                //GPS_STATUS();
                //if(RxBuffer=='A'){
                //    TransmitDAT_USART('8');
                //}else{
                //    TransmitDAT_USART('0');
                //}
                NOP();
                break;
            default:
                NOP();
        }
        
    }else if((PIE1bits.RC1IE == 1) && (PIR1bits.RC1IF == 1)){
        ID = ReceiveCMD_USART();
        switch(ID){
            case '5':
                TransmitCMD_USART('5');
                ControlSample = 0;
                break;
            case '0':
                TransmitCMD_USART('0');
                BufferDAT[0] = 'V';
                BufferDAT[1] = '2';
                BufferDAT[2] = '0';
                BufferDAT[12] = '\0';
                TransmitDAT_USART('0');
                break;
            case '1':
                TransmitCMD_USART('1');
                //---------------------
                //Latitude GPS
                CmdGPS = 1;
                INTCON2bits.INTEDG0 = 0;    // falling edge to interrupt INT_EXT
                INTCONbits.INT0IF = 0;      // Clear bit Flag
                INTCONbits.INT0IE = 1;      // Enable INT0 interrupt
                //---------------------
                break;
            case '2':
                TransmitCMD_USART('2');
                //---------------------
                //Longitude GPS
                CmdGPS = 2;
                INTCON2bits.INTEDG0 = 0;    // falling edge to interrupt INT_EXT
                INTCONbits.INT0IF = 0;      // Clear bit Flag
                INTCONbits.INT0IE = 1;      // Enable INT0 interrupt
                //---------------------
                break;
            case '3':
                TransmitCMD_USART('3');
                //---------------------
                //---------------------
                break;
            case '4':       // Enable continuous acquisition
                TransmitCMD_USART('4');
                //---------------------
                CmdGPS = 0;
                ControlSample = 1;
                //---------------------
                
                //---------------------
                INTCON2bits.INTEDG0 = 0;    // falling edge to interrupt INT_EXT
                INTCONbits.INT0IF = 0;      // Clear bit Flag
                INTCONbits.INT0IE = 1;      // Enable INT0 interrupt
                //---------------------
                break;
            case '6':
                TransmitCMD_USART('6');
                break;
            case '7':
                TransmitCMD_USART('7');
                break;
            case '8':
                TransmitCMD_USART('8');
                //---------------------
                //Sync GPS
                CmdGPS = 3;
                INTCON2bits.INTEDG0 = 0;    // falling edge to interrupt INT_EXT
                INTCONbits.INT0IF = 0;      // Clear bit Flag
                INTCONbits.INT0IE = 1;      // Enable INT0 interrupt
                //---------------------
                break;
            case '9':
                //----------------------------
                PIN_LED = 1;
                //----------------------------
                //Setting TMR0 to T = 200 e-3
                TMR0H = 0x10;
                TMR0L = 0x00;
                T0CON = 0b10000100;     //TMR0: Start,16bits,32 prescaler, INTCLK
                INTCONbits.TMR0IF = 0;  //Clearing flag bit
                INTCONbits.TMR0IE = 1;  //Enable TMR0 INT
                //----------------------------
                TransmitCMD_USART('9');
                //----------------------------
                //Initialization values:
                ContaSample = 0;
                ControlSample = 0;
                //----------------------------
                //Sending JRODATE
                print_string("G-JRODATEXXXXXXXXXXXX\n\r");
                //----------------------------
                //Data Conversion and sending
                //----------------------------
                LATB = ChannelX;
                PIN_TEST1 = 1;
                __delay_ms(delay_channel);
                SumData_X = 0;
                Data_X = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                   Data_X = ReadDataADC();
                   SumData_X = SumData_X + Data_X;
                }
                PIN_TEST1 = 0;
                //----------------------------
                LATB = ChannelY;
                __delay_ms(delay_channel);
                //----------------------------
                SumData_Y = 0;
                Data_Y = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                    Data_Y = ReadDataADC();
                    SumData_Y = SumData_Y + Data_Y;
                }
                //----------------------------
                LATB = ChannelZ;
                __delay_ms(delay_channel);
                //----------------------------
                SumData_Z = 0;
                Data_Z = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                    Data_Z = ReadDataADC();
                    SumData_Z = SumData_Z + Data_Z;
                }
                //----------------------------
                LATB = ChannelTC;
                __delay_ms(delay_channel);
                //----------------------------
                SumData_TC = 0;
                Data_TC = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                    Data_TC = ReadDataADC();
                    SumData_TC = SumData_TC + Data_TC;
                }
                //----------------------------
                LATB = ChannelTS;
                __delay_ms(delay_channel);
                //----------------------------
                SumData_TS = 0;
                Data_TS = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                    Data_TS = ReadDataADC();
                    SumData_TS = SumData_TS + Data_TS;
                }
                //----------------------------
                LATB = ChannelAUX0;
                __delay_ms(delay_channel);
                //----------------------------
                SumData_AUX0 = 0;
                Data_AUX0 = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                    Data_AUX0 = ReadDataADC();
                    SumData_AUX0 = SumData_AUX0 + Data_AUX0;
                }
                //----------------------------
                LATB = ChannelAUX1;
                __delay_ms(delay_channel);
                //----------------------------
                SumData_AUX1 = 0;
                Data_AUX1 = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                    Data_AUX1 = ReadDataADC();
                    SumData_AUX1 = SumData_AUX1 + Data_AUX1;
                }
                //----------------------------
                LATB = ChannelAUX2;
                PIN_TEST1 = 1;
                __delay_ms(delay_channel);
                //----------------------------
                SumData_AUX2 = 0;
                Data_AUX2 = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                    Data_AUX2 = ReadDataADC();
                    SumData_AUX2 = SumData_AUX2 + Data_AUX2;
                }
                PIN_TEST1 = 0;
                //----------------------------
                //Averaging data
                Data_X = (signed short) (SumData_X/n_sample);
                Data_Y = (signed short) (SumData_Y/n_sample);
                Data_Z = (signed short) (SumData_Z/n_sample);
                Data_TC = (signed short) (SumData_TC/n_sample);
                Data_TS = (signed short) (SumData_TS/n_sample);
                Data_AUX0 = (signed short) (SumData_AUX0/n_sample);
                Data_AUX1 = (signed short) (SumData_AUX1/n_sample);
                Data_AUX2 = (signed short) (SumData_AUX2/n_sample);
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
                //-----------------------------
                PIN_LED = 0;
                //-----------------------------
                break;
            default:
                TransmitCMD_USART('X');
                RCSTAbits.CREN = 1;     //Enable continuous reception.
        } 
    }else if((INTCONbits.TMR0IE == 1) && (INTCONbits.TMR0IF == 1)){
        PIN_LED = 1;
        //----------------------------
        INTCONbits.TMR0IF = 0;  //Clearing flag bit
        TMR0H = 0x10;
        TMR0L = 0x00; 
        ContaSample++;
        //-----------------------------------------------------
        ////Data Conversion and sending
        //----------------------------
        LATB = ChannelX;
        PIN_TEST1 = 1;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_X = 0;
        Data_X = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_X = ReadDataADC();
            SumData_X = SumData_X + Data_X;
        }
        PIN_TEST1 = 0;
        //----------------------------
        LATB = ChannelY;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_Y = 0;
        Data_Y = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_Y = ReadDataADC();
            SumData_Y = SumData_Y + Data_Y;
        }
        //----------------------------
        LATB = ChannelZ;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_Z = 0;
        Data_Z = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_Z = ReadDataADC();
            SumData_Z = SumData_Z + Data_Z;
        }
        //----------------------------
        LATB = ChannelTC;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_TC = 0;
        Data_TC = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_TC = ReadDataADC();
            SumData_TC = SumData_TC + Data_TC;
        }
        //----------------------------
        LATB = ChannelTS;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_TS = 0;
        Data_TS = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_TS = ReadDataADC();
            SumData_TS = SumData_TS + Data_TS;
        }
        //----------------------------
        LATB = ChannelAUX0;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_AUX0 = 0;
        Data_AUX0 = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_AUX0 = ReadDataADC();
            SumData_AUX0 = SumData_AUX0 + Data_AUX0;
        }
        //----------------------------
        LATB = ChannelAUX1;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_AUX1 = 0;
        Data_AUX1 = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_AUX1 = ReadDataADC();
            SumData_AUX1 = SumData_AUX1 + Data_AUX1;
        }
        //----------------------------
        LATB = ChannelAUX2;
        PIN_TEST1 = 1;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_AUX2 = 0;
        Data_AUX2 = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_AUX2 = ReadDataADC();
            SumData_AUX2 = SumData_AUX2 + Data_AUX2;
        }
        PIN_TEST1 = 0;
        //----------------------------
        //Averaging data
        Data_X = (signed short) (SumData_X/n_sample);
        Data_Y = (signed short) (SumData_Y/n_sample);
        Data_Z = (signed short) (SumData_Z/n_sample);
        Data_TC = (signed short) (SumData_TC/n_sample);
        Data_TS = (signed short) (SumData_TS/n_sample);
        Data_AUX0 = (signed short) (SumData_AUX0/n_sample);
        Data_AUX1 = (signed short) (SumData_AUX1/n_sample);
        Data_AUX2 = (signed short) (SumData_AUX2/n_sample);
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
        //-----------------------------------------------------
        if(ContaSample == 4){
            INTCONbits.TMR0IE = 0;          //INT_TIMER1 Disabled
            if(ControlSample == 1){
                INTCONbits.INT0IF = 0;      // Clear bit Flag
                INTCONbits.INT0IE = 1;      // Enable INT0 interrupt
            }
        }
        //----------------------------
        PIN_LED = 0;
    }
    return;
}


void main(void) {
    
    //-----------------------------------------------
    //Config IO PORTS
    ADCON1bits.PCFG = 0x07;     //Config PORTA DIGITAL
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    LATCbits.LC6 = 1;           //RC6/TX for IDLE in high
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;
    
    TRISBbits.TRISB0 = 1;       //Input for INT0
    INTCON2bits.NOT_RBPU = 0;   //Pull-up RB0 is enabled
    TRISBbits.TRISB4 = 1;       //Input for PIN_READY
    TRISCbits.TRISC5 = 1;       //Input for PIN_DATA
    TRISCbits.TRISC3 = 1;       //Input for PIN_RXGPS
    //*******************************************************

    //Init pins ADC
    PIN_CS = 1;
    PIN_RC = 1;
    PIN_CLK = 0;
    __delay_ms(100);
    
    //-----------------------------------------------
    //Config Serial USART
    ConfigSerialPort();
    OpenSerialPort();
    //-----------------------------------------------
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
    PIN_TEST1 = 0;
    PIN_TEST2 = 0;
    //-----------------------------------------------
    
    while(1);
    CloseSerialPort();
    return;
}
