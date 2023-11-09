/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LIB258_SPI_H
#define	LIB258_SPI_H

#define PIN_SDI     PORTCbits.RC4

unsigned char BufferSPI[4];

void ConfigPort_SPI(void);
void OpenPort_SPI(void);
void ClosePort_SPI(void);
unsigned char TransferByte_SPI(unsigned char data);
unsigned char ResetCS5532(void);
void InitCS5532(void);
void ReadRegister_4bytesCS5532(unsigned char command);
void WriteRegister_4bytesCS5532(unsigned char command);
signed long ReadAD24(unsigned char flag_convert, unsigned char cmd_convert);
void CleanHolding(void);
#endif	/* XC_HEADER_TEMPLATE_H */

