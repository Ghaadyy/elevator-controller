/**
 * @file    cabin_display.h
 * @author  Antoine Karam (antoinekaram1414@gmail.com)
 * @date    2024-12-21
 */

#ifndef CABIN_DISPLAY_H
#define CABIN_DISPLAY_H

/* Starting point for the display cycle counter*/
#define MIN_COUNT (1)
/* Total number of display options */
#define MAX_COUNT (3)
/* Number of pins used for controlling two 7-segment displays*/
#define MAX_PINS (8)

/**
 * @brief Initialize and start the task
 *
 * @return true if the task is successfully created
 * @return false otherwise
 */
bool init_cabin_display(void);

#endif