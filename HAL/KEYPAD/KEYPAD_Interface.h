/*
 /*
 * KEYPAD_Interface.h
 *
 *  Created on: Sep 24, 2026
 *      Author: TEESLA
 */

#ifndef HAL_KEYPAD_KEYPAD_INTERFACE_H_
#define HAL_KEYPAD_KEYPAD_INTERFACE_H_
#include "../../LIB/STD_TYPES.h"


/* Initialize Keypad */
void KEYPAD_Init(void);


/* Get pressed key */
u8 KEYPAD_GetKey(void);


#endif /* HAL_KEYPAD_KEYPAD_INTERFACE_H_ */
