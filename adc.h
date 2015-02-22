/*
 * adc.h
 *
 *  Created on: 11.02.2015
 *      Author: baumlndn
 */

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void ADC_Init ( void );
uint16_t ADC_Read_Ch0 ( void );
uint16_t ADC_Read_Ch0_Avg (uint8_t samples);
void ADC_End(void);
#endif /* ADC_H_ */
