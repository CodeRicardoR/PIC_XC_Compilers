/*
 * File:   Digitizer12bitsV0.c
 * Author: ricar
 *
 * Created on 12 de abril de 2022, 07:13 PM
 */


/*
 * File:   Digitizer12bitsV0.c
 * Author: ricar
 *
 * Created on 12 de abril de 2022, 07:13 PM
 */


#include "main.h"

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void){
    char uart_data;
    
    if((IEC0bits.U1RXIE == 1)&&(IFS0bits.U1RXIF == 1)){
        IFS0bits.U1RXIF = 0;            //Clear Flag U1RX
        
        uart_data = UART1_receive();
        
        switch(uart_data){
            case '5':
                UART1_Transmit('5');
                ControlSample = 0;      //Stop Continue Adquisition
                break;
            case '0':
                UART1_Transmit('0');
                break;
            case '1':
                UART1_Transmit('1');
                //---------------------
                //Latitude GPS
                CmdGPS = 1;
                Enable_InterruptUART2();
                //---------------------
                break;
            case '2':
                UART1_Transmit('2');
                //---------------------
                //Longitude GPS
                CmdGPS = 2;
                Enable_InterruptUART2();
                //---------------------
                break;
            case '3':
                UART1_Transmit('3');
                //---------------------
                // Date/Time
                CmdGPS = 3;
                Enable_InterruptUART2();
                //---------------------
                break;
            case '4':
                UART1_Transmit('4');
                ControlSample = 255;            // Date/time from GPS
                // Start Adquisition sync with GPS
                CmdGPS = 0;
                Enable_InterruptUART2();
                break;
            case '6':
                UART1_Transmit('6');
                Enable_INT1(0);                 // Rising edge INT1
                ControlSample = 127;            // Date/time from RTC
                break;
            case '7':
                UART1_Transmit('7');
                break;
            case '8':
                UART1_Transmit('8');
                //---------------------
                //Sync GPS
                CmdGPS = 4;
                Enable_InterruptUART2();
                //---------------------
                break;
            case '9':
                UART1_Transmit('9');
                ControlSample = 0;              // Mode Logger (Stop continue adquisition))
                //---------------------
                Enable_TIMER1();             
                //---------------------
                LATBbits.LATB9 = 1;
                print_string_U1("XXXXXXXXXXXX\n\r");
                LATBbits.LATB9 = 0;
                break;
            default:
                UART1_Transmit('X');
        }
    }
    
    return;
}

