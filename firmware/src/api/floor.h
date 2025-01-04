/**
 * @file floor.h
 * @author Ghady Youssef (ghadyyi@gmail.com)
 * @brief The API to access the elevator's floor state.
 * @version 0.1
 * @date 2024-12-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef FLOOR_H
#define FLOOR_H

/**
 * @brief A simple representation of the floor values.
 *
 */
typedef enum floor
{
    GF = 0,
    F1 = 1,
    F2 = 2,
    F3 = 3,
} floor_t;

/**
 * @brief Initializes a task that monitors the floor state.
 *
 * @return true if initialization is successful
 * @return false otherwise
 */
bool init_floor_monitor();

/**
 * @brief Get the current floor
 *
 * @return Floor number
 */
floor_t get_current_floor();

#endif