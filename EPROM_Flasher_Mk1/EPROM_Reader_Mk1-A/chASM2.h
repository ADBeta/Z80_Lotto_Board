/*******************************************************************************
*                 chASM - control hardware /with/ ASM
* Low level hardware control library using Direct Port Manipulation - but with
* only C functions to reduce size usage
*
* (c) ADBeta
*******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHASM2_H
#define CHASM2_H

#include <Arduino.h>

typedef struct {
	/*** NOTE ******************************************************************
	Removing the bitwise invert calculatons for the mask per call saves a few 
	microseconds, and reduces the inlined overhead by a few bytes per call. 
	***************************************************************************/
	uint8_t asm_mask_nom;
	uint8_t asm_mask_inv;
	
	uint8_t asm_port;
	
	volatile uint8_t *asm_ddr; //Data Direction Register (for input/output)
	volatile uint8_t *asm_opr; //Output register for the pin
	volatile uint8_t *asm_ipr; //Input register for the pin
} chasm_t;


//Init the passed chasm_t struct to the pin ID passed
void chasm_init(chasm_t *, uint8_t); 

//Set the mode of the passed chasm_t struct
void chasm_setmode(chasm_t *, uint8_t);


//Writes a binary state to the pin
static inline void chasm_write(chasm_t *, bool) __attribute__((always_inline));
void chasm_write(chasm_t *ptr, bool state)
{
	if(state != 0) {
		*ptr->asm_opr |= ptr->asm_mask_nom; //Set mask bit in output register to HIGH
		return;
	}
	
	*ptr->asm_opr &= ptr->asm_mask_inv; //Set mask bit in output register to LOW
}

//Reads and returns a binary state from the pin
static inline bool chasm_read(chasm_t *) __attribute__((always_inline));
bool chasm_read(chasm_t *ptr)
{
	//Read the input register and mask it. Simple comparation is faster than	
	//greater than/less than etc. If compare with 0 is true, the pin is LOW
	if((*ptr->asm_ipr & ptr->asm_mask_nom) == 0) {
		return 0;
	}
	
	//Return 1 if bit is not 0
	return 1;
}

#endif
#ifdef __cplusplus
}
#endif
