#include "main.h"
#include "stdlib.h"
#include "hardware.h"
#include "system.h"

// Global vars:
system_info current_system_info;

/**
 *
 * @return
 */
bool init_system(void) // returns TRUE if success
{
    bool ret = true;

    enable_interrupts(INT_TIMER1); /*Enable TIMER1 interrupt
                                   (used to generate a 1ms tick for the scheduler)
                                   Refer to timer.c*/

    enable_interrupts(GLOBAL); /*Enable global interrupt*/

    return ret;
}
/**
 *
 */
void init_system_info(void)
{
    // if any
}

/**
 *
 */
void fill_system_params(void)
{
    // if any
}
