


#define DCS 4 // Max number of vectors for columns (can't be changed)
#define DC (LANE*DCS) // Must be multiple of lane
#define DR 4 // Max number rows in kernels (can't be changed)
#define HDR 2 // Max number rows by half

// INNER * DC < L1/2
// INNER * ROWS < L2 
// INNER * COLS < L3
#define INNER_BLOCK ((((ARM_MATH_L1_CACHE_SIZE>>2) >> 1)/DC+DC-1) & ~(DC-1))
#define ROWS_BLOCK ((((ARM_MATH_L2_CACHE_SIZE>>2) >> 0) / INNER_BLOCK + DR - 1) & ~(DR-1) ) 
#define COLS_BLOCK (((ARM_MATH_L3_CACHE_SIZE>>2) / INNER_BLOCK + DC - 1) & ~(DC-1)) 


static __ALIGNED(16) char PACKEDB[4*INNER_BLOCK*DC];
static __ALIGNED(16) char PACKEDA[4*ROWS_BLOCK*INNER_BLOCK];

#if !defined(USE_TMP_REGISTER)
#define tmpld
#define htmp
#endif

// 4 row and 4 column VECtors
__STATIC_INLINE void EXT(kernel_4rx4cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;

  VEC tmp00,tmp01,tmp02,tmp03;

  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += LANE;
  VECACC t01;
  VLOAD_AND_WIDEN(t01,tmpld,p);
  p += LANE;
  VECACC t02;
  VLOAD_AND_WIDEN(t02,tmpld,p);
  p += LANE;
  VECACC t03;
  VLOAD_AND_WIDEN(t03,tmpld,p);
  p += cols - 3*LANE;

  VECACC t10;
  VLOAD_AND_WIDEN(t10,tmpld,p);
  p += LANE;
  VECACC t11;
  VLOAD_AND_WIDEN(t11,tmpld,p);
  p += LANE;
  VECACC t12;
  VLOAD_AND_WIDEN(t12,tmpld,p);
  p += LANE;
  VECACC t13;
  VLOAD_AND_WIDEN(t13,tmpld,p);
  p += cols - 3*LANE;

  VECACC t20;
  VLOAD_AND_WIDEN(t20,tmpld,p);
  p += LANE;
  VECACC t21;
  VLOAD_AND_WIDEN(t21,tmpld,p);
  p += LANE;
  VECACC t22;
  VLOAD_AND_WIDEN(t22,tmpld,p);
  p += LANE;
  VECACC t23;
  VLOAD_AND_WIDEN(t23,tmpld,p);
  p += cols - 3*LANE;

  VECACC t30;
  VLOAD_AND_WIDEN(t30,tmpld,p);
  p += LANE;
  VECACC t31;
  VLOAD_AND_WIDEN(t31,tmpld,p);
  p += LANE;
  VECACC t32;
  VLOAD_AND_WIDEN(t32,tmpld,p);
  p += LANE;
  VECACC t33;
  VLOAD_AND_WIDEN(t33,tmpld,p);
  p += cols - 3*LANE;

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

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    tmp01 = VLOAD(pB0);
    pB0 += LANE;

    tmp02 = VLOAD(pB0);
    pB0 += LANE;

    tmp03 = VLOAD(pB0);
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

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t01);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t02);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t03);
  pC += cols - 3*LANE;

  VSTORE_AND_NARROW(pC,htmp,t10);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t11);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t12);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t13);
  pC += cols - 3*LANE;

  VSTORE_AND_NARROW(pC,htmp,t20);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t21);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t22);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t23);
  pC += cols - 3*LANE;

  VSTORE_AND_NARROW(pC,htmp,t30);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t31);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t32);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t33);
  pC += cols - 3*LANE;
}

