/*
 * main.c
 *
 *  Created on: 11.02.2015
 *      Author: baumlndn
 */

#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "gprs.h"
#include "ds1820.h"
#include <avr/interrupt.h>
#include "config.h"
#include <avr/wdt.h>

// Function Pototype
void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

// Function Implementation
void wdt_init(void)
{
    MCUSR = 0;
    wdt_disable();

    return;
}

#define soft_reset()        \
do                          \
{                           \
    wdt_enable(WDTO_15MS);  \
    for(;;)                 \
    {                       \
    }                       \
} while(0)

int main()
{
	uint8_t tmpRetry = 2;
	char txTemperature[] = "value=+00.0";

	/* Initialize DS1820 */
	DS1820_Init();

	/* Read current temperature */
	DS1820_GetTemperatureASCII(&txTemperature[6]);

	/* Power on and initialize SIM900 */

	while (tmpRetry > 0)
	{
		if ( GPRS_Init() == 0x00 )
		{
			/* If init was successful, initialize HTTP */
			if	( GPRS_HTTPInit("http://google.de") == 0x00 )
			{

				/* If HTTP-Init was successful, send out data */
				GPRS_HTTPSend(&txTemperature[0]);

				/* Read return data from server */
				char tmpArray[255];
				uint8_t tmpCount = GPRS_HTTPRead(&tmpArray[0],254);

#ifdef DEBUG
				USART_Transmit(tmpCount + 0x30);

				for (uint8_t i=0;i<tmpCount;i++)
				{
					USART_Transmit(tmpArray[i]);
				}

				USART_Transmit('\n');
#endif

				/* Terminate HTTP session */
				GPRS_HTTPEnd();
			}

			/* Terminate GPRS session and power SIM900 off */
			GPRS_End();

			tmpRetry = 0;
		}
		else
		{
			tmpRetry--;
		}
	}

	while (1)
	{
		_delay_ms(900000);
		/* Trigger Reset */
		PORTC = 0xFF;
		_delay_ms(1000);
		soft_reset();
	}

	return 0;
}
