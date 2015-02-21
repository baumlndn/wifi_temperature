/*
 * gprs.h
 *
 *  Created on: 12.02.2015
 *      Author: baumlndn
 */

#ifndef WIFI_H_
#define WIFI_H_

#include "usart.h"
#include <avr/io.h>

uint8_t WiFi_Init( void );
void WiFi_End (void);

void WiFi_Send ( char * value, uint8_t length );
uint8_t WiFi_SendConfirm ( char * value );

void WiFi_Reset ( void );

#endif /* WIFI_H_ */
