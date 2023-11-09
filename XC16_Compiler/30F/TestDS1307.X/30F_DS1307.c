/*
 * File:   30F_DS1307.c
 * Author: ricar
 *
 * Created on 24 de abril de 2022, 04:11 PM
 */

#define FCY 29491200UL
#include <xc.h>
#include <libpic30.h>                   //libreria para generar los delays
#include "30F_DS1307.h"

void I2CBitStart(void){
    
    TRIS_SCL = 1;
    __delay_us(5);
    SDA = 0;
    TRIS_SDA = 0;
    __delay_us(5);
    return;
}

void I2CBitStop(void){
    
    SDA = 0;
    TRIS_SDA = 0;
    SCL = 0;
    TRIS_SCL = 0;
    __delay_us(5);
    TRIS_SCL = 1;
    __delay_us(5);
    TRIS_SDA = 1;
    return;
}

void I2CBitACK(void){
    
    SCL = 0;
    TRIS_SCL = 0;
    SDA = 0;
    TRIS_SDA = 0;
    __delay_us(5);
    TRIS_SCL = 1;
    __delay_us(5);
    SCL = 0;
    TRIS_SCL = 0;
    TRIS_SDA = 1;
    return;
}

void I2CBitNACK(void){
    
    TRIS_SDA = 1;
    SCL = 0;
    TRIS_SCL = 0;
    __delay_us(5);
    TRIS_SCL = 1;
    __delay_us(5);
    SCL = 0;
    TRIS_SCL = 0;
    return;
}

unsigned char I2CWrite(unsigned char data){
    unsigned char conta,ack;
    
    ack = 0xFF;
    conta = 0;
    do{
        SCL = 0;
        TRIS_SCL = 0;
        if(data & 0x80){
            TRIS_SDA = 1;
        }else{
            SDA = 0;
            TRIS_SDA = 0;
        }
        __delay_us(5);
        TRIS_SCL = 1;
        __delay_us(5);
        data <<= 1;
        conta++;
    }while(conta < 8);
    SCL = 0;
    TRIS_SCL = 0;
    TRIS_SDA = 1;
    __delay_us(5);
    TRIS_SCL = 1;
    __delay_us(5);
    if(SDA == 1){
        ack = 0xFF;
    }else{
        ack = 0x00;
    }
    SCL = 0;
    TRIS_SCL = 0;
    
    return ack;
}

unsigned char I2CRead(void){
    unsigned char conta,data;
    
    TRIS_SDA = 1;
    conta = 0;
    data = 0;
    do{
        SCL = 0;
        TRIS_SCL = 0;
        __delay_us(5);
        TRIS_SCL = 1;
        __delay_us(5);
        if(SDA == 1){
            data = data | 0x01;
        }
        data <<= 1; 
        conta++;
    }while(conta < 7);
    SCL = 0;
    TRIS_SCL = 0;
    __delay_us(5);
    TRIS_SCL = 1;
    __delay_us(5);
    if(SDA == 1){
        data = data | 0x01;
    }
    SCL = 0;
    TRIS_SCL = 0;
    
    return data;
}
void DS1307_ReadTime(void){
    unsigned char ack;
    
    //Read Time:
    I2CBitStart();
    ack = I2CWrite(0b11010000);
    if(ack == 0){
        ack = I2CWrite(0x00);
        if(ack == 0){
            I2CBitStart();
            ack = I2CWrite(0b11010001);
            if(ack == 0){
                DataTime[0] = I2CRead();    //Receive seconds
                I2CBitACK();
                DataTime[1] = I2CRead();    //Receive minutes
                I2CBitACK();
                DataTime[2] = I2CRead();    //Receive hours
                I2CBitACK();
                ack = I2CRead();            //Receive days
                I2CBitACK();
                DataTime[3] = I2CRead();    //Receive date
                I2CBitACK();
                DataTime[4] = I2CRead();    //Receive month
                I2CBitACK();
                DataTime[5] = I2CRead();    //Receive year
                I2CBitNACK();
                I2CBitStop();
            }
        }
    }
    return;
}

void DS1307_SetRegister(unsigned char address, unsigned char data){
    unsigned char ack;
    
    I2CBitStart();
    ack = I2CWrite(0b11010000);
    if(ack == 0){
        ack = I2CWrite(address);
        if(ack == 0){
            ack = I2CWrite(data);
            I2CBitStop();
        }
    }
    
    return;
}
