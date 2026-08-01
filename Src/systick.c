#include "peripherals_utility.h"
#include <stdint.h>
#include "stm32f4xx.h"
#include "config_utility.h"

volatile uint32_t system_ticks = 0;

void SysTick_Handler(void){
    system_ticks++;
}

void SysTick_Init(void) {
    __disable_irq();

    // 1. Disable SysTick for configuration
    SysTick->CTRL = 0;
    
    // 2. Load the reload value for 1ms at 96MHz
    SysTick->LOAD = 95999;
    
    // 3. Clear current value (also clears COUNTFLAG)
    SysTick->VAL = 0;
    
    // 4. Enable: Processor Clock, Interrupt, and Counter
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk   | SysTick_CTRL_ENABLE_Msk;

    __enable_irq();
}

void delay_ms(uint32_t ms){
    uint32_t start_ticks = system_ticks;

    while((system_ticks - start_ticks) < ms);

}