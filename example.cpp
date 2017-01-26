/*
 * mega128MonV2.cpp
 *
 * Created: 1/25/2017 6:42:07 PM
 * Author : Leonid (DLM)
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "DHT22_AM2302.h"


#define UART_BAUD_RATE		19200	// Set UART baud rate for serial communication.

//Function prototypes
void init_MCU();		// Initialize core MCU values and peripherals.
void check_DHT();		// Function for checking and displaying DHT temp and humidity data

DHT22 dht22(PD6);			// Initialize DHT22 object and set the data input pin to 6 (or PD6) on preset port.

int main()
{
	init_MCU();

	for (;;) 
	{
		check_DHT();	// Read DHT22 data, retrieve values, and display in uart terminal
		_delay_ms(5000);	// Delay 5sec		
	}
	
	return 0;
}

void init_MCU()
{
	//Define Port Functions: I/O & pull-up resistor status.
	DDRA=0xff; PORTA=0xff;
	DDRB=0xff; PORTB=0xff;
	DDRC=0xff; PORTC=0xff;
	DDRD=0xff; PORTD=0xff;
	DDRE=0xff; PORTE=0xff;
	PORTE |= (1<<0);
	DDRF=0xff; PORTF=0xff;
	DDRG=0x1f; PORTG=0x1f;
	
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); // Initialize UART
	sei(); //enable global interrupts
	
	unsigned int i;
	for(i=0;i<6;i++) //blink 3 times
	{
		PORTA=~PORTA;
		_delay_ms(200);
	}
	PORTA &= ~((1<<0) | (1<<1));
	
	uart_puts("-InitComplete");
}

void check_DHT()
{
	char printbuff[100];
	
	if(dht22.readData() != -1)
	{
		dtostrf(dht22.gettemperatureF(), 3, 2, printbuff);
		uart_puts("temperature: "); uart_puts(printbuff); uart_puts(" F");uart_puts("\r\n");	// Print temperature
		
		dtostrf(dht22.gethumidity(), 3, 2, printbuff);
		uart_puts("humidity:    "); uart_puts(printbuff); uart_puts("% RH");uart_puts("\r\n");	// Print humidity

	}
	else { uart_puts("error"); uart_puts("\r\n"); }												// Print error if dht sensor is not available

	uart_puts("\r\n");																	
}
