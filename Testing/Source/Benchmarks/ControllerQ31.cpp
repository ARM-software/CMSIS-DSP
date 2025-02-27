#include "ControllerQ31.h"
#include "Error.h"

   
    void ControllerQ31::test_pid_q31()
    {
       for(int i=0; i < this->nbSamples; i++)
       {
          *this->pDst++ = arm_pid_q31(&instPid, *this->pSrc++);
       }
    } 

    void ControllerQ31::test_clarke_q31() 
    {
       q31_t * Ia;
       q31_t * Ib;

       Ia = this->pSrc;
       Ib = reinterpret_cast<q31_t *> (reinterpret_cast<uintptr_t>(this->pSrc) + (this->nbSamples << 2));
       for(int i=0; i < this->nbSamples; i++)
       {
         arm_clarke_q31(*Ia++,*(--Ib),this->pOuta,this->pOutb);
       }
    }

    void ControllerQ31::test_inv_clarke_q31() 
    {
       q31_t * Ia;
       q31_t * Ib;

       Ia = this->pSrc;
       Ib = reinterpret_cast<q31_t *> (reinterpret_cast<uintptr_t>(this->pSrc) + (this->nbSamples << 2));
       for(int i=0; i < this->nbSamples; i++)
       {
         arm_inv_clarke_q31(*Ia++,*(--Ib),this->pOuta,this->pOutb);
       }
    }

    void ControllerQ31::test_park_q31() 
    {
       q31_t * Ia;
       q31_t * Ib;

       Ia = this->pSrc;
       Ib = reinterpret_cast<q31_t *> (reinterpret_cast<uintptr_t>(this->pSrc) + (this->nbSamples << 2));
       for(int i=0; i < this->nbSamples; i++)
       {
         arm_park_q31(*Ia++,*(--Ib),this->pOuta,this->pOutb,0x3a1c5c56,0x720c8074); //theta = 27(Deg)
       }
    }

    void ControllerQ31::test_inv_park_q31() 
    {
        q31_t * Ia;
        q31_t * Ib;

        Ia = this->pSrc;
        Ib = reinterpret_cast<q31_t *> (reinterpret_cast<uintptr_t>(this->pSrc) + (this->nbSamples << 2));
        for(int i=0; i < this->nbSamples; i++)
        {
          arm_inv_park_q31(*Ia++,*(--Ib),this->pOuta,this->pOutb,0x3a1c5c56,0x720c8074);
        }
    }

    void ControllerQ31::test_sin_cos_q31() 
    {
        q31_t sinVal,cosVal;
        
        for(int i=0; i < this->nbSamples; i++)
        {
           arm_sin_cos_q31(0xccccccd,&sinVal,&cosVal);
        }
    }
    
    void ControllerQ31::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {


       std::vector<Testing::param_t>::iterator it = params.begin();
       this->nbSamples = *it;

       samples.reload(ControllerQ31::SAMPLES_Q31_ID,mgr,this->nbSamples);
       output.create(this->nbSamples,ControllerQ31::OUT_SAMPLES_Q31_ID,mgr);

       switch(id)
       {
           case TEST_PID_Q31_1:
              arm_pid_init_q31(&instPid,1);
           break;

       }

       this->pSrc=samples.ptr();
       this->pDst=output.ptr();
       
    }

    void ControllerQ31::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
    }
