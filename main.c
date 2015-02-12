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
#include <avr/interrupt.h>

int main()
{
	/* Power on and initialize SIM900 */
	if ( GPRS_Init() == 0x00 )
	{
		/* If init was successful, initialize HTTP */
		if	( GPRS_HTTPInit("http://google.de") == 0x00 )
		{
			/* If HTTP-Init was successful, send out data */
			GPRS_HTTPSend("value=55");

			/* Read return data from server */
			char tmpArray[255];
			uint8_t tmpCount = GPRS_HTTPRead(&tmpArray[0],254);

			USART_Transmit('#');
			USART_Transmit(tmpCount + 0x30);

			for (uint8_t i=0;i<tmpCount;i++)
			{
				USART_Transmit(tmpArray[i]);
			}
			USART_Transmit('\n');

			/* Terminate HTTP session */
			GPRS_HTTPEnd();
		}

		/* Terminate GPRS session and power SIM900 off */
		GPRS_End();
 	}

	while (1)
	{
	}

	return 0;
}
