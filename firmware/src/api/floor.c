/**
 * @file floor.c
 * @author Ghady Youssef (ghadyyi@gmail.com)
 * @brief Implementation of the floor monitor API.
 * @version 0.1
 * @date 2024-12-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "../main.h"
#include "floor.h"
#include "motor.h"
#include "../sched/scheduler.h"

/* Global variables */
floor_t current_floor = GF; /*!< Contains the global floor state. */

/* Local prototypes */
/**
 * @brief Task that displays the direction in a heartbeat manner.
 *
 * @param me
 * @param msg
 * @param arg
 */
void display_direction(s_task_handle_t me, s_task_msg_t **msg, void *arg);

/**
 * @brief Task that monitors the floor stop switches and updates global floor state.
 *
 * @param me
 * @param msg
 * @param arg
 */
void floor_state_monitor(s_task_handle_t me, s_task_msg_t **msg, void *arg);

/**
 * @brief Task retrieves global floor state and displays it to 7-segment display.
 *
 * @param me
 * @param msg
 * @param arg
 */
void display_floor(s_task_handle_t me, s_task_msg_t **msg, void *arg);

bool init_floor_monitor()
{
    bool ret = true;
    ret &= s_task_create(true, S_TASK_HIGH_PRIORITY, 150, floor_state_monitor, NULL, NULL);
    ret &= s_task_create(true, S_TASK_NORMAL_PRIORITY, 250, display_floor, NULL, NULL);
    ret &= s_task_create(true, S_TASK_NORMAL_PRIORITY, 250, display_direction, NULL, NULL);
    return ret;
}

void display_direction(s_task_handle_t me, s_task_msg_t **msg, void *arg)
{
    static bool display = true;
    direction_t direction = get_motor_direction();
    if (direction == UP && display)
    {
        output_high(PIN_F5);
        output_high(PIN_F6);
        output_low(PIN_F7);
    }
    else if (direction == DOWN && display)
    {
        output_low(PIN_F5);
        output_high(PIN_F6);
        output_high(PIN_F7);
    }
    else
    {
        output_low(PIN_F5);
        output_low(PIN_F6);
        output_low(PIN_F7);
    }

    display = !display;
}

void floor_state_monitor(s_task_handle_t me, s_task_msg_t **msg, void *arg)
{
    // Flr3_Stop => PIN A1
    // Flr2_Stop => PIN A4
    // Flr1_Stop => PIN A7
    // Flr0_Stop => PIN C2

    if (!input(PIN_A1)) /* Check Floor 3 Stop */
    {
        current_floor = F3;
    }
    else if (!input(PIN_A4)) /* Check Floor 2 Stop */
    {
        current_floor = F2;
    }
    else if (!input(PIN_A7)) /* Check Floor 1 Stop */
    {
        current_floor = F1;
    }
    else if (!input(PIN_C2)) /* Check Floor 0 Stop */
    {
        current_floor = GF;
    }
}

void display_floor(s_task_handle_t me, s_task_msg_t **msg, void *arg)
{
    floor_t floor = get_current_floor();

    for (uint8_t i = 0; i < 4; ++i)
    {
        output_bit(PIN_G0 + i, floor & 0x01);
        floor >>= 1;
    }
}

floor_t get_current_floor()
{
    return current_floor;
}