/**
 * @file    tempr.h
 * @author  Antoine Karam (antoinekaram1414@gmail.com)
 * @date    2024-12-21
 */

#ifndef TEMPR_H
#define TEMPR_H

#define MAX_TEMPERATURE (100.0) // deg C

/**
 * @brief Get the temperature
 *
 * @return float
 */
float read_temperature(void);

/**
 * @brief Get the temperature in BCD format limited to 2 digits
 *
 * @return uint8_t
 */
uint8_t read_temperature_BCD_limited(void);

#endif
