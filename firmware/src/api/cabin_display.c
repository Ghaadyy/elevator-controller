/**
 * @file    cabin_display.c
 * @author  Antoine Karam (antoinekaram1414@gmail.com)
 * @date    2024-12-21
 */
#include "../main.h"
#include "cabin_display.h"
#include "tempr.h"
#include "../hardware.h"
#include "../sched/scheduler.h"
#include "date.h"

/*Local prototypes:*/
void display_process(s_task_handle_t me, s_task_msg_t **msg, void *arg);
void display_value(uint8_t value, uint8_t start_pin);
void show_time();
void show_date();
void show_temperature();

#define DISPLAY_COOLDOWN (10)

/*Global variables:*/
static uint8_t display_cycle = MIN_COUNT;

bool init_cabin_display(void)
{
    return s_task_create(true, S_TASK_LOW_PRIORITY, 1000, display_process, NULL, NULL);
}

/**
 * @brief Task that cycles through and displays time, date, and temperature
 *
 * @param me
 * @param msg
 * @param arg
 */
void display_process(s_task_handle_t me, s_task_msg_t **msg, void *arg)
{
    static uint8_t counter = 0;

    switch (display_cycle)
    {
    case 1:
        show_time(); /* Display time */
        break;
    case 2:
        show_date(); /* Display date */
        break;
    case 3:
        show_temperature(); /* Display temperature */
        break;
    default:
        show_time(); /* Default to displaying time if an invalid option is encountered */
    }

    counter++;
    if (counter == DISPLAY_COOLDOWN)
    {
        display_cycle = (display_cycle % MAX_COUNT) + 1; /* Cycle through the options */
        counter = 0;
    }
}

/**
 * @brief Display a 2-digit value across two 7-segment displays
 *
 * @param value
 * @param start_pin
 */
void display_value(uint8_t value, uint8_t start_pin)
{
    for (int i = 0; i < MAX_PINS; ++i) /* Loop through the pins */
    {
        output_bit(start_pin + i, value & 0x01); /* Set the pin state based on the current bit of the value */
        value >>= 1;                             /* Shift the value right by one bit to move to the next segment */
    }
}

/**
 * @brief Display the current time in (HH:MM) format
 *
 */
void show_time()
{
    static bool separator_display = true;
    uint8_t hour, minutes;
    bool success = get_hour(&hour) & get_minutes(&minutes);

    if (success)
    {
        display_value(hour, PIN_E0); /* Display hours*/
        if (separator_display)
        {
            output_high(PIN_G4); /* Turn on separator ':' */
        }
        else
        {
            output_low(PIN_G4); /* Turn off separator ':' */
        }
        display_value(minutes, PIN_D0); /* Display minutes */
        separator_display = !separator_display;
    }
}

/**
 * @brief Display the current date in (DD MM) format
 *
 */
void show_date()
{
    uint8_t date, month;
    bool success = get_date(&date) && get_month(&month);

    if (success)
    {
        display_value(date, PIN_E0);  /* Display day */
        output_low(PIN_G4);           /* Turn off separator ':' */
        display_value(month, PIN_D0); /* Display month */
    }
}

/**
 * @brief Display the current temperature in (TT dC) format
 *
 */
void show_temperature()
{
    uint8_t temp = read_temperature_BCD_limited();

    display_value(temp, PIN_E0); /* Display the temperature */
    output_low(PIN_G4);
    display_value(0xDC, PIN_D0); /* Display 'dC' */
}
