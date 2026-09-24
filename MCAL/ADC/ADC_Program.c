
 #include "ADC_Interface.h"
#include "ADC_Config.h"
#include "ADC_Register.h"

#include "../../LIB/BIT_MATH.h"

void ADC_Init(void)
{
    ADMUX = (1 << REFS0);

    ADCSRA = (1 << ADEN) |
             (1 << ADPS2) |
             (1 << ADPS1) |
             (1 << ADPS0);
}

u16 ADC_Read(u8 Copy_u8Channel)
{
    ADMUX = (ADMUX & 0xE0) | (Copy_u8Channel & 0x07);

    ADCSRA |= (1 << ADSC);

    while(ADCSRA & (1 << ADSC));

    return ADC;
}

