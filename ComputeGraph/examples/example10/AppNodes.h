/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        AppNodes.h
 * Description:  Application nodes for Example 10
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- 
 *
 * Copyright (C) 2021-2023 ARM Limited or its affiliates. All rights reserved.
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
#include <cstdio>

#include "NullSink.h"

static int count=0;

template<typename OUT,int outputSize>
class SourceOdd;

template<typename OUT,int outputSize>
class SourceEven;

template<>
class SourceOdd<int16_t,1>: public GenericSource<int16_t,1>
{
public:
    SourceOdd(FIFOBase<int16_t> &dst):
    GenericSource<int16_t,1>(dst){};

    int prepareForRunning() override
    {
        if (this->willOverflow())
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };

    int run() override{
        if (count & 1)
        {
           int16_t *b=this->getWriteBuffer();
           b[0] = count;
        }
        return(0);
    };



};

template<>
class SourceEven<int16_t,1>: public GenericSource<int16_t,1>
{
public:
    SourceEven(FIFOBase<int16_t> &dst):
    GenericSource<int16_t,1>(dst){};

    int prepareForRunning() override
    {
        if (this->willOverflow())
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };

    int run() override{
        if (!(count & 1))
        {
           int16_t *b=this->getWriteBuffer();
           b[0] = count;
        }
        return(0);
    };


};

template<typename INA, int inputSizeA,
         typename INB, int inputSizeB,
         typename OUT,int outputSize>
class ProcessingOddEven;

template<>
class ProcessingOddEven<int16_t,1,
                        int16_t,1,
                        int16_t,1>: 
public GenericNode21<int16_t,1,
                     int16_t,1,
                     int16_t,1>
{
public:
    ProcessingOddEven(FIFOBase<int16_t> &srcOdd,
                      FIFOBase<int16_t> &srcEven,
                      FIFOBase<int16_t> &dst):
    GenericNode21<int16_t,1,
                  int16_t,1,
                  int16_t,1>(srcOdd,srcEven,dst){};

    int prepareForRunning() override
    {
        if (this->willOverflow())
        {
            return(CG_SKIP_EXECUTION_ID_CODE);
        }
        return(0);
    };

    int run()  override{
        int16_t oddVal=0;
        int16_t evenVal=0;
        if (!this->willUnderflow1())
        {
           int16_t *odd=this->getReadBuffer1();
           oddVal=odd[0];
        }

        if (!this->willUnderflow2())
        {
           int16_t *even=this->getReadBuffer2();
           evenVal=even[0];
        }

        int16_t *dst=this->getWriteBuffer();
        
        dst[0] = (evenVal + oddVal);
       
        return(0);
    };

};

template<typename IN, int inputSize>
class SinkAsync;

template<int inputSize>
class SinkAsync<int16_t,inputSize>: 
public GenericSink<int16_t, inputSize>
{
public:
    SinkAsync(FIFOBase<int16_t> &src):
    GenericSink<int16_t,inputSize>(src){
    };

    int prepareForRunning() override
    {
        if (this->willUnderflow())
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };

    int run()  override
    {
        int16_t *b=this->getReadBuffer();
        for(int i=0;i<inputSize;i++)
        {
            printf("%d\n",b[i]);
        }
        return(0);
    };

    

};

void compute(int16_t *in,int16_t *out, int nb)
{
   out[0] = in[0];
}
#endif