#include "arm_neon_private.h"

#if !defined(DEBUGV)
#define DEBUGV(A)
#endif 

#if !defined(DEBUGVACC)
#define DEBUGVACC(A)
#endif 

#if !defined(DEBUGS)
#define DEBUGS(A)
#endif 

#if !defined(DEBUGSACC)
#define DEBUGSACC(A)
#endif

#if !defined(LOGKERNEL)
#define LOGKERNEL(A,B)
#endif

#if !defined(DEF_AND_CLEAR_SCALARACC)
#define DEF_AND_CLEAR_SCALARACC(tmp) const SCALARACC tmp = 0
#endif

#define DC (LANE*DCS) // Must be multiple of lane

// 4 row and 4 column VECtors
__STATIC_INLINE void EXT(kernel_4rx4cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_4rx4cv\n",0);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;

  VEC tmp00,tmp01,tmp02,tmp03;

  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += LANE;
  VECACC t01;
  VLOAD_ACC(t01,p);
  p += LANE;
  VECACC t02;
  VLOAD_ACC(t02,p);
  p += LANE;
  VECACC t03;
  VLOAD_ACC(t03,p);
  p += max_cols - 3*LANE;

  VECACC t10;
  VLOAD_ACC(t10,p);
  p += LANE;
  VECACC t11;
  VLOAD_ACC(t11,p);
  p += LANE;
  VECACC t12;
  VLOAD_ACC(t12,p);
  p += LANE;
  VECACC t13;
  VLOAD_ACC(t13,p);
  p += max_cols - 3*LANE;

  VECACC t20;
  VLOAD_ACC(t20,p);
  p += LANE;
  VECACC t21;
  VLOAD_ACC(t21,p);
  p += LANE;
  VECACC t22;
  VLOAD_ACC(t22,p);
  p += LANE;
  VECACC t23;
  VLOAD_ACC(t23,p);
  p += max_cols - 3*LANE;

  VECACC t30;
  VLOAD_ACC(t30,p);
  p += LANE;
  VECACC t31;
  VLOAD_ACC(t31,p);
  p += LANE;
  VECACC t32;
  VLOAD_ACC(t32,p);
  p += LANE;
  VECACC t33;
  VLOAD_ACC(t33,p);
  p += max_cols - 3*LANE;

  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;
  const DTYPE *pAlpha3 = pAlpha2 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;
    const DTYPE alpha3 = *pAlpha3++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VLOAD(tmp01,pB0);
    pB0 += LANE;

    VLOAD(tmp02,pB0);
    pB0 += LANE;

    VLOAD(tmp03,pB0);
    pB0 += LANE;
    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);
    VMAC_N(t20,tmp00,alpha2);
    VMAC_N(t30,tmp00,alpha3);


    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t11,tmp01,alpha1);
    VMAC_N(t21,tmp01,alpha2);
    VMAC_N(t31,tmp01,alpha3);


    VMAC_N(t02,tmp02,alpha0);
    VMAC_N(t12,tmp02,alpha1);
    VMAC_N(t22,tmp02,alpha2);
    VMAC_N(t32,tmp02,alpha3);


    VMAC_N(t03,tmp03,alpha0);
    VMAC_N(t13,tmp03,alpha1);
    VMAC_N(t23,tmp03,alpha2);
    VMAC_N(t33,tmp03,alpha3);


  }

  
  VSTORE_ACC(pC,t00);
  pC += LANE;
  VSTORE_ACC(pC,t01);
  pC += LANE;
  VSTORE_ACC(pC,t02);
  pC += LANE;
  VSTORE_ACC(pC,t03);
  pC += max_cols - 3*LANE;

  VSTORE_ACC(pC,t10);
  pC += LANE;
  VSTORE_ACC(pC,t11);
  pC += LANE;
  VSTORE_ACC(pC,t12);
  pC += LANE;
  VSTORE_ACC(pC,t13);
  pC += max_cols - 3*LANE;

  VSTORE_ACC(pC,t20);
  pC += LANE;
  VSTORE_ACC(pC,t21);
  pC += LANE;
  VSTORE_ACC(pC,t22);
  pC += LANE;
  VSTORE_ACC(pC,t23);
  pC += max_cols - 3*LANE;

  VSTORE_ACC(pC,t30);
  pC += LANE;
  VSTORE_ACC(pC,t31);
  pC += LANE;
  VSTORE_ACC(pC,t32);
  pC += LANE;
  VSTORE_ACC(pC,t33);
  pC += max_cols - 3*LANE;
}

