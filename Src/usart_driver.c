#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "config_utility.h"
#include "peripherals_utility.h"

static const usart_config usart_table[usart_count] = {
    [usart1] = {
        .instance = USART1,
        .clock_reg = &(RCC->APB2ENR),
        .clock_bit = RCC_APB2ENR_USART1EN,
        .bus_freq = 96000000,

        .tx_pin = {
            .PORT_NUM = 0,
            .Port_address = GPIOA,
            .PIN_NUM = 9,
            .MODER = 2,
            .OTYPER = 0,
            .OSPEEDR = 3,
            .PUPDR = 1,
            .AF_VAL = 7
        },

        .rx_pin = {
            .PORT_NUM = 0,
            .Port_address = GPIOA,
            .PIN_NUM = 10,
            .MODER = 2,
            .OTYPER = 0,
            .OSPEEDR = 3,
            .PUPDR = 1,
            .AF_VAL = 7
        }
    },

    [usart2] = {
        .instance = USART2,
        .clock_reg = &(RCC->APB1ENR),
        .clock_bit = RCC_APB1ENR_USART2EN,
        .bus_freq = 48000000,

        .tx_pin = {
            .PORT_NUM = 3,
            .Port_address = GPIOD,
            .PIN_NUM = 5,
            .MODER = 2,
            .OTYPER = 0,
            .OSPEEDR = 3,
            .PUPDR = 1,
            .AF_VAL = 7
        },

        .rx_pin = {
            .PORT_NUM = 3,
            .Port_address = GPIOD,
            .PIN_NUM = 6,
            .MODER = 2,
            .OTYPER = 0,
            .OSPEEDR = 3,
            .PUPDR = 1,
            .AF_VAL = 7
        }
    },

    [usart6] = {
        .instance = USART6,
        .clock_reg = &(RCC->APB2ENR),
        .clock_bit = RCC_APB2ENR_USART6EN,
        .bus_freq = 96000000,

        .tx_pin = {
            .PORT_NUM = 2,
            .Port_address = GPIOC,
            .PIN_NUM = 6,
            .MODER = 2,
            .OTYPER = 0,
            .OSPEEDR = 3,
            .PUPDR = 1,
            .AF_VAL = 8
        },

        .rx_pin = {
            .PORT_NUM = 2,
            .Port_address = GPIOC,
            .PIN_NUM = 7,
            .MODER = 2,
            .OTYPER = 0,
            .OSPEEDR = 3,
            .PUPDR = 1,
            .AF_VAL = 8
        }

    }
};



void usart_init(usart_select id){
    usart_config *config_bucket = &usart_table[id];

    gpio_init(config_bucket->tx_pin);

    
}