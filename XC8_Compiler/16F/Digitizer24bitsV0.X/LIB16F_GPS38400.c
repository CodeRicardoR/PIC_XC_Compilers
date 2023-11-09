
#include "main.h"

//-------------------------------------------------------------
void ReadByteSW_USART(void){
    unsigned char conta;
    
    RxBuffer = 0;
    while(PORTBbits.RB0);
    __delay_us(15);
    for(conta=0; conta<7; conta++){
        __delay_us(15);
        NOP();
        NOP();
        if(PORTBbits.RB0 == 1){
            RxBuffer = RxBuffer | 0x80;
        }else{
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
            NOP();
        }
        RxBuffer >>= 1;
    }
    __delay_us(15);
    NOP();
    NOP();
    NOP();
    if(PORTBbits.RB0 == 1){
        RxBuffer = RxBuffer | 0x80;
    }else{
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
    }
    __delay_us(12);//Original 14
    return;  
}

//-------------------------------------------------------------
//
void GPS_STATUS(void){
    /*
     * $GPRMC,000004,V,1157.1736,S,07652.5638,W,,,210899,001.0,W*74
     * $GPRMC,000005,A,1157.4029,S,07652.5576,W,000.0,000.0,080914,000.4,E*71
     */
    
    unsigned char conta = 0;
    unsigned short LimitConta = 0;
    
    //Wait for 1 minute: 70 bytes x 60 seconds
    while(LimitConta < 4200){
        ReadByteSW_USART();
        if(RxBuffer == 'C'){
            // Read comma
            ReadByteSW_USART();
            //Read Time
            for(conta=0; conta<6; conta++){
                ReadByteSW_USART();
                BufferDAT[conta] = RxBuffer;
            }
            // Read comma
            ReadByteSW_USART();
            // Read Data-A
            ReadByteSW_USART();
            if(RxBuffer == 'A'){
                //Comma counter for 7:
                conta = 0;
                while(conta<7){
                    ReadByteSW_USART();
                    if(RxBuffer == ','){
                        conta++;
                    }
                }
                //Reading Date:
                for(conta=0; conta<6; conta++){
                    ReadByteSW_USART();
                    BufferDAT[conta + 6] = RxBuffer;
                }
                BufferDAT[12] = '\0';
                RxBuffer = 'A';
                LimitConta = 4201;
                break;
            }
        }else{
            LimitConta++;
        }
    }
    return;
}

//---------------------------------------------------------------
//
void GPS_LATITUDE(void){
    /*
     * $GPRMC,000004,V,1157.1736,S,07652.5638,W,,,210899,001.0,W*74
     * $GPRMC,000005,A,1157.4029,S,07652.5576,W,000.0,000.0,080914,000.4,E*71
     */
    unsigned char LimitConta = 0;
    unsigned char conta = 0;
    
    //Wait for 2 times, 70 bytes x 2 seconds = 140 min.
    while(LimitConta < 150){
        ReadByteSW_USART();
        if(RxBuffer == 'C'){
            //Comma counter for 3:
            conta = 0;
            while(conta<3){
                ReadByteSW_USART();
                if(RxBuffer == ','){
                    conta++;
                }
            }
            //Reading Latitude:
            for(conta=0; conta<11; conta++){
                ReadByteSW_USART();
                BufferDAT[conta] = RxBuffer;
            }
            BufferDAT[11] = '\0';
            BufferDAT[12] = '\0';
            LimitConta = 160;
            break;
        }else{
            LimitConta++;
        }
    }
    __delay_ms(14);
    return;
}

//---------------------------------------------------------------
//
void GPS_LONGITUDE(void){
    /*
     * $GPRMC,000004,V,1157.1736,S,07652.5638,W,,,210899,001.0,W*74
     * $GPRMC,000005,A,1157.4029,S,07652.5576,W,000.0,000.0,080914,000.4,E*71
     */
    unsigned char LimitConta = 0;
    unsigned char conta = 0;
    
    //Wait for 2 times, 70 bytes x 2 seconds = 140 min
    while(LimitConta < 150){
        ReadByteSW_USART();
        if(RxBuffer == 'C'){
            //Comma counter for 5:
            conta = 0;
            while(conta<5){
                ReadByteSW_USART();
                if(RxBuffer == ','){
                    conta++;
                }
            }
            //Reading Longitude:
            for(conta=0; conta<12; conta++){
                ReadByteSW_USART();
                BufferDAT[conta] = RxBuffer;
            }
            BufferDAT[12] = '\0';
            LimitConta = 160;
            break;
        }else{
            LimitConta++;
        }
    }
    __delay_ms(14);
    return;
}

//---------------------------------------------------------------
//
void GPS_TIMEDATE(void){
    /*
     * $GPRMC,000004,V,1157.1736,S,07652.5638,W,,,210899,001.0,W*74
     * $GPRMC,000005,A,1157.4029,S,07652.5576,W,000.0,000.0,080914,000.4,E*71
     */
    unsigned char LimitConta = 0;
    unsigned char conta = 0;
    
    //Wait for 2 times, 70 bytes x 2 seconds = 140min
    while(LimitConta < 150){
        ReadByteSW_USART();
        if(RxBuffer == 'C'){
            //Reading comma:
            ReadByteSW_USART();
            //Reading Time:
            for(conta=0; conta<6; conta++){
                ReadByteSW_USART();
                BufferDAT[conta] = RxBuffer;
            }
            //Comma counter for 8:
            conta = 0;
            while(conta<8){
                ReadByteSW_USART();
                if(RxBuffer == ','){
                    conta++;
                }
            }
            //Reading Date:
            for(conta=0; conta<6; conta++){
                ReadByteSW_USART();
                BufferDAT[conta + 6] = RxBuffer;
            }
            BufferDAT[12] = '\0';
            LimitConta = 160;
            break;
        }else{
            LimitConta++;
        }
    }
    //__delay_ms(2);
    return;
}