__STATIC_INLINE void EXT(kernel_4rx3cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_4rx3cv\n",1);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;

  VEC tmp00,tmp01,tmp02;
  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += LANE;
  VECACC t01;
  VLOAD_ACC(t01,p);
  p += LANE;
  VECACC t02;
  VLOAD_ACC(t02,p);
  p += max_cols - 2*LANE;

  VECACC t10;
  VLOAD_ACC(t10,p);
  p += LANE;
  VECACC t11;
  VLOAD_ACC(t11,p);
  p += LANE;
  VECACC t12;
  VLOAD_ACC(t12,p);
  p += max_cols - 2*LANE;

  VECACC t20;
  VLOAD_ACC(t20,p);
  p += LANE;
  VECACC t21;
  VLOAD_ACC(t21,p);
  p += LANE;
  VECACC t22;
  VLOAD_ACC(t22,p);
  p += max_cols - 2*LANE;

  VECACC t30;
  VLOAD_ACC(t30,p);
  p += LANE;
  VECACC t31;
  VLOAD_ACC(t31,p);
  p += LANE;
  VECACC t32;
  VLOAD_ACC(t32,p);
  p += max_cols - 2*LANE;

  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;
  const DTYPE *pAlpha3 = pAlpha2 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;
    const DTYPE alpha3 = *pAlpha3++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VLOAD(tmp01,pB0);
    pB0 += LANE;

    VLOAD(tmp02,pB0);
    pB0 += LANE;
    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);
    VMAC_N(t20,tmp00,alpha2);
    VMAC_N(t30,tmp00,alpha3);


    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t11,tmp01,alpha1);
    VMAC_N(t21,tmp01,alpha2);
    VMAC_N(t31,tmp01,alpha3);


    VMAC_N(t02,tmp02,alpha0);
    VMAC_N(t12,tmp02,alpha1);
    VMAC_N(t22,tmp02,alpha2);
    VMAC_N(t32,tmp02,alpha3);


  }

  VSTORE_ACC(pC,t00);
  pC += LANE;
  VSTORE_ACC(pC,t01);
  pC += LANE;
  VSTORE_ACC(pC,t02);
  pC += max_cols - 2*LANE;

  VSTORE_ACC(pC,t10);
  pC += LANE;
  VSTORE_ACC(pC,t11);
  pC += LANE;
  VSTORE_ACC(pC,t12);
  pC += max_cols - 2*LANE;

  VSTORE_ACC(pC,t20);
  pC += LANE;
  VSTORE_ACC(pC,t21);
  pC += LANE;
  VSTORE_ACC(pC,t22);
  pC += max_cols - 2*LANE;

  VSTORE_ACC(pC,t30);
  pC += LANE;
  VSTORE_ACC(pC,t31);
  pC += LANE;
  VSTORE_ACC(pC,t32);
  pC += max_cols - 2*LANE;

}

__STATIC_INLINE void EXT(kernel_4rx2cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_4rx2cv\n",2);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  VEC tmp00,tmp01;
  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += LANE;
  VECACC t01;
  VLOAD_ACC(t01,p);
  p += max_cols - LANE;

  VECACC t10;
  VLOAD_ACC(t10,p);
  p += LANE;
  VECACC t11;
  VLOAD_ACC(t11,p);
  p += max_cols - LANE;

  VECACC t20;
  VLOAD_ACC(t20,p);
  p += LANE;
  VECACC t21;
  VLOAD_ACC(t21,p);
  p += max_cols - LANE;

  VECACC t30;
  VLOAD_ACC(t30,p);
  p += LANE;
  VECACC t31;
  VLOAD_ACC(t31,p);
  p += max_cols - LANE;

  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;
  const DTYPE *pAlpha3 = pAlpha2 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;
    const DTYPE alpha3 = *pAlpha3++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VLOAD(tmp01,pB0);
    pB0 += LANE;

    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);
    VMAC_N(t20,tmp00,alpha2);
    VMAC_N(t30,tmp00,alpha3);


    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t11,tmp01,alpha1);
    VMAC_N(t21,tmp01,alpha2);
    VMAC_N(t31,tmp01,alpha3);



  }

  VSTORE_ACC(pC,t00);
  pC += LANE;
  VSTORE_ACC(pC,t01);
  pC += max_cols - LANE;

  VSTORE_ACC(pC,t10);
  pC += LANE;
  VSTORE_ACC(pC,t11);
  pC += max_cols - LANE;

  VSTORE_ACC(pC,t20);
  pC += LANE;
  VSTORE_ACC(pC,t21);
  pC += max_cols - LANE;

  VSTORE_ACC(pC,t30);
  pC += LANE;
  VSTORE_ACC(pC,t31);
  pC += max_cols - LANE;

}

__STATIC_INLINE void EXT(kernel_4rx1cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_4rx1cv\n",3);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  VEC tmp00;
  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += max_cols;
  VECACC t10;
  VLOAD_ACC(t10,p);
  p += max_cols;
  VECACC t20;
  VLOAD_ACC(t20,p);
  p += max_cols;
  VECACC t30;
  VLOAD_ACC(t30,p);
  p += max_cols;
 

  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;
  const DTYPE *pAlpha3 = pAlpha2 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;
    const DTYPE alpha3 = *pAlpha3++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);
    VMAC_N(t20,tmp00,alpha2);
    VMAC_N(t30,tmp00,alpha3);



  }

  VSTORE_ACC(pC,t00);
  pC += max_cols;
  VSTORE_ACC(pC,t10);
  pC += max_cols;
  VSTORE_ACC(pC,t20);
  pC += max_cols;
  VSTORE_ACC(pC,t30);
 

}

