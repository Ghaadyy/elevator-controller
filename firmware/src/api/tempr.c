/**
 * @file    tempr.h
 * @author  Antoine Karam (antoinekaram1414@gmail.com)
 * @date    2024-12-21
 */

#include "../main.h"
#include "tempr.h"
#include "../drivers/adc.h"

float read_temperature(void)
{
    uint8_t adc_val;

    adc_val = get_adc(ADC_TEMPR); // Read ADC

    return (float)adc_val * (MAX_TEMPERATURE / (ADC_MAX_LVL - 1));
}

uint8_t read_temperature_BCD_limited(void)
{
    float raw_temp = read_temperature(); /* Read the temperature value */

    if (raw_temp > 99)
    { /* Limit the temperature to a maximum of 99*/
        raw_temp = 99;
    }

    uint8_t temp = (uint8_t)raw_temp; /* Convert float to uint8_t for easier extraction*/

    uint8_t first_digit = temp / 10;  /* Extract the first digit */
    uint8_t second_digit = temp % 10; /* Extract the second digit */

    uint8_t packed_digits = (second_digit & 0x0F) | ((first_digit << 4) & 0xF0); /* Pack the two digits into one byte */

    return packed_digits;
}