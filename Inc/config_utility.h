#ifndef CONFIG_UTILITY_H
#define CONFIG_UTILITY_H

#include <stdint.h>
#include "stm32f4xx.h"

//--------------------------------------------------------------------------------------------------------------------//
typedef struct {
    uint8_t WaitState;
    uint8_t pll_m;
    uint8_t pll_n;
    uint8_t pll_p;
    uint8_t pll_q;
    uint8_t ahb_prescaler;
    uint8_t apb_1_prescalar; // Prescaled as the clockspeed does not crosses over 50MHz.
    uint8_t apb_2_prescalar; 
} clock_config;

extern const clock_config CLOCK_DEFAULTS;  // Values here given for the 96MHz clock speed.

void default_clock_init(void);
void clock_init(const clock_config *);

//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
typedef struct {
    uint8_t PORT_NUM;
    uint8_t PIN_NUM;
    uint8_t MODER;
    uint8_t OTYPER;
    uint8_t OSPEEDR;  
    uint8_t PUPDR;
    uint8_t AF_VAL;     
} gpio_config;

void gpio_cfg(GPIO_TypeDef*, const gpio_config*);

//--------------------------------------------------------------------------------------------------------------------//


#endif