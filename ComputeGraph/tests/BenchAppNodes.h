/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        AppNodes.h
 * Description:  Application nodes for Example 1
 *
 * $Date:        29 July 2021
 * $Revision:    V1.10.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
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

#include <cstdio>
#include "arm_math.h"
#include "cg_status.h"

#include "CFFT.h"
#include "ICFFT.h"

#include "ToComplex.h"
#include "ToReal.h"
#include "SlidingBuffer.h"
#include "OverlapAndAdd.h"


template<typename IN, int inputSize>
class ArraySink:public GenericSink<IN, inputSize>
{
public:
    ArraySink(FIFOBase<IN> &src,IN* outputBuf):
    GenericSink<IN,inputSize>(src),mOutputBuf(outputBuf){
    };

    int prepareForRunning() final
    {
        if (this->willUnderflow())
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };


    int run() final
    {
        IN *b=this->getReadBuffer();
        memcpy(mOutputBuf,b,sizeof(IN)*inputSize);
        
        return(0);
    };

protected:
    IN* mOutputBuf;

};

template<typename OUT,int outputSize>
class ArraySource: GenericSource<OUT,outputSize>
{
public:
    ArraySource(FIFOBase<OUT> &dst, OUT *inputBuf):
    GenericSource<OUT,outputSize>(dst),mInputBuf(inputBuf){};

    int prepareForRunning() final
    {
        if (this->willOverflow())
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };


    int run() final{
        OUT *b=this->getWriteBuffer();
        memcpy(b,mInputBuf,sizeof(OUT)*outputSize);
        
        return(0);
    };
protected:
    OUT *mInputBuf;

};


#endif