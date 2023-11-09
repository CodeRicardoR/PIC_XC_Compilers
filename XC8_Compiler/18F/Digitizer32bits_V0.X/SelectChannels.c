/*
 * File:   SelectChannels.c
 * Author: ricar
 *
 * Created on 18 de octubre de 2022, 02:32 PM
 */


#include <xc.h>
#include "SelectChannels.h"

void SelectChannelX(void){
    
    PIN_SWA = 0;
    PIN_SWB = 1;
    PIN_A1 = 0;
    PIN_A0 = 0;
    return;
}

void SelectChannelY(void){
    
    PIN_SWA = 0;
    PIN_SWB = 1;
    PIN_A1 = 1;
    PIN_A0 = 0;
    return;
}

void SelectChannelZ(void){
    
    PIN_SWA = 1;
    PIN_SWB = 0;
    PIN_A1 = 0;
    PIN_A0 = 0;
    return;
}

void SelectChannelC(void){
    
    PIN_SWA = 1;
    PIN_SWB = 0;
    PIN_A1 = 1;
    PIN_A0 = 0;
    return;
}

void SelectChannelS(void){
    
    PIN_SWA = 1;
    PIN_SWB = 0;
    PIN_A1 = 1;
    PIN_A0 = 1;
    return;
}

void SelectChannel1(void){
    
    PIN_SWA = 0;
    PIN_SWB = 1;
    PIN_A1 = 0;
    PIN_A0 = 1;
    return;
}

void SelectChannel2(void){
    
    PIN_SWA = 0;
    PIN_SWB = 1;
    PIN_A1 = 1;
    PIN_A0 = 1;
    return;
}

void SelectChannel3(void){
    
    PIN_SWA = 1;
    PIN_SWB = 0;
    PIN_A1 = 0;
    PIN_A0 = 1;
    return;
}

void SelectChannelGND(void){
    
    PIN_SWA = 1;
    PIN_SWB = 1;
    return;
}
