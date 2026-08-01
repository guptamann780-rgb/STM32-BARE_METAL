#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "config_utility.h"
#include "peripherals_utility.h"

// Track which USART port is currently used for printf / stdout
static USART_TypeDef *stdout_usart_instance = USART1;

//---------------------------------------------------- Hardware descrpition for the usart.

static const usart_config usart_table[usart_count] = {
    [usart1] = {
        .instance = USART1,
        .clock_reg = &(RCC->APB2ENR),
        .clock_bit = RCC_APB2ENR_USART1EN_Pos,
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
        .clock_bit = RCC_APB1ENR_USART2EN_Pos,
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
        .clock_bit = RCC_APB2ENR_USART6EN_Pos,
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

//----------------------------------------------------------------------------

//---------------------------------------------------- usart_init function description.


void usart_init(usart_select id, uint32_t baudrate) {

    __disable_irq();

    const usart_config *hw = &usart_table[id];

    // 1. ENABLE CLOCK FIRST (Crucial!)
    REG_SET_BIT(hw->clock_reg, hw->clock_bit);

    // 2. Initialize GPIO (Both TX and RX)
    gpio_cfg(&(hw->tx_pin));
    gpio_cfg(&(hw->rx_pin));

    // 3. Disable USART to configure (Good practice)
    hw->instance->CR1 &= ~USART_CR1_UE;

    // 4. Calculate Baud Rate (Assuming 16x oversampling)
    // Formula: (BusFreq + (baudrate * 8)) / (baudrate * 16) for rounding
    uint32_t brr_val = (hw->bus_freq + (baudrate / 2)) / baudrate;
    hw->instance->BRR = brr_val;

    // 5. Configure and Enable
    // M=0 (8-bit), 1 Stop Bit are defaults (0) in CR1/CR2 [12, 13]
    REG_SET_BIT(&(hw->instance->CR1), USART_CR1_TE_Pos); // Transmitter Enable
    REG_SET_BIT(&(hw->instance->CR1), USART_CR1_RE_Pos); // Receiver Enable
    REG_SET_BIT(&(hw->instance->CR1), USART_CR1_UE_Pos); // USART Enable

    stdout_usart_instance = hw->instance;
    setvbuf(stdout, NULL, _IONBF, 0);

    __enable_irq();
}

//---------------------------------------------------- Retargeting the printf function.

int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        // Carriage return expansion for terminal display formatting
        if (ptr[i] == '\n') {
            usart_putchar(stdout_usart_instance, '\r');
        }
        usart_putchar(stdout_usart_instance, ptr[i]);
    }
    return len;
}

//---------------------------------------------------- Underlying transimssion function.

void usart_putchar(USART_TypeDef *usart, char c) {
    // Wait until Transmit Data Register Empty (TXE bit)
    while (!(usart->SR & USART_SR_TXE));
    
    // Write character to Data Register
    usart->DR = (uint8_t)c;
}

//---------------------------------------------------- Underlying reciever function.

char usart_getchar(USART_TypeDef *usart){

    while(!(usart->SR & USART_SR_RXNE));

    return (char)(usart->DR & 0xFF);
}

//---------------------------------------------------- Abstract Reciever function.

