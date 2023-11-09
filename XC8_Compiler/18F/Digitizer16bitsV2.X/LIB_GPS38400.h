
#ifndef LIB_GPS38400_H
#define	LIB_GPS38400_H

#define PIN_RXGPS   PORTCbits.RC3       //input

//Variables USART_SW
unsigned char FlagRX,FlagGPS;
unsigned char RegBuffer;
unsigned char DataBytes[12];

void ReadByteUSART_SW(void);
void CheckGPS(void);
void ReadLatGPS(void);
void ReadLonGPS(void);
void ReadTimeGPS(void);

#endif	/* XC_HEADER_TEMPLATE_H */

