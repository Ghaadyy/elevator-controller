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
