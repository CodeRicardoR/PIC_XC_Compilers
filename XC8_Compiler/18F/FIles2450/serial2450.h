
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SERIAL2450_H
#define	SERIAL2450_H

#include <xc.h> // include processor files - each processor file is guarded.  

void Config_SerialPort(void);
void Open_SerialPort(void);
void Close_SerialPort(void);
char ReadByte_SerialPort(void);
void WriteByte_SerialPort(char data);

#endif	/* XC_HEADER_TEMPLATE_H */

