
#include <xc.h>
#include "main.h"
#include "LIB_ADS7825.h"


//---------------------------------------------------------------
//---------------------------------------------------------------
//Function to Read ADC after conversion
signed short ReadDataADC(void){
    unsigned char i;
    signed short DataADC;
    
    PIN_CS = 1;
    PIN_RC = 1;
    PIN_CLK = 0;
    __delay_us(25);     //wait to ADC
    //-------------------------------------
    //Start Samplig and hold:
    PIN_RC = 0;
    PIN_CS = 0;
    while(PIN_READY);
    PIN_CS = 1;
    PIN_RC = 1;
    while(!PIN_READY);
    //-------------------------------------
    //Start Data reading -> sync pulse
    PIN_CS = 0;
    PIN_CLK = 1;
    while(PIN_READY);
    PIN_CLK = 0;
    PIN_CLK = 1;
    while(!PIN_READY);
    //-------------------------------------
    // Reading 16 Data bits
    DataADC = 0;
    for (i = 0;i < 15; i++){
        PIN_CLK = 0;
        NOP();
        if (PIN_SDI == 1){
            DataADC = DataADC | 0x0001;
        }
        PIN_CLK = 1;
        DataADC <<= 1;
    }
    PIN_CLK = 0;
    NOP();
    if (PIN_SDI == 1){
        DataADC = DataADC | 0x0001;
    }
    //-----------
    PIN_CS = 1;
    return DataADC;
}