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
char ReadByte_USART(void);
void WriteByte_USART(char byte);
void print_string(char *str);
char ReceiveCMD_USART(void);
//unsigned char ReceiveDAT_USART(void);
void TransmitCMD_USART(char CMD_ID);
void TransmitDAT_USART(char DAT_ID);
void print_value(signed short value);

#endif


