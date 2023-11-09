/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// FOSC
#pragma config FOSFPR = XT_PLL16        // Oscillator (XT w/PLL 16x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_4           // POR Timer Value (4ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_OFF         // PBOR Enable (Disabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define FCY 29491200UL

#define UNI 0x0F
#define DEC 0x70

#include <xc.h>                         // include processor files - each processor file is guarded.  
#include <libpic30.h>                   // libreria para generar los delays
#include <stdio.h>
#include "30F_UART.h"                   // libreria para control de UART 1
#include "30F_TIMER.h"                  // libreria para control de TIMER1
#include "30F_ADC.h"                    // Libreria para control de ADC 12bits
#include "GPS_CMD.h"                    // libriria para control de recpcion de datos de GPS
#include "30F_DS1307.h"                 // Libreria para comunicacion I2C por software

//Global variables
char RxBuffer;
char BufferDAT[14];
char CmdGPS;

int ControlSample = 0;
int ContaSample = 0;

unsigned short SumDataAN0, SumDataAN1, SumDataAN2, SumDataAN3, SumDataAN4, SumDataAN5;
unsigned short DataAN0, DataAN1, DataAN2, DataAN3, DataAN4, DataAN5;
char ValueAN0[5], ValueAN1[5], ValueAN2[5], ValueAN3[5], ValueAN4[5], ValueAN5[5];
char ValueConta[3];

unsigned char hou_uni, hou_dec, min_uni, min_dec, sec_dec, sec_uni;
unsigned char dat_uni, dat_dec, mon_uni, mon_dec, yea_dec, yea_uni;
int Second, Minute, Hour, Date, Month, Year;
unsigned char DataTime[6];