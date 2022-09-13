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
static unsigned int schedule[37]=
{ 
6,6,1,5,0,4,3,2,6,1,6,5,0,4,3,2,6,1,5,0,4,3,2,6,1,6,5,0,4,3,2,1,5,0,4,3,2,
};

/***********

FIFO buffers

************/
#define FIFOSIZE0 11
#define FIFOSIZE1 5
#define FIFOSIZE2 5
#define FIFOSIZE3 5
#define FIFOSIZE4 5
#define FIFOSIZE5 5

#define BUFFERSIZE0 11
complex buf0[BUFFERSIZE0]={0};

#define BUFFERSIZE1 5
complex buf1[BUFFERSIZE1]={0};

#define BUFFERSIZE2 5
complex buf2[BUFFERSIZE2]={0};

#define BUFFERSIZE3 5
complex buf3[BUFFERSIZE3]={0};

#define BUFFERSIZE4 5
complex buf4[BUFFERSIZE4]={0};

#define BUFFERSIZE5 5
complex buf5[BUFFERSIZE5]={0};


uint32_t scheduler(int *error,int someVariable)
{
    int cgStaticError=0;
    uint32_t nbSchedule=0;
    int32_t debugCounter=1;

    /*
    Create FIFOs objects
    */
    FIFO<complex,FIFOSIZE0,0> fifo0(buf0);
    FIFO<complex,FIFOSIZE1,1> fifo1(buf1);
    FIFO<complex,FIFOSIZE2,1> fifo2(buf2);
    FIFO<complex,FIFOSIZE3,1> fifo3(buf3);
    FIFO<complex,FIFOSIZE4,1> fifo4(buf4);
    FIFO<complex,FIFOSIZE5,1> fifo5(buf5);

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
    while((cgStaticError==0) && (debugCounter > 0))
    {
        /* Run a schedule iteration */
        for(unsigned long id=0 ; id < 37; id++)
        {
            switch(schedule[id])
            {
                case 0:
                {
                   cgStaticError = dup0.run();
                   CHECKERROR;
                }
                break;

                case 1:
                {
                   cgStaticError = filter.run();
                   CHECKERROR;
                }
                break;

                case 2:
                {
                   cgStaticError = sa.run();
                   CHECKERROR;
                }
                break;

                case 3:
                {
                   cgStaticError = sb.run();
                   CHECKERROR;
                }
                break;

                case 4:
                {
                   cgStaticError = sc.run();
                   CHECKERROR;
                }
                break;

                case 5:
                {
                   cgStaticError = sd.run();
                   CHECKERROR;
                }
                break;

                case 6:
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