void __attribute__((__interrupt__,no_auto_psv)) _U2RXInterrupt(void){
    int conta;
    
    if((IEC1bits.U2RXIE == 1)&&(IFS1bits.U2RXIF == 1)){
        
        Disable_InterruptUART2();
        switch(CmdGPS){
            case 0:
                //---------------------
                Enable_TIMER1();
                //----------------------------------------
                //------ Date/Time from GPS
                LATBbits.LATB9 = 1;
                GPS_TIMEDATE();
                for(conta=0;conta<12;conta++){
                    UART1_Transmit(BufferDAT[conta]);
                }
                print_string_U1("\n\r");
                LATBbits.LATB9 = 0;
                //---------------------
                break;
            case 1:
                GPS_LATITUDE();
                TransmitDAT_USART('1');
                break;
            case 2:
                GPS_LONGITUDE();
                TransmitDAT_USART('2');
                break;
            case 3:
                GPS_TIMEDATE();
                TransmitDAT_USART('3');
                break;
            case 4:
                GPS_STATUS();
                if(RxBuffer=='A'){
                    TransmitDAT_USART('8');
                }else{
                    TransmitDAT_USART('0');
                }
                break;
            default:
                Nop();
        }
    }
    return;
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void){
    int conta;
    
    if((IEC0bits.T1IE == 1)&&(IFS0bits.T1IF == 1)){
        
        IFS0bits.T1IF = 0;      //Clear Flag bit
        ContaSample++;
        LATBbits.LATB10 = 1;
        //-----------------------------------------------------------------
        //Read AN0
        SumDataAN0 = 0;
        for(conta=0; conta<5; conta++){
            DataAN0 = ReadADC(0);
            SumDataAN0 = SumDataAN0 + DataAN0;
        }
        //-------------------------------------
        //Read AN1
        SumDataAN1 = 0;
        for(conta=0; conta<5; conta++){
            DataAN1 = ReadADC(1);
            SumDataAN1 = SumDataAN1 + DataAN1;
        }
        //-------------------------------------
        //Read AN2
        SumDataAN2 = 0;
        for(conta=0; conta<5; conta++){
            DataAN2 = ReadADC(2);
            SumDataAN2 = SumDataAN2 + DataAN2;
        }
        //-------------------------------------
        //Read AN3
        SumDataAN3 = 0;
        for(conta=0; conta<5; conta++){
            DataAN3 = ReadADC(3);
            SumDataAN3 = SumDataAN3 + DataAN3;
        }
        //-------------------------------------
        //Read AN4
        SumDataAN4 = 0;
        for(conta=0; conta<5; conta++){
            DataAN4 = ReadADC(4);
            SumDataAN4 = SumDataAN4 + DataAN4;
        }
        //-------------------------------------
        //Read AN5
        SumDataAN5 = 0;
        for(conta=0; conta<5; conta++){
            DataAN5 = ReadADC(5);
            SumDataAN5 = SumDataAN5 + DataAN5;
        }
        //-------------------------------------
        DataAN0 = SumDataAN0/5;
        DataAN1 = SumDataAN1/5;
        DataAN2 = SumDataAN2/5;
        DataAN3 = SumDataAN3/5;
        DataAN4 = SumDataAN4/5;
        DataAN5 = SumDataAN5/5;
        //-------------------------------------
        sprintf(ValueConta,"%d",ContaSample);
        sprintf(ValueAN0,"%04u",DataAN0);
        sprintf(ValueAN1,"%04u",DataAN1);
        sprintf(ValueAN2,"%04u",DataAN2);
        sprintf(ValueAN3,"%04u",DataAN3);
        sprintf(ValueAN4,"%04u",DataAN4);
        sprintf(ValueAN5,"%04u",DataAN5);
        //-------------------------------------
        print_string_U1(ValueConta);
        print_string_U1(ValueAN0);
        print_string_U1(ValueAN1);
        print_string_U1(ValueAN2);
        print_string_U1(ValueAN3);
        print_string_U1(ValueAN4);
        print_string_U1(ValueAN5);
        print_string_U1("\n\r");
        //-----------------------------------------------------------------
        if(ContaSample == 9){
            ContaSample = 0;
            Disable_TIMER1();           // Disable TIMER1
            if(ControlSample == 127){
                //Eanble INT1 Interrupt for RTC
                Enable_INT1(0);         // Rising edge INT1
            }else if(ControlSample == 255){
                //Enable USART2 Interrutp for GPS
                CmdGPS = 0;
                Enable_InterruptUART2();
            }
        }
        LATBbits.LATB10 = 0;
    }
    return;
}

void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt(void){
    int conta;
    
    if((IEC1bits.INT1IE == 1)&&(IFS1bits.INT1IF == 1)){
        
        Disable_INT1();
        //---------------------
        Enable_TIMER1();
        //---------------------
        
        LATBbits.LATB9 = 1;
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
        
        sprintf(BufferDAT,"%02d%02d%02d%02d%02d%02d",Hour,Minute,Second,Date,Month,Year);
        for(conta=0;conta<12;conta++){
            UART1_Transmit(BufferDAT[conta]);
        }
        print_string_U1("\n\r");
        LATBbits.LATB9 = 0;
    }
    
    return;
}

int main(void){
    //Setting PORTB
    ADPCFG = 0x1FFF;        //All pins are digital
    TRISBbits.TRISB9 = 0;
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB11 = 0;
    LATBbits.LATB9 = 0;
    LATBbits.LATB10 = 0;
    LATBbits.LATB11 = 0;
    
    //Set PORTD INT1-RD8
    TRISDbits.TRISD8 = 1;
    
    //Setting UART2
    UART2_Open();
    UART2_Config();
    
    //Setting UART1
    UART1_Open();
    UART1_Config();
    
    //Setting TIMER1: 100ms
    Config_TIMER1();
    
    //Setting ADC
    ConfigADC();
    
    //Init DS1307 I2C
    TRIS_SDA = 1;
    TRIS_SCL = 1;
    DS1307_SetRegister(0x07,0x10);  //Enable SQW OUT to 1Hz
     
 
    
    //Enablin UART1 interrupt
    IPC2bits.U1RXIP = 7;        //Level 7 (of 7)) level priority
    IFS0bits.U1RXIF = 0;        //Clear Flag U1RX
    IEC0bits.U1RXIE = 1;        //Enabled Interrupt Rx from UART1
    
    //Setting UART2 interrupt level
    IPC6bits.U2RXIP = 6;        //Level 6 (of 7)) level priority
    
    //Setting TIMER1 interrupt level
    IPC0bits.T1IP = 5;          //Level 5 (of 7)) level priority
    
    //Setting INT1 interrupt level
    IPC4bits.INT1IP = 4;        //Level 4 (of 7)) level priority
    
    //Init Variables
    CmdGPS = 0;                 //Read Date
    ControlSample = 0;          //Default mode logger
    
    while(1);
    UART1_Close();
    UART2_Close();
    return 0;
}

