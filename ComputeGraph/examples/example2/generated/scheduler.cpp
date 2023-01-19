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
static unsigned int schedule[302]=
{ 
7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,
1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,
3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,
7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,
1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,
3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,
7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,7,8,2,3,
1,7,8,2,3,1,4,5,7,8,2,3,1,7,8,2,3,1,4,5,6,0,
};

CG_BEFORE_FIFO_BUFFERS
/***********

FIFO buffers

************/
#define FIFOSIZE0 330
#define FIFOSIZE1 160
#define FIFOSIZE2 160
#define FIFOSIZE3 160
#define FIFOSIZE4 160
#define FIFOSIZE5 320
#define FIFOSIZE6 640
#define FIFOSIZE7 250
#define FIFOSIZE8 500

#define BUFFERSIZE1 330
CG_BEFORE_BUFFER
float32_t buf1[BUFFERSIZE1]={0};

#define BUFFERSIZE2 160
CG_BEFORE_BUFFER
float32_t buf2[BUFFERSIZE2]={0};

#define BUFFERSIZE3 160
CG_BEFORE_BUFFER
float32_t buf3[BUFFERSIZE3]={0};

#define BUFFERSIZE4 160
CG_BEFORE_BUFFER
float32_t buf4[BUFFERSIZE4]={0};

#define BUFFERSIZE5 160
CG_BEFORE_BUFFER
float32_t buf5[BUFFERSIZE5]={0};

#define BUFFERSIZE6 320
CG_BEFORE_BUFFER
float32_t buf6[BUFFERSIZE6]={0};

#define BUFFERSIZE7 640
CG_BEFORE_BUFFER
float32_t buf7[BUFFERSIZE7]={0};

#define BUFFERSIZE8 250
CG_BEFORE_BUFFER
float32_t buf8[BUFFERSIZE8]={0};

#define BUFFERSIZE9 500
CG_BEFORE_BUFFER
float32_t buf9[BUFFERSIZE9]={0};


CG_BEFORE_SCHEDULER_FUNCTION
uint32_t scheduler(int *error,int opt1,int opt2)
{
    int cgStaticError=0;
    uint32_t nbSchedule=0;
    int32_t debugCounter=1;

    CG_BEFORE_FIFO_INIT;
    /*
    Create FIFOs objects
    */
    FIFO<float32_t,FIFOSIZE0,0,0> fifo0(buf1,10);
    FIFO<float32_t,FIFOSIZE1,1,0> fifo1(buf2);
    FIFO<float32_t,FIFOSIZE2,1,0> fifo2(buf3);
    FIFO<float32_t,FIFOSIZE3,1,0> fifo3(buf4);
    FIFO<float32_t,FIFOSIZE4,1,0> fifo4(buf5);
    FIFO<float32_t,FIFOSIZE5,0,0> fifo5(buf6);
    FIFO<float32_t,FIFOSIZE6,1,0> fifo6(buf7);
    FIFO<float32_t,FIFOSIZE7,0,0> fifo7(buf8);
    FIFO<float32_t,FIFOSIZE8,1,0> fifo8(buf9);

    CG_BEFORE_NODE_INIT;
    /* 
    Create node objects
    */
    TFLite<float32_t,500> TFLite(fifo8);
    SlidingBuffer<float32_t,640,320> audioWin(fifo5,fifo6);
    MFCC<float32_t,640,float32_t,10> mfcc(fifo6,fifo7);
    SlidingBuffer<float32_t,500,250> mfccWind(fifo7,fifo8);
    StereoSource<float32_t,320> src(fifo0);
    Unzip<float32_t,320,float32_t,160,float32_t,160> toMono(fifo0,fifo1,fifo2);

    /* Run several schedule iterations */
    CG_BEFORE_SCHEDULE;
    while((cgStaticError==0) && (debugCounter > 0))
    {
        /* Run a schedule iteration */
        CG_BEFORE_ITERATION;
        for(unsigned long id=0 ; id < 302; id++)
        {
            CG_BEFORE_NODE_EXECUTION;

            switch(schedule[id])
            {
                case 0:
                {
                   cgStaticError = TFLite.run();
                }
                break;

                case 1:
                {
                   
                  {

                   float32_t* i0;
                   float32_t* i1;
                   float32_t* o2;
                   i0=fifo3.getReadBuffer(160);
                   i1=fifo4.getReadBuffer(160);
                   o2=fifo5.getWriteBuffer(160);
                   arm_add_f32(i0,i1,o2,160);
                   cgStaticError = 0;
                  }
                }
                break;

                case 2:
                {
                   
                  {

                   float32_t* i0;
                   float32_t* o2;
                   i0=fifo1.getReadBuffer(160);
                   o2=fifo3.getWriteBuffer(160);
                   arm_scale_f32(i0,HALF,o2,160);
                   cgStaticError = 0;
                  }
                }
                break;

                case 3:
                {
                   
                  {

                   float32_t* i0;
                   float32_t* o2;
                   i0=fifo2.getReadBuffer(160);
                   o2=fifo4.getWriteBuffer(160);
                   arm_scale_f32(i0,HALF,o2,160);
                   cgStaticError = 0;
                  }
                }
                break;

                case 4:
                {
                   cgStaticError = audioWin.run();
                }
                break;

                case 5:
                {
                   cgStaticError = mfcc.run();
                }
                break;

                case 6:
                {
                   cgStaticError = mfccWind.run();
                }
                break;

                case 7:
                {
                   cgStaticError = src.run();
                }
                break;

                case 8:
                {
                   cgStaticError = toMono.run();
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
