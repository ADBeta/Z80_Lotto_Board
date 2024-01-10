/*******************************************************************************
*                 chASM - control hardware /with/ ASM
* Low level hardware control library using Direct Port Manipulation - but with
* only C functions to reduce size usage
*
* Version 0.0
* 26 Aug 2023
* (c) ADBeta
*******************************************************************************/
#include <Arduino.h>
#include "chASM2.h"

//Init the passed chasm_t struct to the pin ID passed
void chasm_init(chasm_t *ptr, uint8_t pin)
{
	//Set the internal variables using the builtin PROGMEM macros
	ptr->asm_mask_nom = digitalPinToBitMask(pin); //Bitmask of chASM Pin (PGM)
	ptr->asm_mask_inv = ~ptr->asm_mask_nom; //Invert the Bitmask to use during exec
		
	ptr->asm_port = digitalPinToPort(pin); //Port byte (PGM)
	
	ptr->asm_ddr = portModeRegister(ptr->asm_port);   //Data Direction (volatile)
	ptr->asm_opr = portOutputRegister(ptr->asm_port); //Ouput Register (volatile)
	ptr->asm_ipr = portInputRegister(ptr->asm_port);  //Input Register (volatile)
}

//Set the mode register of the pin
void chasm_setmode(chasm_t *ptr, uint8_t mode)
{
	if(mode == OUTPUT) {
		*ptr->asm_ddr |= ptr->asm_mask_nom; //DDR bit to 1
	}
	
	if(mode == INPUT) { 
		*ptr->asm_ddr &= ptr->asm_mask_inv; //DDR bit to 0
		*ptr->asm_opr &= ptr->asm_mask_inv; //Output register to 0
	}
	
	if(mode == INPUT_PULLUP) {
		*ptr->asm_ddr &= ptr->asm_mask_inv; //DDR bit to 0
		*ptr->asm_opr |= ptr->asm_mask_nom; //Output register to 1
	}
}

//NOTE chasm write is inlined. See chASM2.h
