/*
 * voltage.c
 *
 *  Created on: 22.02.2015
 *      Author: baumlndn
 */


#include <avr/io.h>
#include "voltage.h"
#include "adc.h"

uint16_t Voltage_Read_mV ( void )
{
	uint16_t tmpResult = 0;

	/* Activate ADC */
	ADC_Init();

	/* Read out voltage */
	tmpResult = ADC_Read_Ch0_Avg(10);

	/* convert raw to voltage value */
	float tmpResult_f = (float) (tmpResult);
	tmpResult_f = tmpResult_f / 1024.0f;

	tmpResult = (uint16_t) (tmpResult_f * 3300.0f * 86.2f / 17.6f);

	/* Deactivate ADC */
	ADC_End();

	return tmpResult;
}

void Voltage_to_ASCII_mV (char * out, uint16_t in)
{
	out[0] = (in / 10000) + 0x30;
	in = in % 10000;

	out[1] = (in /  1000) + 0x30;
	in = in % 1000;

	out[2] = (in /   100) + 0x30;
	in = in % 100;

	out[3] = (in /    10) + 0x30;
	in = in % 10;

	out[4] = (in        ) + 0x30;
}

void Voltage_to_ASCII_V (char * out, uint16_t in)
{
	out[0] = (in / 10000) + 0x30;
	in = in % 10000;

	out[1] = (in /  1000) + 0x30;
	in = in % 1000;

	out[2] = '.';

	out[3] = (in /   100) + 0x30;
	in = in % 100;

	out[4] = (in /    10) + 0x30;
}
