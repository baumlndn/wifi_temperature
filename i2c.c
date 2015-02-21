/*
 * usart.c
 *
 *  Created on: 11.02.2015
 *      Author: baumlndn
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "i2c.h"
#include <util/twi.h>

void I2C_Init( unsigned int twbr)
{
	TWBR = twbr;
}

uint8_t I2C_Transmit( char device_id, char * data, uint8_t length )
{
	uint8_t tmpError = 0;

	/* Send START condition */
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	/* START condition transmitted */
	while (!(TWCR & (1<<TWINT)))
	{
		/* Do nothing */
	}

	/* Check TWI status */
	if ((TWSR & 0xF8) != TW_START)
	{
		tmpError = 1;
	}

	if (!tmpError)
	{
		/* Send Slave write address */
		TWDR = device_id;
		TWCR = (1<<TWINT) | (1<<TWEN);

		/* Wait for transmission to finish */
		while (!(TWCR & (1<<TWINT)))
		{
			/* do nothing */
		}

		/* Check TWI status */
		if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
		{
			tmpError = 1;
		}
	}

	/* Load data */
	for (uint8_t idx=0;idx<length;idx++)
	{
		if (!tmpError)
		{
			TWDR = data[idx];
			TWCR = (1<<TWINT) | (1<<TWEN);

			/* Wait for transmission to finish */
			while (!(TWCR & (1<<TWINT)))
			{
				/* Do nothing */
			}

			/* Check TWI status */
			if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
			{
				tmpError = 1;
			}
		}
	}

	/* Transmit STOP condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

	return tmpError;
}

unsigned char I2C_Receive( void )
{
	return 0x00;
}
