/*
 * File:   Digitizer16bits.c
 * Author: ricar
 *
 * Created on 8 de abril de 2022, 02:57 PM
 */

#include "main.h"

void __interrupt() Interrups(void){
    unsigned char ID, conta;
    
    if((INTCONbits.INTF == 1) && (INTCONbits.INTE == 1)){
        
        INTCONbits.INTF = 0;        // Clear bit Flag
        INTCONbits.INTE = 0;        // INT_EXT Disabled
        
        switch(CmdGPS){
            case 0:
                if(ControlSample == 1){
                    //----------------------------
                    //Setting TMR1 to T = 200 e-3
                    TMR1H = 0x10;
                    TMR1L = 0x06;
                    T1CON = 0x31;           //Pre: x8
                    PIR1bits.TMR1IF = 0;    //Clearing flag bit timer1
                    PIE1bits.TMR1IE = 1;    // Interrupt for TIMER1 is enabled
                    //----------------------------
                    //Initialization values:
                    ContaSample = 0;
                    //----------------------------
                    //Read DATE/TIME from GPS:
                    GPS_TIMEDATE();
                    //Sending JRODATE
                    print_string("G-JRODATE");
                    print_string(BufferDAT);
                    print_string("\n\r");
                    //----------------------------
                    PIN_LED = 1;
                    //----------------------------
                    //Data Conversion and sending
                    PORTB = ChannelX;
                    __delay_ms(delay_channel);
                    //----------------------------
                    SumData_X = 0;
                    Data_X = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_X = ReadDataADC();
                        SumData_X = SumData_X + Data_X;
                    }
                    //----------------------------
                    PORTB = ChannelY;
                    __delay_ms(delay_channel);
                    //---------------------------
                    SumData_Y = 0;
                    Data_Y = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_Y = ReadDataADC();
                        SumData_Y = SumData_Y + Data_Y;
                    }
                    //---------------------------
                    PORTB = ChannelZ;
                    __delay_ms(delay_channel);
                    //---------------------------
                    SumData_Z = 0;
                    Data_Z = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_Z = ReadDataADC();
                        SumData_Z = SumData_Z + Data_Z;
                    }
                    //---------------------------
                    PORTB = ChannelTC;
                    __delay_ms(delay_channel);
                    //---------------------------
                    SumData_TC = 0;
                    Data_TC = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_TC = ReadDataADC();
                        SumData_TC = SumData_TC + Data_TC;
                    }
                    //---------------------------
                    PORTB = ChannelTS;
                    __delay_ms(delay_channel);
                    //----------------------------
                    SumData_TS = 0;
                    Data_TS = ReadDataADC();
                    for(conta = 0; conta<n_sample; conta++){
                        Data_TS = ReadDataADC();
                        SumData_TS = SumData_TS + Data_TS;
                    }
                    //----------------------------
                    PIN_LED = 0;
                    //----------------------------
                    //Averaging data
                    Data_X = (signed short) (SumData_X/n_sample);
                    Data_Y = (signed short) (SumData_Y/n_sample);
                    Data_Z = (signed short) (SumData_Z/n_sample);
                    Data_TC = (signed short) (SumData_TC/n_sample);
                    Data_TS = (signed short) (SumData_TS/n_sample);
                    //----------------------------
                    //Sending data
                    print_string("JROADQ");
                    WriteByte_USART(ContaSample);
                    print_value(Data_X);
                    print_value(Data_Y);
                    print_value(Data_Z);
                    print_value(Data_TC);
                    print_value(Data_TS);
                    print_string("\n\r");
                    //----------------------------
                }
                break;
            case 1:     // Read Latitude GPS
                GPS_LATITUDE();
                TransmitDAT_USART('1');
                break;
            case 2:     // Read Longitude GPS
                GPS_LONGITUDE();
                TransmitDAT_USART('2');
                break;
            case 3:
                GPS_STATUS();
                if(RxBuffer=='A'){
                    TransmitDAT_USART('8');
                }else{
                    TransmitDAT_USART('0');
                }
                break;
            default:
                NOP();
        }
    }else if((PIR1bits.RCIF == 1) && (PIE1bits.RCIE == 1)){
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
                OPTION_REGbits.INTEDG = 0;  // falling edge to interrupt INT_EXT
                INTCONbits.INTF = 0;        // Clear bit Flag
                INTCONbits.INTE = 1;        // Enable Interrupt.
                //---------------------
                break;
            case '2':
                TransmitCMD_USART('2');
                //---------------------
                //Longitude GPS
                CmdGPS = 2;
                OPTION_REGbits.INTEDG = 0;  // falling edge to interrupt INT_EXT
                INTCONbits.INTF = 0;        // Clear bit Flag
                INTCONbits.INTE = 1;        // Enable Interrupt.
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
                OPTION_REGbits.INTEDG = 0;  // falling edge to interrupt INT_EXT
                INTCONbits.INTF = 0;        // Clear bit Flag
                INTCONbits.INTE = 1;        // Enable Interrupt.
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
                OPTION_REGbits.INTEDG = 0;  // falling edge to interrupt INT_EXT
                INTCONbits.INTF = 0;        // Clear bit Flag
                INTCONbits.INTE = 1;        // Enable Interrupt.
                //---------------------
                break;
            case '9':
                //----------------------------
                //Setting TMR1 to T = 200 e-3
                TMR1H = 0x10;
                TMR1L = 0x06;
                T1CON = 0x31;           //Pre: x8
                PIR1bits.TMR1IF = 0;    //Clearing flag bit timer1
                PIE1bits.TMR1IE = 1;    // Interrupt for TIMER1 is enabled
                //----------------------------
                TransmitCMD_USART('9');
                //----------------------------
                //----------------------------
                //Initialization values:
                ContaSample = 0;
                ControlSample = 0;
                //----------------------------
                //Sending JRODATE
                print_string("G-JRODATEXXXXXXXXXXXX\n\r");
                //----------------------------
                PIN_LED = 1;
                //----------------------------
                //Data Conversion and sending
                PORTB = ChannelX;
                __delay_ms(delay_channel);
                //----------------------------
                SumData_X = 0;
                Data_X = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                   Data_X = ReadDataADC();
                   SumData_X = SumData_X + Data_X;
                }
                //----------------------------
                PORTB = ChannelY;
                __delay_ms(delay_channel);
                //----------------------------
                SumData_Y = 0;
                Data_Y = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                    Data_Y = ReadDataADC();
                    SumData_Y = SumData_Y + Data_Y;
                }
                //----------------------------
                PORTB = ChannelZ;
                __delay_ms(delay_channel);
                //----------------------------
                SumData_Z = 0;
                Data_Z = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                    Data_Z = ReadDataADC();
                    SumData_Z = SumData_Z + Data_Z;
                }
                //----------------------------
                PORTB = ChannelTC;
                __delay_ms(delay_channel);
                //----------------------------
                SumData_TC = 0;
                Data_TC = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                    Data_TC = ReadDataADC();
                    SumData_TC = SumData_TC + Data_TC;
                }
                //----------------------------
                PORTB = ChannelTS;
                __delay_ms(delay_channel);
                //----------------------------
                SumData_TS = 0;
                Data_TS = ReadDataADC();
                for(conta = 0; conta<n_sample; conta++){
                    Data_TS = ReadDataADC();
                    SumData_TS = SumData_TS + Data_TS;
                }
                //----------------------------
                PIN_LED = 0;
                //-----------------------------
                //Averaging data
                Data_X = (signed short) (SumData_X/n_sample);
                Data_Y = (signed short) (SumData_Y/n_sample);
                Data_Z = (signed short) (SumData_Z/n_sample);
                Data_TC = (signed short) (SumData_TC/n_sample);
                Data_TS = (signed short) (SumData_TS/n_sample);
                //----------------------------
                //Sending data
                print_string("JROADQ");
                WriteByte_USART(ContaSample);
                print_value(Data_X);
                print_value(Data_Y);
                print_value(Data_Z);
                print_value(Data_TC);
                print_value(Data_TS);
                print_string("\n\r");
                break;
            default:
                TransmitCMD_USART('X');
                RCSTAbits.CREN = 1;     //Enable continuous reception.
        }   
    }else if((PIR1bits.TMR1IF == 1) && (PIE1bits.TMR1IE == 1)){
        PIR1bits.TMR1IF = 0;        //Clean bit Flag TMR1
        TMR1H = 0x10;
        TMR1L = 0x06;
        ContaSample++;
        //-----------------------------------------------------
        ////Data Conversion and sending
        //----------------------------
        PIN_LED = 1;
        //----------------------------
        //Data Conversion and sending
        PORTB = ChannelX;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_X = 0;
        Data_X = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_X = ReadDataADC();
            SumData_X = SumData_X + Data_X;
        }
        //----------------------------
        PORTB = ChannelY;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_Y = 0;
        Data_Y = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_Y = ReadDataADC();
            SumData_Y = SumData_Y + Data_Y;
        }
        //----------------------------
        PORTB = ChannelZ;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_Z = 0;
        Data_Z = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_Z = ReadDataADC();
            SumData_Z = SumData_Z + Data_Z;
        }
        //----------------------------
        PORTB = ChannelTC;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_TC = 0;
        Data_TC = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_TC = ReadDataADC();
            SumData_TC = SumData_TC + Data_TC;
        }
        //----------------------------
        PORTB = ChannelTS;
        __delay_ms(delay_channel);
        //----------------------------
        SumData_TS = 0;
        Data_TS = ReadDataADC();
        for(conta = 0; conta<n_sample; conta++){
            Data_TS = ReadDataADC();
            SumData_TS = SumData_TS + Data_TS;
        }
        //----------------------------
        PIN_LED = 0;
        //----------------------------
        //Averaging data
        Data_X = (signed short) (SumData_X/n_sample);
        Data_Y = (signed short) (SumData_Y/n_sample);
        Data_Z = (signed short) (SumData_Z/n_sample);
        Data_TC = (signed short) (SumData_TC/n_sample);
        Data_TS = (signed short) (SumData_TS/n_sample);
        //----------------------------
        //Sending data
        print_string("JROADQ");
        WriteByte_USART(ContaSample);
        print_value(Data_X);
        print_value(Data_Y);
        print_value(Data_Z);
        print_value(Data_TC);
        print_value(Data_TS);
        print_string("\n\r");
        //-----------------------------------------------------
        if(ContaSample == 4){
            PIE1bits.TMR1IE = 0;        //INT_TIMER1 Disabled
            if(ControlSample == 1){
                INTCONbits.INTF = 0;    //Clear bit Flag
                INTCONbits.INTE = 1;    //INT_EXT Enabled
            }
        }  
    }
    
    return;
}


void main(void) {
    unsigned char conta;
    //-----------------------------------------------
    // Setting IO PORTS
    ADCON1 = 0x06;
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
    //Init pins ADC
    PIN_CS = 1;
    PIN_RC = 1;
    PIN_CLK = 0;
    __delay_ms(100);
    //-----------------------------------------------
    // Setting Serial Port:
    ConfigPort_USART();
    OpenPort_USART();
    //-----------------------------------------------
    //Setting Interrupt:
    INTCONbits.PEIE = 1;    //Peripheral interrupts  enabled
    INTCONbits.GIE = 1;     //Global interrupt enabled
    //-----------------------------------------------
    //Initialize global variables:
    PIN_LED = 0;
    //-----------------------------------------------
    while(1);
    ClosePort_USART();
    return;
}
