#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "config_utility.h"
#include "peripherals_utility.h"


void gpio_cfg(const gpio_config* gpio_cfg)
{
    GPIO_TypeDef *port = gpio_cfg->Port_address;

    REG_SET_BIT(&(RCC->AHB1ENR), (gpio_cfg->PORT_NUM));
    
    REG_WRITE_FIELD(&(port->MODER), ((gpio_cfg->PIN_NUM) * 2), 2, gpio_cfg->MODER);

    REG_WRITE_FIELD(&(port->OTYPER), (gpio_cfg->PIN_NUM), 1, gpio_cfg->OTYPER);

    REG_WRITE_FIELD(&(port->OSPEEDR), ((gpio_cfg->PIN_NUM) * 2), 2, gpio_cfg->OSPEEDR);

    REG_WRITE_FIELD(&(port->PUPDR), ((gpio_cfg->PIN_NUM) * 2), 2, gpio_cfg->PUPDR);

    if (gpio_cfg->PIN_NUM < 8) {
        // AFRL handles pins 0-7
        REG_WRITE_FIELD(&(port->AFR[0]), (gpio_cfg->PIN_NUM * 4), 4, gpio_cfg->AF_VAL);
    } else {
        // AFRH handles pins 8-15
        // We subtract 8 from PIN_NUM so that Pin 8 starts at bit 0 of this register
        REG_WRITE_FIELD(&(port->AFR[1]), ((gpio_cfg->PIN_NUM - 8) * 4), 4, gpio_cfg->AF_VAL);
    }
} 