#include "test_config.h"
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "stdio.h"

#if defined(MPS3)
#include "cmsis_driver_config.h"
#include "stdout_USART.h"
#endif 

#if defined(RTE_Compiler_EventRecorder)
#include "EventRecorder.h"
#endif 

#include "test.h"


int main(void)
{
#if defined(MPS3)
    stdout_init();
#endif

#if defined(RTE_Compiler_EventRecorder) && !defined(MPS3)
   uint32_t res =  EventRecorderInitialize (EventRecordAll, 1); 
   if (!res)
   {
      printf("Error enabling event recorder\n");
      goto endThread;
   }
#endif
	
    #if !defined(SERIAL_DUMP)
    printf("\033c\r\n\r\n");
    #endif
    printf("\r\n\r\n\r\n----------------------\r\n");
    printf(__TIME__"\r\n");
    #if defined(ARMCM55)
    printf("M55\r\n");
    #endif
    #if defined(ARMCM4_FP)
    printf("ARMCM4_FP\r\n");
    #endif
    #if defined(ARMCM0P)
    printf("ARMCM0P\r\n");
    #endif

    #if defined(MPS3)
    printf("MPS3\r\n");
    #endif
    #if defined(VHT)
    printf("VHT\r\n");
    #endif
    #if defined(IPSS)
    printf("IPSS\r\n");
    #endif

    #if defined(DOT_TEST)
    dot_test();
    #endif
    #if defined(VECTOR_TEST)
    vector_test();
    #endif
    #if defined(ROW_TEST)
    row_test();
    #endif
    #if defined(COL_TEST)
    col_test();
    #endif
    #if defined(MATRIX_TEST)
    matrix_test();
    #endif
    #if 0
    filter_test();
    #endif
    #if defined(FUSION_TEST)
    fusion_test();
    #endif
    //debug_test();

    memory_pool_stats();

#if defined(MPS3)
    while(1);
#else
#if defined(RTE_Compiler_EventRecorder)
endThread:
#endif
    while(0);
#endif
}


