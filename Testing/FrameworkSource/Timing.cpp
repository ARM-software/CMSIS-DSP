#include "Timing.h"

#ifdef CORTEXM

#define SYSTICK_INITIAL_VALUE       0x0FFFFFF
static uint32_t startCycles=0;
#endif

#if defined(CORTEXA) || defined(CORTEXR)
#if !defined(__GNUC_PYTHON__)
#include "cmsis_cp15.h"
#else
#if defined(__aarch64__)
#include "timing_aarch64.h"
#define AARCH64_TIMING
#endif
#endif

#if defined(CORTEXA) && defined(AARCH64_TIMING)
unsigned long long startCycles;
#else
unsigned int startCycles;
#endif 

#define DO_RESET 1
#define ENABLE_DIVIDER 0 
#endif

#if defined(EXTBENCH)  || defined(CACHEANALYSIS)
unsigned long sectionCounter=0;
#endif 

void initCycleMeasurement()
{
#if !defined(NOTIMING)
#ifdef CORTEXM
    SysTick->LOAD = SYSTICK_INITIAL_VALUE;
    SysTick->VAL = 0;
    SysTick->CTRL = 0;
#endif 

#if defined(CORTEXA) || defined(CORTEXR)
#if !defined(AARCH64_TIMING)

    // in general enable all counters (including cycle counter)
    int32_t   value = 1;
    

    // peform reset:
    if (DO_RESET)
    {
        value |= 2;             // reset all counters to zero.
        value |= 4;             // reset cycle counter to zero.
    }

    if (ENABLE_DIVIDER)
        value |= 8;             // enable "by 64" divider for CCNT.
    //value |= 16;

    // program the performance-counter control-register:
    __set_CP(15, 0, value, 9, 12, 0);

    // enable all counters:
    __set_CP(15, 0, 0x8000000f, 9, 12, 1);

    // clear overflows:
    __set_CP(15, 0, 0x8000000f, 9, 12, 3);

    #if defined(ARMCR52)
      __get_CP(15, 0, value, 14, 15, 7);
      value = value | (0x8000 << 12);
      __set_CP(15, 0, value, 14, 15, 7);
    #endif
#else
      enable_timing();
#endif
#endif
#endif
}

void cycleMeasurementStart()
{
#if !defined(NOTIMING)
#ifndef EXTBENCH
#ifdef CORTEXM
   
    SysTick->CTRL = 0;
    SysTick->LOAD = SYSTICK_INITIAL_VALUE;
    SysTick->VAL = 0;

    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;  

    startCycles = SysTick->VAL;


    
#endif

#if (defined(CORTEXA) || defined(CORTEXR))
    #if !defined(AARCH64_TIMING)
    unsigned int value;
    // Read CCNT Register
    __get_CP(15, 0, value, 9, 13, 0);
    startCycles =  value;
    #else 
    startCycles = readCCNT();
    #endif
#endif
#endif 
#endif
}

void cycleMeasurementStop()
{
#if !defined(NOTIMING)
#ifndef EXTBENCH
#ifdef CORTEXM
    SysTick->CTRL = 0;
    SysTick->LOAD = SYSTICK_INITIAL_VALUE;
#endif
#endif
#endif
}

Testing::cycles_t getCycles()
{
#if defined(NOTIMING)
return(0);
#else
#ifdef CORTEXM
    #if defined(NORMALFVP)
    return(0);
    #else
    uint32_t v = SysTick->VAL;
    int32_t result;
   
    result = (int32_t)startCycles - (int32_t)v;

    if (result < 0) 
    {
        result += SYSTICK_INITIAL_VALUE;
    }

    
    
    /* SysTick tested and tuned on IPSS.
       On other FVP, the value is forced to 0
       because measurement is wrong.
    */
    
    return(result);
    #endif
#endif 

#if (defined(CORTEXA) || defined(CORTEXR))
    #if !defined(AARCH64_TIMING)
    unsigned int value;
    // Read CCNT Register
    __get_CP(15, 0, value, 9, 13, 0);
    return(value - startCycles);
    #else
    unsigned long long value;
    value = readCCNT();
    return((Testing::cycles_t)(value - startCycles));
    #endif
#endif
#endif
   
}
