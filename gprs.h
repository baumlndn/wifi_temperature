/*
 * gprs.h
 *
 *  Created on: 12.02.2015
 *      Author: baumlndn
 */

#ifndef GPRS_H_
#define GPRS_H_

#include "usart.h"
#include <avr/io.h>

uint8_t GPRS_Init( void );
void GPRS_End (void);

void GPRS_Send ( unsigned char * value, uint8_t length );
uint8_t GPRS_SendConfirm ( char * value );

uint8_t GPRS_HTTPInit ( char * url );
void GPRS_HTTPSend ( char * value );
uint8_t GPRS_HTTPRead ( char * value, uint8_t max_length );
void GPRS_HTTPEnd (void);

#endif /* GPRS_H_ */
