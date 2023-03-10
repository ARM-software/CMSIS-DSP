/*

Generated with CMSIS-DSP Compute Graph Scripts.
The generated code is not covered by CMSIS-DSP license.

The support classes and code is covered by CMSIS-DSP license.

*/


#include "arm_math.h"
#include "custom_bench.h"
#include "GenericNodes.h"
#include "BenchAppNodes.h"
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
static unsigned int schedule[25]=
{ 
6,2,0,7,3,4,8,1,6,2,0,7,3,4,8,1,5,2,0,7,3,4,8,1,5,
};

CG_BEFORE_FIFO_BUFFERS
/***********

FIFO buffers

************/
#define FIFOSIZE0 256
#define FIFOSIZE1 256
#define FIFOSIZE2 256
#define FIFOSIZE3 512
#define FIFOSIZE4 512
#define FIFOSIZE5 512
#define FIFOSIZE6 256
#define FIFOSIZE7 256

#define BUFFERSIZE1 256
CG_BEFORE_BUFFER
float32_t buf1[BUFFERSIZE1]={0};

#define BUFFERSIZE2 256
CG_BEFORE_BUFFER
float32_t buf2[BUFFERSIZE2]={0};

#define BUFFERSIZE3 256
CG_BEFORE_BUFFER
float32_t buf3[BUFFERSIZE3]={0};

#define BUFFERSIZE4 512
CG_BEFORE_BUFFER
float32_t buf4[BUFFERSIZE4]={0};

#define BUFFERSIZE5 512
CG_BEFORE_BUFFER
float32_t buf5[BUFFERSIZE5]={0};

#define BUFFERSIZE6 512
CG_BEFORE_BUFFER
float32_t buf6[BUFFERSIZE6]={0};

#define BUFFERSIZE7 256
CG_BEFORE_BUFFER
float32_t buf7[BUFFERSIZE7]={0};

#define BUFFERSIZE8 256
CG_BEFORE_BUFFER
float32_t buf8[BUFFERSIZE8]={0};


CG_BEFORE_SCHEDULER_FUNCTION
uint32_t scheduler(int *error,float32_t* inputArray,
                              float32_t* outputArray)
{
    int cgStaticError=0;
    uint32_t nbSchedule=0;
    int32_t debugCounter=10;

    CG_BEFORE_FIFO_INIT;
    /*
    Create FIFOs objects
    */
    FIFO<float32_t,FIFOSIZE0,0,1> fifo0(buf1);
    FIFO<float32_t,FIFOSIZE1,0,1> fifo1(buf2);
    FIFO<float32_t,FIFOSIZE2,0,1> fifo2(buf3);
    FIFO<float32_t,FIFOSIZE3,0,1> fifo3(buf4);
    FIFO<float32_t,FIFOSIZE4,0,1> fifo4(buf5);
    FIFO<float32_t,FIFOSIZE5,0,1> fifo5(buf6);
    FIFO<float32_t,FIFOSIZE6,0,1> fifo6(buf7);
    FIFO<float32_t,FIFOSIZE7,0,1> fifo7(buf8);

    CG_BEFORE_NODE_INIT;
    /* 
    Create node objects
    */
    OverlapAdd<float32_t,256,128> audioOverlap(fifo6,fifo7);
    SlidingBuffer<float32_t,256,128> audioWin(fifo0,fifo1);
    CFFT<float32_t,512,float32_t,512> cfft(fifo3,fifo4);
    ICFFT<float32_t,512,float32_t,512> icfft(fifo4,fifo5);
    ArraySink<float32_t,192> sink(fifo7,outputArray);
    ArraySource<float32_t,192> src(fifo0,inputArray);
    ToComplex<float32_t,256,float32_t,512> toCmplx(fifo2,fifo3);
    ToReal<float32_t,512,float32_t,256> toReal(fifo5,fifo6);

    /* Run several schedule iterations */
    CG_BEFORE_SCHEDULE;
    while((cgStaticError==0) && (debugCounter > 0))
    {
        /* Run a schedule iteration */
        CG_BEFORE_ITERATION;
        for(unsigned long id=0 ; id < 25; id++)
        {
            CG_BEFORE_NODE_EXECUTION;

            cgStaticError = 0;
            switch(schedule[id])
            {
                case 0:
                {
                                        
                    bool canRun=true;
                    canRun &= !fifo1.willUnderflowWith(256);
                    canRun &= !fifo2.willOverflowWith(256);

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
                    cgStaticError = audioOverlap.prepareForRunning();
                }
                break;

                case 2:
                {
                    cgStaticError = audioWin.prepareForRunning();
                }
                break;

                case 3:
                {
                    cgStaticError = cfft.prepareForRunning();
                }
                break;

                case 4:
                {
                    cgStaticError = icfft.prepareForRunning();
                }
                break;

                case 5:
                {
                    cgStaticError = sink.prepareForRunning();
                }
                break;

                case 6:
                {
                    cgStaticError = src.prepareForRunning();
                }
                break;

                case 7:
                {
                    cgStaticError = toCmplx.prepareForRunning();
                }
                break;

                case 8:
                {
                    cgStaticError = toReal.prepareForRunning();
                }
                break;

                default:
                break;
            }

            if (cgStaticError == CG_SKIP_EXECUTION_ID_CODE)
            { 
              cgStaticError = 0;
              continue;
            }

            CHECKERROR;

            switch(schedule[id])
            {
                case 0:
                {
                   
                  {

                   float32_t* i0;
                   float32_t* o2;
                   i0=fifo1.getReadBuffer(256);
                   o2=fifo2.getWriteBuffer(256);
                   arm_mult_f32(i0,HANN,o2,256);
                   cgStaticError = 0;
                  }
                }
                break;

                case 1:
                {
                   cgStaticError = audioOverlap.run();
                }
                break;

                case 2:
                {
                   cgStaticError = audioWin.run();
                }
                break;

                case 3:
                {
                   cgStaticError = cfft.run();
                }
                break;

                case 4:
                {
                   cgStaticError = icfft.run();
                }
                break;

                case 5:
                {
                   cgStaticError = sink.run();
                }
                break;

                case 6:
                {
                   cgStaticError = src.run();
                }
                break;

                case 7:
                {
                   cgStaticError = toCmplx.run();
                }
                break;

                case 8:
                {
                   cgStaticError = toReal.run();
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
