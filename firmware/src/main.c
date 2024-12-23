#include "main.h"
#include "drivers/adc.h"
#include "hardware.h"
#include "sched/scheduler.h"
#include "system.h"
#include "timer.h"
#include "drivers/ds1307.h"
#include "api/display_time_date_temp.h"

void main()
{
    bool ret;

    delay_ms(100); /*power up delay, wait a 100ms*/

    /*Initialize scheduler:*/
    ret = scheduler_init(get_ticks_counter);

    if (true == ret)
    {
        /*Initialize system:*/
        ret &= init_hw();     /* initialize HW and create LED heartbeat task */
        ret &= init_system(); /* initialize system and enable interrupt(s) (events) */

        ret &= init_adc(); /* initialize ADC sampling */
        ret &= init_rtc(); /* initilize RTC */

        ret &= init_display_time_date_temp(); /* initialize task to cycle and display time, date, and temperature */

        if (true == ret) /*success?*/
        {
            while (true)
            {
                scheduler(); /*run scheduler forever*/
            }
        }
        else
        {
            while (true)
            {
            }; /*stall*/
        }
    }
    else
    {
        while (true)
        {
        }; /*stall*/
    }
}