__STATIC_INLINE void EXT(kernel_4rx3cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;

  VEC tmp00,tmp01,tmp02;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += LANE;
  VECACC t01;
  VLOAD_AND_WIDEN(t01,tmpld,p);
  p += LANE;
  VECACC t02;
  VLOAD_AND_WIDEN(t02,tmpld,p);
  p += cols - 2*LANE;

  VECACC t10;
  VLOAD_AND_WIDEN(t10,tmpld,p);
  p += LANE;
  VECACC t11;
  VLOAD_AND_WIDEN(t11,tmpld,p);
  p += LANE;
  VECACC t12;
  VLOAD_AND_WIDEN(t12,tmpld,p);
  p += cols - 2*LANE;

  VECACC t20;
  VLOAD_AND_WIDEN(t20,tmpld,p);
  p += LANE;
  VECACC t21;
  VLOAD_AND_WIDEN(t21,tmpld,p);
  p += LANE;
  VECACC t22;
  VLOAD_AND_WIDEN(t22,tmpld,p);
  p += cols - 2*LANE;

  VECACC t30;
  VLOAD_AND_WIDEN(t30,tmpld,p);
  p += LANE;
  VECACC t31;
  VLOAD_AND_WIDEN(t31,tmpld,p);
  p += LANE;
  VECACC t32;
  VLOAD_AND_WIDEN(t32,tmpld,p);
  p += cols - 2*LANE;

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

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    tmp01 = VLOAD(pB0);
    pB0 += LANE;

    tmp02 = VLOAD(pB0);
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

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t01);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t02);
  pC += cols - 2*LANE;

  VSTORE_AND_NARROW(pC,htmp,t10);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t11);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t12);
  pC += cols - 2*LANE;

  VSTORE_AND_NARROW(pC,htmp,t20);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t21);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t22);
  pC += cols - 2*LANE;

  VSTORE_AND_NARROW(pC,htmp,t30);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t31);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t32);
  pC += cols - 2*LANE;

}

__STATIC_INLINE void EXT(kernel_4rx2cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += LANE;
  VECACC t01;
  VLOAD_AND_WIDEN(t01,tmpld,p);
  p += cols - LANE;

  VECACC t10;
  VLOAD_AND_WIDEN(t10,tmpld,p);
  p += LANE;
  VECACC t11;
  VLOAD_AND_WIDEN(t11,tmpld,p);
  p += cols - LANE;

  VECACC t20;
  VLOAD_AND_WIDEN(t20,tmpld,p);
  p += LANE;
  VECACC t21;
  VLOAD_AND_WIDEN(t21,tmpld,p);
  p += cols - LANE;

  VECACC t30;
  VLOAD_AND_WIDEN(t30,tmpld,p);
  p += LANE;
  VECACC t31;
  VLOAD_AND_WIDEN(t31,tmpld,p);
  p += cols - LANE;

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

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    tmp01 = VLOAD(pB0);
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

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t01);
  pC += cols - LANE;

  VSTORE_AND_NARROW(pC,htmp,t10);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t11);
  pC += cols - LANE;

  VSTORE_AND_NARROW(pC,htmp,t20);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t21);
  pC += cols - LANE;

  VSTORE_AND_NARROW(pC,htmp,t30);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t31);
  pC += cols - LANE;

}

__STATIC_INLINE void EXT(kernel_4rx1cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  VECACC t10;
  VLOAD_AND_WIDEN(t10,tmpld,p);
  VECACC t20;
  VLOAD_AND_WIDEN(t20,tmpld,p);
  VECACC t30;
  VLOAD_AND_WIDEN(t30,tmpld,p);
 

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

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);
    VMAC_N(t20,tmp00,alpha2);
    VMAC_N(t30,tmp00,alpha3);



  }

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += cols;
  VSTORE_AND_NARROW(pC,htmp,t10);
  pC += cols;
  VSTORE_AND_NARROW(pC,htmp,t20);
  pC += cols;
  VSTORE_AND_NARROW(pC,htmp,t30);
 

}

__STATIC_INLINE void EXT(kernel_3rx4cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01,tmp02,tmp03;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += LANE;
  VECACC t01;
  VLOAD_AND_WIDEN(t01,tmpld,p);
  p += LANE;
  VECACC t02;
  VLOAD_AND_WIDEN(t02,tmpld,p);
  p += LANE;
  VECACC t03;
  VLOAD_AND_WIDEN(t03,tmpld,p);
  p += cols - 3*LANE;

  VECACC t10;
  VLOAD_AND_WIDEN(t10,tmpld,p);
  p += LANE;
  VECACC t11;
  VLOAD_AND_WIDEN(t11,tmpld,p);
  p += LANE;
  VECACC t12;
  VLOAD_AND_WIDEN(t12,tmpld,p);
  p += LANE;
  VECACC t13;
  VLOAD_AND_WIDEN(t13,tmpld,p);
  p += cols - 3*LANE;

  VECACC t20;
  VLOAD_AND_WIDEN(t20,tmpld,p);
  p += LANE;
  VECACC t21;
  VLOAD_AND_WIDEN(t21,tmpld,p);
  p += LANE;
  VECACC t22;
  VLOAD_AND_WIDEN(t22,tmpld,p);
  p += LANE;
  VECACC t23;
  VLOAD_AND_WIDEN(t23,tmpld,p);
  p += cols - 3*LANE;

  

  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    tmp01 = VLOAD(pB0);
    pB0 += LANE;

    tmp02 = VLOAD(pB0);
    pB0 += LANE;

    tmp03 = VLOAD(pB0);
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

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t01);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t02);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t03);
  pC += cols - 3*LANE;

  VSTORE_AND_NARROW(pC,htmp,t10);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t11);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t12);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t13);
  pC += cols - 3*LANE;

  VSTORE_AND_NARROW(pC,htmp,t20);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t21);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t22);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t23);
  pC += cols - 3*LANE;



}

