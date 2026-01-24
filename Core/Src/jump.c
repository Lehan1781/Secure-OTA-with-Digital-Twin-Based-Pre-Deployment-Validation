#include "jump.h"
#include "stm32f4xx_hal.h"
#include "core_cm4.h"   // for SCB, __set_MSP

typedef void (*app_entry_t)(void);

void jump_to_app(uint32_t app_addr)
{
    uint32_t sp = *(uint32_t*)app_addr;
    uint32_t pc = *(uint32_t*)(app_addr + 4);

    __disable_irq();

    HAL_DeInit();

    /* Stop SysTick */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    /* VERY IMPORTANT: relocate vector table */
    SCB->VTOR = app_addr;

    /* Set stack pointer */
    __set_MSP(sp);

    /* Jump to reset handler */
    app_entry_t app = (app_entry_t)pc;
    app();

    /* Should never return */
    while (1);
}
