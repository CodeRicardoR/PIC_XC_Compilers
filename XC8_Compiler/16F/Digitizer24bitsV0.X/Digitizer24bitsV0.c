/*
 * File:   Digitizer24bitsV0.c
 * Author: ricar
 *
 * Created on 7 de julio de 2023, 10:06 PM
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
                    PIN_LED = 1;
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
                    PIN_TEST1 = 1;
                    GPS_TIMEDATE();
                    PIN_TEST1 = 0;
                    //----------------------------
                    //Sending JRODATE
                    print_string("G-JRODATE");
                    print_string(BufferDAT);
                    print_string("\n\r");
                    //----------------------------
                    //Data Conversion and sending
                    //---------------------------
                    CleanHolding();
                    //---------------------------
                    PORTB = ChannelX;
                    PIN_TEST1 = 1;
                    __delay_ms(delay_channel);
                    SumData_X = 0;
                    ReadAD24(0,0x00);   //Ignore this first data
                    for(conta=0; conta<n_sample; conta++){
                        Data_X = ReadAD24(1,0x00);    //Reading valid data
                        SumData_X = SumData_X + Data_X;
                    }
                    PIN_TEST1 = 0;
                    //----------------------------
                    CleanHolding();
                    //---------------------------
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
                    //---------------------------
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
                    //---------------------------
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
                    //---------------------------
                    PORTB = ChannelTS;
                    __delay_ms(delay_channel);
                    SumData_TS = 0;
                    ReadAD24(0,0x00);   //Ignore this first data
                    for(conta=0; conta<n_sample; conta++){
                        Data_TS = ReadAD24(1,0x00);    //Reading valid data
                        SumData_TS = SumData_TS + Data_TS;
                    }
                    //----------------------------
                    //Averaging data
                    Data_X = SumData_X/n_sample;
                    Data_Y = SumData_Y/n_sample;
                    Data_Z = SumData_Z/n_sample;
                    Data_TC = SumData_TC/n_sample;
                    Data_TS = SumData_TS/n_sample;
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
                    PIN_LED = 0;
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
                //Setting ADC CS5532
                BufferDAT[0] = ResetCS5532();
                InitCS5532();
                //Read Offset Register
                ReadRegister_4bytesCS5532(0x09);
                BufferDAT[1] = BufferSPI[0];
                BufferDAT[2] = BufferSPI[1];
                BufferDAT[3] = BufferSPI[2];
                BufferDAT[4] = BufferSPI[3];
                        
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
            case '4':       // Enable continuous acquisition
                TransmitCMD_USART('4');
                //---------------------
                CmdGPS = 0;
                ControlSample = 1;
                TransferByte_SPI(0xC0);     // Start sampling ADC CS5532
                OPTION_REGbits.INTEDG = 0;  // falling edge to interrupt INT_EXT
                INTCONbits.INTF = 0;        // Clear bit Flag
                INTCONbits.INTE = 1;        // Enable Interrupt.
                //---------------------
                break;
            case '6':       //Read Data channel GND
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
                PIN_LED = 1;
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
                TransferByte_SPI(0xC0);     // Start sampling ADC CS5532
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
                CleanHolding();
                //----------------------------
                PORTB = ChannelX;
                PIN_TEST1 = 1;
                __delay_ms(delay_channel);
                SumData_X = 0;
                ReadAD24(0,0x00);   //Ignore this first data
                for(conta=0; conta<n_sample; conta++){
                    Data_X = ReadAD24(1,0x00);    //Reading valid data
                    SumData_X = SumData_X + Data_X;
                }
                PIN_TEST1 = 0;
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
                //Averaging data
                Data_X = SumData_X/n_sample;
                Data_Y = SumData_Y/n_sample;
                Data_Z = SumData_Z/n_sample;
                Data_TC = SumData_TC/n_sample;
                Data_TS = SumData_TS/n_sample;
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
                //-----------------------------
                PIN_LED = 0;
                //-----------------------------
                break;
            default:
                TransmitCMD_USART('X');
                RCSTAbits.CREN = 1;     //Enable continuous reception.
        }   
    }else if((PIR1bits.TMR1IF == 1) && (PIE1bits.TMR1IE == 1)){
        //----------------------------
        PIN_LED = 1;
        //----------------------------
        PIR1bits.TMR1IF = 0;        //Clean bit Flag TMR1
        TMR1H = 0x10;
        TMR1L = 0x06;
        ContaSample++;
        //-----------------------------------------------------
        ////Data Conversion and sending
        //----------------------------
        CleanHolding();
        //----------------------------
        PORTB = ChannelX;
        PIN_TEST1 = 1;
        __delay_ms(delay_channel);
        SumData_X = 0;
        ReadAD24(0,0x00);   //Ignore this first data
        for(conta=0; conta<n_sample; conta++){
            Data_X = ReadAD24(1,0x00);    //Reading valid data
            SumData_X = SumData_X + Data_X;
        }
        PIN_TEST1 = 0;
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
        //Averaging data
        Data_X = SumData_X/n_sample;
        Data_Y = SumData_Y/n_sample;
        Data_Z = SumData_Z/n_sample;
        Data_TC = SumData_TC/n_sample;
        Data_TS = SumData_TS/n_sample;
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
        PIN_LED = 0;
        //-----------------------------------------------------
        if(ContaSample == 4){
            PIE1bits.TMR1IE = 0;        //INT_TIMER1 Disabled
            if(ControlSample == 1){
                INTCONbits.INTF = 0;    //Clear bit Flag
                INTCONbits.INTE = 1;    //INT_EXT Enabled
            }else{
                ReadAD24(0,0xFF);       //Stop sampling ADC CS5532
            }
        }
        
    }
    
    return;
}


void main(void) {
    
    //-----------------------------------------------
    // Setting IO PORTS
    ADCON1 = 0x06;
    PORTA = 0;
    PORTAbits.RA1 = 1;          // '1' IDLE TX GPS
    PORTB = 0;
    PORTC = 0;
    TRISA = 0x01;               // RA0 Input GPS-RX
    TRISB = 0x01;               // RB0 Input GPS-RX edge
    TRISC = 0x00;               // Default All output
    OPTION_REGbits.nRBPU = 0;   // Pull-up RB0 is enabled
    __delay_ms(100);
    //-----------------------------------------------
    // Setting SPI Port
    ConfigPort_SPI();
    OpenPort_SPI();
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
    PIN_TEST1 = 0;
    PIN_TEST2 = 0;
    //-----------------------------------------------
    while(1);
    ClosePort_USART();
    ClosePort_SPI();
    return;
}
