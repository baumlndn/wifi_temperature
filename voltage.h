/*
 * voltage.h
 *
 *  Created on: 22.02.2015
 *      Author: baumlndn
 */

#ifndef VOLTAGE_H_
#define VOLTAGE_H_

uint16_t Voltage_Read_mV ( void );

void Voltage_to_ASCII_mV (char * out, uint16_t in);
void Voltage_to_ASCII_V  (char * out, uint16_t in);

#endif /* VOLTAGE_H_ */
