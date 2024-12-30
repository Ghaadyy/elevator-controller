/**
 * @file buttons.c
 * @author Ghady Youssef (ghadyyi@gmail.com)
 * @author Antoine Karam (antoinekaram1414@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-12-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "../main.h"
#include "../sched/scheduler.h"
#include "buttons.h"
#include "elevator.h"

/* Local prototypes */
void monitor_buttons(s_task_handle_t me, s_task_msg_t **msg, void *arg);
void check_floor_up_buttons(void);
void check_floor_down_buttons(void);
void check_cabin_buttons(void);

/* Global variables */

/* Cabin */
bool flr3_pressed = false;
bool flr2_pressed = false;
bool flr1_pressed = false;
bool flr0_pressed = false;

/* Floor */

/* Up buttons */
bool flr0_btn_up_pressed = false;
bool flr1_btn_up_pressed = false;
bool flr2_btn_up_pressed = false;

/* Down buttons */
bool flr1_btn_down_pressed = false;
bool flr2_btn_down_pressed = false;
bool flr3_btn_down_pressed = false;

bool init_buttons(void)
{
    return s_task_create(true, S_TASK_NORMAL_PRIORITY, 400, monitor_buttons, NULL, NULL);
}

void monitor_buttons(s_task_handle_t me, s_task_msg_t **msg, void *arg)
{
    check_floor_up_buttons();
    check_floor_down_buttons();
    check_cabin_buttons();
}

void check_floor_up_buttons(void)
{
    /*Up buttons*/
    if (!input(PIN_B5)) /* Floor GF Up */
    {
        flr0_btn_up_pressed = true;
    }
    else
    {
        if (flr0_btn_up_pressed)
        {
            flr0_btn_up_pressed = false;
            add_floor_request(GF, UP);
        }
    }

    if (!input(PIN_B3)) /* Floor F1 Up */
    {
        flr1_btn_up_pressed = true;
    }
    else
    {
        if (flr1_btn_up_pressed)
        {
            flr1_btn_up_pressed = false;
            add_floor_request(F1, UP);
        }
    }

    if (!input(PIN_B1)) /* Floor F2 Up */
    {
        flr2_btn_up_pressed = true;
    }
    else
    {
        if (flr2_btn_up_pressed)
        {
            flr2_btn_up_pressed = false;
            add_floor_request(F2, UP);
        }
    }
}

void check_floor_down_buttons(void)
{
    /*Down buttons*/
    if (!input(PIN_B0)) /* Floor F3 down */
    {
        flr3_btn_down_pressed = true;
    }
    else
    {
        if (flr3_btn_down_pressed)
        {
            flr3_btn_down_pressed = false;
            add_floor_request(F3, DOWN);
        }
    }

    if (!input(PIN_B2)) /* Floor F2 down */
    {
        flr2_btn_down_pressed = true;
    }
    else
    {
        if (flr2_btn_down_pressed)
        {
            flr2_btn_down_pressed = false;
            add_floor_request(F2, DOWN);
        }
    }

    if (!input(PIN_B4)) /* Floor F1 down */
    {
        flr1_btn_down_pressed = true;
    }
    else
    {
        if (flr1_btn_down_pressed)
        {
            flr1_btn_down_pressed = false;
            add_floor_request(F1, DOWN);
        }
    }
}

void check_cabin_buttons(void)
{
    if (!input(PIN_B6)) /* Floor F3 */
    {
        flr3_pressed = true;
    }
    else
    {
        if (flr3_pressed)
        {
            flr3_pressed = false;
            add_cabin_request(F3);
        }
    }

    if (!input(PIN_B7)) /* Floor F2 */
    {
        flr2_pressed = true;
    }
    else
    {
        if (flr2_pressed)
        {
            flr2_pressed = false;
            add_cabin_request(F2);
        }
    }

    if (!input(PIN_C6)) /* Floor F1 */
    {
        flr1_pressed = true;
    }
    else
    {
        if (flr1_pressed)
        {
            flr1_pressed = false;
            add_cabin_request(F1);
        }
    }

    if (!input(PIN_C7)) /* Floor GF */
    {
        flr0_pressed = true;
    }
    else
    {
        if (flr0_pressed)
        {
            flr0_pressed = false;
            add_cabin_request(GF);
        }
    }
}