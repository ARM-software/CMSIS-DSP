#include <cstdio>
#include <cstdint>
#if defined(COMMAND_LINE)
#include <cstdlib>
#endif
#include "scheduler.h"

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#if defined(RTE_Compiler_EventRecorder)
#include "EventRecorder.h"
#endif 

void app_main (void *argument)
{
    int error;
    uint32_t res=1;
    uint32_t nbSched = 0;
    (void)argument;

    printf("Start\n\r");

#if defined(RTE_Compiler_EventRecorder)
   res = EventRecorderEnable (EventRecordAll, EvtSched, EvtSched);
   if (!res)
   {
      printf("Error enabling event recorder for scheduler\n");
      goto endThread;
   }
#endif

    nbSched=scheduler(&error);
    printf("Number of schedule iterations = %d\n\r",nbSched);
    printf("Error code = %d\n\r",error);

#if defined(RTE_Compiler_EventRecorder)
   res = EventRecorderDisable (EventRecordAll, EvtSched, EvtSched);
   if (!res)
   {
      printf("Error disabling event recorder\n");
   }
#endif

endThread:
#if defined(COMMAND_LINE)
    exit(0);
#else
    osThreadExit();
#endif
}

int main(void)
{
  // System Initialization
  SystemCoreClockUpdate();


  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
