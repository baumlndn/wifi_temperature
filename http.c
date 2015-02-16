/*
 * http.c
 *
 *  Created on: 16.02.2015
 *      Author: baumlndn
 */

#include "http.h"
#include "usart.h"
#include "gprs.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "config.h"

uint8_t HTTP_Init ( char * url )
{
	/* local variable used for error detection */
	uint8_t tmpError = 0;

	/* command prefix */
	char tmpFullString[255];
	char tmpPrefix[] = "AT+HTTPPARA=\"URL\",\"";

	strcpy(&(tmpFullString[0]),tmpPrefix);

	if ( (strlen(tmpPrefix) + strlen(url)) <= 250)
	{
		strcpy(&(tmpFullString[strlen(tmpPrefix)]),url);
	}

	strcpy(&(tmpFullString[strlen(tmpFullString)]),"\"");

	/* Init HTTP service */
	tmpError = GPRS_SendConfirm("AT+HTTPINIT");

	/* Set HTTP parameter */
	if (tmpError == 0)
	{
		tmpError = GPRS_SendConfirm(&(tmpFullString[0]));
	}

	if (tmpError == 0)
	{
		tmpError = GPRS_SendConfirm("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"");
	}

	return tmpError;
}

void HTTP_Send ( char * value )
{
	/* local variable used for error detection */
	uint8_t tmpError = 0;

	/* Init string to be sent */
	char initString[] = "AT+HTTPDATA=000,10000";

	/* convert length to string */
	uint8_t tmp_length = strlen(value);

	if (tmp_length > 100)
	{
		initString[12] = ( (tmp_length / 100) + 0x30 );
	}
	tmp_length = (tmp_length % 100);

	if (tmp_length > 10)
	{
		initString[13] = ( (tmp_length / 10) + 0x30 );
	}
	tmp_length = (tmp_length % 10);

	initString[14] = ( (tmp_length) + 0x30 );

	GPRS_Send((unsigned char *) &(initString[0]),strlen(initString));
	_delay_ms(2000);

	if (tmpError == 0x00)
	{
		uint8_t idx;
		for (idx=0;idx<strlen(value);idx++)
		{
			USART_Transmit(value[idx]);
		}
		_delay_ms(500);

		/* Send data to server */
		(void) GPRS_SendConfirm("AT+HTTPACTION=1");
		_delay_ms(5000);
	}
}

uint8_t HTTP_Read ( char * value, uint8_t max_length )
{
	uint8_t tmpReturn = 0;

	if (GPRS_SendConfirm("AT+HTTPREAD") == 0x00)
	{
		char tmpBuffer[255];
		uint8_t tmpLength = 0;
		uint8_t tmpCount = USART_ReadBuffer (&tmpBuffer[0],254);
		tmpBuffer[tmpCount] = '\0';

		char * tmpStart = strstr(&tmpBuffer[0],":");
		tmpStart++;
		char * tmpEnd = &tmpStart[0];
		while (*tmpEnd != 0x0D)
		{
			tmpEnd++;
			tmpLength++;
		}

		switch (tmpLength)
		{
		case 1:
		{
			tmpReturn = tmpStart[0] - 0x30;
		}
		break;
		case 2:
		{
			tmpReturn = ((tmpStart[0] - 0x30) * 10) + (tmpStart[1] - 0x30);
		}
		break;
		case 3:
		{
			tmpReturn = ((tmpStart[0] - 0x30) * 100) + ((tmpStart[1] - 0x30) * 10) + (tmpStart[2] - 0x30);
		}
		break;
		default:
		{
			/* Do nothing */
		}
		break;
		}

		tmpEnd++;
		tmpEnd++;

		for (int i=0;i<tmpReturn;i++)
		{
			value[i] = tmpEnd[i];
		}
	}

	return tmpReturn;
}

void HTTP_End (void)
{
	(void) GPRS_SendConfirm("AT+HTTPTERM");
}
