/*
 * ds1820.c
 *
 *  Created on: 13.02.2015
 *      Author: baumlndn
 */

#include <avr/io.h>
#include <util/delay.h>

#include "ds1820.h"
#include "config.h"


uint8_t DS1820_Reset()
{
	uint8_t retVal = 1;
	DS1820_PORT &= ~(1<<DS1820_BIT);
	_delay_us(500);
	DS1820_PORT |=  (1<<DS1820_BIT);
	_delay_us(100);
	if (DS1820_PIN & (1<<DS1820_BIT))
	{
		retVal = 1;
	}
	else
	{
		retVal = 0;
	}
	_delay_us(500);
	return retVal;
}

void DS1820_WriteOne()
{
	DS1820_PORT &= ~(1<<DS1820_BIT);
	_delay_us(5);
	DS1820_PORT |=  (1<<DS1820_BIT);
	_delay_us(95);
}

void DS1820_WriteZero()
{
	DS1820_PORT &= ~(1<<DS1820_BIT);
	_delay_us(65);
	DS1820_PORT |=  (1<<DS1820_BIT);
	_delay_us(35);
}

uint8_t DS1820_ReadBit()
{
	uint8_t retVal;
	DS1820_PORT &= ~(1<<DS1820_BIT);
	_delay_us(3);
	DS1820_PORT |=  (1<<DS1820_BIT);
	_delay_us(10);
	if (DS1820_PIN & (1<<DS1820_BIT))
	{
		retVal = 1;
	}
	else
	{
		retVal = 0;
	}
	_delay_us(82);
	return retVal;
}

void DS1820_Init()
{
	DS1820_DDR  |= (1<<DS1820_BIT);
	DS1820_PORT |= (1<<DS1820_BIT);
	_delay_ms(10);
}

void DS1820_SendByte(uint8_t data)
{
	uint8_t idx;
	for (idx=0;idx<8;idx++)
	{
		if (data & (1<<idx))
		{
			DS1820_WriteOne();
		}
		else
		{
			DS1820_WriteZero();
		}
	}
}

int16_t DS1820_GetTemperature()
{
	uint16_t retVal = 0;
	if (!DS1820_Reset())
	{
		/* Skip ROM */
		DS1820_SendByte(0xCC);
		/* Convert */
		DS1820_SendByte(0x44);
		/* wait for some time */
		_delay_ms(2000);
		if (!DS1820_Reset())
		{
			/* Skip ROM */
			DS1820_SendByte(0xCC);
			/* Read Scratchpad */
			DS1820_SendByte(0xBE);

			uint8_t idx;
			for (idx=0;idx<16;idx++)
			{
				retVal |= (DS1820_ReadBit()<<idx);
			}

			DS1820_Reset();
		}
	}

	if (retVal & 0xFF00)
	{
		return (0 - ( (uint8_t)retVal) + 1 );
	}
	else
	{
		return ((uint8_t)retVal);
	}

}

void DS1820_GetTemperatureASCII( char * destBuffer)
{
	int16_t tmpRead = DS1820_GetTemperature();

	if (tmpRead < 0)
	{
		destBuffer[0] = '-';
		tmpRead = (0-tmpRead);
	}
	else
	{
		destBuffer[0] = '+';
	}

	if (tmpRead & 0x0001)
	{
		destBuffer[4] = '5';
	}
	else
	{
		destBuffer[4] = '0';
	}

	destBuffer[3] = '.';

	tmpRead = ((tmpRead & 0x00FF)>>1);

	destBuffer[2] = (tmpRead % 10) + 0x30;
	destBuffer[1] = (tmpRead / 10) + 0x30;
}
