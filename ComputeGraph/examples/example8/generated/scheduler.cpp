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

#if !defined(CHECKERROR)
#define CHECKERROR       if (cgStaticError < 0) \
       {\
         goto errorHandling;\
       }

#endif

#if !defined(CG_BEFORE_ITERATION)
#define CG_BEFORE_ITERATION
#endif 

#if !defined(CG_AFTER_ITERATION)
#define CG_AFTER_ITERATION
#endif 

#if !defined(CG_BEFORE_SCHEDULE)
#define CG_BEFORE_SCHEDULE
#endif

#if !defined(CG_AFTER_SCHEDULE)
#define CG_AFTER_SCHEDULE
#endif

#if !defined(CG_BEFORE_BUFFER)
#define CG_BEFORE_BUFFER
#endif

#if !defined(CG_BEFORE_FIFO_BUFFERS)
#define CG_BEFORE_FIFO_BUFFERS
#endif

#if !defined(CG_BEFORE_FIFO_INIT)
#define CG_BEFORE_FIFO_INIT
#endif

#if !defined(CG_BEFORE_NODE_INIT)
#define CG_BEFORE_NODE_INIT
#endif

#if !defined(CG_AFTER_INCLUDES)
#define CG_AFTER_INCLUDES
#endif

#if !defined(CG_BEFORE_SCHEDULER_FUNCTION)
#define CG_BEFORE_SCHEDULER_FUNCTION
#endif

#if !defined(CG_BEFORE_NODE_EXECUTION)
#define CG_BEFORE_NODE_EXECUTION
#endif

#if !defined(CG_AFTER_NODE_EXECUTION)
#define CG_AFTER_NODE_EXECUTION
#endif

CG_AFTER_INCLUDES


/*

Description of the scheduling. 

*/
static unsigned int schedule[37]=
{ 
6,6,1,5,0,2,3,4,6,1,5,0,2,3,4,6,6,1,5,0,2,3,4,6,1,5,0,2,3,4,6,1,5,0,2,3,4,
};

CG_BEFORE_FIFO_BUFFERS
/***********

FIFO buffers

************/
#define FIFOSIZE0 11
#define FIFOSIZE1 5
#define FIFOSIZE2 5
#define FIFOSIZE3 5
#define FIFOSIZE4 5
#define FIFOSIZE5 5

#define BUFFERSIZE1 11
CG_BEFORE_BUFFER
complex buf1[BUFFERSIZE1]={0};

#define BUFFERSIZE2 5
CG_BEFORE_BUFFER
complex buf2[BUFFERSIZE2]={0};

#define BUFFERSIZE3 5
CG_BEFORE_BUFFER
complex buf3[BUFFERSIZE3]={0};

#define BUFFERSIZE4 5
CG_BEFORE_BUFFER
complex buf4[BUFFERSIZE4]={0};

#define BUFFERSIZE5 5
CG_BEFORE_BUFFER
complex buf5[BUFFERSIZE5]={0};

#define BUFFERSIZE6 5
CG_BEFORE_BUFFER
complex buf6[BUFFERSIZE6]={0};


CG_BEFORE_SCHEDULER_FUNCTION
uint32_t scheduler(int *error,int someVariable)
{
    int cgStaticError=0;
    uint32_t nbSchedule=0;
    int32_t debugCounter=1;

    CG_BEFORE_FIFO_INIT;
    /*
    Create FIFOs objects
    */
    FIFO<complex,FIFOSIZE0,0,0> fifo0(buf1);
    FIFO<complex,FIFOSIZE1,1,0> fifo1(buf2);
    FIFO<complex,FIFOSIZE2,1,0> fifo2(buf3);
    FIFO<complex,FIFOSIZE3,1,0> fifo3(buf4);
    FIFO<complex,FIFOSIZE4,1,0> fifo4(buf5);
    FIFO<complex,FIFOSIZE5,1,0> fifo5(buf6);

    CG_BEFORE_NODE_INIT;
    /* 
    Create node objects
    */
    Duplicate3<complex,5,complex,5,complex,5,complex,5> dup0(fifo2,fifo3,fifo4,fifo5);
    ProcessingNode<complex,7,complex,5,complex,5> filter(fifo0,fifo2,fifo1,4,"Test",someVariable);
    Sink<complex,5> sa(fifo3);
    Sink<complex,5> sb(fifo4);
    Sink<complex,5> sc(fifo5);
    Sink<complex,5> sd(fifo1);
    Source<complex,5> source(fifo0);

    /* Run several schedule iterations */
    CG_BEFORE_SCHEDULE;
    while((cgStaticError==0) && (debugCounter > 0))
    {
        /* Run a schedule iteration */
        CG_BEFORE_ITERATION;
        for(unsigned long id=0 ; id < 37; id++)
        {
            CG_BEFORE_NODE_EXECUTION;

            switch(schedule[id])
            {
                case 0:
                {
                   cgStaticError = dup0.run();
                }
                break;

                case 1:
                {
                   cgStaticError = filter.run();
                }
                break;

                case 2:
                {
                   cgStaticError = sa.run();
                }
                break;

                case 3:
                {
                   cgStaticError = sb.run();
                }
                break;

                case 4:
                {
                   cgStaticError = sc.run();
                }
                break;

                case 5:
                {
                   cgStaticError = sd.run();
                }
                break;

                case 6:
                {
                   cgStaticError = source.run();
                }
                break;

                default:
                break;
            }
            CG_AFTER_NODE_EXECUTION;
            CHECKERROR;
        }
       debugCounter--;
       CG_AFTER_ITERATION;
       nbSchedule++;
    }

errorHandling:
    CG_AFTER_SCHEDULE;
    *error=cgStaticError;
    return(nbSchedule);
}
