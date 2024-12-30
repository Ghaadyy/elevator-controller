/**
 * @file queue.h
 * @author Ghady Youssef (ghadyyi@gmail.com)
 * @author Antoine Karam (antoinekaram1414@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-12-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "floor.h"

typedef struct floor_queue
{
    struct floor_queue *next;
    struct floor_queue *prev;
    floor_t floor;
} floor_queue_t;

/**
 * @brief Returns the first floor in the queue
 *
 * @param q
 * @return enum floor
 */
floor_t top(floor_queue_t *q);

/**
 * @brief Insert the floor at the given position and return the new queue
 *
 * @param head The queue head
 * @param pos Position to insert in the queue
 * @param new_floor The new floor to be inserted
 * @return `floor_queue_t*` The new queue head
 */
floor_queue_t *insert(floor_queue_t *head, uint8_t pos, floor_t new_floor);

/**
 * @brief Remove the first floor from the queue and return the new queue
 *
 * @param q The queue
 * @return `floor_queue_t*` The new queue head
 */
floor_queue_t *dequeue(floor_queue_t *q);

/**
 * @brief Check if the queue is empty
 *
 * @param q The queue
 * @return true if the queue is empty
 * @return false otherwise
 */
bool is_empty(floor_queue_t *q);

#endif