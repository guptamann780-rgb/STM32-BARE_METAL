#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "config_utility.h"
#include "peripherals_utility.h"

/* Default configuration for 96 MHz SYSCLK using 25 MHz HSE */
const clock_config CLOCK_DEFAULTS = {
    .WaitState = FLASH_ACR_LATENCY_3WS, // 3 Wait States required for 96MHz at 3.3V
    .pll_m = 25,                         // 25MHz / 25 = 1MHz VCO input
    .pll_n = 192,                        // 1MHz * 192 = 192MHz VCO output
    .pll_p = 0,                          // 00 = Division by 2 -> 96MHz SYSCLK
    .pll_q = 4,                          // 192MHz / 4 = 48MHz (for USB/SDIO)
    .ahb_prescaler = 0,                  // Raw value 0 (DIV1) -> AHB = 96MHz
    .apb_1_prescalar = 4,                // Raw value 4 (DIV2) -> APB1 = 48MHz (Max 50MHz)
    .apb_2_prescalar = 0                 // Raw value 0 (DIV1) -> APB2 = 96MHz (Max 100MHz)
};

void default_clock_init(void)
{
    clock_init(&CLOCK_DEFAULTS);
}

void clock_init(const clock_config *clock_param)
{
    __disable_irq();   //disabling interrupt to avoid the interuption in between clock set up.

    // 1. Enable HSE and wait for hardware ready flag
    REG_SET_MASK_CMSIS(&(RCC->CR), RCC_CR_HSEON);
    while(((RCC->CR) & RCC_CR_HSERDY) == 0);

    // 2. Enable Power Controller Clock
    REG_SET_MASK_CMSIS(&(RCC->APB1ENR), RCC_APB1ENR_PWREN);
    
    // Dummy read to ensure PWR clock is active before writing to PWR registers (prevent register write drop)
    (void)PWR->CR;

    // 3. Set Voltage Scale 1 (11 = Scale 1, supports up to 100 MHz)
    PWR->CR |= (3U << PWR_CR_VOS_Pos);

    // Wait for VOSRDY only if Scale 1 transition is in progress
    // while((PWR->CSR & PWR_CSR_VOSRDY) == 0);

    // 4. Configure Flash Latency and Prefetch/Cache Buffers
    REG_MODIFY_FIELD_CMSIS(&(FLASH->ACR), FLASH_ACR_LATENCY_Msk, FLASH_ACR_LATENCY_Pos, clock_param->WaitState);
    REG_SET_MASK_CMSIS(&(FLASH->ACR), FLASH_ACR_PRFTEN);
    REG_SET_MASK_CMSIS(&(FLASH->ACR), FLASH_ACR_DCEN);
    REG_SET_MASK_CMSIS(&(FLASH->ACR), FLASH_ACR_ICEN);

    // 5. Configure PLL parameters (M, N, P, Q) and select HSE as PLL source
    REG_MODIFY_FIELD_CMSIS(&(RCC->PLLCFGR), RCC_PLLCFGR_PLLM_Msk, RCC_PLLCFGR_PLLM_Pos, clock_param->pll_m);
    REG_MODIFY_FIELD_CMSIS(&(RCC->PLLCFGR), RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_Pos, clock_param->pll_n);
    REG_MODIFY_FIELD_CMSIS(&(RCC->PLLCFGR), RCC_PLLCFGR_PLLP_Msk, RCC_PLLCFGR_PLLP_Pos, clock_param->pll_p);
    REG_MODIFY_FIELD_CMSIS(&(RCC->PLLCFGR), RCC_PLLCFGR_PLLQ_Msk, RCC_PLLCFGR_PLLQ_Pos, clock_param->pll_q);
    
    REG_SET_MASK_CMSIS(&(RCC->PLLCFGR), RCC_PLLCFGR_PLLSRC_HSE);

    // 6. Configure Bus Prescalers (AHB, APB1, APB2) BEFORE switching system clock
    REG_MODIFY_FIELD_CMSIS(&(RCC->CFGR), RCC_CFGR_HPRE_Msk, RCC_CFGR_HPRE_Pos, clock_param->ahb_prescaler);
    REG_MODIFY_FIELD_CMSIS(&(RCC->CFGR), RCC_CFGR_PPRE1_Msk, RCC_CFGR_PPRE1_Pos, clock_param->apb_1_prescalar);
    REG_MODIFY_FIELD_CMSIS(&(RCC->CFGR), RCC_CFGR_PPRE2_Msk, RCC_CFGR_PPRE2_Pos, clock_param->apb_2_prescalar);

    // 7. Turn on main PLL and wait for PLLRDY lock flag
    REG_SET_MASK_CMSIS(&(RCC->CR), RCC_CR_PLLON);    
    while(((RCC->CR) & RCC_CR_PLLRDY) == 0);

    //A. Check for succesfull setting of VOS scaling to make CPU scaling work properly.
    while(((PWR->CSR) & PWR_CSR_VOSRDY) == 0);

    // 8. Switch System Clock (SYSCLK) source to PLL
    REG_MODIFY_FIELD_CMSIS(&(RCC->CFGR), RCC_CFGR_SW_Msk, RCC_CFGR_SW_Pos, RCC_CFGR_SW_PLL);

    // 9. Wait for System Clock Switch Status (SWS) bits to acknowledge PLL selection
    while(REG_GET_FIELD_CMSIS(&(RCC->CFGR), RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_Pos) != 2);

    __NVIC_SetPriorityGrouping(7); // To avoid preemption in the interrupts.

    __enable_irq();
}