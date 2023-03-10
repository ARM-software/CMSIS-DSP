#include "RTE_Components.h"
#include  CMSIS_device_header

#define SYSTICK_INITIAL_VALUE       0x0FFFFFF
static int32_t startCycles=0;

void initCycleMeasurement()
{
    SysTick->LOAD = SYSTICK_INITIAL_VALUE;
    SysTick->VAL = 0;
    SysTick->CTRL = 0;
}

void cycleMeasurementStart()
{

   
    SysTick->CTRL = 0;
    SysTick->LOAD = SYSTICK_INITIAL_VALUE;
    SysTick->VAL = 0;

    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;  

   /* For AVH simulation ONLY */
    while(SysTick->VAL == 0);

    startCycles = SysTick->VAL;
}

void cycleMeasurementStop()
{
    SysTick->CTRL = 0;
    SysTick->LOAD = SYSTICK_INITIAL_VALUE;
}

int32_t getCycles()
{
    int32_t v = SysTick->VAL;
    int32_t result;

   
    result = (int32_t)startCycles - (int32_t)v;

    if (result < 0) 
    {
        result += SYSTICK_INITIAL_VALUE;
    }

    return(result);
}