__STATIC_INLINE void EXT(kernel_3rx3cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01,tmp02;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += LANE;
  VECACC t01;
  VLOAD_AND_WIDEN(t01,tmpld,p);
  p += LANE;
  VECACC t02;
  VLOAD_AND_WIDEN(t02,tmpld,p);
  p += cols - 2*LANE;

  VECACC t10;
  VLOAD_AND_WIDEN(t10,tmpld,p);
  p += LANE;
  VECACC t11;
  VLOAD_AND_WIDEN(t11,tmpld,p);
  p += LANE;
  VECACC t12;
  VLOAD_AND_WIDEN(t12,tmpld,p);
  p += cols - 2*LANE;

  VECACC t20;
  VLOAD_AND_WIDEN(t20,tmpld,p);
  p += LANE;
  VECACC t21;
  VLOAD_AND_WIDEN(t21,tmpld,p);
  p += LANE;
  VECACC t22;
  VLOAD_AND_WIDEN(t22,tmpld,p);
  p += cols - 2*LANE;

  

  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    tmp01 = VLOAD(pB0);
    pB0 += LANE;

    tmp02 = VLOAD(pB0);
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

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t01);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t02);
  pC += cols - 2*LANE;

  VSTORE_AND_NARROW(pC,htmp,t10);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t11);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t12);
  pC += cols - 2*LANE;

  VSTORE_AND_NARROW(pC,htmp,t20);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t21);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t22);
  pC += cols - 2*LANE;



}

__STATIC_INLINE void EXT(kernel_3rx2cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += LANE;
  VECACC t01;
  VLOAD_AND_WIDEN(t01,tmpld,p);
  p += cols - LANE;

  VECACC t10;
  VLOAD_AND_WIDEN(t10,tmpld,p);
  p += LANE;
  VECACC t11;
  VLOAD_AND_WIDEN(t11,tmpld,p);
  p += cols - LANE;

  VECACC t20;
  VLOAD_AND_WIDEN(t20,tmpld,p);
  p += LANE;
  VECACC t21;
  VLOAD_AND_WIDEN(t21,tmpld,p);
  p += cols - LANE;

  

  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    tmp01 = VLOAD(pB0);
    pB0 += LANE;

  
    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);
    VMAC_N(t20,tmp00,alpha2);


    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t11,tmp01,alpha1);
    VMAC_N(t21,tmp01,alpha2);


 

  }

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t01);
  pC += cols - LANE;

  VSTORE_AND_NARROW(pC,htmp,t10);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t11);
  pC += cols - LANE;

  VSTORE_AND_NARROW(pC,htmp,t20);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t21);
  pC += cols - LANE;



}

__STATIC_INLINE void EXT(kernel_3rx1cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += cols ;

  VECACC t10;
  VLOAD_AND_WIDEN(t10,tmpld,p);
  p += cols ;

  VECACC t20;
  VLOAD_AND_WIDEN(t20,tmpld,p);
  p += cols;

  

  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;
    const DTYPE alpha2 = *pAlpha2++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

   
  
    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);
    VMAC_N(t20,tmp00,alpha2);


  
 

  }

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += cols ;

  VSTORE_AND_NARROW(pC,htmp,t10);
  pC += cols ;

  VSTORE_AND_NARROW(pC,htmp,t20);
  pC += cols ;



}

