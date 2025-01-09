/**
 * @file date.c
 * @author Ghady Youssef (ghadyyi@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "main.h"
#include "date.h"
#include "../drivers/ds1307.h"

bool get_full_date(date_info_t *bcd_date)
{
    bool ack_state = true;
    uint8_t date, month;
    ack_state &= get_date(&date);
    ack_state &= get_month(&month);
    bcd_date->date = date;
    bcd_date->month = month;
    return ack_state;
}

bool get_time(time_info_t *bcd_time)
{
    bool ack_state = true;
    uint8_t hrs, mins, seconds;
    ack_state &= get_hour(&hrs);
    ack_state &= get_minutes(&mins);
    ack_state &= get_seconds(&seconds);
    bcd_time->hours = hrs;
    bcd_time->minutes = mins;
    bcd_time->seconds = seconds;
    return ack_state;
}

bool get_hour(uint8_t *hour)
{
    return read_reg(HRS_ADDR, hour);
}

bool get_minutes(uint8_t *minutes)
{
    return read_reg(MINS_ADDR, minutes);
}

bool get_seconds(uint8_t *seconds)
{
    return read_reg(SECS_ADDR, seconds);
}

bool get_day(uint8_t *day)
{
    return read_reg(DAY_ADDR, day);
}

bool get_date(uint8_t *date)
{
    return read_reg(DATE_ADDR, date);
}

bool get_month(uint8_t *month)
{
    return read_reg(MONTH_ADDR, month);
}

bool get_year(uint8_t *year)
{
    return read_reg(YEAR_ADDR, year);
}