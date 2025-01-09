/**
 * @file ds1307.h
 * @author Ghady Youssef (ghadyyi@gmail.com)
 * @brief The DS1307 driver.
 * @version 0.2
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _DS1307_H
#define _DS1307_H

/* DS1307 Slave Addresses for I2C communication */
#define _DS1307_SLAVE_ADDR_W (0x68 << 1)                   /*!< Slave Write Address */
#define _DS1307_SLAVE_ADDR_R (_DS1307_SLAVE_ADDR_W | 0x01) /*!< Slave Read Address */

/* Register addresses to acces RTC data */
#define SECS_ADDR (0x00)
#define MINS_ADDR (0x01)
#define HRS_ADDR (0x02)
#define DAY_ADDR (0x03)
#define DATE_ADDR (0x04)
#define MONTH_ADDR (0x05)
#define YEAR_ADDR (0x06)

/**
 * @brief Initializes the DS1307 and sets the required register values.
 *
 * @return true
 * @return false
 */
bool init_rtc(void);

/**
 * @brief Reads a DS1307 register value.
 *
 * @param reg_addr The register's address.
 * @param val Pointer to the extracted value
 * @return true if read operation was successfull
 * @return false otherwise
 */
bool read_reg(uint8_t reg_addr, uint8_t *val);

#endif