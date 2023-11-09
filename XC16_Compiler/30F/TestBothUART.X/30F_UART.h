/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

extern char RxBuffer;
extern char BufferDAT[14];

void UART1_Config(void);
void UART1_Open(void);
void UART1_Close(void);
void UART1_Transmit(char data);
char UART1_receive(void);
void print_string_U1(char *str);

char ReceiveCMD_UART1(void);
void TransmitCMD_UART1(char CMD_ID);
void TransmitDAT_USART(char DAT_ID);

void UART2_Config(void);
void UART2_Open(void);
void UART2_Close(void);
void UART2_Transmit(char data);
void UART2_receive(void);
void Enable_InterruptUART2(void);
void Disable_InterruptUART2(void);



