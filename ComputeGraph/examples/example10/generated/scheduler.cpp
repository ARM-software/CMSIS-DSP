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
static unsigned int schedule[7]=
{ 
5,6,2,0,1,3,4,
};

CG_BEFORE_FIFO_BUFFERS
/***********

FIFO buffers

************/
#define FIFOSIZE0 2
#define FIFOSIZE1 2
#define FIFOSIZE2 2
#define FIFOSIZE3 2
#define FIFOSIZE4 2
#define FIFOSIZE5 2

#define BUFFERSIZE1 2
CG_BEFORE_BUFFER
int16_t buf1[BUFFERSIZE1]={0};

#define BUFFERSIZE2 2
CG_BEFORE_BUFFER
int16_t buf2[BUFFERSIZE2]={0};

#define BUFFERSIZE3 2
CG_BEFORE_BUFFER
int16_t buf3[BUFFERSIZE3]={0};

#define BUFFERSIZE4 2
CG_BEFORE_BUFFER
int16_t buf4[BUFFERSIZE4]={0};

#define BUFFERSIZE5 2
CG_BEFORE_BUFFER
int16_t buf5[BUFFERSIZE5]={0};

#define BUFFERSIZE6 2
CG_BEFORE_BUFFER
int16_t buf6[BUFFERSIZE6]={0};


CG_BEFORE_SCHEDULER_FUNCTION
uint32_t scheduler(int *error)
{
    int cgStaticError=0;
    uint32_t nbSchedule=0;
    int32_t debugCounter=10;

    CG_BEFORE_FIFO_INIT;
    /*
    Create FIFOs objects
    */
    FIFO<int16_t,FIFOSIZE0,0,1> fifo0(buf1);
    FIFO<int16_t,FIFOSIZE1,0,1> fifo1(buf2);
    FIFO<int16_t,FIFOSIZE2,0,1> fifo2(buf3);
    FIFO<int16_t,FIFOSIZE3,0,1> fifo3(buf4);
    FIFO<int16_t,FIFOSIZE4,0,1> fifo4(buf5);
    FIFO<int16_t,FIFOSIZE5,0,1> fifo5(buf6);

    CG_BEFORE_NODE_INIT;
    /* 
    Create node objects
    */
    Duplicate2<int16_t,1,int16_t,1,int16_t,1> dup0(fifo3,fifo4,fifo5);
    ProcessingOddEven<int16_t,1,int16_t,1,int16_t,1> proc(fifo0,fifo1,fifo2);
    SinkAsync<int16_t,1> sinka(fifo4);
    SinkAsync<int16_t,1> sinkb(fifo5);
    SourceEven<int16_t,1> sourceEven(fifo1);
    SourceOdd<int16_t,1> sourceOdd(fifo0);

    /* Run several schedule iterations */
    CG_BEFORE_SCHEDULE;
    while((cgStaticError==0) && (debugCounter > 0))
    {
        /* Run a schedule iteration */
        CG_BEFORE_ITERATION;
        for(unsigned long id=0 ; id < 7; id++)
        {
            CG_BEFORE_NODE_EXECUTION;

            cgStaticError = 0;
            switch(schedule[id])
            {
                case 0:
                {
                                        
                    bool canRun=true;
                    canRun &= !fifo2.willUnderflowWith(1);
                    canRun &= !fifo3.willOverflowWith(1);

                    if (!canRun)
                    {
                      cgStaticError = CG_SKIP_EXECUTION_ID_CODE;
                    }
                    else
                    {
                        cgStaticError = 0;
                    }
                }
                break;

                case 1:
                {
                    cgStaticError = dup0.prepareForRunning();
                }
                break;

                case 2:
                {
                    cgStaticError = proc.prepareForRunning();
                }
                break;

                case 3:
                {
                    cgStaticError = sinka.prepareForRunning();
                }
                break;

                case 4:
                {
                    cgStaticError = sinkb.prepareForRunning();
                }
                break;

                case 5:
                {
                    cgStaticError = sourceEven.prepareForRunning();
                }
                break;

                case 6:
                {
                    cgStaticError = sourceOdd.prepareForRunning();
                }
                break;

                default:
                break;
            }

            if (cgStaticError == CG_SKIP_EXECUTION_ID_CODE)
                continue;

            CHECKERROR;

            switch(schedule[id])
            {
                case 0:
                {
                   
                  {

                   int16_t* i0;
                   int16_t* o1;
                   i0=fifo2.getReadBuffer(1);
                   o1=fifo3.getWriteBuffer(1);
                   compute(i0,o1,1);
                   cgStaticError = 0;
                  }
                }
                break;

                case 1:
                {
                   cgStaticError = dup0.run();
                }
                break;

                case 2:
                {
                   cgStaticError = proc.run();
                }
                break;

                case 3:
                {
                   cgStaticError = sinka.run();
                }
                break;

                case 4:
                {
                   cgStaticError = sinkb.run();
                }
                break;

                case 5:
                {
                   cgStaticError = sourceEven.run();
                }
                break;

                case 6:
                {
                   cgStaticError = sourceOdd.run();
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