__STATIC_INLINE void EXT(kernel_3rx4cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_3rx4cv\n",4);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  VEC tmp00,tmp01,tmp02,tmp03;
  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += LANE;
  VECACC t01;
  VLOAD_ACC(t01,p);
  p += LANE;
  VECACC t02;
  VLOAD_ACC(t02,p);
  p += LANE;
  VECACC t03;
  VLOAD_ACC(t03,p);
  p += max_cols - 3*LANE;

  VECACC t10;
  VLOAD_ACC(t10,p);
  p += LANE;
  VECACC t11;
  VLOAD_ACC(t11,p);
  p += LANE;
  VECACC t12;
  VLOAD_ACC(t12,p);
  p += LANE;
  VECACC t13;
  VLOAD_ACC(t13,p);
  p += max_cols - 3*LANE;

  VECACC t20;
  VLOAD_ACC(t20,p);
  p += LANE;
  VECACC t21;
  VLOAD_ACC(t21,p);
  p += LANE;
  VECACC t22;
  VLOAD_ACC(t22,p);
  p += LANE;
  VECACC t23;
  VLOAD_ACC(t23,p);
  p += max_cols - 3*LANE;

  

  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VLOAD(tmp01,pB0);
    pB0 += LANE;

    VLOAD(tmp02,pB0);
    pB0 += LANE;

    VLOAD(tmp03,pB0);
    pB0 += LANE;
    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);
    VMAC_N(t20,tmp00,alpha2);


    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t11,tmp01,alpha1);
    VMAC_N(t21,tmp01,alpha2);


    VMAC_N(t02,tmp02,alpha0);
    VMAC_N(t12,tmp02,alpha1);
    VMAC_N(t22,tmp02,alpha2);


    VMAC_N(t03,tmp03,alpha0);
    VMAC_N(t13,tmp03,alpha1);
    VMAC_N(t23,tmp03,alpha2);


  }

  VSTORE_ACC(pC,t00);
  pC += LANE;
  VSTORE_ACC(pC,t01);
  pC += LANE;
  VSTORE_ACC(pC,t02);
  pC += LANE;
  VSTORE_ACC(pC,t03);
  pC += max_cols - 3*LANE;

  VSTORE_ACC(pC,t10);
  pC += LANE;
  VSTORE_ACC(pC,t11);
  pC += LANE;
  VSTORE_ACC(pC,t12);
  pC += LANE;
  VSTORE_ACC(pC,t13);
  pC += max_cols - 3*LANE;

  VSTORE_ACC(pC,t20);
  pC += LANE;
  VSTORE_ACC(pC,t21);
  pC += LANE;
  VSTORE_ACC(pC,t22);
  pC += LANE;
  VSTORE_ACC(pC,t23);
  pC += max_cols - 3*LANE;



}

__STATIC_INLINE void EXT(kernel_3rx3cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_3rx3cv\n",5);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  VEC tmp00,tmp01,tmp02;
  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += LANE;
  VECACC t01;
  VLOAD_ACC(t01,p);
  p += LANE;
  VECACC t02;
  VLOAD_ACC(t02,p);
  p += max_cols - 2*LANE;

  VECACC t10;
  VLOAD_ACC(t10,p);
  p += LANE;
  VECACC t11;
  VLOAD_ACC(t11,p);
  p += LANE;
  VECACC t12;
  VLOAD_ACC(t12,p);
  p += max_cols - 2*LANE;

  VECACC t20;
  VLOAD_ACC(t20,p);
  p += LANE;
  VECACC t21;
  VLOAD_ACC(t21,p);
  p += LANE;
  VECACC t22;
  VLOAD_ACC(t22,p);
  p += max_cols - 2*LANE;

  

  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VLOAD(tmp01,pB0);
    pB0 += LANE;

    VLOAD(tmp02,pB0);
    pB0 += LANE;

  
    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);
    VMAC_N(t20,tmp00,alpha2);


    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t11,tmp01,alpha1);
    VMAC_N(t21,tmp01,alpha2);


    VMAC_N(t02,tmp02,alpha0);
    VMAC_N(t12,tmp02,alpha1);
    VMAC_N(t22,tmp02,alpha2);



  }

  VSTORE_ACC(pC,t00);
  pC += LANE;
  VSTORE_ACC(pC,t01);
  pC += LANE;
  VSTORE_ACC(pC,t02);
  pC += max_cols - 2*LANE;

  VSTORE_ACC(pC,t10);
  pC += LANE;
  VSTORE_ACC(pC,t11);
  pC += LANE;
  VSTORE_ACC(pC,t12);
  pC += max_cols - 2*LANE;

  VSTORE_ACC(pC,t20);
  pC += LANE;
  VSTORE_ACC(pC,t21);
  pC += LANE;
  VSTORE_ACC(pC,t22);
  pC += max_cols - 2*LANE;



}

__STATIC_INLINE void EXT(kernel_3rx2cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_3rx2cv\n",6);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  VEC tmp00,tmp01;
  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += LANE;
  VECACC t01;
  VLOAD_ACC(t01,p);
  p += max_cols - LANE;

  VECACC t10;
  VLOAD_ACC(t10,p);
  p += LANE;
  VECACC t11;
  VLOAD_ACC(t11,p);
  p += max_cols - LANE;

  VECACC t20;
  VLOAD_ACC(t20,p);
  p += LANE;
  VECACC t21;
  VLOAD_ACC(t21,p);
  p += max_cols - LANE;

  

  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VLOAD(tmp01,pB0);
    pB0 += LANE;

  
    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);
    VMAC_N(t20,tmp00,alpha2);


    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t11,tmp01,alpha1);
    VMAC_N(t21,tmp01,alpha2);


 

  }

  VSTORE_ACC(pC,t00);
  pC += LANE;
  VSTORE_ACC(pC,t01);
  pC += max_cols - LANE;

  VSTORE_ACC(pC,t10);
  pC += LANE;
  VSTORE_ACC(pC,t11);
  pC += max_cols - LANE;

  VSTORE_ACC(pC,t20);
  pC += LANE;
  VSTORE_ACC(pC,t21);
  pC += max_cols - LANE;



}

