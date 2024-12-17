#include "main.h"
#include "timer.h"

// Globals:
cntr_size_t local_sys_tick_counter = 0; // tick counter

/**
 * ISR for TIMER1 in order to generate a 1ms local_sys_tick
 *
 * @author m_richa
 * @date
 */
#int_TIMER1
void TIMER1_isr(void)
{
    set_timer1(get_timer1() - TMR1Reload); /*re-load timer*/

    local_sys_tick_counter++;
}

/**
 * Function to return system ticks counter in ms, passed to scheduler
 *
 * @author m_richa
 * @date
 *
 * @return cntr_size_t system ticks count in ms
 */
cntr_size_t get_ticks_counter(void)
{
    return local_sys_tick_counter;
}
