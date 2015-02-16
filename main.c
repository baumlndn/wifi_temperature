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
#include "http.h"
#include "ds1820.h"
#include <avr/interrupt.h>
#include "config.h"
#include "powersave.h"

int main()
{
	uint8_t tmpRetry = 2;
	char txTemperature[] = "value=+00.0";

	/* Initialize PowerSave */
	PowerSave_Init();
	PowerSave_StartTimer_s(600);
	DDRC |= 0x02;
	PORTC &= ~(0x02);

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
				HTTP_Send(&txTemperature[0]);

				/* Read return data from server */
				char tmpArray[255];

#ifdef DEBUG
				uint8_t tmpCount = HTTP_Read(&tmpArray[0],254);

				USART_Transmit(tmpCount + 0x30);

				for (uint8_t i=0;i<tmpCount;i++)
				{
					USART_Transmit(tmpArray[i]);
				}

				USART_Transmit('\n');

#else

				(void) HTTP_Read(&tmpArray[0],254);

#endif

				/* Terminate HTTP session */
				HTTP_End();
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

	PORTC |=  (0x02);
	PowerSave_Sleep();
	PowerSave_Reset();

	while (1)
	{
		/* Do nothing */
		/* This point should never be reached */
	}

	return 0;
}
