
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LIB16F_GPS38400_H
#define	LIB16F_GPS38400_H

unsigned char RxBuffer;

void ReadByteSW_USART(void);
void GPS_STATUS(void);
void GPS_LATITUDE(void);
void GPS_LONGITUDE(void);
void GPS_TIMEDATE(void);

#endif	/* XC_HEADER_TEMPLATE_H */

