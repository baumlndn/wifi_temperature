/*
 * usart.c
 *
 *  Created on: 11.02.2015
 *      Author: baumlndn
 */

#include "gprs.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "config.h"

uint8_t GPRS_Init( void )
{
	/* retry counter */
	uint8_t retryCnt = 5;

	/* Serach string - ini end */
	char searchPattern[] = "Call Ready";

	/* Initialize SIM900 power pin */
#ifdef SIM900_POWER_USAGE

	SIM900_POWER_DDR 	|= (1<<SIM900_POWER_PIN);

#endif

#ifdef DEBUG
	DDRC = 0xFF;
#endif

	/* Initialize USART */
	USART_Init(51u);

	/* activate global interrupt */
	sei();

	_delay_ms(1000);

	/* Power on SIM900 */
#ifdef SIM900_POWER_USAGE

	SIM900_POWER_PORT |=  (1<<SIM900_POWER_PIN);
	_delay_ms(500);
	SIM900_POWER_PORT &=  ~(1<<SIM900_POWER_PIN);

#endif

	/* Initialize SIM900 */
	_delay_ms(5000);

	/* Check for init */
	while (retryCnt > 0)
	{
		/* Disable global interrupt */
		cli();

		/* If pattern was found, set retryCnt to zero */
		char tmpBuffer[255];
		(void) USART_ReadBuffer(&tmpBuffer[0],254);

		if( strstr( &tmpBuffer[0], searchPattern) != NULL)
		{
			retryCnt = 0;
		}
		else
		{
			retryCnt--;
		}

		/* Reenable interrupt */
		sei();

		/* wait for some time */
		_delay_ms(1000);
	}

	/* local variable used for error deteciton */
	uint8_t tmpError = 0;

	/* Switch echo off */
	if (tmpError == 0)
	{
#ifdef DEBUG
		tmpError = GPRS_SendConfirm("ATE1");
#else
		tmpError = GPRS_SendConfirm("ATE0");
#endif
	}

	/* Set type to GPRS */
	if (tmpError == 0)
	{
		tmpError = GPRS_SendConfirm("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
	}

	/* Set GPRS parameter */
	if (tmpError == 0)
	{
		tmpError = GPRS_SendConfirm("AT+SAPBR=3,1,\"APN\",\"internet\"");
	}

	/* wait for some time */
	_delay_ms(2000);

	/* Open GPRS context */
	retryCnt = 5;

	while ( ( retryCnt > 0 ) && (tmpError == 0) )
	{
		if ( GPRS_SendConfirm("AT+SAPBR=1,1") == 0x00)
		{
			retryCnt = 0;
		}
		else
		{
			if (retryCnt == 1)
			{
				tmpError = 0x01;
			}
			else
			{
				retryCnt--;
			}
		}
	}

	/* Wait for connection to be established */
	_delay_ms(3000);

	/* query GPRS status */
	if (tmpError == 0)
	{
		tmpError = GPRS_SendConfirm("AT+SAPBR=2,1");
	}

	return tmpError;
}

void GPRS_End ( void )
{
	(void) GPRS_SendConfirm("AT+SAPBR=0,1");

	/* Power off SIM900 */
#ifdef SIM900_POWER_USAGE

	_delay_ms(2000);
	SIM900_POWER_PORT |=  (1<<SIM900_POWER_PIN);
	_delay_ms(1000);
	SIM900_POWER_PORT &=  ~(1<<SIM900_POWER_PIN);

#endif
}

void GPRS_Send ( unsigned char * value, uint8_t length )
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

uint8_t GPRS_SendConfirm ( char * value )
{
	/* return value */
	uint8_t retVal = 1;

	/* retry varible */
	uint8_t retryCnt = 5;

	/* Send data via USART */
	GPRS_Send ((unsigned char *)value,strlen(value));

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

#ifdef DEBUG
	/* Display status in PORTC LEDs */
	if (retVal == 1)
	{
		PORTC = 0x55;
	}
	else
	{
		PORTC = 0x00;
	}
#endif //PORT_DEBUG

	return retVal;
}

uint8_t GPRS_HTTPInit ( char * url )
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

void GPRS_HTTPSend ( char * value )
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

uint8_t GPRS_HTTPRead ( char * value, uint8_t max_length )
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

void GPRS_HTTPEnd (void)
{
	(void) GPRS_SendConfirm("AT+HTTPTERM");
}
