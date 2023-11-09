/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

extern char RxBuffer;

void UART1_Config(void);
void UART1_Open(void);
void UART1_Close(void);
void UART1_Transmit(char data);
char UART1_receive(void);
void print_string_U1(char *str);

void UART2_Config(void);
void UART2_Open(void);
void UART2_Close(void);
void UART2_Transmit(char data);
void UART2_receive(void);



