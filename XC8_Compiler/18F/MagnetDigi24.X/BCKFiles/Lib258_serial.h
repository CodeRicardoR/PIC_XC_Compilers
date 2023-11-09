
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LIB258_SERIAL_H
#define	LIB258_SERIAL_H

#include <xc.h>         // include processor files - each processor file is guarded.
#include <stdio.h>      // To call fprint

char BufferDAT[13]="XXXXXXXXXXXX";

void ConfigSerialPort(void);
void OpenSerialPort(void);
void CloseSerialPort(void);
char ReadByte_USART(void);
char ReadWait_USART(void);
void WriteByte_USART(char byte);
void print_string(char *str);
char ReceiveCMD_USART(void);
void TransmitCMD_USART(char CMD_ID);
void TransmitDAT_USART(char DAT_ID);
void print_value(unsigned long value);

#endif	/* XC_HEADER_TEMPLATE_H */

