/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        AppNodes.h
 * Description:  Application nodes for Example cyclo
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
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

template<typename IN, int inputSize>
class Sink: public GenericSink<IN, inputSize>
{
public:
    Sink(FIFOBase<IN> &src):GenericSink<IN,inputSize>(src){};

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
        printf("Sink\n");
        for(int i=0;i<inputSize;i++)
        {
            std::cout << (int)b[i] << std::endl;
        }
        return(0);
    };

};

template<typename OUT,int outputSize>
class Source: public GenericSource<OUT,outputSize>
{
public:
    Source(FIFOBase<OUT> &dst):GenericSource<OUT,outputSize>(dst),
    mPeriod(0),mValuePeriodStart(0){};

    int getSamplesForPeriod() const 
    {
        if (mPeriod == 0)
        { 
            return(3);
        }
        return(2);
    }

    void updatePeriod(){
        mPeriod++;
        mValuePeriodStart = 3;
        if (mPeriod == 2)
        {
            mPeriod = 0;
            mValuePeriodStart = 0;
        }
    }

    int prepareForRunning() final
    {
        /* Cyclo static scheduling do not make sense in
           asynchronous mode so the default outputSize is used.
           This function is never used in cyclo-static scheduling
        */
        if (this->willOverflow())
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };

    int run() final{
        OUT *b=this->getWriteBuffer(getSamplesForPeriod());

        printf("Source\n");
        for(int i=0;i<getSamplesForPeriod();i++)
        {
            b[i] = mValuePeriodStart + (OUT)i;

        }
        updatePeriod();
        return(0);
    };

protected:
    int mPeriod;
    OUT mValuePeriodStart;

};

template<typename IN, int inputSize,typename OUT,int outputSize>
class ProcessingNode;


template<typename IN, int inputOutputSize>
class ProcessingNode<IN,inputOutputSize,IN,inputOutputSize>: 
      public GenericNode<IN,inputOutputSize,IN,inputOutputSize>
{
public:
    ProcessingNode(FIFOBase<IN> &src,
                   FIFOBase<IN> &dst):GenericNode<IN,inputOutputSize,
                                                  IN,inputOutputSize>(src,dst){};

    int prepareForRunning() final
    {
        if (this->willOverflow() ||
            this->willUnderflow())
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };
    
    int run() final{
        printf("ProcessingNode\n");
        IN *a=this->getReadBuffer();
        IN *b=this->getWriteBuffer();
        for(int i=0;i<inputOutputSize;i++)
        {
            b[i] = a[i]+1;
        }
        return(0);
    };

};

#endif