__STATIC_INLINE void EXT(kernel_2rx4cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01,tmp02,tmp03;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += LANE;
  VECACC t01;
  VLOAD_AND_WIDEN(t01,tmpld,p);
  p += LANE;
  VECACC t02;
  VLOAD_AND_WIDEN(t02,tmpld,p);
  p += LANE;
  VECACC t03;
  VLOAD_AND_WIDEN(t03,tmpld,p);
  p += cols - 3*LANE;

  VECACC t10;
  VLOAD_AND_WIDEN(t10,tmpld,p);
  p += LANE;
  VECACC t11;
  VLOAD_AND_WIDEN(t11,tmpld,p);
  p += LANE;
  VECACC t12;
  VLOAD_AND_WIDEN(t12,tmpld,p);
  p += LANE;
  VECACC t13;
  VLOAD_AND_WIDEN(t13,tmpld,p);
  p += cols - 3*LANE;

 
  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    tmp01 = VLOAD(pB0);
    pB0 += LANE;

    tmp02 = VLOAD(pB0);
    pB0 += LANE;

    tmp03 = VLOAD(pB0);
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

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t01);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t02);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t03);
  pC += cols - 3*LANE;

  VSTORE_AND_NARROW(pC,htmp,t10);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t11);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t12);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t13);
  pC += cols - 3*LANE;


}

__STATIC_INLINE void EXT(kernel_2rx3cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01,tmp02;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += LANE;
  VECACC t01;
  VLOAD_AND_WIDEN(t01,tmpld,p);
  p += LANE;
  VECACC t02;
  VLOAD_AND_WIDEN(t02,tmpld,p);
  p += cols - 2*LANE;

  VECACC t10;
  VLOAD_AND_WIDEN(t10,tmpld,p);
  p += LANE;
  VECACC t11;
  VLOAD_AND_WIDEN(t11,tmpld,p);
  p += LANE;
  VECACC t12;
  VLOAD_AND_WIDEN(t12,tmpld,p);
  p += cols - 2*LANE;

 
  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    tmp01 = VLOAD(pB0);
    pB0 += LANE;

    tmp02 = VLOAD(pB0);
    pB0 += LANE;

    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);


    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t11,tmp01,alpha1);
   

    VMAC_N(t02,tmp02,alpha0);
    VMAC_N(t12,tmp02,alpha1);
   


  }

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t01);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t02);
  pC += cols - 2*LANE;

  VSTORE_AND_NARROW(pC,htmp,t10);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t11);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t12);
  pC += cols - 2*LANE;


}

__STATIC_INLINE void EXT(kernel_2rx2cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += LANE;
  VECACC t01;
  VLOAD_AND_WIDEN(t01,tmpld,p);
  p += cols - LANE;

  VECACC t10;
  VLOAD_AND_WIDEN(t10,tmpld,p);
  p += LANE;
  VECACC t11;
  VLOAD_AND_WIDEN(t11,tmpld,p);
  p += cols - LANE;

 
  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    tmp01 = VLOAD(pB0);
    pB0 += LANE;

    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);


    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t11,tmp01,alpha1);
  


  }

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t01);
  pC += cols - LANE;

  VSTORE_AND_NARROW(pC,htmp,t10);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t11);
  pC += cols - LANE;


}

__STATIC_INLINE void EXT(kernel_2rx1cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  VECACC t10;
  VLOAD_AND_WIDEN(t10,tmpld,p);
 

 
  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;
    const DTYPE alpha1 = *pAlpha1++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t10,tmp00,alpha1);



  }

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += cols;

  VSTORE_AND_NARROW(pC,htmp,t10);
  pC += cols ;


}

__STATIC_INLINE void EXT(kernel_1rx4cv)(int cols,DTYPE *pC,int xp,int r) {

  (void)cols;
  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01,tmp02,tmp03;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += LANE;
  VECACC t01;
  VLOAD_AND_WIDEN(t01,tmpld,p);
  p += LANE;
  VECACC t02;
  VLOAD_AND_WIDEN(t02,tmpld,p);
  p += LANE;
  VECACC t03;
  VLOAD_AND_WIDEN(t03,tmpld,p);

  const DTYPE *pAlpha0 = &packedA[xp * r];

  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    tmp01 = VLOAD(pB0);
    pB0 += LANE;

    tmp02 = VLOAD(pB0);
    pB0 += LANE;

    tmp03 = VLOAD(pB0);
    pB0 += LANE;
    
    VMAC_N(t00,tmp00,alpha0);
    VMAC_N(t01,tmp01,alpha0);
    VMAC_N(t02,tmp02,alpha0);
    VMAC_N(t03,tmp03,alpha0);
    
  }

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t01);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t02);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t03);


}

