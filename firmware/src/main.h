#include <18F67K22.h>
#device ADC = 8 /*ADC resulution in bits*/
#device ANSI    /*ANSI C*/

/*Chip configuration*/
#FUSES NOWDT      // No Watch Dog Timer
#FUSES PUT        // Power Up Timer
#FUSES NOBROWNOUT // No brownout reset
#FUSES BORV20     // Brownout reset at 2.xV

#use delay(internal = 64000000)

#use I2C(master, slow, I2C1) /*I2C HAL*/

#use fast_io(ALL)

#include "target_port.h"
