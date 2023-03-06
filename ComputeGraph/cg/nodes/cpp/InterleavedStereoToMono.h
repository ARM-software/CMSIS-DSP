/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        InterleavedStereoToMono.h
 * Description:  Interleaved Stereo to mono stream in Q15
 *
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

#ifndef _STEREOTOMONO_H_
#define _STEREOTOMONO_H_ 


template<typename IN, int inputSize,typename OUT,int outputSize>
class InterleavedStereoToMono;

template<int inputSize,int outputSize>
class InterleavedStereoToMono<q15_t,inputSize,q15_t,outputSize>: public GenericNode<q15_t,inputSize,q15_t,outputSize>
{
public:
    InterleavedStereoToMono(FIFOBase<q15_t> &src,FIFOBase<q15_t> &dst):
    GenericNode<q15_t,inputSize,q15_t,outputSize>(src,dst){};

    int prepareForRunning() final
    {
        if (this->willOverflow() ||
            this->willUnderflow()
           )
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };

    int run() final
    {
        q15_t *a=this->getReadBuffer();
        q15_t *b=this->getWriteBuffer();
        for(int i = 0; i<outputSize; i++)
        {
           b[i] = (a[2*i]>>1) + (a[2*i+1]>>1);
        }
        return(0);
    };

};

template<int inputSize,int outputSize>
class InterleavedStereoToMono<q31_t,inputSize,q31_t,outputSize>: public GenericNode<q31_t,inputSize,q31_t,outputSize>
{
public:
    InterleavedStereoToMono(FIFOBase<q31_t> &src,FIFOBase<q31_t> &dst):
    GenericNode<q31_t,inputSize,q31_t,outputSize>(src,dst){};

    int prepareForRunning() final
    {
        if (this->willOverflow() ||
            this->willUnderflow()
           )
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };

    int run() final
    {
        q31_t *a=this->getReadBuffer();
        q31_t *b=this->getWriteBuffer();
        for(int i = 0; i<outputSize; i++)
        {
           b[i] = (a[2*i]>>1) + (a[2*i+1]>>1);
        }
        return(0);
    };

};

template<int inputSize,int outputSize>
class InterleavedStereoToMono<float32_t,inputSize,float32_t,outputSize>: public GenericNode<float32_t,inputSize,float32_t,outputSize>
{
public:
    InterleavedStereoToMono(FIFOBase<float32_t> &src,FIFOBase<float32_t> &dst):
    GenericNode<float32_t,inputSize,float32_t,outputSize>(src,dst){};

    int prepareForRunning() final
    {
        if (this->willOverflow() ||
            this->willUnderflow()
           )
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };
    
    int run() final
    {
        float32_t *a=this->getReadBuffer();
        float32_t *b=this->getWriteBuffer();
        for(int i = 0; i<outputSize; i++)
        {
           b[i] = 0.5f * (a[2*i] + a[2*i+1]);
        }
        return(0);
    };

};

#endif