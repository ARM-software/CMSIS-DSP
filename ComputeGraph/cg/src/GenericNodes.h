/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        GenericNodes.h
 * Description:  C++ support templates for the compute graph with static scheduler
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

#ifndef _SCHEDGEN_H_
#define _SCHEDGEN_H_

#include <vector>

/* 
Defined in cg_status.h by default but user
may want to use a different header to define the 
error codes of the application
*/
#define CG_SKIP_EXECUTION_ID_CODE (-5)
#define CG_BUFFER_ERROR_ID_CODE (-6)

// FIFOS 

#ifdef DEBUGSCHED

#include <iostream>

template<typename T>
struct debugtype{
    typedef T type;
};

template<>
struct debugtype<char>{
    typedef int type;
};

template<typename T>
using Debug = struct debugtype<T>;

#endif

template<typename T>
class FIFOBase{
public:
    virtual T* getWriteBuffer(int nb)=0;
    virtual T* getReadBuffer(int nb)=0;
    virtual bool willUnderflowWith(int nb) const = 0;
    virtual bool willOverflowWith(int nb) const = 0;
    virtual int nbSamplesInFIFO() const = 0;

};

template<typename T, int length, int isArray=0, int isAsync = 0>
class FIFO;

/* Real FIFO, Synchronous */
template<typename T, int length>
class FIFO<T,length,0,0>: public FIFOBase<T> 
{
    public:
        FIFO(T *buffer,int delay=0):mBuffer(buffer),readPos(0),writePos(delay) {};
        FIFO(uint8_t *buffer,int delay=0):mBuffer((T*)buffer),readPos(0),writePos(delay) {};

        /* Not used in synchronous mode */
        bool willUnderflowWith(int nb) const final {(void)nb;return false;};
        bool willOverflowWith(int nb) const final {(void)nb;return false;};
        int nbSamplesInFIFO() const final {return 0;};

        T * getWriteBuffer(int nb) final
        {
            
            T *ret;
            if (readPos > 0)
            {
                /* This is re-aligning the read buffer.
                   Aligning buffer is better for vectorized code.
                   But it has an impact since more memcpy are
                   executed than required.
                   This is likely to be not so useful in practice
                   so a future version will optimize the memcpy usage
                   */
                memcpy((void*)mBuffer,(void*)(mBuffer+readPos),(writePos-readPos)*sizeof(T));
                writePos -= readPos;
                readPos = 0;
            }
            
            ret = mBuffer + writePos;
            writePos += nb; 
            return(ret);
        };

        T* getReadBuffer(int nb) final
        {
            
            T *ret = mBuffer + readPos;
            readPos += nb;
            return(ret);
        }