__STATIC_INLINE void EXT(kernel_3rx1cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_3rx1cv\n",7);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  VEC tmp00;
  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += max_cols ;

  VECACC t10;
  VLOAD_ACC(t10,p);
  p += max_cols ;

  VECACC t20;
  VLOAD_ACC(t20,p);
  p += max_cols;

  

  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

   
  
    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);
    VMAC_N(t20,tmp00,alpha2);


  
 

  }

  VSTORE_ACC(pC,t00);
  pC += max_cols ;

  VSTORE_ACC(pC,t10);
  pC += max_cols ;

  VSTORE_ACC(pC,t20);
  pC += max_cols ;



}

__STATIC_INLINE void EXT(kernel_2rx4cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_2rx4cv\n",8);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  VEC tmp00,tmp01,tmp02,tmp03;
  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += LANE;
  VECACC t01;
  VLOAD_ACC(t01,p);
  p += LANE;
  VECACC t02;
  VLOAD_ACC(t02,p);
  p += LANE;
  VECACC t03;
  VLOAD_ACC(t03,p);
  p += max_cols - 3*LANE;

  VECACC t10;
  VLOAD_ACC(t10,p);
  p += LANE;
  VECACC t11;
  VLOAD_ACC(t11,p);
  p += LANE;
  VECACC t12;
  VLOAD_ACC(t12,p);
  p += LANE;
  VECACC t13;
  VLOAD_ACC(t13,p);
  p += max_cols - 3*LANE;

 
  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VLOAD(tmp01,pB0);
    pB0 += LANE;

    VLOAD(tmp02,pB0);
    pB0 += LANE;

    VLOAD(tmp03,pB0);
    pB0 += LANE;
    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);


    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t11,tmp01,alpha1);
   

    VMAC_N(t02,tmp02,alpha0);
    VMAC_N(t12,tmp02,alpha1);
   

    VMAC_N(t03,tmp03,alpha0);
    VMAC_N(t13,tmp03,alpha1);
    

  }

  VSTORE_ACC(pC,t00);
  pC += LANE;
  VSTORE_ACC(pC,t01);
  pC += LANE;
  VSTORE_ACC(pC,t02);
  pC += LANE;
  VSTORE_ACC(pC,t03);
  pC += max_cols - 3*LANE;

  VSTORE_ACC(pC,t10);
  pC += LANE;
  VSTORE_ACC(pC,t11);
  pC += LANE;
  VSTORE_ACC(pC,t12);
  pC += LANE;
  VSTORE_ACC(pC,t13);
  pC += max_cols - 3*LANE;


}

__STATIC_INLINE void EXT(kernel_2rx3cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_2rx3cv\n",9);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  VEC tmp00,tmp01,tmp02;
  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += LANE;
  VECACC t01;
  VLOAD_ACC(t01,p);
  p += LANE;
  VECACC t02;
  VLOAD_ACC(t02,p);
  p += max_cols - 2*LANE;

  VECACC t10;
  VLOAD_ACC(t10,p);
  p += LANE;
  VECACC t11;
  VLOAD_ACC(t11,p);
  p += LANE;
  VECACC t12;
  VLOAD_ACC(t12,p);
  p += max_cols - 2*LANE;

 
  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VLOAD(tmp01,pB0);
    pB0 += LANE;

    VLOAD(tmp02,pB0);
    pB0 += LANE;

    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);


    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t11,tmp01,alpha1);
   

    VMAC_N(t02,tmp02,alpha0);
    VMAC_N(t12,tmp02,alpha1);
   


  }

  VSTORE_ACC(pC,t00);
  pC += LANE;
  VSTORE_ACC(pC,t01);
  pC += LANE;
  VSTORE_ACC(pC,t02);
  pC += max_cols - 2*LANE;

  VSTORE_ACC(pC,t10);
  pC += LANE;
  VSTORE_ACC(pC,t11);
  pC += LANE;
  VSTORE_ACC(pC,t12);
  pC += max_cols - 2*LANE;


}

__STATIC_INLINE void EXT(kernel_2rx2cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_2rx2cv\n",10);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  VEC tmp00,tmp01;
  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += LANE;
  VECACC t01;
  VLOAD_ACC(t01,p);
  p += max_cols - LANE;

  VECACC t10;
  VLOAD_ACC(t10,p);
  p += LANE;
  VECACC t11;
  VLOAD_ACC(t11,p);
  p += max_cols - LANE;

 
  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VLOAD(tmp01,pB0);
    pB0 += LANE;

    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);


    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t11,tmp01,alpha1);
  


  }

  VSTORE_ACC(pC,t00);
  pC += LANE;
  VSTORE_ACC(pC,t01);
  pC += max_cols - LANE;

  VSTORE_ACC(pC,t10);
  pC += LANE;
  VSTORE_ACC(pC,t11);
  pC += max_cols - LANE;


}

__STATIC_INLINE void EXT(kernel_2rx1cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_2rx1cv\n",11);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  VEC tmp00;

  TMPMAC
  
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += max_cols;
  VECACC t10;
  VLOAD_ACC(t10,p);
  p += max_cols;
 

 
  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);



  }

  VSTORE_ACC(pC,t00);
  pC += max_cols;

  VSTORE_ACC(pC,t10);
  pC += max_cols ;


}