__STATIC_INLINE void EXT(kernel_1rx3cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01,tmp02;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += LANE;
  VECACC t01;
  VLOAD_AND_WIDEN(t01,tmpld,p);
  p += LANE;
  VECACC t02;
  VLOAD_AND_WIDEN(t02,tmpld,p);
  p += cols - 2*LANE;

 
  const DTYPE *pAlpha0 = &packedA[xp * r];


  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    tmp01 = VLOAD(pB0);
    pB0 += LANE;

    tmp02 = VLOAD(pB0);
    pB0 += LANE;

    
    VMAC_N(t00,tmp00,alpha0);


    VMAC_N(t01,tmp01,alpha0);
   

    VMAC_N(t02,tmp02,alpha0);
   

    

  }

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t01);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t02);
  pC += cols - 2*LANE;


}

__STATIC_INLINE void EXT(kernel_1rx2cv)(int cols,DTYPE *pC,int xp,int r) {

  VEC tmp00,tmp01;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);
  p += LANE;
  VECACC t01;
  VLOAD_AND_WIDEN(t01,tmpld,p);
  p += cols - LANE;

 
  const DTYPE *pAlpha0 = &packedA[xp * r];


  for (int k=0; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    tmp01 = VLOAD(pB0);
    pB0 += LANE;

    
    
    VMAC_N(t00,tmp00,alpha0);


    VMAC_N(t01,tmp01,alpha0);
   

   

    

  }

  VSTORE_AND_NARROW(pC,htmp,t00);
  pC += LANE;
  VSTORE_AND_NARROW(pC,htmp,t01);


}

__STATIC_INLINE void EXT(kernel_1rx1cv)(int cols,DTYPE *pC,int xp,int r) {

  (void)cols;
  VEC tmp00,tmp01;
  #if defined(USE_TMP_REGISTER)
  VEC tmpld;
  HVEC htmp;
  #endif

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
   
  const DTYPE *p = pC;

  VECACC t00;
  VLOAD_AND_WIDEN(t00,tmpld,p);

 
  const DTYPE *pAlpha0 = &packedA[xp * r];


  int k=0;
  for (; k <= (r-2) ; k += 2) 
  {
    const DTYPE alpha00 = *pAlpha0++;
    const DTYPE alpha01 = *pAlpha0++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;
    tmp01 = VLOAD(pB0);
    pB0 += LANE;

    VMAC_N(t00,tmp00,alpha00);
    VMAC_N(t00,tmp01,alpha01);
  }

  for (; k < r ; k ++) 
  {
    const DTYPE alpha0 = *pAlpha0++;

    tmp00 = VLOAD(pB0);
    pB0 += LANE;

    VMAC_N(t00,tmp00,alpha0);
  }

  VSTORE_AND_NARROW(pC,htmp,t00);

}

__STATIC_INLINE void EXT(kernel_4rxc)(int cols,DTYPE *pC,int xp,int r,int nbc) {

  
  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  DTYPE *p0 = pC;
  DTYPE *p1 = p0+cols;
  DTYPE *p2 = p1+cols;
  DTYPE *p3 = p2+cols;


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
    SCALAR_LOAD_AND_WIDEN(v0[i],p0+i);
    SCALAR_LOAD_AND_WIDEN(v1[i],p1+i);
    SCALAR_LOAD_AND_WIDEN(v2[i],p2+i);
    SCALAR_LOAD_AND_WIDEN(v3[i],p3+i);
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
     SCALAR_STORE_AND_NARROW(p0+i,v0[i]);
     SCALAR_STORE_AND_NARROW(p1+i,v1[i]);
     SCALAR_STORE_AND_NARROW(p2+i,v2[i]);
     SCALAR_STORE_AND_NARROW(p3+i,v3[i]);
  }
}

