/*
 * http.c
 *
 *  Created on: 16.02.2015
 *      Author: baumlndn
 */

#include "http.h"
#include "usart.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "config.h"
#include "wifi.h"

void HTTP_Send ( char * value )
{
	/* local variable used for error detection */
	uint8_t tmpError = 0;

	/* Init string to be sent */
	char initString[] = "AT+CIPSEND=000";

	/* convert length to string */
	uint8_t tmp_length = strlen(value) + 2;

	if (tmp_length > 100)
	{
		initString[11] = ( (tmp_length / 100) + 0x30 );
	}
	tmp_length = (tmp_length % 100);

	if (tmp_length > 10)
	{
		initString[12] = ( (tmp_length / 10) + 0x30 );
	}
	tmp_length = (tmp_length % 10);

	initString[13] = ( (tmp_length) + 0x30 );

	WiFi_SendConfirm("AT+CIPSTART=\"TCP\",\"192.168.1.10\",82");

	WiFi_Send(&(initString[0]),strlen(initString));
	_delay_ms(500);

	if (tmpError == 0x00)
	{
		uint8_t idx;
		for (idx=0;idx<strlen(value);idx++)
		{
			USART_Transmit(value[idx]);
		}
		USART_Transmit(0x0D);
		USART_Transmit(0x0A);
		_delay_ms(500);

	}
}

void HTTP_End (void)
{
	(void) WiFi_SendConfirm("AT+CIPCLOSE");
}
