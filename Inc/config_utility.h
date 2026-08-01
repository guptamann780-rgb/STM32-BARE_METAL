#ifndef CONFIG_UTILITY_H
#define CONFIG_UTILITY_H

#include <stdint.h>
#include "stm32f411xe.h"
#include "stm32f4xx.h"

//CLOCK and NVIC setup
//--------------------------------------------------------------------------------------------------------------------//
typedef struct {
    uint8_t WaitState;
    uint8_t pll_m;
    uint8_t pll_n;
    uint8_t pll_p;
    uint8_t pll_q;
    uint8_t ahb_prescaler;
    uint32_t apb_1_prescalar; // Prescaled as the clockspeed does not crosses over 50MHz.
    uint8_t apb_2_prescalar; 
} clock_config;

extern const clock_config CLOCK_DEFAULTS;  // Values here given for the 96MHz clock speed.

void default_clock_init(void);
void clock_init(const clock_config *);

//--------------------------------------------------------------------------------------------------------------------//

//GPIO setup
//--------------------------------------------------------------------------------------------------------------------//
typedef struct {
    uint8_t PORT_NUM;
    GPIO_TypeDef* Port_address;
    uint8_t PIN_NUM;
    uint8_t MODER;
    uint8_t OTYPER;
    uint8_t OSPEEDR;  
    uint8_t PUPDR;
    uint8_t AF_VAL;     
} gpio_config;

void gpio_cfg(const gpio_config*);

//--------------------------------------------------------------------------------------------------------------------//

// USART setup
//--------------------------------------------------------------------------------------------------------------------//
typedef enum{
    usart1,             // 0
    usart2,             // 1
    usart6,             // 2
    usart_count         // 3, the total count of the USART we have on board.
}usart_select;

typedef struct{
    USART_TypeDef* instance;
    volatile uint32_t *clock_reg;
    uint32_t clock_bit;
    uint32_t bus_freq;
    
    gpio_config tx_pin;
    gpio_config rx_pin;
}usart_config;

void usart_init(usart_select id, uint32_t baudrate);

int _write(int file, char *ptr, int len);

void usart_putchar(USART_TypeDef *usart, char c);

char usart_getchar(USART_TypeDef *usart);

//--------------------------------------------------------------------------------------------------------------------//

//SYSTICK setup
//--------------------------------------------------------------------------------------------------------------------//
void SysTick_Handler(void);

void SysTick_Init(void);

void delay_ms(uint32_t ms);

uint32_t get_system_ticks(void);

//--------------------------------------------------------------------------------------------------------------------//

#endif