__STATIC_INLINE void EXT(kernel_1rx4cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_1rx4cv\n",12);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  VEC tmp00,tmp01,tmp02,tmp03;

  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += LANE;
  VECACC t01;
  VLOAD_ACC(t01,p);
  p += LANE;
  VECACC t02;
  VLOAD_ACC(t02,p);
  p += LANE;
  VECACC t03;
  VLOAD_ACC(t03,p);

  const DTYPE *pAlpha0 = &packedA[xp * r];

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VLOAD(tmp01,pB0);
    pB0 += LANE;

    VLOAD(tmp02,pB0);
    pB0 += LANE;

    VLOAD(tmp03,pB0);
    pB0 += LANE;
    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t02,tmp02,alpha0);
    VMAC_N(t03,tmp03,alpha0);
    
  }

  VSTORE_ACC(pC,t00);
  pC += LANE;
  VSTORE_ACC(pC,t01);
  pC += LANE;
  VSTORE_ACC(pC,t02);
  pC += LANE;
  VSTORE_ACC(pC,t03);

}

__STATIC_INLINE void EXT(kernel_1rx3cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_1rx3cv\n",13);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  VEC tmp00,tmp01,tmp02;
  TMPMAC
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += LANE;
  VECACC t01;
  VLOAD_ACC(t01,p);
  p += LANE;
  VECACC t02;
  VLOAD_ACC(t02,p);
  p += max_cols - 2*LANE;

 
  const DTYPE *pAlpha0 = &packedA[xp * r];


  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VLOAD(tmp01,pB0);
    pB0 += LANE;

    VLOAD(tmp02,pB0);
    pB0 += LANE;

    
    VMAC_N(t00,tmp00,alpha0);


    VMAC_N(t01,tmp01,alpha0);
   

    VMAC_N(t02,tmp02,alpha0);
   

    

  }

  VSTORE_ACC(pC,t00);
  pC += LANE;
  VSTORE_ACC(pC,t01);
  pC += LANE;
  VSTORE_ACC(pC,t02);
  pC += max_cols - 2*LANE;


}

__STATIC_INLINE void EXT(kernel_1rx2cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_1rx2cv\n",14);
  VEC tmp00,tmp01;
  TMPMAC

  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);
  p += LANE;
  VECACC t01;
  VLOAD_ACC(t01,p);
  p += max_cols - LANE;

 
  const DTYPE *pAlpha0 = &packedA[xp * r];


  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VLOAD(tmp01,pB0);
    pB0 += LANE;

    
    
    VMAC_N(t00,tmp00,alpha0);


    VMAC_N(t01,tmp01,alpha0);
   

   

    

  }

  VSTORE_ACC(pC,t00);
  pC += LANE;
  VSTORE_ACC(pC,t01);


}

__STATIC_INLINE void EXT(kernel_1rx1cv)(SCALARACC *packedc,int col,int max_cols,int xp,int r) {

  LOGKERNEL("kernel_1rx1cv\n",15);
  VEC tmp00,tmp01;
  TMPMAC

  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
   
  SCALARACC *pC = &packedc[xp*max_cols+col];
  const SCALARACC *p = pC;
  

  VECACC t00;
  VLOAD_ACC(t00,p);

 
  const DTYPE *pAlpha0 = &packedA[xp * r];


  int k=0;
  for (; k <= (r-2) ; k += 2) 
  {
    const DTYPE alpha00 = *pAlpha0++;
    const DTYPE alpha01 = *pAlpha0++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;
    VLOAD(tmp01,pB0);
    pB0 += LANE;

    VMAC_N(t00,tmp00,alpha00);
    VMAC_N(t00,tmp01,alpha01);
  }

  for (; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;

    VLOAD(tmp00,pB0);
    pB0 += LANE;

    VMAC_N(t00,tmp00,alpha0);
  }

  VSTORE_ACC(pC,t00);

}

__STATIC_INLINE void EXT(kernel_4rxc)(SCALARACC *packedc,int col,int max_cols,int xp,int r,int nbc) {

  LOGKERNEL("kernel_4rxc\n",16);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  SCALARACC *p0 = &packedc[xp*max_cols+col];
  SCALARACC *p1 = p0+max_cols;
  SCALARACC *p2 = p1+max_cols;
  SCALARACC *p3 = p2+max_cols;


  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;
  const DTYPE *pAlpha3 = pAlpha2 + r;

  
  SCALARACC v0[LANE-1];
  SCALARACC v1[LANE-1];
  SCALARACC v2[LANE-1];
  SCALARACC v3[LANE-1];

  for(int i=0;i<nbc;i++)
  {
    v0[i]=p0[i];
    v1[i]=p1[i];
    v2[i]=p2[i];
    v3[i]=p3[i];
  }
    
  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;
    const DTYPE alpha3 = *pAlpha3++;
    for (int c=0;c<nbc;c++)
    {
        DTYPE b = *pB0++;
        
        SCALAR_MAC_N(v0[c],alpha0,b);
        SCALAR_MAC_N(v1[c],alpha1,b);
        SCALAR_MAC_N(v2[c],alpha2,b);
        SCALAR_MAC_N(v3[c],alpha3,b);
    }
  }

  for(int i=0;i<nbc;i++)
  {
     p0[i]=v0[i];
     p1[i]=v1[i];
     p2[i]=v2[i];
     p3[i]=v3[i];
  }
}

