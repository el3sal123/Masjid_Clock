#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "LIB/STD_TYPES.h"

#include "HAL/LCD/LCD_Interface.h"
#include "HAL/KEYPAD/KEYPAD_Interface.h"

#include "MCAL/ADC/ADC_Interface.h"


/* ============================= Bit Macros ============================= */

#define SET_BIT(REG, BIT)    ((REG) |=  (1 << (BIT)))
#define CLEAR_BIT(REG, BIT)  ((REG) &= ~(1 << (BIT)))
#define READ_BIT(REG, BIT)   (((REG) >> (BIT)) & 1)


/* ========================= Hardware Pin Mapping ======================= */

#define INT0_PIN        PD2
#define INT1_PIN        PD3

#define BTN_SET         PB2


/* ============================== Settings ============================== */

#define DEBOUNCE_MS       30
#define TIMER0_RELOAD     6
#define TIMER1_COMPARE    62499
#define DISPLAY_DIGITS    6
#define MAX_HOURS         99


/* ================================ Types ================================ */

typedef enum
{
    INCREMENT,
    COUNTDOWN

} Mode_t;


typedef enum
{
    STOPPED,
    RUNNING,
    SETTING,
    ALARM

} State_t;


/* ========================== Clock Variables =========================== */

volatile u8 sec = 0;
volatile u8 min = 0;
volatile u8 hr  = 0;

volatile Mode_t current_mode = INCREMENT;

volatile State_t current_state = RUNNING;


/* ========================== Function Prototypes ======================== */

void System_Init(void);

static void Handle_SetButton(void);

static void Set_Clock_Time(void);


/* ========================================================================
 *                         SYSTEM INITIALIZATION
 * ======================================================================== */

void System_Init(void)
{

    /* ==============================================================
     * 7447 BCD
     *
     * PB3 -> A
     * PB4 -> B
     * PB5 -> C
     * PB6 -> D
     * ============================================================== */

    DDRB |= (1 << PB3) |
            (1 << PB4) |
            (1 << PB5) |
            (1 << PB6);


    PORTB &= ~((1 << PB3) |
               (1 << PB4) |
               (1 << PB5) |
               (1 << PB6));


    /* ==============================================================
     * 6 Seven Segment Digit Enables
     *
     * PA0 -> Digit 1
     * PA1 -> Digit 2
     * PA2 -> Digit 3
     * PA3 -> Digit 4
     * PA4 -> Digit 5
     * PA5 -> Digit 6
     * ============================================================== */

    DDRA |= 0x3F;

    PORTA &= 0xC0;


    /* ==============================================================
     * PB2 = Push Button
     *
     * Input + Internal Pull-up
     * ============================================================== */

    CLEAR_BIT(DDRB, BTN_SET);

    SET_BIT(PORTB, BTN_SET);


    /* ==============================================================
     * INT0 / INT1
     * ============================================================== */

    DDRD &= ~((1 << INT0_PIN) |
              (1 << INT1_PIN));


    /* ==============================================================
     * INT0 + INT1 Pull-up
     * ============================================================== */

    SET_BIT(PORTD, INT0_PIN);
    SET_BIT(PORTD, INT1_PIN);


    /* ==============================================================
     * INT0 Falling Edge
     * ============================================================== */

    MCUCR |= (1 << ISC01);

    MCUCR &= ~(1 << ISC00);


    /* ==============================================================
     * INT1 Rising Edge
     * ============================================================== */

    MCUCR |= (1 << ISC11) |
             (1 << ISC10);


    /* ==============================================================
     * Enable INT0 + INT1
     * ============================================================== */

    GICR |= (1 << INT0) |
            (1 << INT1);


    /* ==============================================================
     * TIMER1
     *
     * CTC
     *
     * F_CPU = 16 MHz
     * Prescaler = 256
     *
     * OCR1A = 62499
     *
     * Interrupt every 1 second
     * ============================================================== */

    TCCR1B = (1 << WGM12) |
             (1 << CS12);

    OCR1A = TIMER1_COMPARE;

    TIMSK |= (1 << OCIE1A);


    /* ==============================================================
     * TIMER0
     *
     * Seven Segment Multiplexing
     * Prescaler = 64
     * ============================================================== */

    TCCR0 = (1 << CS01) |
            (1 << CS00);

    TCNT0 = TIMER0_RELOAD;

    TIMSK |= (1 << TOIE0);
}


/* ========================================================================
 *                           TIMER0 ISR
 *
 *                    7-Segment Multiplexing
 * ======================================================================== */

ISR(TIMER0_OVF_vect)
{
    static u8 current_digit = 0;
    u8 val = 0;

    TCNT0 = TIMER0_RELOAD;

    /* Turn OFF all digits */
    PORTA &= 0xC0;

    /* Select digit value */
    switch(current_digit)
    {
        case 0:
            val = hr / 10;
            break;

        case 1:
            val = hr % 10;
            break;

        case 2:
            val = min / 10;
            break;

        case 3:
            val = min % 10;
            break;

        case 4:
            val = sec / 10;
            break;

        case 5:
            val = sec % 10;
            break;

        default:
            current_digit = 0;
            val = 0;
            break;
    }

    /* Send BCD to 7447 */
    PORTB &= 0x87;
    PORTB |= (val << 3);

    /* Enable current digit */
    PORTA |= (1 << current_digit);

    /* Next digit */
    current_digit++;

    if(current_digit >= DISPLAY_DIGITS)
    {
        current_digit = 0;
    }
}
/* ========================================================================
 *                           TIMER1 ISR
 *
 *                            1 Second
 * ======================================================================== */

