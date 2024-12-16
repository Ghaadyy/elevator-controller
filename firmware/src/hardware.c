#include "hardware.h"
#include "main.h"
#include "scheduler.h"

/*local prototypes:*/
void heart_beat_led(s_task_handle_t me, s_task_msg_t **msg, void *arg);

/**
 * @brief Function to setup all IOs
 *
 */
void init_io_ports(void)
{
    set_tris_a(0xFF); /*A: all inputs*/
    output_a(0x00);   /*all ZEROs*/

    set_tris_b(0xFF); /*B: All inputs*/
    output_b(0x00);   /*all ZEROs*/

    set_tris_c(0xDF); /*C: all inputs, except RC5 (not used)*/
    output_c(0x00);   /*all ZEROs, except EE_WP*/

    set_tris_d(0x00); /*D: all ouputs*/
    output_d(0x00);   /*all ZEROs*/

    set_tris_e(0x00); /*E: all ouputs*/
    output_e(0x00);   /*all ZEROs*/

    set_tris_f(0x00); /*F: all ouputs*/
    output_f(0x00);   /*all ZEROs*/

    set_tris_g(0xE0); /*G: all ouputs*/
    output_g(0x00);   /*all ZEROs*/
}
///////////////////////////////////////////////////////////////////////////////
/**
 *
 * @return
 */
bool init_hw(void)
{
    init_io_ports();

    // Timer1: used for scheduler
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // for 1ms interrupt

    return true;
}

#if 0
/**
 * 
 * @param me
 * @param msg
 */
void heart_beat_led(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
   static uint8_t heart_beat_cnt = 0;

   //Heart Beat
    if (bit_test(heart_beat_cnt, 4))
    {
        heart_beat_cnt = 0;
        LED1OFF();
        restart_wdt();  //reset WDT
    } else if ((!bit_test(heart_beat_cnt, 2)) && (!bit_test(heart_beat_cnt, 3)))
    {
        LED1TOGGLE();
        heart_beat_cnt++;
    } else
    {
        heart_beat_cnt++;
    }
}
#endif
