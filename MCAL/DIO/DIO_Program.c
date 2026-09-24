#include <avr/io.h>
#include "DIO_Interface.h"

#include "../../LIB/BIT_MATH.h"


void DIO_SetPortDirection(u8 Copy_u8Port, u8 Copy_u8Direction)
{
    switch(Copy_u8Port)
    {
        case DIO_PORTA:
            DDRA = Copy_u8Direction;
            break;

        case DIO_PORTB:
            DDRB = Copy_u8Direction;
            break;

        case DIO_PORTC:
            DDRC = Copy_u8Direction;
            break;

        case DIO_PORTD:
            DDRD = Copy_u8Direction;
            break;
    }
}


void DIO_SetPortValue(u8 Copy_u8Port, u8 Copy_u8Value)
{
    switch(Copy_u8Port)
    {
        case DIO_PORTA:
            PORTA = Copy_u8Value;
            break;

        case DIO_PORTB:
            PORTB = Copy_u8Value;
            break;

        case DIO_PORTC:
            PORTC = Copy_u8Value;
            break;

        case DIO_PORTD:
            PORTD = Copy_u8Value;
            break;
    }
}


void DIO_SetPinDirection(u8 Copy_u8Port,
                         u8 Copy_u8Pin,
                         u8 Copy_u8Direction)
{
    switch(Copy_u8Port)
    {
        case DIO_PORTA:

            if(Copy_u8Direction == DIO_OUTPUT)
            {
                SET_BIT(DDRA, Copy_u8Pin);
            }
            else
            {
                CLR_BIT(DDRA, Copy_u8Pin);
            }

            break;


        case DIO_PORTB:

            if(Copy_u8Direction == DIO_OUTPUT)
            {
                SET_BIT(DDRB, Copy_u8Pin);
            }
            else
            {
                CLR_BIT(DDRB, Copy_u8Pin);
            }

            break;


        case DIO_PORTC:

            if(Copy_u8Direction == DIO_OUTPUT)
            {
                SET_BIT(DDRC, Copy_u8Pin);
            }
            else
            {
                CLR_BIT(DDRC, Copy_u8Pin);
            }

            break;


        case DIO_PORTD:

            if(Copy_u8Direction == DIO_OUTPUT)
            {
                SET_BIT(DDRD, Copy_u8Pin);
            }
            else
            {
                CLR_BIT(DDRD, Copy_u8Pin);
            }

            break;
    }
}


void DIO_SetPinValue(u8 Copy_u8Port,
                     u8 Copy_u8Pin,
                     u8 Copy_u8Value)
{
    switch(Copy_u8Port)
    {
        case DIO_PORTA:

            if(Copy_u8Value == DIO_HIGH)
            {
                SET_BIT(PORTA, Copy_u8Pin);
            }
            else
            {
                CLR_BIT(PORTA, Copy_u8Pin);
            }

            break;


        case DIO_PORTB:

            if(Copy_u8Value == DIO_HIGH)
            {
                SET_BIT(PORTB, Copy_u8Pin);
            }
            else
            {
                CLR_BIT(PORTB, Copy_u8Pin);
            }

            break;


        case DIO_PORTC:

            if(Copy_u8Value == DIO_HIGH)
            {
                SET_BIT(PORTC, Copy_u8Pin);
            }
            else
            {
                CLR_BIT(PORTC, Copy_u8Pin);
            }

            break;


        case DIO_PORTD:

            if(Copy_u8Value == DIO_HIGH)
            {
                SET_BIT(PORTD, Copy_u8Pin);
            }
            else
            {
                CLR_BIT(PORTD, Copy_u8Pin);
            }

            break;
    }
}


void DIO_GetPinValue(u8 Copy_u8Port,
                     u8 Copy_u8Pin,
                     u8 *Copy_pu8Value)
{
    switch(Copy_u8Port)
    {
        case DIO_PORTA:

            *Copy_pu8Value = GET_BIT(PINA, Copy_u8Pin);

            break;


        case DIO_PORTB:

            *Copy_pu8Value = GET_BIT(PINB, Copy_u8Pin);

            break;


        case DIO_PORTC:

            *Copy_pu8Value = GET_BIT(PINC, Copy_u8Pin);

            break;


        case DIO_PORTD:

            *Copy_pu8Value = GET_BIT(PIND, Copy_u8Pin);

            break;
    }
}

