
/* 
 * File: LIB18F_USART   
 * Author: Ricardo V. Rojas Quispe
 * Comments: Library to control of USART peripheral.
 * Revision history: April 30, 21
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LIB16F_USART_H
#define	LIB16F_USART_H

//Functions to use:
void ConfigPort_USART(void);
void OpenPort_USART(void);
void ClosePort_USART(void);
unsigned char ReadByte_USART(void);
void WriteByte_USART(unsigned char byte);
void print_string(char *str);
unsigned char ReceiveCMD_USART(void);
//unsigned char ReceiveDAT_USART(void);
void TransmitCMD_USART(unsigned char CMD_ID);
void TransmitDAT_USART(unsigned char DAT_ID);
void print_value(signed long value);

#endif


