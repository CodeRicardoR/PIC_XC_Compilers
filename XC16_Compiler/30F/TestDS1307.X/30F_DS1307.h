/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#define SDA PORTDbits.RD2
#define SCL PORTFbits.RF6
#define TRIS_SDA TRISDbits.TRISD2
#define TRIS_SCL TRISFbits.TRISF6

extern unsigned char DataTime[6];

void I2CBitStart(void);
void I2CBitStop(void);
void I2CBitACK(void);
void I2CBitNACK(void);
unsigned char I2CWrite(unsigned char data);
unsigned char I2CRead(void);
void DS1307_ReadTime(void);
void DS1307_SetRegister(unsigned char address, unsigned char data);



