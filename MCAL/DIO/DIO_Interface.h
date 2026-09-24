/*
 * DIO_Interface.h
 *
 *  Created on: Sep 23, 2026
 *      Author: TEESLA
 */

#ifndef MCAL_DIO_DIO_INTERFACE_H_
#define MCAL_DIO_DIO_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"

#define DIO_PORTA  0
#define DIO_PORTB  1
#define DIO_PORTC  2
#define DIO_PORTD  3

#define DIO_INPUT   0
#define DIO_OUTPUT  1

#define DIO_LOW     0
#define DIO_HIGH    1


void DIO_SetPortDirection(u8 Copy_u8Port, u8 Copy_u8Direction);

void DIO_SetPortValue(u8 Copy_u8Port, u8 Copy_u8Value);


void DIO_SetPinDirection(u8 Copy_u8Port,
                         u8 Copy_u8Pin,
                         u8 Copy_u8Direction);


void DIO_SetPinValue(u8 Copy_u8Port,
                     u8 Copy_u8Pin,
                     u8 Copy_u8Value);


void DIO_GetPinValue(u8 Copy_u8Port,
                     u8 Copy_u8Pin,
                     u8 *Copy_pu8Value);


#endif /* MCAL_DIO_DIO_INTERFACE_H_ */
