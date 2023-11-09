

#include <xc.h>
#include "LIBGPS_38400.h"
#include "LIB18F_USART.h"

//-------------------------------------------------------------
//
void ReadByteSW_USART(void){
    unsigned char conta;
    
    RxBuffer = 0;
    while(PORTBbits.RB0);
    __delay_us(12);
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    for(conta=0; conta<7; conta++){
        __delay_us(24);
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        if(PORTBbits.RB0 == 1){
            RxBuffer = RxBuffer | 0x80;
        }
        RxBuffer >>= 1;
    }
    __delay_us(24);
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    if(PORTBbits.RB0 == 1){
        RxBuffer = RxBuffer | 0x80;
    }
    __delay_us(24);
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
        LimitConta++;
        if(RxBuffer == 'C'){
            // Read comma
            ReadByteSW_USART();
            LimitConta++;
            //Read Time
            for(conta=0; conta<6; conta++){
                ReadByteSW_USART();
                LimitConta++;
                BufferDAT[conta] = RxBuffer;
            }
            // Read comma
            ReadByteSW_USART();
            LimitConta++;
            // Read Data-A
            ReadByteSW_USART();
            LimitConta++;
            if(RxBuffer == 'A'){
                //Comma counter for 7:
                conta = 0;
                while(conta<7){
                    ReadByteSW_USART();
                    LimitConta++;
                    if(RxBuffer == ','){
                        conta++;
                    }
                }
                //Reading Date:
                for(conta=0; conta<6; conta++){
                    ReadByteSW_USART();
                    LimitConta++;
                    BufferDAT[conta + 6] = RxBuffer;
                }
                BufferDAT[12] = '\0';
                RxBuffer = 'A';
                break;
            }
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
    
    //Wait for 2 times, 70 bytes x 2 seconds
    while(LimitConta < 140){
        ReadByteSW_USART();
        LimitConta++;
        if(RxBuffer == 'C'){
            for(conta=0; conta<10; conta++){
                ReadByteSW_USART();
                LimitConta++;
            }
            //Reading Latitude:
            for(conta=0; conta<11; conta++){
                ReadByteSW_USART();
                LimitConta++;
                BufferDAT[conta] = RxBuffer;
            }
            BufferDAT[11] = '\0';
            LimitConta = 150;
            break;
        }
    }
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
    
    //Wait for 2 times, 70 bytes x 2 seconds
    while(LimitConta < 140){
        ReadByteSW_USART();
        LimitConta++;
        if(RxBuffer == 'C'){
            //Comma counter for 5:
            conta = 0;
            while(conta<5){
                ReadByteSW_USART();
                LimitConta++;
                if(RxBuffer == ','){
                    conta++;
                }
            }
            //Reading Longitude:
            for(conta=0; conta<12; conta++){
                ReadByteSW_USART();
                LimitConta++;
                BufferDAT[conta] = RxBuffer;
            }
            BufferDAT[12] = '\0';
            LimitConta = 150;
            break;
        }
    }
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
    
    //Wait for 2 times, 70 bytes x 2 seconds
    while(LimitConta < 140){
        ReadByteSW_USART();
        LimitConta++;
        if(RxBuffer == 'C'){
            //Reading comma:
            ReadByteSW_USART();
            LimitConta++;
            //Reading Time:
            for(conta=0; conta<6; conta++){
                ReadByteSW_USART();
                LimitConta++;
                DateTimeGPS[conta] = RxBuffer;
            }
            //Comma counter for 8:
            conta = 0;
            while(conta<8){
                ReadByteSW_USART();
                LimitConta++;
                if(RxBuffer == ','){
                    conta++;
                }
            }
            //Reading Date:
            for(conta=0; conta<6; conta++){
                ReadByteSW_USART();
                LimitConta++;
                DateTimeGPS[conta + 6] = RxBuffer;
            }
            DateTimeGPS[12] = '\0';
            LimitConta = 150;
            break;
        }
    }
    return;
}
