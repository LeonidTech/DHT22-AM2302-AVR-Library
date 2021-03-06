// 
// Library for communicating with DHT22 and AM2302 temperature and humidity sensors using pin polling.
// Author : Leonid (DLM)
//
// Update your PORT and pin configuration for your device in the beginning of this header.
// Create DHT22 object in program, call readData function, then you can access the updated values using provided functions.
// readData needs to be called every time you want to update the temp and humidity values, it will return -1 if error occurs.
//


//prevent multiple inclusions of header file
#ifndef DHT22_AM2302_V2_H
#define DHT22_AM2302_V2_H

//setup DHT22 Port only (-CONFIGURATION-) <- You should only have to edit these 3 defines: (data input pin number is specified when object is initialized)
#define DHT_DDR DDRD			// Data Direction Register (DDRX)
#define DHT_PORT PORTD			// Pin Output Register (PORTX)		
#define DHT_PIN PIND			// Pin Input Register (PINX)

//timeout retries
#define DHT_TIMEOUT 200			// Used to timeout of trying to read sensor if error occurs.

class DHT22 //Class for DHT22 sensor to read and return data.
{
public:
	DHT22();					// Constructor initializes private variables and pin to 0.
	DHT22(int);					// Constructor to select inputPin for the DHT22.
	int8_t readData();			// acquire data from sensor, this function updates the Temp and Humidity values.
	float gettemperatureC();	// returns float temperature in Celsius
	float gettemperatureF();	// returns float temperature in Fahrenheit
	float gethumidity();		// returns float Humidity
private:
	float temperature;			// Float variable to store Temperature in Celsius
	float humidity;				// Float variable to store Relative Humidity 
	uint8_t intputPin;			// integer to store the Input Pin being used for the sensor
};

#endif

/*
Mesiti Electronics 2017
*/