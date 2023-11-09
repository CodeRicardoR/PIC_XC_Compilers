
#include <xc.h>
#include "main.h"
#include "LIB_GPS38400.h"

void ReadByteUSART_SW(void){
    unsigned char i;
    
    RegBuffer = 0x00;
    FlagRX = 0x00;
    
    while(PIN_RXGPS);//Start Bit
    __delay_us(11);
    
    for(i = 0; i < 7; i++){
        __delay_us(23);
        if (PIN_RXGPS == 1){
            RegBuffer = RegBuffer | 0x80;
        }
        RegBuffer >>= 1;
    }
    __delay_us(23);
    if (PIN_RXGPS == 1){
        RegBuffer = RegBuffer | 0x80;
    }
    __delay_us(23);
    if (PIN_RXGPS == 1){// Stop Bit
        FlagRX = 0xFF;
    }
    
    return;
}

void CheckGPS(void){
    //$GPRMC,000004,V,1157.1736,S,07652.5638,W,,,210899,001.0,W*74
    //$GPRMC,000005,A,1157.4029,S,07652.5576,W,000.0,000.0,080914,000.4,E*71
    unsigned char i;
    unsigned char comas;
    unsigned char ModeGPS;
    
    ModeGPS = 'V';
    do{
        ReadByteUSART_SW();
        if (FlagRX == 0xFF){
            if (RegBuffer == 'C'){
                
                ReadByteUSART_SW();
                    
                for (i = 0; i < 6; i++){
                    ReadByteUSART_SW();
                    if (FlagRX == 0xFF){
                        DataBytes[i] = RegBuffer;
                    }else{
                        return;
                    }
                }
                ReadByteUSART_SW();     // ,
                ReadByteUSART_SW();     // A
                if (FlagRX == 0xFF){
                    ModeGPS = RegBuffer;
                }else{
                    return;
                }
                
                comas = 7;
                do{
                    ReadByteUSART_SW();
                    if (FlagRX == 0xFF){
                        if (RegBuffer == ','){
                            comas--;
                        }
                    }else{
                        return;
                    }
                }while(comas > 0);
                
                for (i = 0; i < 6; i++){
                    ReadByteUSART_SW();
                    if (FlagRX == 0xFF){
                        DataBytes[i+6] = RegBuffer;
                    }else{
                        return;
                    }
                }
                break;
            }
        }else{
            break;
        }
    }while(ModeGPS != 'A');
    return;
}

void ReadLatGPS(void){
    //$GPRMC,000004,V,1157.1736,S,07652.5638,W,,,210899,001.0,W*74
    //$GPRMC,000005,A,1157.4029,S,07652.5576,W,000.0,000.0,080914,000.4,E*71
    unsigned char i;
    do{
        ReadByteUSART_SW();
        if (FlagRX == 0xFF){
            if (RegBuffer == 'C'){
                for (i = 0; i < 10; i++){
                    ReadByteUSART_SW();
                    if (FlagRX != 0xFF){
                        return;
                    }
                }
                for (i = 0; i < 11; i++){
                    ReadByteUSART_SW();
                    if (FlagRX == 0xFF){
                        DataBytes[i] = RegBuffer;
                    }else{
                        return;
                    }
                }
                break;
            }
        }else{
            break;
        }
    }while(1);
    return;
}

void ReadLonGPS(void){
    //$GPRMC,000004,V,1157.1736,S,07652.5638,W,,,210899,001.0,W*74
    //$GPRMC,000005,A,1157.4029,S,07652.5576,W,000.0,000.0,080914,000.4,E*71
    unsigned char i;
    do{
        ReadByteUSART_SW();
        if (FlagRX == 0xFF){
            if (RegBuffer == 'C'){
                for (i = 0; i < 22; i++){
                    ReadByteUSART_SW();
                    if (FlagRX != 0xFF){
                        return;
                    }
                }
                for (i = 0; i < 12; i++){
                    ReadByteUSART_SW();
                    if (FlagRX == 0xFF){
                        DataBytes[i] = RegBuffer;
                    }else{
                        return;
                    }
                }
                break;
            }
        }else{
            break;
        }
    }while(1);
    return;
}

void ReadTimeGPS(void){
    //$GPRMC,000004,V,1157.1736,S,07652.5638,W,,,210899,001.0,W*74
    //$GPRMC,000005,A,1157.4029,S,07652.5576,W,000.0,000.0,080914,000.4,E*71
    unsigned char i;
    unsigned char comas;
    do{
        ReadByteUSART_SW();
        if (FlagRX == 0xFF){
            if (RegBuffer == 'C'){
                
                ReadByteUSART_SW();     //,
                    
                for (i = 0; i < 6; i++){
                    ReadByteUSART_SW();
                    if (FlagRX == 0xFF){
                        DataBytes[i] = RegBuffer;
                    }else{
                        return;
                    }
                }
                
                comas = 8;
                do{
                    ReadByteUSART_SW();
                    if (FlagRX == 0xFF){
                        if (RegBuffer == ','){
                            comas--;
                        }
                    }else{
                        return;
                    }
                }while(comas > 0);
                
                for (i = 0; i < 6; i++){
                    ReadByteUSART_SW();
                    if (FlagRX == 0xFF){
                        DataBytes[i+6] = RegBuffer;
                    }else{
                        return;
                    }
                }
                break;
            }
        }else{
            break;
        }
    }while(1);
    return; 
}

