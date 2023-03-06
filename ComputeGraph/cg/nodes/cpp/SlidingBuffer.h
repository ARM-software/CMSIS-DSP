/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        SlidingBuffer.h
 * Description:  Sliding buffer
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

#ifndef _SLIDINGBUFFER_H_
#define _SLIDINGBUFFER_H_ 


template<typename IN,int windowSize, int overlap>
class SlidingBuffer: public GenericNode<IN,windowSize-overlap,IN,windowSize>
{
public:
    SlidingBuffer(FIFOBase<IN> &src,FIFOBase<IN> &dst):GenericNode<IN,windowSize-overlap,IN,windowSize>(src,dst)
    {
        static_assert((windowSize-overlap)>0, "Overlap is too big");
        memory.resize(overlap);
    };

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
        IN *a=this->getReadBuffer();
        IN *b=this->getWriteBuffer();
        memcpy((void*)b,(void*)memory.data(),overlap*sizeof(IN));
        memcpy((void*)(b+overlap),(void*)a,(windowSize-overlap)*sizeof(IN));
        memcpy((void*)memory.data(),(void*)(b+windowSize-overlap),overlap*sizeof(IN)) ;
        return(0);
    };
protected:
    std::vector<IN> memory;

};

#endif