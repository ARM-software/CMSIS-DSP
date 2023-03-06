/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        OverlapAndAdd.h
 * Description:  Overlap And Add
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

#ifndef _OVERLAPANDADD_H_
#define _OVERLAPANDADD_H_ 


template<typename IN,int windowSize, int overlap>
class OverlapAdd: public GenericNode<IN,windowSize,IN,windowSize-overlap>
{
public:
    OverlapAdd(FIFOBase<IN> &src,FIFOBase<IN> &dst):GenericNode<IN,windowSize,IN,windowSize-overlap>(src,dst)
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
        int i;
        IN *a=this->getReadBuffer();
        IN *b=this->getWriteBuffer();

        for(i=0;i<overlap;i++)
        {
            memory[i] = a[i] + memory[i];
        }

        if (2*overlap - windowSize > 0)
        {
            
            memcpy((void*)b,(void*)memory.data(),(windowSize-overlap)*sizeof(IN));

            memmove(memory.data(),memory.data()+windowSize-overlap,(2*overlap - windowSize)*sizeof(IN));
            memcpy(memory.data()+2*overlap - windowSize,a+overlap,(windowSize-overlap)*sizeof(IN));
        }
        else if (2*overlap - windowSize < 0)
        {
            memcpy((void*)b,(void*)memory.data(),overlap*sizeof(IN));
            memcpy((void*)(b+overlap),(void*)(a+overlap),(windowSize - 2*overlap)*sizeof(IN));

            memcpy((void*)memory.data(),(void*)(a+windowSize-overlap),overlap*sizeof(IN));
        }
        else
        {
            memcpy((void*)b,(void*)memory.data(),overlap*sizeof(IN));

            memcpy((void*)memory.data(),(void*)(a+overlap),overlap*sizeof(IN));
        }
        
        return(0);
    };
protected:
    std::vector<IN> memory;

};

#endif