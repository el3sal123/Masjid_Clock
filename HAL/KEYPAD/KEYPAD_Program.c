/*
 * KEYPAD_Program.c
 *
 *  Created on: Sep 24, 2026
 *      Author: TEESLA
 */
#include "KEYPAD_Interface.h"
#include "KEYPAD_Config.h"

#include "../../MCAL/DIO/DIO_Interface.h"

#include <util/delay.h>


/* Keypad Mapping */
static const u8 KEYPAD_Keys[4][4] =
{
    {'7', '8', '9', '/'},
    {'4', '5', '6', 'x'},
    {'1', '2', '3', '-'},
    {'D', '0', '=', '+'}
};

/* Initialize Keypad */
void KEYPAD_Init(void)
{
    /* Rows → Output */
    DIO_SetPinDirection(KEYPAD_PORT, KEYPAD_R0, DIO_OUTPUT);
    DIO_SetPinDirection(KEYPAD_PORT, KEYPAD_R1, DIO_OUTPUT);
    DIO_SetPinDirection(KEYPAD_PORT, KEYPAD_R2, DIO_OUTPUT);
    DIO_SetPinDirection(KEYPAD_PORT, KEYPAD_R3, DIO_OUTPUT);

    /* Columns → Input */
    DIO_SetPinDirection(KEYPAD_PORT, KEYPAD_C0, DIO_INPUT);
    DIO_SetPinDirection(KEYPAD_PORT, KEYPAD_C1, DIO_INPUT);
    DIO_SetPinDirection(KEYPAD_PORT, KEYPAD_C2, DIO_INPUT);
    DIO_SetPinDirection(KEYPAD_PORT, KEYPAD_C3, DIO_INPUT);

    /* Enable internal Pull-up resistors */
    DIO_SetPinValue(KEYPAD_PORT, KEYPAD_C0, DIO_HIGH);
    DIO_SetPinValue(KEYPAD_PORT, KEYPAD_C1, DIO_HIGH);
    DIO_SetPinValue(KEYPAD_PORT, KEYPAD_C2, DIO_HIGH);
    DIO_SetPinValue(KEYPAD_PORT, KEYPAD_C3, DIO_HIGH);

    /* Rows initially HIGH */
    DIO_SetPinValue(KEYPAD_PORT, KEYPAD_R0, DIO_HIGH);
    DIO_SetPinValue(KEYPAD_PORT, KEYPAD_R1, DIO_HIGH);
    DIO_SetPinValue(KEYPAD_PORT, KEYPAD_R2, DIO_HIGH);
    DIO_SetPinValue(KEYPAD_PORT, KEYPAD_R3, DIO_HIGH);
}


/* Get pressed key */
u8 KEYPAD_GetKey(void)
{
    u8 Local_u8Row;
    u8 Local_u8Column;
    u8 Local_u8ColumnValue;

    u8 Local_u8Key = 0;


    /* Scan each row */
    for(Local_u8Row = 0;
        Local_u8Row < 4;
        Local_u8Row++)
    {
        /* Make all rows HIGH */
        DIO_SetPinValue(KEYPAD_PORT, KEYPAD_R0, DIO_HIGH);
        DIO_SetPinValue(KEYPAD_PORT, KEYPAD_R1, DIO_HIGH);
        DIO_SetPinValue(KEYPAD_PORT, KEYPAD_R2, DIO_HIGH);
        DIO_SetPinValue(KEYPAD_PORT, KEYPAD_R3, DIO_HIGH);


        /* Make current row LOW */
        DIO_SetPinValue(KEYPAD_PORT,
                        Local_u8Row,
                        DIO_LOW);


        /* Check columns */
        for(Local_u8Column = 0;
            Local_u8Column < 4;
            Local_u8Column++)
        {
            DIO_GetPinValue(KEYPAD_PORT,
                            KEYPAD_C0 + Local_u8Column,
                            &Local_u8ColumnValue);


            /* Key pressed */
            if(Local_u8ColumnValue == DIO_LOW)
            {
                Local_u8Key =
                    KEYPAD_Keys[Local_u8Row][Local_u8Column];


                /* Debounce */
                _delay_ms(20);


                /* Wait until key is released */
                do
                {
                    DIO_GetPinValue(
                        KEYPAD_PORT,
                        KEYPAD_C0 + Local_u8Column,
                        &Local_u8ColumnValue
                    );

                } while(Local_u8ColumnValue == DIO_LOW);


                return Local_u8Key;
            }
        }
    }


    return 0;
}