__STATIC_INLINE void EXT(kernel_3rxc)(SCALARACC *packedc,int col,int max_cols,int xp,int r,int nbc) {

  LOGKERNEL("kernel_3rxc\n",17);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  SCALARACC *p0 = &packedc[xp*max_cols+col];
  SCALARACC *p1 = p0+max_cols;
  SCALARACC *p2 = p1+max_cols;


  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;

  
  SCALARACC v0[LANE-1];
  SCALARACC v1[LANE-1];
  SCALARACC v2[LANE-1];

  for(int i=0;i<nbc;i++)
  {
    v0[i]=p0[i];
    v1[i]=p1[i];
    v2[i]=p2[i];
  }
  
  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;
    for (int c=0;c<nbc;c++)
    {
        DTYPE b = *pB0++;
        
        SCALAR_MAC_N(v0[c],alpha0,b);
        SCALAR_MAC_N(v1[c],alpha1,b);
        SCALAR_MAC_N(v2[c],alpha2,b);
    }

  }

  for(int i=0;i<nbc;i++)
  {
    p0[i]=v0[i];
    p1[i]=v1[i];
    p2[i]=v2[i];
  }

}

__STATIC_INLINE void EXT(kernel_2rxc)(SCALARACC *packedc,int col,int max_cols,int xp,int r,int nbc) {

  LOGKERNEL("kernel_2rxc\n",18);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  SCALARACC *p0 = &packedc[xp*max_cols+col];
  SCALARACC *p1 = p0+max_cols;


  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;

  
  SCALARACC v0[LANE-1];
  SCALARACC v1[LANE-1];

  for(int i=0;i<nbc;i++)
  {
    v0[i]=p0[i];
    v1[i]=p1[i];
  }
  
  
  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;

    for (int c=0;c<nbc;c++)
    {
        DTYPE b = *pB0++;
        
        SCALAR_MAC_N(v0[c],alpha0,b);
        SCALAR_MAC_N(v1[c],alpha1,b);
    }

  }

  for(int i=0;i<nbc;i++)
  {
    p0[i]=v0[i];
    p1[i]=v1[i];
  }

}

__STATIC_INLINE void EXT(kernel_1rxc)(SCALARACC *packedc,int col,int max_cols,int xp,int r,int nbc) {

  LOGKERNEL("kernel_1rxc\n",19);
  const DTYPE *pB0 = (DTYPE*)ARM_CACHE_PACKEDB;
  const DTYPE *packedA = (DTYPE*)ARM_CACHE_PACKEDA;
  SCALARACC *p0 = &packedc[xp*max_cols+col];


  const DTYPE *pAlpha0 = &packedA[xp * r];

 
  SCALARACC v0[LANE-1];

  for(int i=0;i<nbc;i++)
  {
    v0[i]=p0[i];
  }
       
  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;

    for (int c=0;c<nbc;c++)
    {
      DTYPE b = *pB0++;
             
      SCALAR_MAC_N(v0[c],alpha0,b);
    }

  }

  for(int i=0;i<nbc;i++)
  {
    p0[i]=v0[i];
  }
  
}

#define PACK(BUF,ARM_CACHE_PACKED,WIDTH,HEIGHT,RB,CB,ROW,COL)      \
{                                                        \
    const DTYPE *p = &(BUF)[(COL)+(ROW)*(WIDTH)];        \
    DTYPE *packed=(DTYPE*)(ARM_CACHE_PACKED);                      \
    const int nb_rows = MIN((HEIGHT),(ROW)+(RB))-(ROW);  \
    const int nb_cols = MIN((WIDTH),(COL)+(CB)) - (COL); \
    for(int i=0;i<nb_rows;i++)                           \
    {                                                    \
        int j=0;                                         \
        for(;j<=(nb_cols-LANE);j+=LANE)                  \
        {                                                \
            VEC tmp;                                     \
            VLOAD(tmp,p);                                \
            p += LANE;                                   \
            VSTORE(packed,tmp);                          \
            packed += LANE;                              \
        }                                                \
        for(;j<nb_cols;j++)                              \
        {                                                \
            *packed++ = *p++;                            \
        }                                                \
        packed += (CB) - nb_cols;                        \
        p += WIDTH - nb_cols;                            \
    }                                                    \
}

#define CONCAT2(a, b) a##b
#define CONCAT(a, b) CONCAT2(a, b)
#define CLEARFUNC CONCAT(clearpacked_, FUNCNAME)

static void CLEARFUNC (char *ARM_CACHE_PACKED,int WIDTH,int HEIGHT,int RB,int CB,int ROW,int COL)
{                                                        
    volatile SCALARACC *packed=(SCALARACC*)(ARM_CACHE_PACKED);              
    const int nb_rows = MIN((HEIGHT),(ROW)+(RB))-(ROW);  
    const int nb_cols = MIN((WIDTH),(COL)+(CB)) - (COL); 
    /*memset(packed,0,sizeof(SCALARACC)*((CB)*nb_rows)); }*/ 
    for(int i=0;i<nb_rows;i++)                           
    {                                                    
        int j=0;                                         
        VECACC vtmp ;                                    
        CLEAR_ACC(vtmp);                                 
        for(;j<=(nb_cols-LANE);j+=LANE)                  
        {                                                
            VSTORE_ACC((SCALARACC *)packed,vtmp);                     
            packed += LANE;                              
        }                                                
        DEF_AND_CLEAR_SCALARACC(tmp);                    
        for(;j<nb_cols;j++)                              
        {                                                
            *packed++ = tmp;                             
        }                                                
        packed += (CB) - nb_cols;                        
    }                                                    
}


