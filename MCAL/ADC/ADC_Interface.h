/*
 * ADC_Interface.h
 *
 *  Created on: Sep 23, 2026
 *      Author: TEESLA
 */

#ifndef MCAL_ADC_ADC_INTERFACE_H_
#define MCAL_ADC_ADC_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"

void ADC_Init(void);

u16 ADC_Read(u8 Copy_u8Channel);

#endif /* MCAL_ADC_ADC_INTERFACE_H_ */
