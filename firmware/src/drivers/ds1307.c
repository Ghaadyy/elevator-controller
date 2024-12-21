/**
 * @file    ds1307.c
 * @author  Ghaadyy
 * @date    21/12/2024
 */

#include "main.h"
#include "../sched/scheduler.h"
#include "ds1307.h"

#define _AND_ACK (1)  /*!< Slave ACK */
#define _AND_NACK (0) /*!< Slave NACK */

bool init_rtc_clock(void)
{
    bool ack_state = 0;

    i2c_start();                                  /* Issue START condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_R); /* Specify DS1307 slave with READ */
    ack_state |= i2c_write(SECS_ADDR);            /* Write the seconds register */
    uint8_t seconds = i2c_read(_AND_NACK);        /* Read existing value in register */
    i2c_start();                                  /* Issue RESTART condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_W); /* Specify DS1307 slave with WRITE */
    ack_state |= i2c_write(SECS_ADDR);            /* Write the seconds register */
    ack_state |= i2c_write(seconds & 0x7F);       /* Unset the Halt bit  */
    i2c_stop();                                   /* Issue STOP condition */

    return (0 == ack_state) ? true : false;
}

bool get_full_date(date_info_t *bcd_date)
{
    bool ack_state = 0;
    uint8_t date, month;
    ack_state &= get_date(&date);
    ack_state &= get_month(&month);
    bcd_date->date = date;
    bcd_date->month = month;
    return ack_state;
}

bool get_time(time_info_t *bcd_time)
{
    bool ack_state = 0;
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
    bool ack_state = 0;

    i2c_start();                                  /* Issue START condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_W); /* Specify DS1307 slave with WRITE */
    ack_state |= i2c_write(HRS_ADDR);             /* Select hours register */
    i2c_start();                                  /* Issue RESTART condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_R); /* Specify DS1307 slave with READ */
    *hour = i2c_read(_AND_NACK);                  /* Read hour register */
    i2c_stop();                                   /* Issue STOP condition */

    return (0 == ack_state) ? true : false;
}

bool get_minutes(uint8_t *minutes)
{
    bool ack_state = 0;

    i2c_start();                                  /* Issue START condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_W); /* Specify DS1307 slave with WRITE */
    ack_state |= i2c_write(MINS_ADDR);            /* Select minutes register */
    i2c_start();                                  /* Issue RESTART condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_R); /* Specify DS1307 slave with READ */
    *minutes = i2c_read(_AND_NACK);               /* Read minutes register */
    i2c_stop();                                   /* Issue STOP condition */

    return (0 == ack_state) ? true : false;
}

bool get_seconds(uint8_t *seconds)
{
    bool ack_state = 0;

    i2c_start();                                  /* Issue START condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_W); /* Specify DS1307 slave with WRITE */
    ack_state |= i2c_write(SECS_ADDR);            /* Select seconds register */
    i2c_start();                                  /* Issue RESTART condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_R); /* Specify DS1307 slave with READ */
    *seconds = i2c_read(_AND_NACK) & 0x7F;        /* Read seconds register */
    i2c_stop();                                   /* Issue STOP condition */

    return (0 == ack_state) ? true : false;
}

bool get_day(uint8_t *day)
{
    bool ack_state = 0;

    i2c_start();                                  /* Issue START condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_W); /* Specify DS1307 slave */
    ack_state |= i2c_write(DAY_ADDR);             /* Get day */
    i2c_start();                                  /* Issue RESTART condition */
    uint8_t day_raw = i2c_read(_AND_NACK);        /* Read day register */
    i2c_stop();                                   /* Issue STOP condition */

    return (0 == ack_state) ? true : false;
}

bool get_date(uint8_t *date)
{
    bool ack_state = 0;

    i2c_start();                                  /* Issue START condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_W); /* Specify DS1307 slave with WRITE */
    ack_state |= i2c_write(DATE_ADDR);            /* Select date register */
    i2c_start();                                  /* Issue RESTART condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_R); /* Specify DS1307 slave with READ */
    *date = i2c_read(_AND_NACK);                  /* Read date register */
    i2c_stop();                                   /* Issue STOP condition */

    return (0 == ack_state) ? true : false;
}

bool get_month(uint8_t *month)
{
    bool ack_state = 0;

    i2c_start();                                  /* Issue START condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_W); /* Specify DS1307 slave with WRITE */
    ack_state |= i2c_write(MONTH_ADDR);           /* Select month register */
    i2c_start();                                  /* Issue RESTART condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_R); /* Specify DS1307 slave with READ */
    *month = i2c_read(_AND_NACK);                 /* Read month register */
    i2c_stop();                                   /* Issue STOP condition */

    return (0 == ack_state) ? true : false;
}

bool get_year(uint8_t *year)
{
    bool ack_state = 0;

    i2c_start();                                  /* Issue START condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_W); /* Specify DS1307 slave with WRITE */
    ack_state |= i2c_write(YEAR_ADDR);            /* Select year register */
    i2c_start();                                  /* Issue RESTART condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_R); /* Specify DS1307 slave with READ */
    *year = i2c_read(_AND_NACK);                  /* Read year register */
    i2c_stop();                                   /* Issue STOP condition */

    return (0 == ack_state) ? true : false;
}