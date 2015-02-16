/*
 * http.h
 *
 *  Created on: 16.02.2015
 *      Author: baumlndn
 */

#ifndef HTTP_H_
#define HTTP_H_

#include <avr/io.h>

uint8_t HTTP_Init ( char * url );
void HTTP_Send ( char * value );
uint8_t HTTP_Read ( char * value, uint8_t max_length );
void HTTP_End (void);

#endif /* GPRS_H_ */
