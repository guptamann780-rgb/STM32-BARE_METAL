#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "config_utility.h"
#include "peripherals_utility.h"

const clock_config CLOCK_DEFAULTS = {
    .WaitState = FLASH_ACR_LATENCY_3WS,
    .pll_m = 25,
    .pll_n = 192,
    .pll_p = 0,  // Divison by 2 
    .pll_q = 4,
    .ahb_prescaler = RCC_CFGR_HPRE_DIV1,
    .apb_1_prescalar = RCC_CFGR_PPRE1_DIV2,  // should be kept below 50MHz value
    .apb_2_prescalar = RCC_CFGR_PPRE2_DIV1
};


void default_clock_init(void)
{
    clock_init(&CLOCK_DEFAULTS);
}


void clock_init(const clock_config *clock_param)
{
    REG_SET_MASK_CMSIS(&(RCC->CR),RCC_CR_HSEON);
    while(((RCC->CR) & RCC_CR_HSERDY) == 0);

    REG_SET_MASK_CMSIS(&(RCC->APB1ENR),RCC_APB1ENR_PWREN);
    REG_MODIFY_FIELD_CMSIS(&(PWR->CR), PWR_CR_VOS_Msk, PWR_CR_VOS_Pos, 3);
    while((PWR->CSR & PWR_CSR_VOSRDY) == 0);

    REG_MODIFY_FIELD_CMSIS(&(FLASH->ACR),FLASH_ACR_LATENCY_Msk,FLASH_ACR_LATENCY_Pos, clock_param->WaitState);
    REG_SET_MASK_CMSIS(&(FLASH->ACR),FLASH_ACR_PRFTEN);
    REG_SET_MASK_CMSIS(&(FLASH->ACR),FLASH_ACR_DCEN);
    REG_SET_MASK_CMSIS(&(FLASH->ACR),FLASH_ACR_ICEN);



    REG_MODIFY_FIELD_CMSIS(&(RCC->PLLCFGR), RCC_PLLCFGR_PLLM_Msk, RCC_PLLCFGR_PLLM_Pos, clock_param->pll_m);
    REG_MODIFY_FIELD_CMSIS(&(RCC->PLLCFGR), RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_Pos, clock_param->pll_n);
    REG_MODIFY_FIELD_CMSIS(&(RCC->PLLCFGR), RCC_PLLCFGR_PLLP_Msk, RCC_PLLCFGR_PLLP_Pos, clock_param->pll_p);
    REG_MODIFY_FIELD_CMSIS(&(RCC->PLLCFGR), RCC_PLLCFGR_PLLQ_Msk, RCC_PLLCFGR_PLLQ_Pos, clock_param->pll_q);

    REG_MODIFY_FIELD_CMSIS(&(RCC->CFGR), RCC_CFGR_HPRE_Msk, RCC_CFGR_HPRE_Pos, clock_param->ahb_prescaler);
    REG_MODIFY_FIELD_CMSIS(&(RCC->CFGR), RCC_CFGR_PPRE1_Msk, RCC_CFGR_PPRE1_Pos, clock_param->apb_1_prescalar);
    REG_MODIFY_FIELD_CMSIS(&(RCC->CFGR), RCC_CFGR_PPRE2_Msk, RCC_CFGR_PPRE2_Pos, clock_param->apb_2_prescalar);

    REG_SET_MASK_CMSIS(&(RCC->PLLCFGR),RCC_PLLCFGR_PLLSRC_HSE);

    REG_SET_MASK_CMSIS(&(RCC->CR),RCC_CR_PLLON);    
    while(((RCC->CR) & RCC_CR_PLLRDY) == 0);

    REG_MODIFY_FIELD_CMSIS(&(RCC->CFGR), RCC_CFGR_SW_Msk, RCC_CFGR_SW_Pos, RCC_CFGR_SW_PLL);
    while(REG_GET_FIELD_CMSIS(&(RCC->CFGR), RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_Pos) != 2);
}