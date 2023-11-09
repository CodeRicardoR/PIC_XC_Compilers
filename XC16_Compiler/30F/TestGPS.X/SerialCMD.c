/*
 * File:   SerialCMD.c
 * Author: ricar
 *
 * Created on 9 de abril de 2022, 06:53 PM
 */

#include "xc.h"
#include "SerialCMD.h"
#include "30F_UART.h"


void GPS_STATUS(void){
    /*
     * $GPRMC,,V,,,,,,,,,,N*53
     * $GPRMC,224552.00,V,,,,,,,,,,N*7B
     * $GPRMC,224558.00,V,,,,,,,090422,,,N*7C
     * $GPRMC,224835.00,A,1210.58659,S,07658.19167,W,0.164,,090422,,,A*7A
     */
    
    unsigned char conta = 0;
    unsigned short LimitConta = 0;
    
    //Wait for 1 minute: 66 bytes x 60 seconds
    while(LimitConta < 3960){
        UART2_receive();
        if(RxBuffer == 'C'){
            // Read comma two commas
            conta = 0;
            while(conta<2){
                UART2_receive();
                if(RxBuffer == ','){
                    conta++;
                }
            }
            // Read Data-A
            UART2_receive();
            if(RxBuffer == 'A'){
                LimitConta = 3961;
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
     * 
     * $GPRMC,,V,,,,,,,,,,N*53
     * $GPRMC,224552.00,V,,,,,,,,,,N*7B
     * $GPRMC,224558.00,V,,,,,,,090422,,,N*7C
     * $GPRMC,224835.00,A,1210.58659,S,07658.19167,W,0.164,,090422,,,A*7A
     */
    unsigned char LimitConta = 0;
    unsigned char conta = 0;
    
    //Wait for 2 times, 66 bytes x 2 seconds = 132 min.
    while(LimitConta < 132){
        UART2_receive();
        if(RxBuffer == 'C'){
            //Comma counter for 3:
            conta = 0;
            while(conta<3){
                UART2_receive();
                if(RxBuffer == ','){
                    conta++;
                }
            }
            //Reading Latitude:
            for(conta=0; conta<12; conta++){
                UART2_receive();
                BufferDAT[conta] = RxBuffer;
            }
            BufferDAT[12] = '\0';
            BufferDAT[13] = '\0';
            LimitConta = 133;
            break;
        }else{
            LimitConta++;
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
     * 
     * $GPRMC,,V,,,,,,,,,,N*53
     * $GPRMC,224552.00,V,,,,,,,,,,N*7B
     * $GPRMC,224558.00,V,,,,,,,090422,,,N*7C
     * $GPRMC,224835.00,A,1210.58659,S,07658.19167,W,0.164,,090422,,,A*7A
     */
    unsigned char LimitConta = 0;
    unsigned char conta = 0;
    
    //Wait for 2 times, 66 bytes x 2 seconds = 132 min
    while(LimitConta < 132){
        UART2_receive();
        if(RxBuffer == 'C'){
            //Comma counter for 5:
            conta = 0;
            while(conta<5){
                UART2_receive();
                if(RxBuffer == ','){
                    conta++;
                }
            }
            //Reading Longitude:
            for(conta=0; conta<13; conta++){
                UART2_receive();
                BufferDAT[conta] = RxBuffer;
            }
            BufferDAT[13] = '\0';
            LimitConta = 133;
            break;
        }else{
            LimitConta++;
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
     * 
     * $GPRMC,,V,,,,,,,,,,N*53
     * $GPRMC,224552.00,V,,,,,,,,,,N*7B
     * $GPRMC,224558.00,V,,,,,,,090422,,,N*7C
     * $GPRMC,224835.00,A,1210.58659,S,07658.19167,W,0.164,,090422,,,A*7A
     */
    unsigned char LimitConta = 0;
    unsigned char conta = 0;
    
    //Wait for 2 times, 66 bytes x 2 seconds = 132min
    while(LimitConta < 132){
        UART2_receive();
        if(RxBuffer == 'C'){
            //Reading comma:
            UART2_receive();
            //Reading Time:
            for(conta=0; conta<6; conta++){
                UART2_receive();
                BufferDAT[conta] = RxBuffer;
            }
            //Comma counter for 8:
            conta = 0;
            while(conta<8){
                UART2_receive();
                if(RxBuffer == ','){
                    conta++;
                }
            }
            //Reading Date:
            for(conta=0; conta<6; conta++){
                UART2_receive();
                BufferDAT[conta + 6] = RxBuffer;
            }
            BufferDAT[12] = '\0';
            BufferDAT[13] = '\0';
            LimitConta = 133;
            break;
        }else{
            LimitConta++;
        }
    }
    //Ignore rest bytes
    for(conta=0;conta<7;conta++){
        UART2_receive();
    }
    return;
}
