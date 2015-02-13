/*
 * ds1820.h
 *
 *  Created on: 13.02.2015
 *      Author: baumlndn
 */

#ifndef DS1820_H_
#define DS1820_H_

uint8_t DS1820_Reset();
void DS1820_WriteOne();
void DS1820_WriteZero();
uint8_t DS1820_ReadBit();

void DS1820_Init();
void DS1820_SendByte(uint8_t data);
int16_t DS1820_GetTemperature();
void DS1820_GetTemperatureASCII( char * destBuffer);

#endif /* DS1820_H_ */
