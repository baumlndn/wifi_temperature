/*
 * powersave.h
 *
 *  Created on: 15.02.2015
 *      Author: baumlndn
 */

#ifndef POWERSAVE_H_
#define POWERSAVE_H_

#include <avr/wdt.h>

void PowerSave_Init( void );
void PowerSave_StartTimer_s( uint16_t sec);
void PowerSave_Sleep( void );

// Function Pototype
void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

#define PowerSave_Reset()        \
do                          \
{                           \
    wdt_enable(WDTO_15MS);  \
    for(;;)                 \
    {                       \
    }                       \
} while(0)


#endif /* POWERSAVE_H_ */
