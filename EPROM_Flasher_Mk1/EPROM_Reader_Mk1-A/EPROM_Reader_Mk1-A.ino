/*******************************************************************************
* This is an extremely basic EPROM Reader using an Arduino Mega, it reads each
* byte, and prints it to the serial device as a raw binary byte. This is then
* caught on the host PC by using
*    (stty raw; cat > read.bin) < /dev/ttyACM0
*
* ADBeta (c) 11 Jan 2024
*******************************************************************************/
#include "turbo595.h"
#include "chASM2.h"

#include <string.h>

#define DATA 11
#define SCLK 12
#define RCLK 13

turbo595_t addr_sr;
#define MAX_ADDR 0x8000

chasm_t data[8];

void setup()
{
	Serial.begin(115200);

	//Initialize the shift register
	turbo595_init(&addr_sr, SCLK, RCLK, DATA);
	
	for(uint8_t pin = 0; pin < 8; pin++) {
		chasm_init(&data[pin], 46 + pin);
		chasm_setmode(&data[pin], INPUT);
	}
	
	for(uint16_t addr = 0; addr < MAX_ADDR; addr++) {
		//Write the address to the shift register
		turbo595_write_16(&addr_sr, addr);
		
		//Get the data from the bus and storw it in the data_byte variable
		uint8_t data_byte = 0;
		for(uint8_t pin = 0; pin < 8; pin++) {
			uint8_t bit = chasm_read(&data[pin]);
			
			data_byte |= bit << pin;
		}
		Serial.write(data_byte);	
	}
}

void loop() 
{
}
