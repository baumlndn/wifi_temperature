/*
 * adc.c
 *
 *  Created on: 11.02.2015
 *      Author: baumlndn
 */

#include <avr/io.h>
#include "adc.h"

void ADC_Init(void) {

	/* use AREF pin */
	ADMUX = (0<<REFS1) | (0<<REFS0) | (0000<<MUX0);

	/* Set ADC to single conversion - prescaler 64 */
	ADCSRA  = ((1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0));
	ADCSRA |= (1<<ADEN);

	/* Read Dummy */
	ADCSRA |= (1<<ADSC);

	while (ADCSRA & (1<<ADSC) )
	{
		/* wait for conversion to finish */
	}

	/*Read out result to enable overwriting for new result */
	(void) ADCW;
}

void ADC_End(void)
{
	ADCSRA &= ~(1<<ADEN);
}

uint16_t ADC_Read_Ch0 ( void )
{
	ADCSRA |= (1<<ADSC);

	while (ADCSRA & (1<<ADSC) )
	{
		/* wait for conversion to finish */
	}

	/*Read out result */
	return ADCW;
}

uint16_t ADC_Read_Ch0_Avg (uint8_t samples)
{
	uint32_t tmpSum = 0;

	for (uint8_t n=0;n<samples;n++)
	{
		tmpSum = tmpSum + ADC_Read_Ch0();
	}

	return (uint16_t) (tmpSum/samples);
}
