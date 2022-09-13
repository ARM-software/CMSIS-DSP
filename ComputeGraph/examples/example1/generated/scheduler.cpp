/*

Generated with CMSIS-DSP Compute Graph Scripts.
The generated code is not covered by CMSIS-DSP license.

The support classes and code is covered by CMSIS-DSP license.

*/


#include "arm_math.h"
#include "custom.h"
#include "GenericNodes.h"
#include "AppNodes.h"
#include "scheduler.h"

/*

Description of the scheduling. It is a list of nodes to call.
The values are indexes in the previous array.

*/
static unsigned int schedule[17]=
{ 
2,2,0,1,2,0,1,2,2,0,1,2,0,1,2,0,1,
};

/***********

FIFO buffers

************/
#define FIFOSIZE0 11
#define FIFOSIZE1 5

#define BUFFERSIZE0 11
float32_t buf0[BUFFERSIZE0]={0};

#define BUFFERSIZE1 5
float32_t buf1[BUFFERSIZE1]={0};


uint32_t scheduler(int *error,int someVariable)
{
    int cgStaticError=0;
    uint32_t nbSchedule=0;
    int32_t debugCounter=1;

    /*
    Create FIFOs objects
    */
    FIFO<float32_t,FIFOSIZE0,0> fifo0(buf0);
    FIFO<float32_t,FIFOSIZE1,1> fifo1(buf1);

    /* 
    Create node objects
    */
    ProcessingNode<float32_t,7,float32_t,5> filter(fifo0,fifo1,4,"Test",someVariable);
    Sink<float32_t,5> sink(fifo1);
    Source<float32_t,5> source(fifo0);

    /* Run several schedule iterations */
    while((cgStaticError==0) && (debugCounter > 0))
    {
        /* Run a schedule iteration */
        for(unsigned long id=0 ; id < 17; id++)
        {
            switch(schedule[id])
            {
                case 0:
                {
                   cgStaticError = filter.run();
                   CHECKERROR;
                }
                break;

                case 1:
                {
                   cgStaticError = sink.run();
                   CHECKERROR;
                }
                break;

                case 2:
                {
                   cgStaticError = source.run();
                   CHECKERROR;
                }
                break;

                default:
                break;
            }
        }
       debugCounter--;
       nbSchedule++;
    }

errorHandling:
    *error=cgStaticError;
    return(nbSchedule);
}