__STATIC_INLINE void EXT(kernel_3rxc)(int cols,DTYPE *pC,int xp,int r,int nbc) {


  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  DTYPE *p0 = pC;
  DTYPE *p1 = p0+cols;
  DTYPE *p2 = p1+cols;


  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;

  
  SCALARACC v0[LANE-1];
  SCALARACC v1[LANE-1];
  SCALARACC v2[LANE-1];

  for(int i=0;i<nbc;i++)
  {
    SCALAR_LOAD_AND_WIDEN(v0[i],p0+i);
    SCALAR_LOAD_AND_WIDEN(v1[i],p1+i);
    SCALAR_LOAD_AND_WIDEN(v2[i],p2+i);
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
    SCALAR_STORE_AND_NARROW(p0+i,v0[i]);
    SCALAR_STORE_AND_NARROW(p1+i,v1[i]);
    SCALAR_STORE_AND_NARROW(p2+i,v2[i]);
  }

}

__STATIC_INLINE void EXT(kernel_2rxc)(int cols,DTYPE *pC,int xp,int r,int nbc) {


  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  DTYPE *p0 = pC;
  DTYPE *p1 = p0+cols;


  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;

  
  SCALARACC v0[LANE-1];
  SCALARACC v1[LANE-1];

  for(int i=0;i<nbc;i++)
  {
    SCALAR_LOAD_AND_WIDEN(v0[i],p0+i);
    SCALAR_LOAD_AND_WIDEN(v1[i],p1+i);
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
    SCALAR_STORE_AND_NARROW(p0+i,v0[i]);
    SCALAR_STORE_AND_NARROW(p1+i,v1[i]);
  }

}

__STATIC_INLINE void EXT(kernel_1rxc)(int cols,DTYPE *pC,int xp,int r,int nbc) {
  (void)cols;


  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  DTYPE *p0 = pC;


  const DTYPE *pAlpha0 = &packedA[xp * r];

 
  SCALARACC v0[LANE-1];

  for(int i=0;i<nbc;i++)
  {
    SCALAR_LOAD_AND_WIDEN(v0[i],p0+i);
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
    SCALAR_STORE_AND_NARROW(p0+i,v0[i]);
  }
  
}

#define PACK(BUF,PACKED,WIDTH,HEIGHT,RB,CB,ROW,COL)     \
{                                                       \
    const DTYPE *p = &(BUF)[(COL)+(ROW)*(WIDTH)];       \
    DTYPE *packed=(DTYPE*)(PACKED);                     \
    const int nb_rows = MIN((HEIGHT),(ROW)+(RB))-(ROW); \
    const int nb_cols = MIN((WIDTH),(COL)+(CB)) - (COL);\
    for(int i=0;i<nb_rows;i++)                          \
    {                                                   \
        int j=0;                                        \
        for(;j<=(nb_cols-LANE);j+=LANE)                 \
        {                                               \
            VEC tmp = VLOAD(p);                     \
            p += LANE;                                  \
            VSTORE(packed,tmp);                      \
            packed += LANE;                             \
        }                                               \
        for(;j<nb_cols;j++)                             \
        {                                               \
            *packed++ = *p++;                           \
        }                                               \
        packed += (CB) - nb_cols;                       \
        p += WIDTH - nb_cols;                           \
    }                                                   \
}


ARM_DSP_ATTRIBUTE arm_status EXT(arm_mat_mult) (
#if defined(HAS_TEMP_BUFFER)
  const MATTYPE * pSrcA,
  const MATTYPE * pSrcB,
  MATTYPE * pDst,
  DTYPE   * pState)
#else
  const MATTYPE * pSrcA,
  const MATTYPE * pSrcB,
  MATTYPE * pDst)
