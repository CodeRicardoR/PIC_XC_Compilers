/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SELECTCHANNELS_H
#define	SELECTCHANNELS_H

#include <xc.h> // include processor files - each processor file is guarded.  

//Definiciones para seleccion de canal de ADC: MAX337
/* PINES       SW_A    SW_B    A1   A0
 * CHANNEL      RA5     RA4    RC0  RC1
 *    X          0       1      0    0
 *    Y          0       1      1    0
 *    Z          1       0      0    0
 *   TC          1       0      1    0
 *   TS          1       0      1    1
 * AUX0          0       1      0    1
 * AUX1          0       1      1    1
 * AUX2          1       0      0    1
 *  GND          0       0      X    X
 */
#define PIN_SWA LATAbits.LA5
#define PIN_SWB LATAbits.LA4
#define PIN_A1 LATCbits.LC0
#define PIN_A0 LATCbits.LC1


void SelectChannelX(void);
void SelectChannelY(void);
void SelectChannelZ(void);
void SelectChannelC(void);
void SelectChannelS(void);
void SelectChannel1(void);
void SelectChannel2(void);
void SelectChannel3(void);
void SelectChannelGND(void);


#endif	/* XC_HEADER_TEMPLATE_H */

