/*
 * ds1337.c
 *
 *  Created on: 21.02.2015
 *      Author: baumlndn
 */


#include <avr/io.h>
#include <util/delay.h>
#include "ds1337.h"
#include "i2c.h"


void DS1337_Init ( void )
{
	/* DS1337 write address */
	char i2c_ds1337_adr = 0xD0;

	/* DS1337 control register set to 0 -> 1 Hz output at INTB */
	char i2c_data[2] = { 0x0E , 0x00 };

	/* Initialize I2C with SCL = 100kHz */
	I2C_Init(32);
	_delay_ms(500);

	I2C_Transmit(i2c_ds1337_adr,&i2c_data[0],2);
	_delay_ms(500);
}
