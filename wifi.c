/*
 * gprs.c
 *
 *  Created on: 11.02.2015
 *      Author: baumlndn
 */

#include "wifi.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "config.h"

uint8_t WiFi_Init( void )
{
	/* retry counter */
//	uint8_t retryCnt = 5;

#ifdef WIFI_POWER_USAGE

	WIFI_POWER_DDR  |= (1<<WIFI_POWER_PIN);
	WIFI_POWER_PORT |= (1<<WIFI_POWER_PIN);

	_delay_ms(500);
#endif

	/* Initialize USART */
	USART_Init(51u);

	/* activate global interrupt */
	sei();

	_delay_ms(1000);

	WiFi_Reset();

	_delay_ms(1000);

	/* local variable used for error deteciton */
	uint8_t tmpError = 0;

	/* Switch echo off */
	if (tmpError == 0)
	{
#ifdef DEBUG
		tmpError = WiFi_SendConfirm("ATE1");
#else
		tmpError = WiFi_SendConfirm("ATE0");
#endif
	}

	return tmpError;
}

void WiFi_End ( void )
{
	WiFi_SendConfirm("AT+GSLP=10000");

#ifdef WIFI_POWER_USAGE

	WIFI_POWER_PORT &= ~(1<<WIFI_POWER_PIN);

	_delay_ms(500);
#endif
}

void WiFi_Reset ( void )
{
	char tmp[] = "AT+RST";
	WiFi_Send(&tmp[0],strlen(tmp));
	_delay_ms(2000);
}

void WiFi_Send ( char * value, uint8_t length )
{
	uint8_t idx;

	for (idx=0;idx<length;idx++)
	{
		USART_Transmit( value[idx] );
	}

	/* Transmit CR and LF */
	USART_Transmit(0x0D);
	USART_Transmit(0x0A);
}

uint8_t WiFi_SendConfirm ( char * value )
{
	/* return value */
	uint8_t retVal = 1;

	/* retry varible */
	uint8_t retryCnt = 5;

	/* Send data via USART */
	WiFi_Send (value,strlen(value));

	/* Wait for some time */
	_delay_ms(DELAY_MS_DEFAULT);

	/* Wait and check for answer from SIM900 */
	while ( (retryCnt > 0) && (retVal == 1) )
	{
		/* Disable interrupt */
		cli();

		/* temporary copy of rx buffer */
		char tmpBuffer[255];
		uint8_t tmpCnt = USART_ReadBuffer( &tmpBuffer[0], 254);

		if (tmpCnt >= 6u)
		{
			if (
					(tmpBuffer[tmpCnt-4] == 'O') &&
					(tmpBuffer[tmpCnt-3] == 'K')
			   )
			{
				retVal = 0;
			}
		}

		/* Reenable interrupt */
		sei();

		/* decrement retryCnt */
		retryCnt--;

		/* wait for some time */
		_delay_ms(DELAY_MS_DEFAULT);
	}

	return retVal;
}
