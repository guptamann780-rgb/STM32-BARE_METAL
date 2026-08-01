#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "config_utility.h"
#include "peripherals_utility.h"
#include "ring_buffer.h"
#include <string.h>


int main(void)
{   
    default_clock_init();
    SysTick_Init();

    gpio_config PC13 = {
    .PORT_NUM = 2,
    .Port_address = GPIOC,
    .PIN_NUM = 13,
    .MODER = 1,
    .OTYPER = 0,
    .OSPEEDR = 0,  
    .PUPDR = 0,
    .AF_VAL = 0
    };

    gpio_cfg(&PC13);
    REG_CLEAR_BIT(&(GPIOC->ODR), 13);

    usart_init(usart1, (uint32_t)115200);


    
    /* Loop forever */
	for(;;)
    {
        printf("U");
        delay_ms(5000);
    }

}
