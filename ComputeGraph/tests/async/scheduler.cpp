/*

Generated with CMSIS-DSP Compute Graph Scripts.
The generated code is not covered by CMSIS-DSP license.

The support classes and code is covered by CMSIS-DSP license.

*/


#include "custom.h"
#include "GenericNodes.h"
#include "ComplexAppNodes.h"
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
static unsigned int schedule[1864]=
{ 
16,6,7,8,0,2,3,12,13,5,15,3,12,13,9,1,4,14,10,11,11,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,
4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,
12,13,2,5,15,3,12,13,9,1,10,11,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,
2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,
3,12,13,9,1,10,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,
12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,
11,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,
4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,4,14,2,
5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,
5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,11,4,14,2,5,15,4,14,3,
12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,
12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,
14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,
14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,
5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,
0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,
12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,
5,15,3,12,13,9,1,10,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,
14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,
1,10,11,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,
5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,4,
14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,
13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,11,4,14,2,5,15,4,
14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,
14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,4,14,2,5,15,4,14,3,12,13,2,5,
15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,
15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,
13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,
7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,
3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,
2,5,15,3,12,13,9,1,10,11,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,
15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,
13,9,1,10,11,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,
13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,
11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,
3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,11,4,14,2,5,
15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,
15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,4,14,2,5,15,4,14,3,12,13,
2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,
2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,
3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,
16,6,7,8,0,3,12,13,2,5,15,3,12,13,9,1,10,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,
4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,
12,13,2,5,15,3,12,13,9,1,10,11,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,
2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,16,6,7,8,0,3,12,13,2,5,15,
3,12,13,9,1,10,11,11,11,4,14,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,
3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,3,12,13,2,5,15,4,14,
};

CG_BEFORE_FIFO_BUFFERS
/***********

FIFO buffers

************/
#define FIFOSIZE0 128
#define FIFOSIZE1 128
#define FIFOSIZE2 128
#define FIFOSIZE3 352
#define FIFOSIZE4 16
#define FIFOSIZE5 16
#define FIFOSIZE6 16
#define FIFOSIZE7 16
#define FIFOSIZE8 16
#define FIFOSIZE9 16
#define FIFOSIZE10 16
#define FIFOSIZE11 128
#define FIFOSIZE12 128
#define FIFOSIZE13 128
#define FIFOSIZE14 128
#define FIFOSIZE15 128
#define FIFOSIZE16 128
#define FIFOSIZE17 128

#define BUFFERSIZE1 128
CG_BEFORE_BUFFER
float32_t buf1[BUFFERSIZE1]={0};

#define BUFFERSIZE2 128
CG_BEFORE_BUFFER
float32_t buf2[BUFFERSIZE2]={0};

#define BUFFERSIZE3 128
CG_BEFORE_BUFFER
float32_t buf3[BUFFERSIZE3]={0};

#define BUFFERSIZE4 352
CG_BEFORE_BUFFER
float32_t buf4[BUFFERSIZE4]={0};

#define BUFFERSIZE5 16
CG_BEFORE_BUFFER
float32_t buf5[BUFFERSIZE5]={0};

#define BUFFERSIZE6 16
CG_BEFORE_BUFFER
float32_t buf6[BUFFERSIZE6]={0};

#define BUFFERSIZE7 16
CG_BEFORE_BUFFER
float32_t buf7[BUFFERSIZE7]={0};

#define BUFFERSIZE8 16
CG_BEFORE_BUFFER
float32_t buf8[BUFFERSIZE8]={0};

#define BUFFERSIZE9 16
CG_BEFORE_BUFFER
float32_t buf9[BUFFERSIZE9]={0};

#define BUFFERSIZE10 16
CG_BEFORE_BUFFER
float32_t buf10[BUFFERSIZE10]={0};

#define BUFFERSIZE11 16
CG_BEFORE_BUFFER
float32_t buf11[BUFFERSIZE11]={0};

#define BUFFERSIZE12 128
CG_BEFORE_BUFFER
float32_t buf12[BUFFERSIZE12]={0};

#define BUFFERSIZE13 128
CG_BEFORE_BUFFER
float32_t buf13[BUFFERSIZE13]={0};

#define BUFFERSIZE14 128
CG_BEFORE_BUFFER
float32_t buf14[BUFFERSIZE14]={0};

#define BUFFERSIZE15 128
CG_BEFORE_BUFFER
float32_t buf15[BUFFERSIZE15]={0};

#define BUFFERSIZE16 128
CG_BEFORE_BUFFER
float32_t buf16[BUFFERSIZE16]={0};

#define BUFFERSIZE17 128
CG_BEFORE_BUFFER
float32_t buf17[BUFFERSIZE17]={0};

#define BUFFERSIZE18 128
CG_BEFORE_BUFFER
float32_t buf18[BUFFERSIZE18]={0};


