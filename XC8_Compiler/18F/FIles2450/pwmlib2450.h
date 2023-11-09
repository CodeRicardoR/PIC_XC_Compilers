
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PWMLIB2450_H
#define	PWMLIB2450_H

#include <xc.h> // include processor files - each processor file is guarded.  

void ConfigPWM(void);
void ConfigDutyCycle(unsigned char value);

#endif	/* XC_HEADER_TEMPLATE_H */

