# wifi_temperature
Project to send data from ATMega328p via ESP8266 to server using PHP and GET data

File description:

config.h - configuration (power pin connection, debug support activated)
ds1337.* - DS1337 RTC clock initialization
ds1820.* - DS1820 temperature sensor functions
http.* - HTTP protocol stuff
i2c.* - basic I2C functionality for DS1337
main.c - main program
powersave.* - Timer2 asynchronous configuration for sleep mode
usart.* - ATMeag328p USART functions based on manual
wifi.* - ESP8266 functions