CG_BEFORE_SCHEDULER_FUNCTION
uint32_t scheduler(int *error)
{
    int cgStaticError=0;
    uint32_t nbSchedule=0;
    int32_t debugCounter=1;

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
    FIFO<float32_t,FIFOSIZE8,0,1> fifo8(buf9);
    FIFO<float32_t,FIFOSIZE9,0,1> fifo9(buf10);
    FIFO<float32_t,FIFOSIZE10,0,1> fifo10(buf11);
    FIFO<float32_t,FIFOSIZE11,0,1> fifo11(buf12);
    FIFO<float32_t,FIFOSIZE12,0,1> fifo12(buf13);
    FIFO<float32_t,FIFOSIZE13,0,1> fifo13(buf14);
    FIFO<float32_t,FIFOSIZE14,0,1> fifo14(buf15);
    FIFO<float32_t,FIFOSIZE15,0,1> fifo15(buf16);
    FIFO<float32_t,FIFOSIZE16,0,1> fifo16(buf17);
    FIFO<float32_t,FIFOSIZE17,0,1> fifo17(buf18);

    CG_BEFORE_NODE_INIT;
    /* 
    Create node objects
    */
    Duplicate3<float32_t,128,float32_t,128,float32_t,128,float32_t,128> dup0(fifo11,fifo12,fifo13,fifo14);
    Duplicate2<float32_t,128,float32_t,128,float32_t,128> dup1(fifo15,fifo16,fifo17);
    ProcessingNode12<float32_t,16,float32_t,16,float32_t,16> proc12(fifo13,fifo4,fifo5);
    ProcessingNode13<float32_t,16,float32_t,16,float32_t,16,float32_t,16> proc13(fifo14,fifo6,fifo7,fifo8);
    ProcessingNode21<float32_t,16,float32_t,16,float32_t,16> proc21A(fifo17,fifo4,fifo9);
    ProcessingNode21<float32_t,16,float32_t,16,float32_t,16> proc21B(fifo5,fifo6,fifo10);
    ProcessingNode<float32_t,128,float32_t,128> procA(fifo0,fifo1);
    ProcessingNode<float32_t,128,float32_t,128> procB(fifo1,fifo2);
    ProcessingNode<float32_t,128,float32_t,128> procC(fifo2,fifo11);
    ProcessingNode<float32_t,128,float32_t,128> procD(fifo12,fifo15);
    ProcessingNode<float32_t,128,float32_t,256> procE(fifo16,fifo3);
    Sink<float32_t,100> sink(fifo3);
    Sink<float32_t,16> sinkB(fifo7);
    Sink<float32_t,16> sinkC(fifo8);
    Sink<float32_t,16> sinkD(fifo9);
    Sink<float32_t,16> sinkE(fifo10);
    Source<float32_t,128> source(fifo0);

    /* Run several schedule iterations */
    CG_BEFORE_SCHEDULE;
    while((cgStaticError==0) && (debugCounter > 0))
    {
        /* Run a schedule iteration */
        CG_BEFORE_ITERATION;
        for(unsigned long id=0 ; id < 1864; id++)
        {
            CG_BEFORE_NODE_EXECUTION;

            cgStaticError = 0;
            switch(schedule[id])
            {
                case 0:
                {
                    cgStaticError = dup0.prepareForRunning();
                }
                break;

                case 1:
                {
                    cgStaticError = dup1.prepareForRunning();
                }
                break;

                case 2:
                {
                    cgStaticError = proc12.prepareForRunning();
                }
                break;

                case 3:
                {
                    cgStaticError = proc13.prepareForRunning();
                }
                break;

                case 4:
                {
                    cgStaticError = proc21A.prepareForRunning();
                }
                break;

                case 5:
                {
                    cgStaticError = proc21B.prepareForRunning();
                }
                break;

                case 6:
                {
                    cgStaticError = procA.prepareForRunning();
                }
                break;

                case 7:
                {
                    cgStaticError = procB.prepareForRunning();
                }
                break;

                case 8:
                {
                    cgStaticError = procC.prepareForRunning();
                }
                break;

                case 9:
                {
                    cgStaticError = procD.prepareForRunning();
                }
                break;

                case 10:
                {
                    cgStaticError = procE.prepareForRunning();
                }
                break;

                case 11:
                {
                    cgStaticError = sink.prepareForRunning();
                }
                break;

                case 12:
                {
                    cgStaticError = sinkB.prepareForRunning();
                }
                break;

                case 13:
                {
                    cgStaticError = sinkC.prepareForRunning();
                }
                break;

                case 14:
                {
                    cgStaticError = sinkD.prepareForRunning();
                }
                break;

                case 15:
                {
                    cgStaticError = sinkE.prepareForRunning();
                }
                break;

                case 16:
                {
                    cgStaticError = source.prepareForRunning();
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
                   cgStaticError = dup0.run();
                }
                break;

                case 1:
                {
                   cgStaticError = dup1.run();
                }
                break;

                case 2:
                {
                   cgStaticError = proc12.run();
                }
                break;

                case 3:
                {
                   cgStaticError = proc13.run();
                }
                break;

                case 4:
                {
                   cgStaticError = proc21A.run();
                }
                break;

                case 5:
                {
                   cgStaticError = proc21B.run();
                }
                break;

                case 6:
                {
                   cgStaticError = procA.run();
                }
                break;

                case 7:
                {
                   cgStaticError = procB.run();
                }
                break;

                case 8:
                {
                   cgStaticError = procC.run();
                }
                break;

                case 9:
                {
                   cgStaticError = procD.run();
                }
                break;

                case 10:
                {
                   cgStaticError = procE.run();
                }
                break;

                case 11:
                {
                   cgStaticError = sink.run();
                }
                break;

                case 12:
                {
                   cgStaticError = sinkB.run();
                }
                break;

                case 13:
                {
                   cgStaticError = sinkC.run();
                }
                break;

                case 14:
                {
                   cgStaticError = sinkD.run();
                }
                break;

                case 15:
                {
                   cgStaticError = sinkE.run();
                }
                break;

                case 16:
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
