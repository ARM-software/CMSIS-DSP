#include "ControllerF32.h"
#include "Error.h"


    void ControllerF32::test_pid_f32()
    {
       for(int i=0; i < this->nbSamples; i++)
       {
          *this->pDst++ = arm_pid_f32(&instPid, *this->pSrc++);
       }
    }

    void ControllerF32::test_clarke_f32()
    {
       float32_t * Ia;
       float32_t * Ib;
       float32_t * pOuta;
       float32_t * pOutb;

       Ia = this->pSrc;
       Ib = this->pSrc + this->nbSamples;
       pOuta = this->pOuta;
       pOutb = this->pOutb;
       for(int i=0; i < this->nbSamples; i++)
       {
         arm_clarke_f32(*Ia++,*(--Ib),pOuta++,pOutb++);
       }
    }

    void ControllerF32::test_inv_clarke_f32()
    {
       float32_t * Ia;
       float32_t * Ib;
       float32_t * pOuta;
       float32_t * pOutb;

       Ia = this->pSrc;
       Ib = this->pSrc + this->nbSamples;
       pOuta = this->pOuta;
       pOutb = this->pOutb;
       for(int i=0; i < this->nbSamples; i++)
       {
         arm_inv_clarke_f32(*Ia++,*(--Ib),pOuta++,pOutb++);
       }
    }

    void ControllerF32::test_park_f32()
    {
       float32_t * Ia;
       float32_t * Ib;
       float32_t * pOuta;
       float32_t * pOutb;

       Ia = this->pSrc;
       Ib = this->pSrc + this->nbSamples;
       pOuta = this->pOuta;
       pOutb = this->pOutb;
       for(int i=0; i < this->nbSamples; i++)
       {
         arm_park_f32(*Ia++,*(--Ib),pOuta++,pOutb++,0.4539905,0.8910065); //theta = 27(Deg)
       }
    }

    void ControllerF32::test_inv_park_f32()
    {
       float32_t * Ia;
       float32_t * Ib;
       float32_t * pOuta;
       float32_t * pOutb;

       Ia = this->pSrc;
       Ib = this->pSrc + this->nbSamples;
       pOuta = this->pOuta;
       pOutb = this->pOutb;
       for(int i=0; i < this->nbSamples; i++)
       { 
         arm_inv_park_f32(*Ia++,*(--Ib),pOuta++,pOutb++,0.4539905,0.8910065); //theta = 27(Deg)
       }
    }

    void ControllerF32::test_sin_cos_f32()
    {
        float32_t * pOuta;
        float32_t * pOutb;

        pOuta = this->pOuta;
        pOutb = this->pOutb;
        for(int i=0; i < this->nbSamples; i++)
        {
           arm_sin_cos_f32(0.1,pOuta++,pOutb++);
        }
    }

    void ControllerF32::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {
       std::vector<Testing::param_t>::iterator it = params.begin();
       this->nbSamples = *it;

       samples.reload(ControllerF32::SAMPLES_F32_ID,mgr,this->nbSamples);

       switch(id)
       {
           case TEST_PID_F32_1:
              output.create(this->nbSamples,ControllerF32::OUT_SAMPLES_F32_ID,mgr);
              arm_pid_init_f32(&instPid,1);
              this->pDst=output.ptr();
           break;

           default:
              /* Allocate enough storage for pOuta and pOutb outputs of the controller functions. */
              output.create(this->nbSamples * 2,ControllerF32::OUT_SAMPLES_F32_ID,mgr);
              this->pOuta=output.ptr();
              this->pOutb=output.ptr() + this->nbSamples;
           break;
       }

       this->pSrc=samples.ptr();
    }

    void ControllerF32::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
    }
