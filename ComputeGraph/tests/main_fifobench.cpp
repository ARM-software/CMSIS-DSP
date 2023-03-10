#include <cstdio>
#include <cstdint>
#if defined(COMMAND_LINE)
#include <cstdlib>
#endif

#include "arm_math.h"
#include "scheduler.h"

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

float32_t input_buffer[192]={0};
float32_t output_buffer[192]={0};

extern "C" {
extern void initCycleMeasurement();
extern void cycleMeasurementStart();
extern void cycleMeasurementStop();
extern int32_t getCycles();
}

#define OVERHEADLOOP 100

uint32_t memtest[50]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,
29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};

void mem(const uint32_t * ptr)
{
__asm volatile ("LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
                "LDR R0,[%[src],#4]\n"
               :[src] "+r" (ptr)
               :
               :"memory","r0"
               );
}

int main(void)
{
  // System Initialization
  SystemCoreClockUpdate();


  int error;
  uint32_t nbSched = 0;
  int32_t overhead = 0;


  printf("Start\n\r");

  initCycleMeasurement();

  for(int i=0;i<OVERHEADLOOP;i++)
   {
      cycleMeasurementStart();
      mem(memtest);
      cycleMeasurementStop();
      overhead += getCycles() - 50;
   }

  overhead = overhead / OVERHEADLOOP;
  printf("Measurement overhead %d\r\n",overhead);

  cycleMeasurementStart();
  nbSched=scheduler(&error,input_buffer,output_buffer);
  cycleMeasurementStop();
  int32_t cycles = (getCycles() - overhead)/nbSched;

  printf("Number of schedule iterations = %d\n\r",nbSched);
  printf("Error code = %d\n\r",error);
  printf("Cycles per iteration = %d\n\r",cycles);


#if defined(COMMAND_LINE)
    exit(0);
#else
    osThreadExit();
#endif
}
