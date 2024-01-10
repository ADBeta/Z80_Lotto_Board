#include "turbo595.h"
#include "chASM2.h"

#define DATA 11
#define SCLK 12
#define RCLK 13

turbo595_t ADDR;

void setup()
{
	//Initialize the shift register
	turbo595_init(&ADDR, SCLK, RCLK, DATA);

	for(int x = 0; x < 255; x++) {
		//Write 16bits to the Shift Register
		turbo595_write_8(&ADDR, x);
		
		delay(100);
	}

	
}

void loop() 
{
}
