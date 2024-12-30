/**
 * @file    motor.h
 * @author  Antoine Karam (antoinekaram1414@gmail.com)
 * @date    2024-12-28
 */

#ifndef MOTOR_H
#define MOTOR_H

/**
 * @brief Enumration for motor direction.
 *
 */
typedef enum direction
{
    UP = 1,
    NONE = 0,
    DOWN = -1,
} direction_t;

/**
 * @brief Enumeration for motor speed.
 *
 */
enum speed
{
    FAST = 2,
    SLOW = 1,
    STOP = 0,
};

/**
 * @brief Initializes the motor task but does not activate it initially.
 *
 * @return true
 * @return false
 */
bool init_motor(void);

/**
 * @brief Move the elevator to the specified floor
 *
 * @param target_floor
 */
void goto_floor(uint8_t target_floor);

/**
 * @brief Get the motor direction
 *
 * @return enum direction
 */
direction_t get_motor_direction();

#endif
