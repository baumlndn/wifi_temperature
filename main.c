/*
 * main.c
 *
 *  Created on: 11.02.2015
 *      Author: baumlndn
 */

#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "http.h"
#include "ds1820.h"
#include <avr/interrupt.h>
#include "config.h"
#include "powersave.h"
#include "wifi.h"

int main()
{
	/* Initialize DS1820 */
	DS1820_Init();

	/* Initialize PowerSave */
	PowerSave_Init();

	/* Start sleep timer */
	PowerSave_StartTimer_s(600);

	while (1)
	{
		char txTemperature[] = "GET /new.php?temperature=+85.0 HTTP/1.0\r\n";
		/* Read current temperature */
		DS1820_GetTemperatureASCII(&txTemperature[25]);

		/* Initialize WiFi and transmit data */
		WiFi_Init();
		_delay_ms(2000);
		HTTP_Send(&txTemperature[0]);
		_delay_ms(1000);
		HTTP_End();
		_delay_ms(1000);
		WiFi_End();

		/* Put ATMeag to sleep */
		PowerSave_Sleep();

		/* ATMega will wake up here */
		PowerSave_StartTimer_s(600);
	}
	

	while (1)
	{
		/* Do nothing */
		/* This point should never be reached */
	}

	return 0;
}
