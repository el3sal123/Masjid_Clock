/*
 * LCD_Program.c
 *
 *  Created on: Sep 24, 2026
 *      Author: TEESLA
 */
#include "LCD_Interface.h"
#include "LCD_Config.h"

#include "../../MCAL/DIO/DIO_Interface.h"

#include <util/delay.h>


/*========================================================
 * Function: LCD_Send4Bits
 * Description:
 * Send 4 bits to LCD data pins D4-D7
 *========================================================*/
static void LCD_Send4Bits(u8 Copy_u8Data)
{
    DIO_SetPinValue(LCD_DATA_PORT,
                    LCD_D4_PIN,
                    (Copy_u8Data >> 0) & 1);

    DIO_SetPinValue(LCD_DATA_PORT,
                    LCD_D5_PIN,
                    (Copy_u8Data >> 1) & 1);

    DIO_SetPinValue(LCD_DATA_PORT,
                    LCD_D6_PIN,
                    (Copy_u8Data >> 2) & 1);

    DIO_SetPinValue(LCD_DATA_PORT,
                    LCD_D7_PIN,
                    (Copy_u8Data >> 3) & 1);
}


/*========================================================
 * Function: LCD_EnablePulse
 * Description:
 * Generate Enable pulse
 *========================================================*/
static void LCD_EnablePulse(void)
{
    DIO_SetPinValue(LCD_CONTROL_PORT,
                    LCD_EN_PIN,
                    DIO_HIGH);

    _delay_us(1);

    DIO_SetPinValue(LCD_CONTROL_PORT,
                    LCD_EN_PIN,
                    DIO_LOW);

    _delay_us(100);
}


/*========================================================
 * Function: LCD_SendCommand
 * Description:
 * Send command to LCD
 * RS = 0
 *========================================================*/
void LCD_SendCommand(u8 Copy_u8Command)
{
    /* RS = 0 → Command */
    DIO_SetPinValue(LCD_CONTROL_PORT,
                    LCD_RS_PIN,
                    DIO_LOW);

    /* Send Higher 4 bits */
    LCD_Send4Bits(Copy_u8Command >> 4);

    LCD_EnablePulse();

    /* Send Lower 4 bits */
    LCD_Send4Bits(Copy_u8Command & 0x0F);

    LCD_EnablePulse();

    _delay_ms(2);
}


/*========================================================
 * Function: LCD_SendChar
 * Description:
 * Send one character
 * RS = 1
 *========================================================*/
void LCD_SendChar(u8 Copy_u8Char)
{
    /* RS = 1 → Data */
    DIO_SetPinValue(LCD_CONTROL_PORT,
                    LCD_RS_PIN,
                    DIO_HIGH);

    /* Send Higher 4 bits */
    LCD_Send4Bits(Copy_u8Char >> 4);

    LCD_EnablePulse();

    /* Send Lower 4 bits */
    LCD_Send4Bits(Copy_u8Char & 0x0F);

    LCD_EnablePulse();

    _delay_ms(1);
}


/*========================================================
 * Function: LCD_Init
 * Description:
 * Initialize LCD in 4-bit mode
 *========================================================*/
void LCD_Init(void)
{
    /*---------------- Control Pins ----------------*/

    DIO_SetPinDirection(LCD_CONTROL_PORT,
                        LCD_RS_PIN,
                        DIO_OUTPUT);

    DIO_SetPinDirection(LCD_CONTROL_PORT,
                        LCD_EN_PIN,
                        DIO_OUTPUT);


    /*---------------- Data Pins ----------------*/

    DIO_SetPinDirection(LCD_DATA_PORT,
                        LCD_D4_PIN,
                        DIO_OUTPUT);

    DIO_SetPinDirection(LCD_DATA_PORT,
                        LCD_D5_PIN,
                        DIO_OUTPUT);

    DIO_SetPinDirection(LCD_DATA_PORT,
                        LCD_D6_PIN,
                        DIO_OUTPUT);

    DIO_SetPinDirection(LCD_DATA_PORT,
                        LCD_D7_PIN,
                        DIO_OUTPUT);


    /*---------------- LCD Power-Up Delay ----------------*/

    _delay_ms(20);


    /* RS = 0 */
    DIO_SetPinValue(LCD_CONTROL_PORT,
                    LCD_RS_PIN,
                    DIO_LOW);


    /*====================================================
     * 4-Bit Initialization Sequence
     *====================================================*/

    LCD_Send4Bits(0x03);
    LCD_EnablePulse();

    _delay_ms(5);


    LCD_Send4Bits(0x03);
    LCD_EnablePulse();

    _delay_us(150);


    LCD_Send4Bits(0x03);
    LCD_EnablePulse();


    LCD_Send4Bits(0x02);
    LCD_EnablePulse();


    /*====================================================
     * LCD Configuration
     *====================================================*/

    /* 4-bit mode
       2 lines
       5x8 font
    */
    LCD_SendCommand(0x28);


    /* Display ON
       Cursor OFF
       Blink OFF
    */
    LCD_SendCommand(0x0C);


    /* Clear Display */
    LCD_SendCommand(0x01);


    /* Entry Mode
       Cursor moves right
    */
    LCD_SendCommand(0x06);
}


/*========================================================
 * Function: LCD_SendString
 * Description:
 * Send a complete string
 *========================================================*/
void LCD_SendString(const char *Copy_pcString)
{
    while(*Copy_pcString != '\0')
    {
        LCD_SendChar(*Copy_pcString);

        Copy_pcString++;
    }
}


/*========================================================
 * Function: LCD_SendNumber
 * Description:
 * Display unsigned integer number
 *========================================================*/
void LCD_SendNumber(u16 Copy_u16Number)
{
    char Local_acNumber[6];

    u8 Local_u8Index = 0;
    u8 Local_u8I;


    /* Special case: number = 0 */
    if(Copy_u16Number == 0)
    {
        LCD_SendChar('0');

        return;
    }


    /* Convert number to characters */
    while(Copy_u16Number > 0)
    {
        Local_acNumber[Local_u8Index] =
                (Copy_u16Number % 10) + '0';

        Copy_u16Number /= 10;

        Local_u8Index++;
    }


    /* Print characters in correct order */
    for(Local_u8I = Local_u8Index;
        Local_u8I > 0;
        Local_u8I--)
    {
        LCD_SendChar(Local_acNumber[Local_u8I - 1]);
    }
}


/*========================================================
 * Function: LCD_Clear
 * Description:
 * Clear LCD display
 *========================================================*/
void LCD_Clear(void)
{
    LCD_SendCommand(0x01);

    _delay_ms(2);
}


/*========================================================
 * Function: LCD_SetCursor
 * Description:
 * Set cursor position
 *
 * Row 0 → First line
 * Row 1 → Second line
 *========================================================*/
void LCD_SetCursor(u8 Copy_u8Row,
                   u8 Copy_u8Column)
{
    u8 Local_u8Address;


    if(Copy_u8Row == 0)
    {
        Local_u8Address =
                0x80 + Copy_u8Column;
    }
    else
    {
        Local_u8Address =
                0xC0 + Copy_u8Column;
    }


    LCD_SendCommand(Local_u8Address);
}

