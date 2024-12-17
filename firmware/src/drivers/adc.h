#ifndef ADC_H
#define ADC_H

#define ADC_VREF (5.0) // V
#define ADC_MAX_LVL (256)
#define ADC_LSB (ADC_VREF / ADC_MAX_LVL)

#define MAX_ADC_CHANNELS 1
#define ADC_WINDOW_SIZE 8 /*SHOULD BE POWER OF 2*/
#define ADC_WINDOW_SIZE_MASK (ADC_WINDOW_SIZE - 1)
#define ADC_WINDOW_BIT_SHIFT 3 /*2^3 = ADC_WINDOW_SIZE*/

typedef struct _adc_info
{
    uint16_t current_value;
    uint16_t average_value;
    uint16_t window[8];
    uint8_t window_index;
} adc_info_t;

/*Channels:*/
typedef enum adc_chan
{
    ADC_TEMPR = 0
} adc_chan_t;

bool init_adc(void);
uint16_t get_adc(uint8_t chan_index);
uint16_t get_adc_live(uint8_t chan_index);

#endif /*EOF*/