        #ifdef DEBUGSCHED
        void dump()
        {
            int nb=0;
            std::cout << std::endl;
            for(int i=0; i < length ; i++)
            {
                std::cout << (typename Debug<T>::type)mBuffer[i] << " ";
                nb++;
                if (nb == 10)
                {
                    nb=0;
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
            std::cout << std::endl;
        }
        #endif

    protected:
        T *mBuffer;
        int readPos,writePos;
};

/* Buffer, Synchronous */
template<typename T, int length>
class FIFO<T,length,1,0>: public FIFOBase<T> 
{
    public:
        FIFO(T *buffer,int delay=0):mBuffer(buffer),readPos(0),writePos(delay) {};
        FIFO(uint8_t *buffer,int delay=0):mBuffer((T*)buffer),readPos(0),writePos(delay) {};

        /* Not used in synchronous mode */
        bool willUnderflowWith(int nb) const final {(void)nb;return false;};
        bool willOverflowWith(int nb) const final {(void)nb;return false;};
        int nbSamplesInFIFO() const final {return 0;};

        T * getWriteBuffer(int nb) final
        {
            (void)nb;
            return(mBuffer);
        };

        T* getReadBuffer(int nb) final
        {
            (void)nb;
            return(mBuffer);
        }

        #ifdef DEBUGSCHED
        void dump()
        {
            int nb=0;
            std::cout << std::endl;
            for(int i=0; i < length ; i++)
            {
                std::cout << (typename Debug<T>::type)mBuffer[i] << " ";
                nb++;
                if (nb == 10)
                {
                    nb=0;
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
            std::cout << std::endl;
        }
        #endif

    protected:
        T *mBuffer;
        int readPos,writePos;
};

/* Real FIFO, Asynchronous */
template<typename T, int length>
class FIFO<T,length,0,1>: public FIFOBase<T> 
{
    public:
        FIFO(T *buffer,int delay=0):mBuffer(buffer),readPos(0),writePos(delay),nbSamples(delay) {};
        FIFO(uint8_t *buffer,int delay=0):mBuffer((T*)buffer),readPos(0),writePos(delay),nbSamples(delay) {};

        /* 

        Check for overflow must have been done
        before using this function 
        
        */
        T * getWriteBuffer(int nb) final
        {
            
            T *ret;
            if (readPos > 0)
            {
                memcpy((void*)mBuffer,(void*)(mBuffer+readPos),(writePos-readPos)*sizeof(T));
                writePos -= readPos;
                readPos = 0;
            }
            
            ret = mBuffer + writePos;
            writePos += nb; 
            nbSamples += nb;
            return(ret);
        };

        /* 
        
        Check for undeflow must have been done
        before using this function 
        
        */
        T* getReadBuffer(int nb) final
        {
           
            T *ret = mBuffer + readPos;
            readPos += nb;
            nbSamples -= nb;
            return(ret);
        }

        bool willUnderflowWith(int nb) const final
        {
            return((nbSamples - nb)<0);
        }

        bool willOverflowWith(int nb) const final
        {
            return((nbSamples + nb)>length);
        }

        int nbSamplesInFIFO() const final {return nbSamples;};

        #ifdef DEBUGSCHED
        void dump()
        {
            int nb=0;
            std::cout << std::endl;
            std::cout << "FIFO nb samples = " << nbSamples << std::endl;
            for(int i=0; i < length ; i++)
            {
                std::cout << (typename Debug<T>::type)mBuffer[i] << " ";
                nb++;
                if (nb == 10)
                {
                    nb=0;
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
            std::cout << std::endl;
        }
        #endif

    protected:
        T *mBuffer;
        int readPos,writePos;
        int nbSamples;
};

// GENERIC NODES 

class NodeBase
{
public:
    virtual int run()=0;
    virtual int prepareForRunning()=0;
};

template<typename IN, int inputSize,typename OUT, int outputSize>
class GenericNode:public NodeBase
{
public:
     GenericNode(FIFOBase<IN> &src,FIFOBase<OUT> &dst):mSrc(src),mDst(dst){};

protected:
     OUT * getWriteBuffer(int nb = outputSize){return mDst.getWriteBuffer(nb);};
     IN * getReadBuffer(int nb = inputSize){return mSrc.getReadBuffer(nb);};

     bool willOverflow(int nb = outputSize){return mDst.willOverflowWith(nb);};
     bool willUnderflow(int nb = inputSize){return mSrc.willUnderflowWith(nb);};

private:
    FIFOBase<IN> &mSrc;
    FIFOBase<OUT> &mDst;
};

template<typename IN, int inputSize,typename OUT1, int output1Size,typename OUT2, int output2Size>
class GenericNode12:public NodeBase
{
public:
     GenericNode12(FIFOBase<IN> &src,FIFOBase<OUT1> &dst1,FIFOBase<OUT2> &dst2):mSrc(src),
     mDst1(dst1),mDst2(dst2){};

protected:
     OUT1 * getWriteBuffer1(int nb=output1Size){return mDst1.getWriteBuffer(nb);};
     OUT2 * getWriteBuffer2(int nb=output2Size){return mDst2.getWriteBuffer(nb);};
     IN * getReadBuffer(int nb=inputSize){return mSrc.getReadBuffer(nb);};

     bool willOverflow1(int nb = output1Size){return mDst1.willOverflowWith(nb);};
     bool willOverflow2(int nb = output2Size){return mDst2.willOverflowWith(nb);};

     bool willUnderflow(int nb = inputSize){return mSrc.willUnderflowWith(nb);};

private:
    FIFOBase<IN> &mSrc;
    FIFOBase<OUT1> &mDst1;
    FIFOBase<OUT2> &mDst2;
};

template<typename IN,   int inputSize,
         typename OUT1, int output1Size,
         typename OUT2, int output2Size,
         typename OUT3, int output3Size>
class GenericNode13:public NodeBase
{
public:
     GenericNode13(FIFOBase<IN> &src,
                   FIFOBase<OUT1> &dst1,
                   FIFOBase<OUT2> &dst2,
                   FIFOBase<OUT3> &dst3
                   ):mSrc(src),
     mDst1(dst1),mDst2(dst2),mDst3(dst3){};

protected:
     OUT1 * getWriteBuffer1(int nb=output1Size){return mDst1.getWriteBuffer(nb);};
     OUT2 * getWriteBuffer2(int nb=output2Size){return mDst2.getWriteBuffer(nb);};
     OUT3 * getWriteBuffer3(int nb=output3Size){return mDst3.getWriteBuffer(nb);};

     IN * getReadBuffer(int nb=inputSize){return mSrc.getReadBuffer(nb);};

     bool willOverflow1(int nb = output1Size){return mDst1.willOverflowWith(nb);};
     bool willOverflow2(int nb = output2Size){return mDst2.willOverflowWith(nb);};
     bool willOverflow3(int nb = output3Size){return mDst3.willOverflowWith(nb);};

     bool willUnderflow(int nb = inputSize){return mSrc.willUnderflowWith(nb);};

private:
    FIFOBase<IN> &mSrc;
    FIFOBase<OUT1> &mDst1;
    FIFOBase<OUT2> &mDst2;
    FIFOBase<OUT3> &mDst3;

};

template<typename IN1, int input1Size,typename IN2, int input2Size,typename OUT, int outputSize>
class GenericNode21:public NodeBase
{
public:
     GenericNode21(FIFOBase<IN1> &src1,FIFOBase<IN2> &src2,FIFOBase<OUT> &dst):mSrc1(src1),
     mSrc2(src2),
     mDst(dst){};

protected:
     OUT * getWriteBuffer(int nb=outputSize){return mDst.getWriteBuffer(nb);};
     IN1 * getReadBuffer1(int nb=input1Size){return mSrc1.getReadBuffer(nb);};
     IN2 * getReadBuffer2(int nb=input2Size){return mSrc2.getReadBuffer(nb);};

     bool willOverflow(int nb = outputSize){return mDst.willOverflowWith(nb);};
     bool willUnderflow1(int nb = input1Size){return mSrc1.willUnderflowWith(nb);};
     bool willUnderflow2(int nb = input2Size){return mSrc2.willUnderflowWith(nb);};

private:
    FIFOBase<IN1> &mSrc1;
    FIFOBase<IN2> &mSrc2;
    FIFOBase<OUT> &mDst;
};



template<typename OUT, int outputSize>
class GenericSource:public NodeBase
{
public:
     GenericSource(FIFOBase<OUT> &dst):mDst(dst){};

protected:
     OUT * getWriteBuffer(int nb=outputSize){return mDst.getWriteBuffer(nb);};

     bool willOverflow(int nb = outputSize){return mDst.willOverflowWith(nb);};

private:
    FIFOBase<OUT> &mDst;
};

template<typename IN,int inputSize>
class GenericSink:public NodeBase
{
public:
     GenericSink(FIFOBase<IN> &src):mSrc(src){};

protected:
     IN * getReadBuffer(int nb=inputSize){return mSrc.getReadBuffer(nb);};

     bool willUnderflow(int nb = inputSize){return mSrc.willUnderflowWith(nb);};

private:
    FIFOBase<IN> &mSrc;
};


#define REPEAT(N) for(int i=0;i<N;i++)


template<typename IN, int inputSize,typename OUT1,int output1Size,typename OUT2,int output2Size>
class Duplicate2;

template<typename IN, int inputSize>
class Duplicate2<IN,inputSize,IN,inputSize,IN,inputSize>: public GenericNode12<IN,inputSize,IN,inputSize,IN,inputSize>
{
public:
    Duplicate2(FIFOBase<IN> &src,FIFOBase<IN> &dst1,FIFOBase<IN> &dst2):
    GenericNode12<IN,inputSize,IN,inputSize,IN,inputSize>(src,dst1,dst2){};

    int prepareForRunning() final
    {
        if (this->willUnderflow() || 
            this->willOverflow1() ||
            this->willOverflow2())
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };

    int run() final {
        IN *a=this->getReadBuffer();
        IN *b1=this->getWriteBuffer1();
        IN *b2=this->getWriteBuffer2();
        for(int i = 0; i<inputSize; i++)
        {
           b1[i] = a[i];
           b2[i] = a[i];
        }
        return(0);
    };

};

template<typename IN, int inputSize,
         typename OUT1,int output1Size,
         typename OUT2,int output2Size,
         typename OUT3,int output3Size>
class Duplicate3;

template<typename IN, int inputSize>
class Duplicate3<IN,inputSize,
                 IN,inputSize,
                 IN,inputSize,
                 IN,inputSize>: 
                 public GenericNode13<IN,inputSize,
                                      IN,inputSize,
                                      IN,inputSize,
                                      IN,inputSize>
{
public:
    Duplicate3(FIFOBase<IN> &src,
               FIFOBase<IN> &dst1,
               FIFOBase<IN> &dst2,
               FIFOBase<IN> &dst3):
    GenericNode13<IN,inputSize,
                  IN,inputSize,
                  IN,inputSize,
                  IN,inputSize>(src,dst1,dst2,dst3){};

    int prepareForRunning() final
    {
        if (this->willUnderflow() || 
            this->willOverflow1() ||
            this->willOverflow2() ||
            this->willOverflow3()
            )
        {
           return(CG_SKIP_EXECUTION_ID_CODE); // Skip execution
        }

        return(0);
    };

    int run() final {
        IN *a=this->getReadBuffer();
        IN *b1=this->getWriteBuffer1();
        IN *b2=this->getWriteBuffer2();
        IN *b3=this->getWriteBuffer3();
        for(int i = 0; i<inputSize; i++)
        {
           b1[i] = a[i];
           b2[i] = a[i];
           b3[i] = a[i];
        }
        return(0);
    };

};


       

#endif
