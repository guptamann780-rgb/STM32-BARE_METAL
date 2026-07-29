#include <stdint.h>

#define REG_SET_BIT(ADDR,BIT) ((*(volatile uint32_t*)(ADDR)) |= (((uint32_t)1) << (BIT)))

#define REG_CLEAR_BIT(ADDR,BIT) ((*(volatile uint32_t*)(ADDR)) &= ~(((uint32_t)1) << (BIT)))

#define REG_WRITE_FIELD(ADDR,SHIFT,WIDTH,VAL) ((*(volatile uint32_t*)(ADDR))= (((*(volatile uint32_t*)(ADDR))& (~(((((uint32_t)1)<<(WIDTH))-((uint32_t)1))<<(SHIFT)))) | (((uint32_t)(VAL) & ((((uint32_t)1)<<(WIDTH))-((uint32_t)1)))<< SHIFT)))

#define REG_READ_FIELD(ADDR,SHIFT,WIDTH) (((*(volatile uint32_t*)(ADDR)) & (((((uint32_t)1) << (WIDTH)) -1)<<(SHIFT))) >> (SHIFT))

/* CMSIS blended utility */

#define REG_SET_MASK_CMSIS(ADDR, MSK)   (*(volatile uint32_t*)(ADDR) |= (MSK))
#define REG_CLEAR_MASK_CMSIS(ADDR, MSK) (*(volatile uint32_t*)(ADDR) &= ~(MSK))
#define REG_MODIFY_FIELD_CMSIS(ADDR, MSK, POS, VAL) (*(volatile uint32_t*)(ADDR) = ((*(volatile uint32_t*)(ADDR) & ~(MSK)) | (((uint32_t)(VAL) << (POS)) & (MSK))))
#define REG_GET_FIELD_CMSIS(ADDR, MSK, POS) (((*(volatile uint32_t*)(ADDR)) & (MSK)) >> (POS))