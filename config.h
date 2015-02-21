/*
 * config.h
 *
 *  Created on: 12.02.2015
 *      Author: baumlndn
 */

#ifndef CONFIG_H_
#define CONFIG_H_


#define SIM900_POWER_USAGE	1

#define SIM900_POWER_DDR	DDRD
#define SIM900_POWER_PORT	PORTD
#define SIM900_POWER_PIN	PD3

#define SIM900_RST_USAGE	1

#define SIM900_RST_DDR		DDRD
#define SIM900_RST_PORT		PORTD
#define SIM900_RST_PIN		PD2

#define WIFI_POWER_USAGE	1

#define WIFI_POWER_DDR		DDRD
#define WIFI_POWER_PORT		PORTD
#define WIFI_POWER_PIN		PD4

#define DELAY_MS_DEFAULT	500
#define DEBUG				1

#define DS1820_DDR			DDRD
#define DS1820_PORT			PORTD
#define DS1820_PIN			PIND
#define DS1820_BIT			PD7

#endif /* CONFIG_H_ */
