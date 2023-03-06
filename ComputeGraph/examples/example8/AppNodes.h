/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        AppNodes.h
 * Description:  Application nodes for Example 8
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- 
*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _APPNODES_H_
#define _APPNODES_H_

#include <iostream>

template<typename IN, int inputSize>
class Sink: public GenericSink<IN, inputSize>
{
public:
    Sink(FIFOBase<IN> &src):GenericSink<IN,inputSize>(src){};

    int prepareForRunning() override
    {
        if (this->willUnderflow()
           )
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };

    int run()
    {
        IN *b=this->getReadBuffer();
        printf("Sink\n");
        for(int i=0;i<inputSize;i++)
        {
            std::cout << b[i].re << " + I " << b[i].im << std::endl;
        }
        return(0);
    };

};

template<typename OUT,int outputSize>
class Source;

template<int outputSize>
class Source<complex,outputSize>: GenericSource<complex,outputSize>
{
public:
    Source(FIFOBase<complex> &dst):GenericSource<complex,outputSize>(dst),mCounter(0){};

    int prepareForRunning() override
    {
        if (this->willOverflow()
           )
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };

    int run(){
        complex *b=this->getWriteBuffer();

        printf("Source\n");
        for(int i=0;i<outputSize;i++)
        {
            b[i].re = (float32_t)mCounter++;
            b[i].im = 0.0;
        }
        return(0);
    };

    int mCounter;

};

template<typename IN, int inputSize,
         typename OUT1,int outputSize1,
         typename OUT2,int outputSize2>
class ProcessingNode: public GenericNode12<IN,inputSize,
                                           OUT1,outputSize1,
                                           OUT2,outputSize2>
{
public:
    ProcessingNode(FIFOBase<IN> &src,
                   FIFOBase<OUT1> &dst1,
                   FIFOBase<OUT2> &dst2,
                   int,const char*,int):
    GenericNode12<IN,inputSize,
                  OUT1,outputSize1,
                  OUT2,outputSize2>(src,dst1,dst2){};

    int prepareForRunning() override
    {
        if (this->willOverflow1() ||
            this->willOverflow2() ||
            this->willUnderflow()
           )
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };
    
    int run(){
        printf("ProcessingNode\n");
        IN *a=this->getReadBuffer();
        OUT1 *b=this->getWriteBuffer1();
        OUT2 *c=this->getWriteBuffer2();
        b[0] =(OUT1)a[3];
        c[0] =(OUT2)a[3];
        return(0);
    };

};

#endif