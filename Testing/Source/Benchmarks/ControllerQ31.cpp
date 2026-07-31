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
       q31_t * pOuta;
       q31_t * pOutb;

       Ia = this->pSrc;
       Ib = this->pSrc + this->nbSamples;
       pOuta = this->pOuta;
       pOutb = this->pOutb;
       for(int i=0; i < this->nbSamples; i++)
       {
         arm_clarke_q31(*Ia++,*(--Ib),pOuta++,pOutb++);
       }
    }

    void ControllerQ31::test_inv_clarke_q31()
    {
       q31_t * Ia;
       q31_t * Ib;
       q31_t * pOuta;
       q31_t * pOutb;

       Ia = this->pSrc;
       Ib = this->pSrc + this->nbSamples;
       pOuta = this->pOuta;
       pOutb = this->pOutb;
       for(int i=0; i < this->nbSamples; i++)
       {
         arm_inv_clarke_q31(*Ia++,*(--Ib),pOuta++,pOutb++);
       }
    }

    void ControllerQ31::test_park_q31()
    {
       q31_t * Ia;
       q31_t * Ib;
       q31_t * pOuta;
       q31_t * pOutb;

       Ia = this->pSrc;
       Ib = this->pSrc + this->nbSamples;
       pOuta = this->pOuta;
       pOutb = this->pOutb;
       for(int i=0; i < this->nbSamples; i++)
       {
         arm_park_q31(*Ia++,*(--Ib),pOuta++,pOutb++,0x3a1c5c56,0x720c8074); //theta = 27(Deg)
       }
    }

    void ControllerQ31::test_inv_park_q31()
    {
        q31_t * Ia;
        q31_t * Ib;
        q31_t * pOuta;
        q31_t * pOutb;

        Ia = this->pSrc;
        Ib = this->pSrc + this->nbSamples;
        pOuta = this->pOuta;
        pOutb = this->pOutb;
        for(int i=0; i < this->nbSamples; i++)
        {
          arm_inv_park_q31(*Ia++,*(--Ib),pOuta++,pOutb++,0x3a1c5c56,0x720c8074);
        }
    }

    void ControllerQ31::test_sin_cos_q31()
    {
        q31_t * pOuta;
        q31_t * pOutb;

        pOuta = this->pOuta;
        pOutb = this->pOutb;
        for(int i=0; i < this->nbSamples; i++)
        {
          arm_sin_cos_q31(0xccccccd,pOuta++,pOutb++);
        }
    }

    void ControllerQ31::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {
       std::vector<Testing::param_t>::iterator it = params.begin();
       this->nbSamples = *it;

       samples.reload(ControllerQ31::SAMPLES_Q31_ID,mgr,this->nbSamples);

       switch(id)
       {
           case TEST_PID_Q31_1:
              output.create(this->nbSamples,ControllerQ31::OUT_SAMPLES_Q31_ID,mgr);
              arm_pid_init_q31(&instPid,1);
              this->pDst=output.ptr();
           break;

           default:
              /* Allocate enough storage for pOuta and pOutb outputs of the controller functions. */
              output.create(this->nbSamples * 2,ControllerQ31::OUT_SAMPLES_Q31_ID,mgr);
              this->pOuta=output.ptr();
              this->pOutb=output.ptr() + this->nbSamples;
           break;
       }

       this->pSrc=samples.ptr();
    }

    void ControllerQ31::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
    }
