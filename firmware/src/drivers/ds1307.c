/**
 * @file ds1307.c
 * @author Ghady Youssef (ghadyyi@gmail.com)
 * @brief The DS1307 driver.
 * @version 0.2
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "main.h"
#include "ds1307.h"

#define _AND_ACK (1)  /*!< Slave ACK */
#define _AND_NACK (0) /*!< Slave NACK */

bool init_rtc(void)
{
    uint8_t ack_state = 0;

    /* Begin an I2C transaction to copy the seconds register */
    i2c_start();                                  /* Issue START condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_W); /* Specify DS1307 slave with WRITE */
    ack_state |= i2c_write(SECS_ADDR);            /* Write the seconds register */
    i2c_start();                                  /* Issue RESTART condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_R); /* Specify DS1307 slave with READ */
    uint8_t seconds = i2c_read(_AND_NACK);        /* Read existing value in register */
    i2c_stop();

    /* Begin another I2C transaction to write the updated seconds register */
    i2c_start();                                  /* Issue START condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_W); /* Specify DS1307 slave with WRITE */
    ack_state |= i2c_write(SECS_ADDR);            /* Write the seconds register */
    ack_state |= i2c_write(seconds & 0x7F);       /* Unset the Halt bit  */
    i2c_stop();                                   /* Issue STOP condition */

    return (0 == ack_state) ? true : false;
}

bool read_reg(uint8_t reg_addr, uint8_t *val)
{
    uint8_t ack_state = 0;

    i2c_start();                                  /* Issue START condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_W); /* Specify DS1307 slave with WRITE */
    ack_state |= i2c_write(reg_addr);             /* Select register address */
    i2c_start();                                  /* Issue RESTART condition */
    ack_state |= i2c_write(_DS1307_SLAVE_ADDR_R); /* Specify DS1307 slave with READ */
    *val = i2c_read(_AND_NACK);                   /* Read register */
    i2c_stop();                                   /* Issue STOP condition */

    return (0 == ack_state) ? true : false;
}