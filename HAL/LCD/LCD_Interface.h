/*
 * LCD_Interface.h
 *
 *  Created on: Sep 24, 2026
 *      Author: TEESLA
 */

#ifndef HAL_LCD_LCD_INTERFACE_H_
#define HAL_LCD_LCD_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"

/* Initialize LCD */
void LCD_Init(void);

/* Send Command */
void LCD_SendCommand(u8 Copy_u8Command);

/* Send Character */
void LCD_SendChar(u8 Copy_u8Char);

/* Send String */
void LCD_SendString(const char *Copy_pcString);

/* Send Number */
void LCD_SendNumber(u16 Copy_u16Number);

/* Clear LCD */
void LCD_Clear(void);

/* Set Cursor Position */
void LCD_SetCursor(u8 Copy_u8Row, u8 Copy_u8Column);

#endif
