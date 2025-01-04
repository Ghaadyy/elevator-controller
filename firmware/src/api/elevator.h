/**
 * @file elevator.h
 * @author Ghady Youssef (ghadyyi@gmail.com)
 * @author Antoine Karam (antoinekaram1414@gmail.com)
 * @brief The elevator scheduler API.
 *
 * @details The elevator scheduler processes incoming requests from the cabin and from each floor. It selects the next
 * target floor for the elevator, taking into account priorites, and staying in the same direction, while minimizing
 * long trips if possible.
 *
 * @version 0.2
 * @date 2024-12-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "floor.h"
#include "motor.h"

/**
 * @brief Initializes the elevator scheduler's task.
 *
 * @return true if initialization is successful
 * @return false otherwise
 */
bool init_elevator_scheduler(void);

/**
 * @brief Processes a request incoming from inside the cabin.
 *
 * @details The user inside the cabin can request to go either up or down depending on the floor they pick.
 *
 * @param dst_floor Destination floor
 */
void add_cabin_request(floor_t dst_floor);

/**
 * @brief Processes a request incoming from a floor.
 *
 * @details The floor user can request to go either up or down.
 *
 * @param dst_floor Destination floor
 * @param dir Direction requested
 */
void add_floor_request(floor_t dst_floor, direction_t dir);

#endif