ISR(TIMER1_COMPA_vect)
{

    /* Do not count while stopped or setting */

    if(current_state != RUNNING)
    {
        return;
    }


    /* ==============================================================
     * INCREMENT MODE
     * ============================================================== */

    if(current_mode == INCREMENT)
    {

        sec++;


        if(sec > 59)
        {
            sec = 0;

            min++;
        }


        if(min > 59)
        {
            min = 0;

            hr++;
        }


        if(hr > MAX_HOURS)
        {
            hr = 0;
        }
    }


    /* ==============================================================
     * COUNTDOWN MODE
     * ============================================================== */

    else
    {

        if(sec == 0 &&
           min == 0 &&
           hr == 0)
        {
            current_state = ALARM;
        }

        else if(sec == 0)
        {

            sec = 59;

            if(min == 0)
            {
                min = 59;

                hr--;
            }

            else
            {
                min--;
            }
        }

        else
        {
            sec--;
        }
    }
}


/* ========================================================================
 *                              INT0
 *
 *                              RESET
 * ======================================================================== */

ISR(INT0_vect)
{
    sec = 0;
    min = 0;
    hr  = 0;

    current_state = STOPPED;
}


/* ========================================================================
 *                              INT1
 *
 *                              PAUSE
 * ======================================================================== */

ISR(INT1_vect)
{
    if(current_state == RUNNING)
    {
        current_state = STOPPED;
    }
}


/* ========================================================================
 *                         SET BUTTON HANDLER
 * ======================================================================== */

static void Handle_SetButton(void)
{

    /* Button not pressed */

    if(READ_BIT(PINB, BTN_SET))
    {
        return;
    }


    /* Debounce */

    _delay_ms(DEBOUNCE_MS);


    /* Check again */

    if(READ_BIT(PINB, BTN_SET))
    {
        return;
    }


    /* Enter SETTING mode */

    current_state = SETTING;


    /* Wait until button is released */

    while(!READ_BIT(PINB, BTN_SET));


    /* Start time setting */

    Set_Clock_Time();
}


/* ========================================================================
 *                           SET CLOCK TIME
 * ======================================================================== */

static void Set_Clock_Time(void)
{

    u8 key;

    u8 digit_count = 0;

    u8 digits[6];


    LCD_Clear();

    LCD_SetCursor(0, 0);

    LCD_SendString("SET TIME");

    LCD_SetCursor(1, 0);


    while(digit_count < 6)
    {

        key = KEYPAD_GetKey();


        if(key >= '0' && key <= '9')
        {

            digits[digit_count] = key - '0';

            LCD_SendChar(key);

            digit_count++;
        }
    }


    u8 new_hour;

    u8 new_minute;

    u8 new_second;


    new_hour =
        (digits[0] * 10) +
        digits[1];


    new_minute =
        (digits[2] * 10) +
        digits[3];


    new_second =
        (digits[4] * 10) +
        digits[5];


    if(new_hour > 99 ||
       new_minute > 59 ||
       new_second > 59)
    {

        LCD_Clear();

        LCD_SetCursor(0, 0);

        LCD_SendString("INVALID TIME");

        _delay_ms(1500);

        current_state = STOPPED;

        return;
    }


    hr  = new_hour;
    min = new_minute;
    sec = new_second;


    current_state = RUNNING;


    LCD_Clear();

    LCD_SetCursor(0, 0);

    LCD_SendString("TIME SET");

    _delay_ms(1000);

    LCD_Clear();
}



/* ========================================================================
 *                                MAIN
 *
 *                    TEMPORARY DEBUG VERSION
 * ======================================================================== */

int main(void)
{
    u8 key;

    LCD_Init();
    KEYPAD_Init();
    ADC_Init();

    System_Init();
    sei();

    while(1)
    {
        /* Push Button SET */
        Handle_SetButton();

        /* Read Keypad */
        key = KEYPAD_GetKey();

        /* Prayer Times */
        if(key == 'D')
        {
            LCD_Clear();
            LCD_SendString("FAJR 05:18");
        }
        else if(key == '/')
        {
            LCD_Clear();
            LCD_SendString("DHUHR 12:48");
        }
        else if(key == 'x')
        {
            LCD_Clear();
            LCD_SendString("ASR 4:15");
        }
        else if(key == '-')
        {
            LCD_Clear();
            LCD_SendString("MAGHRIB 6:50");
        }
        else if(key == '+')
        {
            LCD_Clear();
            LCD_SendString("ISHA 8:08");
        }

        /* Temperature */
        else if(key == '=')
        {
            u16 adc_value;
            u16 temperature;

            adc_value = ADC_Read(7);

            temperature = ((u32)adc_value * 500UL) / 1023UL;
            LCD_Clear();
            LCD_SendString("Temp: ");
            LCD_SendNumber(temperature);
        }
    }

    return 0;
}
