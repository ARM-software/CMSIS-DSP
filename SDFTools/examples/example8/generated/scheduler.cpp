/*

Generated with CMSIS-DSP SDF Scripts.
The generated code is not covered by CMSIS-DSP license.

The support classes and code is covered by CMSIS-DSP license.

*/


#include "arm_math.h"
#include "custom.h"
#include "GenericNodes.h"
#include "AppNodes.h"
#include "scheduler.h"

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
    int sdfError=0;
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
    while((sdfError==0) && (debugCounter > 0))
    {
       /* Run a schedule iteration */
       sdfError = source.run();
       CHECKERROR;
       sdfError = source.run();
       CHECKERROR;
       sdfError = filter.run();
       CHECKERROR;
       sdfError = sd.run();
       CHECKERROR;
       sdfError = dup0.run();
       CHECKERROR;
       sdfError = sc.run();
       CHECKERROR;
       sdfError = sb.run();
       CHECKERROR;
       sdfError = sa.run();
       CHECKERROR;
       sdfError = source.run();
       CHECKERROR;
       sdfError = filter.run();
       CHECKERROR;
       sdfError = source.run();
       CHECKERROR;
       sdfError = sd.run();
       CHECKERROR;
       sdfError = dup0.run();
       CHECKERROR;
       sdfError = sc.run();
       CHECKERROR;
       sdfError = sb.run();
       CHECKERROR;
       sdfError = sa.run();
       CHECKERROR;
       sdfError = source.run();
       CHECKERROR;
       sdfError = filter.run();
       CHECKERROR;
       sdfError = sd.run();
       CHECKERROR;
       sdfError = dup0.run();
       CHECKERROR;
       sdfError = sc.run();
       CHECKERROR;
       sdfError = sb.run();
       CHECKERROR;
       sdfError = sa.run();
       CHECKERROR;
       sdfError = source.run();
       CHECKERROR;
       sdfError = filter.run();
       CHECKERROR;
       sdfError = source.run();
       CHECKERROR;
       sdfError = sd.run();
       CHECKERROR;
       sdfError = dup0.run();
       CHECKERROR;
       sdfError = sc.run();
       CHECKERROR;
       sdfError = sb.run();
       CHECKERROR;
       sdfError = sa.run();
       CHECKERROR;
       sdfError = filter.run();
       CHECKERROR;
       sdfError = sd.run();
       CHECKERROR;
       sdfError = dup0.run();
       CHECKERROR;
       sdfError = sc.run();
       CHECKERROR;
       sdfError = sb.run();
       CHECKERROR;
       sdfError = sa.run();
       CHECKERROR;

       debugCounter--;
       nbSchedule++;
    }
    *error=sdfError;
    return(nbSchedule);
}
