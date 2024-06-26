#if !defined(HOST)
#if !defined(NORTE)
   #include "RTE_Components.h"
   #include  CMSIS_device_header
#endif
#endif

#ifdef   __cplusplus

#include <cstdint>
#else
#include <stdint.h>
#endif

#ifdef   __cplusplus


extern "C"
{
#endif

extern  uint32_t start_time;
extern  uint32_t stop_time;
extern  uint32_t cycle_count;

#if defined(HOST) || defined(TESTMODE)
#define INIT_SYSTICK 
#define START_CYCLE_MEASUREMENT \
  start_time = 0;
#define STOP_CYCLE_MEASUREMENT                     \
  stop_time = 0;                                   \
  cycle_count = start_time - stop_time;            \
  printf ("Cycle count = %d\r\n",(int)cycle_count);
#else 
#define INIT_SYSTICK \
 SysTick->CTRL=0;         \
 SysTick->LOAD=0xFFFFFFUL;\
 SysTick->VAL=0;          \
 SysTick->CTRL=5;         \
 while (SysTick->VAL==0)\
    ;  

#define START_CYCLE_MEASUREMENT \
    start_time= SysTick->VAL;

#define STOP_CYCLE_MEASUREMENT \
    stop_time= SysTick->VAL;                  \
    SysTick->CTRL=0;                          \
    cycle_count = start_time - stop_time;     \
    printf ("Cycle count = %d\r\n",(int)cycle_count);
#endif 

#if !defined(HOST) && (__ARM_ARCH > 6)
#define dbgInst(imm) __asm volatile("DBG %0\n\t" : :"Ir" ((imm)) )
#define startSectionNB(num) dbgInst(((num) & 0x7) | 0x0)
#define stopSectionNB(num)  dbgInst(((num) & 0x7) | 0x8)
#else 
#define startSectionNB(num)
#define stopSectionNB(num) 
#endif

#ifdef   __cplusplus
}
#endif