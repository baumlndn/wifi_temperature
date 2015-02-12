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
			GPRS_HTTPRead("aaa",3);

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
