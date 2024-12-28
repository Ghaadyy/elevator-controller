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

/**
 * @brief A simple representation of the floor values.
 *
 */
enum floor
{
    F3 = 3,
    F2 = 2,
    F1 = 1,
    GF = 0,
};

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
enum floor get_current_floor();