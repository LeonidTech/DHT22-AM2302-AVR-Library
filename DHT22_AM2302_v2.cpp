/*
DHT22 & AM2302 Library
 Author : Leonid (DLM)
*/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "DHT22_AM2302_v2.h"

//Default Constructor
DHT22::DHT22()
{
	intputPin = 0;
	temperature = humidity = 0;
}

DHT22::DHT22(int datapin)
{
	intputPin = datapin;
	temperature = humidity = 0;
}

// get data from sensor
int8_t DHT22::readData()
{
	//*** uint8_t bits[5];
	uint8_t bits[5] = {};
	uint8_t i,j = 0;

	//reset port
	DHT_DDR |= (1<<intputPin); //output
	DHT_PORT |= (1<<intputPin); //high
	_delay_ms(100);

	//send request
	DHT_PORT &= ~(1<<intputPin); //low
	_delay_us(500);
	DHT_PORT |= (1<<intputPin); //high
	DHT_DDR &= ~(1<<intputPin); //input
	_delay_us(40);

	//check start condition 1
	if((DHT_PIN & (1<<intputPin))) {
		return -1;
	}
	_delay_us(80);
	
	//check start condition 2
	if(!(DHT_PIN & (1<<intputPin))) {
		return -1;
	}
	_delay_us(80);

	//read the data
	uint16_t timeoutcounter = 0;
	for (j=0; j<5; j++) { //read 5 byte
		uint8_t result=0;
		for(i=0; i<8; i++) {//read every bit
			timeoutcounter = 0;
			while(!(DHT_PIN & (1<<intputPin))) { //wait for an high input (non blocking)
				timeoutcounter++;
				if(timeoutcounter > DHT_TIMEOUT) {
					return -1; //timeout
				}
			}
			_delay_us(30);
			if(DHT_PIN & (1<<intputPin)) //if input is high after 30 us, get result
			result |= (1<<(7-i));
			timeoutcounter = 0;
			while(DHT_PIN & (1<<intputPin)) { //wait until input get low (non blocking)
				timeoutcounter++;
				if(timeoutcounter > DHT_TIMEOUT) {
					return -1; //timeout
				}
			}
		}
		bits[j] = result;
	}

	//reset port
	DHT_DDR |= (1<<intputPin); //output
	DHT_PORT |= (1<<intputPin); //low
	_delay_ms(100);

	//check checksum
	if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4])
	{
		//return temperature and humidity
		uint16_t rawhumidity = bits[0]<<8 | bits[1];
		uint16_t rawtemperature = bits[2]<<8 | bits[3];
		if(rawtemperature & 0x8000)
		{
			temperature = (float)((rawtemperature & 0x7FFF) / 10.0) * -1.0;
		}
		else
		{
			temperature = (float)(rawtemperature)/10.0;
		}
		humidity = (float)(rawhumidity)/10.0;
		return 0;
	}

	return -1;
}

//get temperature in Celsius
float DHT22::gettemperatureC()
{
	return temperature;
}

//get Humidity
float DHT22::gethumidity()
{
	return humidity;
}

//get temperature in Fahrenheit 
float DHT22::gettemperatureF()
{
	return (temperature * 1.8) + 32; //Return temp in F
}