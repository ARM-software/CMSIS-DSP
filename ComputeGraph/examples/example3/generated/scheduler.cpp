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
static unsigned int schedule[25]=
{ 
6,2,0,7,3,4,8,1,6,2,0,7,3,4,8,2,0,7,3,4,1,5,8,1,5,
};

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

#define BUFFERSIZE0 256
float32_t buf0[BUFFERSIZE0]={0};

#define BUFFERSIZE1 256
float32_t buf1[BUFFERSIZE1]={0};

#define BUFFERSIZE2 256
float32_t buf2[BUFFERSIZE2]={0};

#define BUFFERSIZE3 512
float32_t buf3[BUFFERSIZE3]={0};

#define BUFFERSIZE4 512
float32_t buf4[BUFFERSIZE4]={0};

#define BUFFERSIZE5 512
float32_t buf5[BUFFERSIZE5]={0};

#define BUFFERSIZE6 256
float32_t buf6[BUFFERSIZE6]={0};

#define BUFFERSIZE7 256
float32_t buf7[BUFFERSIZE7]={0};


uint32_t scheduler(int *error)
{
    int cgStaticError=0;
    uint32_t nbSchedule=0;
    int32_t debugCounter=40;

    /*
    Create FIFOs objects
    */
    FIFO<float32_t,FIFOSIZE0,0> fifo0(buf0);
    FIFO<float32_t,FIFOSIZE1,1> fifo1(buf1);
    FIFO<float32_t,FIFOSIZE2,1> fifo2(buf2);
    FIFO<float32_t,FIFOSIZE3,1> fifo3(buf3);
    FIFO<float32_t,FIFOSIZE4,1> fifo4(buf4);
    FIFO<float32_t,FIFOSIZE5,1> fifo5(buf5);
    FIFO<float32_t,FIFOSIZE6,1> fifo6(buf6);
    FIFO<float32_t,FIFOSIZE7,0> fifo7(buf7);

    /* 
    Create node objects
    */
    OverlapAdd<float32_t,256,128> audioOverlap(fifo6,fifo7);
    SlidingBuffer<float32_t,256,128> audioWin(fifo0,fifo1);
    CFFT<float32_t,512,float32_t,512> cfft(fifo3,fifo4);
    ICFFT<float32_t,512,float32_t,512> icfft(fifo4,fifo5);
    FileSink<float32_t,192> sink(fifo7,"output_example3.txt");
    FileSource<float32_t,192> src(fifo0,"input_example3.txt");
    ToComplex<float32_t,256,float32_t,512> toCmplx(fifo2,fifo3);
    ToReal<float32_t,512,float32_t,256> toReal(fifo5,fifo6);

    /* Run several schedule iterations */
    while((cgStaticError==0) && (debugCounter > 0))
    {
        /* Run a schedule iteration */
        for(unsigned long id=0 ; id < 25; id++)
        {
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
                   CHECKERROR;
                }
                break;

                case 1:
                {
                   cgStaticError = audioOverlap.run();
                   CHECKERROR;
                }
                break;

                case 2:
                {
                   cgStaticError = audioWin.run();
                   CHECKERROR;
                }
                break;

                case 3:
                {
                   cgStaticError = cfft.run();
                   CHECKERROR;
                }
                break;

                case 4:
                {
                   cgStaticError = icfft.run();
                   CHECKERROR;
                }
                break;

                case 5:
                {
                   cgStaticError = sink.run();
                   CHECKERROR;
                }
                break;

                case 6:
                {
                   cgStaticError = src.run();
                   CHECKERROR;
                }
                break;

                case 7:
                {
                   cgStaticError = toCmplx.run();
                   CHECKERROR;
                }
                break;

                case 8:
                {
                   cgStaticError = toReal.run();
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