#endif
{
  #if defined(HAS_TEMP_BUFFER)
  (void)pState;
  #endif
  const DTYPE *a = pSrcA->pData;
  const DTYPE *b = pSrcB->pData;
  DTYPE *c = pDst->pData;

  int rows,cols,inners;
  rows=pSrcA->numRows;
  inners=pSrcA->numCols;
  cols=pSrcB->numCols;

   memset(c,0,sizeof(DTYPE)*cols*rows);

    for (int block_col = 0; block_col < cols; block_col += COLS_BLOCK)
    { 
      for (int block_row = 0; block_row < rows; block_row += ROWS_BLOCK) 
      {    
        for (int block_inner = 0; block_inner < inners; block_inner += INNER_BLOCK)
        {
          int max_inner = MIN(INNER_BLOCK, inners-block_inner);
          int max_rows = MIN(0 + ROWS_BLOCK, rows - block_row);
          PACK(a,PACKEDA,inners,rows,max_rows,max_inner,block_row,block_inner);

          int col=0;

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-(4*LANE)); col += (4*LANE))
          {
            int row=0;
            PACK(b,PACKEDB,cols,inners,max_inner,(4*LANE),block_inner,block_col+col);

            for (; row <= (max_rows-DR); row += DR)
            {
              EXT(kernel_4rx4cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }

            int remaining = max_rows - row;

            //for (; row <= (max_rows-HDR); row += HDR)
            if (remaining == 3)
            {
              EXT(kernel_3rx4cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
            else if (remaining == 2)
            {
              EXT(kernel_2rx4cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
            else if (remaining == 1)
            //(; row < max_rows; row ++)
            {
              EXT(kernel_1rx4cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            } // end remaining row
          } // end block col


          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-(3*LANE)); col += (3*LANE))
          {
             int row=0;
             PACK(b,PACKEDB,cols,inners,max_inner,(3*LANE),block_inner,block_col+col);

             for (; row <= (max_rows-DR); row += DR)
             {
               EXT(kernel_4rx3cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
             }

             int remaining = max_rows - row;

             if (remaining == 3)
             {
               EXT(kernel_3rx3cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
             }
             else if (remaining == 2)
             {
               EXT(kernel_2rx3cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
             }
             else if (remaining == 1)
             {
               EXT(kernel_1rx3cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
             }
          }

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-(2*LANE)); col += (2*LANE))
          {
            int row=0;
            PACK(b,PACKEDB,cols,inners,max_inner,(2*LANE),block_inner,block_col+col);

            for (; row <= (max_rows-DR); row += DR)
            {
              EXT(kernel_4rx2cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }

            int remaining = max_rows - row;

            if (remaining == 3)
            {
              EXT(kernel_3rx2cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
            else if (remaining == 2)
            {
              EXT(kernel_2rx2cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
            else if (remaining == 1)
            {
              EXT(kernel_1rx2cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
          }

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-(1*LANE)); col += (1*LANE))
          {
            int row=0;
            PACK(b,PACKEDB,cols,inners,max_inner,(1*LANE),block_inner,block_col+col);

            for (; row <= (max_rows-DR); row += DR)
            {
              EXT(kernel_4rx1cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }

            int remaining = max_rows - row;

            if (remaining == 3)
            {
              EXT(kernel_3rx1cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
            else if (remaining == 2)
            {
              EXT(kernel_2rx1cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
            else if (remaining == 1)
            {
              EXT(kernel_1rx1cv)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
          }

          int maxnc = (MIN(0 + COLS_BLOCK, cols - block_col)) - col;
          if (maxnc > 0)
          {
             int row=0;
             PACK(b,PACKEDB,cols,inners,max_inner,maxnc,block_inner,block_col+col);

             for (; row <= (max_rows-DR); row += DR)
             {
               EXT(kernel_4rxc)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner,maxnc);
             }

             int remaining = max_rows - row;

             if (remaining == 3)
             {
               EXT(kernel_3rxc)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner,maxnc);
             }
             else if (remaining == 2)
             {
               EXT(kernel_2rxc)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner,maxnc);
             }
             else if (remaining == 1)
             {
              EXT(kernel_1rxc)(cols,c+(block_row+row)*cols + block_col+col,row,max_inner,maxnc);
             }
          }

         
       } // end inner loop
     } // end row loop
   } // end col loop

   return (ARM_MATH_SUCCESS);
}


#undef LANE 
#undef DTYPE 
#undef VEC 
#undef VECACC
#undef HVEC 
#undef VLOAD
#undef VSTORE
#undef VMAC_N
#undef MATTYPE 
#undef EXT
#undef SCALARACC
#undef VLOAD_AND_WIDEN
#undef VSTORE_AND_NARROW
#undef SCALAR_LOAD_AND_WIDEN
#undef SCALAR_STORE_AND_NARROW
#undef SCALAR_MAC_N
#if !defined(USE_TMP_REGISTER)
#undef tmpld
#undef htmp
#endif

#if defined(HAS_TEMP_BUFFER)
#undef HAS_TEMP_BUFFER
#endif
#if defined(USE_TMP_REGISTER)
#undef USE_TMP_REGISTER
#endif



#undef DC 
#undef DR 
#undef HDR 
#undef DCS 


#undef INNER_BLOCK
#undef ROWS_BLOCK
#undef COLS_BLOCK 


