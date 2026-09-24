/*
 * BUTTON_Program.c
 *
 *  Created on: Sep 23, 2026
 *      Author: TEESLA
 */
#include "BUTTON_Interface.h"
#include "BUTTON_Config.h"

#include "../MCAL/DIO/DIO_Interface.h"


void BUTTON_Init(void)
{
    /* Configure Button Pin as Input */
    DIO_SetPinDirection(BUTTON_PORT,
                        BUTTON_PIN,
                        DIO_INPUT);

    /* Enable Internal Pull-Up */
    DIO_SetPinValue(BUTTON_PORT,
                    BUTTON_PIN,
                    DIO_HIGH);
}


u8 BUTTON_GetState(void)
{
    u8 Local_u8State;

    DIO_GetPinValue(BUTTON_PORT,
                    BUTTON_PIN,
                    &Local_u8State);

    if(Local_u8State == DIO_LOW)
    {
        return 1;
    }

    return 0;
}

