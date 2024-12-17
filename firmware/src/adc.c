#include "main.h"
#include "scheduler.h"
#include "adc.h"

void sample_next_adc_ch(s_task_handle_t me, s_task_msg_t **msg, void *arg);

/*list ADC channels*/
uint8_t adc_channels[MAX_ADC_CHANNELS] = {0}; // JUST ONE CHANNEL FOR THIS APP
adc_info_t adc_info[MAX_ADC_CHANNELS];

/**
 *
 *
 * @author m_richa (07/17/20)
 */
bool init_adc(void)
{
    uint8_t adc_index;

    /*Analog*/
    setup_adc_ports(sAN0, VSS_VDD);
    setup_adc(ADC_CLOCK_DIV_64 | ADC_TAD_MUL_8);
    set_adc_channel(adc_channels[0]); /*select 1st channel*/

    for (adc_index = 0; adc_index < MAX_ADC_CHANNELS; adc_index++)
    {
        adc_info[adc_index].window_index = 0; /*reset index*/
    }

    return s_task_create(true, S_TASK_LOW_PRIORITY, 100, sample_next_adc_ch, NULL, NULL); /*runs every 100ms*/
}

/**
 *
 *
 * @author m_richa (07/17/20)
 *
 * @param chan_index
 *
 * @return uint16_t
 */
uint16_t get_adc(uint8_t chan_index)
{
    return adc_info[chan_index].average_value;
}

/**
 *
 *
 * @author m_richa (07/17/20)
 *
 * @param chan_index
 *
 * @return uint16_t
 */
uint16_t get_adc_live(uint8_t chan_index)
{
    return adc_info[chan_index].current_value;
}

/**
 *
 *
 * @author m_richa (07/17/20)
 */
void sample_next_adc_ch(s_task_handle_t me, s_task_msg_t **msg, void *arg)
{
    static uint8_t chan_index = 0;
    uint16_t adc_val;
    uint32_t adc_avg = 0;
    uint8_t adc_i;

    adc_val = read_adc();                         /*read current channel*/
    adc_info[chan_index].current_value = adc_val; /*store it*/
    adc_info[chan_index].window[(adc_info[chan_index].window_index++) & ADC_WINDOW_SIZE_MASK] =
        adc_val;                                      /*add to window*/
    for (adc_i = 0; adc_i < ADC_WINDOW_SIZE; adc_i++) /*scan window*/
    {
        adc_avg += adc_info[chan_index].window[adc_i]; /*accumulate*/
    }
    adc_info[chan_index].average_value =
        adc_avg >> ADC_WINDOW_BIT_SHIFT; /*get average value (same as div by ADC_WINDOW_SIZE)*/

    chan_index = (chan_index + 1) % MAX_ADC_CHANNELS; /*next channel*/
    set_adc_channel(adc_channels[chan_index]);        /*select it*/

#ifdef USE_MESSAGING
    // A GOOD PRACTICE IS TO ALWAYS FLUSH MESSAGES BEFORE EXIT:
    s_task_flush_msgs(msg);
#endif
}
