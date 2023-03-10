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

#include <cstring>
#include <cstdio>


template<typename IN, int inputSize>
class Sink;

template<int inputSize>
class Sink<float32_t,inputSize>: 
public GenericSink<float32_t, inputSize>
{
public:
    Sink(FIFOBase<float32_t> &src):
    GenericSink<float32_t,inputSize>(src){
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
        float32_t *b=this->getReadBuffer();
        (void)b;
        
        return(0);
    };

    

};

template<typename OUT,int outputSize>
class Source;

template<int outputSize>
class Source<float32_t,outputSize>: GenericSource<float32_t,outputSize>
{
public:
    Source(FIFOBase<float32_t> &dst):
    GenericSource<float32_t,outputSize>(dst){};

    int prepareForRunning() final
    {
        if (this->willOverflow())
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };


    int run() final{
        float32_t *b=this->getWriteBuffer();
        (void)b;
        
        return(0);
    };


};

template<typename IN, int inputSize,typename OUT,int outputSize>
class ProcessingNode;



template<int inputSize,int outputSize>
class ProcessingNode<float32_t,inputSize,
                     float32_t,outputSize>: 
public GenericNode<float32_t,inputSize,
                   float32_t,outputSize>
{
public:
    ProcessingNode(FIFOBase<float32_t> &src,
                   FIFOBase<float32_t> &dst):
    GenericNode<float32_t,inputSize,
                float32_t,outputSize>(src,dst){};

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
        float32_t *a=this->getReadBuffer();
        float32_t *b=this->getWriteBuffer();
        (void)a;
        (void)b;
        
        return(0);
    };

};

template<typename IN, int inputSize,
         typename OUTA,int outputSizeA,
         typename OUTB,int outputSizeB>
class ProcessingNode12:public GenericNode12<IN, inputSize,
                     OUTA,outputSizeA,
                     OUTB,outputSizeB>
{
public:
    ProcessingNode12(FIFOBase<IN> &src,
                   FIFOBase<OUTA> &dst1,
                   FIFOBase<OUTB> &dst2):
    GenericNode12<IN,inputSize,
                  OUTA,outputSizeA,
                  OUTB,outputSizeB>(src,dst1,dst2){};

    int prepareForRunning() final
    {
        if (this->willOverflow1() ||
            this->willOverflow2() ||
            this->willUnderflow())
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };
    
    int run() final{
        IN *a=this->getReadBuffer();
        OUTA *ba=this->getWriteBuffer1();
        OUTB *bb=this->getWriteBuffer2();

        (void)a;
        (void)ba;
        (void)bb;

        return(0);
    };

};

template<typename IN, int inputSize,
         typename OUTA,int outputSizeA,
         typename OUTB,int outputSizeB,
         typename OUTC,int outputSizeC>
class ProcessingNode13:public GenericNode13<IN,inputSize,
                     OUTA,outputSizeA,
                     OUTB,outputSizeB,
                     OUTC,outputSizeC>
{
public:
    ProcessingNode13(FIFOBase<IN> &src,
                   FIFOBase<OUTA> &dst1,
                   FIFOBase<OUTB> &dst2,
                   FIFOBase<OUTC> &dst3):
    GenericNode13<IN,inputSize,
                  OUTA,outputSizeA,
                  OUTB,outputSizeB,
                  OUTC,outputSizeC>(src,dst1,dst2,dst3){};

    int prepareForRunning() final
    {
        if (this->willOverflow1() ||
            this->willOverflow2() ||
            this->willOverflow3() ||
            this->willUnderflow())
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };
    
    int run() final{
        IN *a=this->getReadBuffer();
        OUTA *ba=this->getWriteBuffer1();
        OUTB *bb=this->getWriteBuffer2();
        OUTC *bc=this->getWriteBuffer3();

        (void)a;
        (void)ba;
        (void)bb;
        (void)bc;

        return(0);
    };

};

template<typename INA, int inputSizeA,
         typename INB, int inputSizeB,
         typename OUT, int outputSize
         >
class ProcessingNode21: 
      public GenericNode21<INA,inputSizeA,
                           INB,inputSizeB,
                           OUT,outputSize>
{
public:
    ProcessingNode21(FIFOBase<INA> &srcA,
                     FIFOBase<INB> &srcB,
                     FIFOBase<OUT> &dst):GenericNode21<INA,inputSizeA,
                                                     INB,inputSizeB,
                                                     OUT,outputSize>(srcA,srcB,dst){};

    int prepareForRunning() final
    {
        if (this->willOverflow() ||
            this->willUnderflow1() ||
            this->willUnderflow2())
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };
    
    int run() final{
        printf("ProcessingNode\n");
        INA *a=this->getReadBuffer1();
        INB *b=this->getReadBuffer2();
        OUT *c=this->getWriteBuffer();
        (void)a;
        (void)b;
        (void)c;
        
        return(0);
    };

};


#endif