#define UNPACK_AND_NARROW(BUF,ARM_CACHE_PACKED,WIDTH,HEIGHT,RB,CB,ROW,COL) \
{                                                                \
    TMPST;                                                       \
    DTYPE *p = &(BUF)[(COL)+(ROW)*(WIDTH)];                      \
    const SCALARACC *packed=(const SCALARACC*)(ARM_CACHE_PACKED);          \
    const int nb_rows = MIN((HEIGHT),(ROW)+(RB))-(ROW);          \
    const int nb_cols = MIN((WIDTH),(COL)+(CB)) - (COL);         \
    for(int i=0;i<nb_rows;i++)                                   \
    {                                                            \
        int j=0;                                                 \
        for(;j<=(nb_cols-LANE);j+=LANE)                          \
        {                                                        \
            VECACC tmp ;                                         \
            VLOAD_ACC(tmp,packed);                               \
            packed += LANE;                                      \
            VSTORE_AND_NARROW(p,tmp);                            \
            p += LANE;                                           \
        }                                                        \
        for(;j<nb_cols;j++)                                      \
        {                                                        \
          SCALAR_STORE_AND_NARROW(p,*packed);                    \
          packed++;                                              \
          p++;                                                   \
        }                                                        \
        packed += (CB) - nb_cols;                                \
        p += WIDTH - nb_cols;                                    \
    }                                                            \
}


ARM_DSP_ATTRIBUTE arm_status FUNCNAME (
#if defined(HAS_TEMP_BUFFER)
  const MATTYPE * pSrcA,
  const MATTYPE * pSrcB,
  MATTYPE * pDst,
  HEADERTYPE   * pState)
#else
  const MATTYPE * pSrcA,
  const MATTYPE * pSrcB,
  MATTYPE * pDst)
