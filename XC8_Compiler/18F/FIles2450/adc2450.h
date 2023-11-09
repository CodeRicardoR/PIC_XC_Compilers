/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC2450_H
#define	ADC2450_H

#include <xc.h> // include processor files - each processor file is guarded.  

void ConfigADC(void);
unsigned int ReadChannelADC(char channel);

#endif	/* XC_HEADER_TEMPLATE_H */

