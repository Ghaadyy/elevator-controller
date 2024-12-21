/**
 * @file    ds1307.h
 * @author  Ghaadyy
 * @date    21/12/2024
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

typedef struct date_info
{
    uint8_t day;   /*!< The day value represented in BCD */
    uint8_t date;  /*!< The date value represented in BCD */
    uint8_t month; /*!< The month value represented in BCD */
    uint8_t year;  /*!< The year value represented in BCD */
} date_info_t;

typedef struct time_info
{
    uint16_t hours;   /*!< The hours value represented in BCD */
    uint16_t minutes; /*!< The minutes value represented in BCD */
    uint16_t seconds; /*!< The seconds value represented in BCD */
} time_info_t;

/**
 * @brief Initializes the DS1307 and sets the required register values.
 *
 * @return true
 * @return false
 */
bool init_rtc(void);

/**
 * @brief Get the date in BCD format
 *
 * @return true
 * @return false
 */
bool get_full_date(date_info_t *bcd_date);

/**
 * @brief Get the time in BCD format
 *
 * @return true
 * @return false
 */
bool get_time(time_info_t *bcd_time);

/**
 * @brief Get the hour in BCD format
 *
 * @param hour
 * @return true
 * @return false
 */
bool get_hour(uint8_t *hour);

/**
 * @brief Get the minutes in BCD format
 *
 * @param minutes
 * @return true
 * @return false
 */
bool get_minutes(uint8_t *minutes);

/**
 * @brief Get the seconds in BCD format
 *
 * @param seconds
 * @return true
 * @return false
 */
bool get_seconds(uint8_t *seconds);

/**
 * @brief Get the date in BCD format
 *
 * @param date
 * @return true
 * @return false
 */
bool get_date(uint8_t *date);

/**
 * @brief Get the day in BCD format
 *
 * @param day
 * @return true
 * @return false
 */
bool get_day(uint8_t *day);

/**
 * @brief Get the month in BCD format
 *
 * @param month
 * @return true
 * @return false
 */
bool get_month(uint8_t *month);

/**
 * @brief Get the year in BCD format
 *
 * @param year
 * @return true
 * @return false
 */
bool get_year(uint8_t *year);

#endif