#endif
{
  #if defined(HAS_TEMP_BUFFER)
  (void)pState;
  #endif
  const DTYPE *a = (const DTYPE *)pSrcA->pData;
  const DTYPE *b = (const DTYPE *)pSrcB->pData;
  DTYPE *c = (DTYPE *)pDst->pData;
  SCALARACC *packedc = (SCALARACC*)ARM_CACHE_PACKEDC;
  #if defined(FLOATALGO)
  int mustcopy=0;
  #endif

  int rows,cols,inners;
  rows=pSrcA->numRows;
  inners=pSrcA->numCols;
  cols=pSrcB->numCols;

  #if defined(FLOATALGO)
  /*
  
  For float, the accumulator has same datatype as output datatype.
  So, we can use directly the output matrix to accumulate.
  It is what we do for small matrixes.
  For bigger matrixes, it still makes sense (for cache effects) to use a
  temporary smaller matrix and copy it into some part of the output matrix.

  For fixed point, we use the temporary matrix in all cases since the
  accumulator datatype is different from the output datatype.
  
  */
  if ((cols > COLS_BLOCK) || (rows > ROWS_BLOCK))
  {
    mustcopy = 1;
  }
  else
  {
    memset(c,0,sizeof(DTYPE)*cols*rows);
  }
  #endif


    for (int block_col = 0; block_col < cols; block_col += COLS_BLOCK)
    { 
      for (int block_row = 0; block_row < rows; block_row += ROWS_BLOCK) 
      {    
        int max_cols = MIN(0 + COLS_BLOCK, cols - block_col);
        int max_rows = MIN(0 + ROWS_BLOCK, rows - block_row);
        #if defined(FLOATALGO)
        if (mustcopy)
        {
           CLEARFUNC (ARM_CACHE_PACKEDC,cols,rows,max_rows,max_cols,block_row,block_col);
           packedc = (SCALARACC*)ARM_CACHE_PACKEDC;
        }
        else 
        {
          packedc = (SCALARACC*)c;
        }
        #else

        CLEARFUNC (ARM_CACHE_PACKEDC,cols,rows,max_rows,max_cols,block_row,block_col);
        #endif
        for (int block_inner = 0; block_inner < inners; block_inner += INNER_BLOCK)
        {
          int max_inner = MIN(INNER_BLOCK, inners-block_inner);
          PACK(a,ARM_CACHE_PACKEDA,inners,rows,max_rows,max_inner,block_row,block_inner);

          int col=0;

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-(4*LANE)); col += (4*LANE))
          {
            int row=0;
            PACK(b,ARM_CACHE_PACKEDB,cols,inners,max_inner,(4*LANE),block_inner,block_col+col);

            for (; row <= (max_rows-DR); row += DR)
            {
              EXT(kernel_4rx4cv)(packedc,col,max_cols,row,max_inner);
            }

            int remaining = max_rows - row;

            //for (; row <= (max_rows-HDR); row += HDR)
            if (remaining == 3)
            {
              EXT(kernel_3rx4cv)(packedc,col,max_cols,row,max_inner);
            }
            else if (remaining == 2)
            {
              EXT(kernel_2rx4cv)(packedc,col,max_cols,row,max_inner);
            }
            else if (remaining == 1)
            //(; row < max_rows; row ++)
            {
              EXT(kernel_1rx4cv)(packedc,col,max_cols,row,max_inner);
            } // end remaining row
          } // end block col


          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-(3*LANE)); col += (3*LANE))
          {
             int row=0;
             PACK(b,ARM_CACHE_PACKEDB,cols,inners,max_inner,(3*LANE),block_inner,block_col+col);

             for (; row <= (max_rows-DR); row += DR)
             {
               EXT(kernel_4rx3cv)(packedc,col,max_cols,row,max_inner);
             }

             int remaining = max_rows - row;

             if (remaining == 3)
             {
               EXT(kernel_3rx3cv)(packedc,col,max_cols,row,max_inner);
             }
             else if (remaining == 2)
             {
               EXT(kernel_2rx3cv)(packedc,col,max_cols,row,max_inner);
             }
             else if (remaining == 1)
             {
               EXT(kernel_1rx3cv)(packedc,col,max_cols,row,max_inner);
             }
          }

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-(2*LANE)); col += (2*LANE))
          {
            int row=0;
            PACK(b,ARM_CACHE_PACKEDB,cols,inners,max_inner,(2*LANE),block_inner,block_col+col);

            for (; row <= (max_rows-DR); row += DR)
            {
              EXT(kernel_4rx2cv)(packedc,col,max_cols,row,max_inner);
            }

            int remaining = max_rows - row;

            if (remaining == 3)
            {
              EXT(kernel_3rx2cv)(packedc,col,max_cols,row,max_inner);
            }
            else if (remaining == 2)
            {
              EXT(kernel_2rx2cv)(packedc,col,max_cols,row,max_inner);
            }
            else if (remaining == 1)
            {
              EXT(kernel_1rx2cv)(packedc,col,max_cols,row,max_inner);
            }
          }

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-(1*LANE)); col += (1*LANE))
          {
            int row=0;
            PACK(b,ARM_CACHE_PACKEDB,cols,inners,max_inner,(1*LANE),block_inner,block_col+col);

            for (; row <= (max_rows-DR); row += DR)
            {
              EXT(kernel_4rx1cv)(packedc,col,max_cols,row,max_inner);
            }

            int remaining = max_rows - row;

            if (remaining == 3)
            {
              EXT(kernel_3rx1cv)(packedc,col,max_cols,row,max_inner);
            }
            else if (remaining == 2)
            {
              EXT(kernel_2rx1cv)(packedc,col,max_cols,row,max_inner);
            }
            else if (remaining == 1)
            {
              EXT(kernel_1rx1cv)(packedc,col,max_cols,row,max_inner);
            }
          }

          int maxnc = (MIN(0 + COLS_BLOCK, cols - block_col)) - col;
          if (maxnc > 0)
          {
             int row=0;
             PACK(b,ARM_CACHE_PACKEDB,cols,inners,max_inner,maxnc,block_inner,block_col+col);

             for (; row <= (max_rows-DR); row += DR)
             {
               EXT(kernel_4rxc)(packedc,col,max_cols,row,max_inner,maxnc);
             }

             int remaining = max_rows - row;

             if (remaining == 3)
             {
               EXT(kernel_3rxc)(packedc,col,max_cols,row,max_inner,maxnc);
             }
             else if (remaining == 2)
             {
               EXT(kernel_2rxc)(packedc,col,max_cols,row,max_inner,maxnc);
             }
             else if (remaining == 1)
             {
              EXT(kernel_1rxc)(packedc,col,max_cols,row,max_inner,maxnc);
             }
          }
        } // end inner loop
        #if defined(FLOATALGO)
        if (mustcopy)
        {
          UNPACK_AND_NARROW(c,ARM_CACHE_PACKEDC,cols,rows,max_rows,max_cols,block_row,block_col);
        }
        #else
        UNPACK_AND_NARROW(c,ARM_CACHE_PACKEDC,cols,rows,max_rows,max_cols,block_row,block_col);
        #endif
     } // end row loop
   } // end col loop

   return (ARM_MATH_SUCCESS);
}


#undef LANE 
#undef DTYPE 
#undef HEADERTYPE
#undef VEC 
#undef VECACC
#undef HVEC 
#undef VLOAD
#undef VSTORE
#undef VMAC_N
#undef MATTYPE 
#undef EXT
#undef SCALARACC
#undef VLOAD
#undef VSTORE
#if defined(SCALAR_LOAD_AND_WIDEN)
#undef SCALAR_LOAD_AND_WIDEN
#endif
#undef SCALAR_STORE_AND_NARROW
#undef SCALAR_MAC_N
#if defined(VLOAD_AND_WIDEN)
#undef VLOAD_AND_WIDEN
#endif
#undef VSTORE_AND_NARROW
#undef EXT
#undef CLEAR_ACC
#undef DEF_AND_CLEAR_SCALARACC

#undef VLOAD_ACC
#undef VSTORE_ACC

#undef FUNCNAME

#undef PACK
#undef PACK_AND_WIDEN
#undef UNPACK_AND_NARROW
#undef CLEAR

#undef TMPMAC
#undef TMPLD
#undef TMPST

#if defined(DEBUGV)
#undef DEBUGV
#endif

#if defined(DEBUGVACC)
#undef DEBUGVACC
#endif

#if defined(DEBUGS)
#undef DEBUGS
#endif

#if defined(DEBUGSACC)
#undef DEBUGSACC
#endif

#if defined(LOGKERNEL)
#undef LOGKERNEL
#endif

#if defined(HAS_TEMP_BUFFER)
#undef HAS_TEMP_BUFFER
#endif

#if defined(FLOATALGO)
#undef FLOATALGO
#endif




#undef DC 
