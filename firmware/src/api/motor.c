/**
 * @file    motor.c
 * @author  Antoine Karam (antoinekaram1414@gmail.com)
 * @date    2024-12-28
 */

#include "../main.h"
#include "motor.h"
#include "floor.h"
#include "../sched/scheduler.h"

/*Local prototypes:*/
void monitor_motor(s_task_handle_t me, s_task_msg_t **msg, void *arg);
void set_motor_direction(enum direction direction);
void set_motor_speed(enum speed speed);
bool check_has_approached_target_floor();
bool check_has_left_start_floor();
bool check_has_arrived_destination(enum floor current_floor);

/*Global variables:*/
s_task_handle_t monitor_motor_hndl;
bool has_left_start_floor = false;
uint8_t start_floor;
uint8_t end_floor;
uint8_t up_pins[4] = {PIN_C1, PIN_A6, PIN_A3, -1};   /* Upward direction pins for floors F0 to F3, excluding F3 */
uint8_t down_pins[4] = {-1, PIN_C0, PIN_A5, PIN_A2}; /* Downward direction pins for floors F3 to F0, excluding F0 */

bool init_motor(void)
{
    return s_task_create(false, S_TASK_HIGH_PRIORITY, 200, monitor_motor, &monitor_motor_hndl, NULL);
}

void goto_floor(uint8_t target_floor)
{
    start_floor = get_current_floor(); /* Record the current floor as the starting point */

    end_floor = target_floor; /* Set the target floor */

    enum direction current_direction = get_motor_direction(); /* Get current motor direction */

    enum direction new_direction = NONE; /* Determine the new direction */
    if (start_floor < end_floor)
    { /* Move upward */
        new_direction = UP;
    }
    else if (start_floor > end_floor)
    { /* Move downward */
        new_direction = DOWN;
    }

    /* Continue only if the motor is not moving or moving in the same direction */
    if (current_direction == NONE || current_direction == new_direction)
    {
        set_motor_direction(new_direction);
        s_task_resume(monitor_motor_hndl, true); /* Immediately resume the task */
    }
}

enum direction get_motor_direction()
{
    if (input(PIN_F3))
        return UP;
    if (input(PIN_F4))
        return DOWN;
    return NONE;
}

/**
 * @brief Monitors and adjusts the motor's speed and direction in real-time
 *
 * @param me
 * @param msg
 * @param arg
 */
void monitor_motor(s_task_handle_t me, s_task_msg_t **msg, void *arg)
{
    enum floor current_floor = get_current_floor(); /* Get the current floor */

    if (check_has_arrived_destination(current_floor))
    {
        set_motor_speed(STOP);        /* Stop the motor */
        set_motor_direction(NONE);    /* Reset the direction */
        has_left_start_floor = false; /* Reset the starting floor status */
        s_task_suspend(me);           /* Suspend this task (me) */
        return;
    }

    if (!has_left_start_floor)
    {                          /* If the motor hasn't left the starting floor */
        set_motor_speed(SLOW); /* Start moving slowly */
        if (check_has_left_start_floor())
        {                          /* Check if the motor has left */
            set_motor_speed(FAST); /* Speed up if it has left the starting floor */
        }
    }
    else
    {
        if (check_has_approached_target_floor())
        {
            set_motor_speed(SLOW); /* Slow down the motor if we are near the target floor */
        }
    }
}

/**
 * @brief Sets the motor's direction to move up, down, or stay idle
 *
 * @param direction
 */
void set_motor_direction(enum direction direction)
{
    switch (direction)
    {
    case UP:
        output_low(PIN_F4);
        output_high(PIN_F3);
        break;
    case DOWN:
        output_low(PIN_F3);
        output_high(PIN_F4);
        break;
    case NONE:
    default:
        output_low(PIN_F3);
        output_low(PIN_F4);
        break;
    }
}

/**
 * @brief Sets the motor's speed to fast, slow, or stop
 *
 * @param speed
 */
void set_motor_speed(enum speed speed)
{
    switch (speed)
    {
    case FAST:
        output_low(PIN_F2);
        output_high(PIN_F1);
        break;

    case SLOW:
        output_low(PIN_F1);
        output_high(PIN_F2);
        break;
    case STOP:
    default:
        output_low(PIN_F1);
        output_low(PIN_F2);
        break;
    }
}

/**
 * @brief Checks if the motor is approaching the target floor
 *
 * @return true
 * @return false
 */
bool check_has_approached_target_floor()
{
    if (end_floor == F3)
    { /* For the top floor, check downward approach */
        return !input(down_pins[end_floor]);
    }
    else if (end_floor == GF)
    { /* For the ground floor, check upward approach */
        return !input(up_pins[end_floor]);
    }
    else
    { /* For intermediate floors, check both upward and downward approach */
        return !input(up_pins[end_floor]) || !input(down_pins[end_floor]);
    }
}

/**
 * @brief Checks if the motor has left the starting floor
 *
 * @return true
 * @return false
 */
bool check_has_left_start_floor()
{
    if (start_floor == F3)
    {
        has_left_start_floor = !input(down_pins[start_floor]);
    }
    else if (start_floor == GF)
    {
        has_left_start_floor = !input(up_pins[start_floor]);
    }
    else
    {
        has_left_start_floor = !input(up_pins[start_floor]) || !input(down_pins[start_floor]);
    }

    return has_left_start_floor;
}

/**
 * @brief Checks if the motor has reached the target floor
 *
 * @param current_floor
 * @return true
 * @return false
 */
bool check_has_arrived_destination(enum floor current_floor)
{
    return current_floor == end_floor;
}
