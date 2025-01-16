#include <stdio.h>

#define DR 4
#define HDR 2
#define DCS 4

#define LANE 4
#define DC (LANE*DCS) // Must be multiple of lane
#define vec float32x4_t
#define hvec float32x2_t


// INNER * DC < L1/2
// INNER * ROWS < L2 
// INNER * COLS < L3
#define INNER_BLOCK ((((ARM_MATH_L1_CACHE_SIZE>>2) >> 1)/DC+DC-1) & ~(DC-1))
#define ROWS_BLOCK ((((ARM_MATH_L2_CACHE_SIZE>>2) >> 0) / INNER_BLOCK + DR - 1) & ~(DR-1) ) 
#define COLS_BLOCK (((ARM_MATH_L3_CACHE_SIZE>>2) / INNER_BLOCK + DC - 1) & ~(DC-1)) 


static __ALIGNED(16) float PACKEDB[INNER_BLOCK*DC];
static __ALIGNED(16) float PACKEDA[ROWS_BLOCK*INNER_BLOCK];

__STATIC_INLINE void kernel_4x16(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01,tmp02,tmp03;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += LANE;
  vec t02 = vld1q_f32(p);
  p += LANE;
  vec t03 = vld1q_f32(p);
  p += cols - 3*LANE;

  vec t10 = vld1q_f32(p);
  p += LANE;
  vec t11 = vld1q_f32(p);
  p += LANE;
  vec t12 = vld1q_f32(p);
  p += LANE;
  vec t13 = vld1q_f32(p);
  p += cols - 3*LANE;

  vec t20 = vld1q_f32(p);
  p += LANE;
  vec t21 = vld1q_f32(p);
  p += LANE;
  vec t22 = vld1q_f32(p);
  p += LANE;
  vec t23 = vld1q_f32(p);
  p += cols - 3*LANE;

  vec t30 = vld1q_f32(p);
  p += LANE;
  vec t31 = vld1q_f32(p);
  p += LANE;
  vec t32 = vld1q_f32(p);
  p += LANE;
  vec t33 = vld1q_f32(p);
  p += cols - 3*LANE;

  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;
  const float *pAlpha2 = pAlpha1 + r;
  const float *pAlpha3 = pAlpha2 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;
    const float alpha2 = *pAlpha2++;
    const float alpha3 = *pAlpha3++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp02 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp03 = vld1q_f32(pB0);
    pB0 += LANE;
    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);
    t20 = vfmaq_n_f32(t20,tmp00,alpha2);
    t30 = vfmaq_n_f32(t30,tmp00,alpha3);


    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
    t11 = vfmaq_n_f32(t11,tmp01,alpha1);
    t21 = vfmaq_n_f32(t21,tmp01,alpha2);
    t31 = vfmaq_n_f32(t31,tmp01,alpha3);


    t02 = vfmaq_n_f32(t02,tmp02,alpha0);
    t12 = vfmaq_n_f32(t12,tmp02,alpha1);
    t22 = vfmaq_n_f32(t22,tmp02,alpha2);
    t32 = vfmaq_n_f32(t32,tmp02,alpha3);


    t03 = vfmaq_n_f32(t03,tmp03,alpha0);
    t13 = vfmaq_n_f32(t13,tmp03,alpha1);
    t23 = vfmaq_n_f32(t23,tmp03,alpha2);
    t33 = vfmaq_n_f32(t33,tmp03,alpha3);


  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);
  pC += LANE;
  vst1q_f32(pC,t02);
  pC += LANE;
  vst1q_f32(pC,t03);
  pC += cols - 3*LANE;

  vst1q_f32(pC,t10);
  pC += LANE;
  vst1q_f32(pC,t11);
  pC += LANE;
  vst1q_f32(pC,t12);
  pC += LANE;
  vst1q_f32(pC,t13);
  pC += cols - 3*LANE;

  vst1q_f32(pC,t20);
  pC += LANE;
  vst1q_f32(pC,t21);
  pC += LANE;
  vst1q_f32(pC,t22);
  pC += LANE;
  vst1q_f32(pC,t23);
  pC += cols - 3*LANE;

  vst1q_f32(pC,t30);
  pC += LANE;
  vst1q_f32(pC,t31);
  pC += LANE;
  vst1q_f32(pC,t32);
  pC += LANE;
  vst1q_f32(pC,t33);
  pC += cols - 3*LANE;
}

__STATIC_INLINE void kernel_4x12(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01,tmp02;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += LANE;
  vec t02 = vld1q_f32(p);
  p += cols - 2*LANE;

  vec t10 = vld1q_f32(p);
  p += LANE;
  vec t11 = vld1q_f32(p);
  p += LANE;
  vec t12 = vld1q_f32(p);
  p += cols - 2*LANE;

  vec t20 = vld1q_f32(p);
  p += LANE;
  vec t21 = vld1q_f32(p);
  p += LANE;
  vec t22 = vld1q_f32(p);
  p += cols - 2*LANE;

  vec t30 = vld1q_f32(p);
  p += LANE;
  vec t31 = vld1q_f32(p);
  p += LANE;
  vec t32 = vld1q_f32(p);
  p += cols - 2*LANE;

  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;
  const float *pAlpha2 = pAlpha1 + r;
  const float *pAlpha3 = pAlpha2 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;
    const float alpha2 = *pAlpha2++;
    const float alpha3 = *pAlpha3++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp02 = vld1q_f32(pB0);
    pB0 += LANE;
    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);
    t20 = vfmaq_n_f32(t20,tmp00,alpha2);
    t30 = vfmaq_n_f32(t30,tmp00,alpha3);


    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
    t11 = vfmaq_n_f32(t11,tmp01,alpha1);
    t21 = vfmaq_n_f32(t21,tmp01,alpha2);
    t31 = vfmaq_n_f32(t31,tmp01,alpha3);


    t02 = vfmaq_n_f32(t02,tmp02,alpha0);
    t12 = vfmaq_n_f32(t12,tmp02,alpha1);
    t22 = vfmaq_n_f32(t22,tmp02,alpha2);
    t32 = vfmaq_n_f32(t32,tmp02,alpha3);


  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);
  pC += LANE;
  vst1q_f32(pC,t02);
  pC += cols - 2*LANE;

  vst1q_f32(pC,t10);
  pC += LANE;
  vst1q_f32(pC,t11);
  pC += LANE;
  vst1q_f32(pC,t12);
  pC += cols - 2*LANE;

  vst1q_f32(pC,t20);
  pC += LANE;
  vst1q_f32(pC,t21);
  pC += LANE;
  vst1q_f32(pC,t22);
  pC += cols - 2*LANE;

  vst1q_f32(pC,t30);
  pC += LANE;
  vst1q_f32(pC,t31);
  pC += LANE;
  vst1q_f32(pC,t32);
  pC += cols - 2*LANE;

}

__STATIC_INLINE void kernel_4x8(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += cols - LANE;

  vec t10 = vld1q_f32(p);
  p += LANE;
  vec t11 = vld1q_f32(p);
  p += cols - LANE;

  vec t20 = vld1q_f32(p);
  p += LANE;
  vec t21 = vld1q_f32(p);
  p += cols - LANE;

  vec t30 = vld1q_f32(p);
  p += LANE;
  vec t31 = vld1q_f32(p);
  p += cols - LANE;

  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;
  const float *pAlpha2 = pAlpha1 + r;
  const float *pAlpha3 = pAlpha2 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;
    const float alpha2 = *pAlpha2++;
    const float alpha3 = *pAlpha3++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);
    t20 = vfmaq_n_f32(t20,tmp00,alpha2);
    t30 = vfmaq_n_f32(t30,tmp00,alpha3);


    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
    t11 = vfmaq_n_f32(t11,tmp01,alpha1);
    t21 = vfmaq_n_f32(t21,tmp01,alpha2);
    t31 = vfmaq_n_f32(t31,tmp01,alpha3);



  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);
  pC += cols - LANE;

  vst1q_f32(pC,t10);
  pC += LANE;
  vst1q_f32(pC,t11);
  pC += cols - LANE;

  vst1q_f32(pC,t20);
  pC += LANE;
  vst1q_f32(pC,t21);
  pC += cols - LANE;

  vst1q_f32(pC,t30);
  pC += LANE;
  vst1q_f32(pC,t31);
  pC += cols - LANE;

}

__STATIC_INLINE void kernel_4x4(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  vec t10 = vld1q_f32(p);
  vec t20 = vld1q_f32(p);
  vec t30 = vld1q_f32(p);
 

  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;
  const float *pAlpha2 = pAlpha1 + r;
  const float *pAlpha3 = pAlpha2 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;
    const float alpha2 = *pAlpha2++;
    const float alpha3 = *pAlpha3++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);
    t20 = vfmaq_n_f32(t20,tmp00,alpha2);
    t30 = vfmaq_n_f32(t30,tmp00,alpha3);



  }

  vst1q_f32(pC,t00);
  pC += cols;
  vst1q_f32(pC,t10);
  pC += cols;
  vst1q_f32(pC,t20);
  pC += cols;
  vst1q_f32(pC,t30);
 

}

__STATIC_INLINE void kernel_3x16(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01,tmp02,tmp03;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += LANE;
  vec t02 = vld1q_f32(p);
  p += LANE;
  vec t03 = vld1q_f32(p);
  p += cols - 3*LANE;

  vec t10 = vld1q_f32(p);
  p += LANE;
  vec t11 = vld1q_f32(p);
  p += LANE;
  vec t12 = vld1q_f32(p);
  p += LANE;
  vec t13 = vld1q_f32(p);
  p += cols - 3*LANE;

  vec t20 = vld1q_f32(p);
  p += LANE;
  vec t21 = vld1q_f32(p);
  p += LANE;
  vec t22 = vld1q_f32(p);
  p += LANE;
  vec t23 = vld1q_f32(p);
  p += cols - 3*LANE;

  

  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;
  const float *pAlpha2 = pAlpha1 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;
    const float alpha2 = *pAlpha2++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp02 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp03 = vld1q_f32(pB0);
    pB0 += LANE;
    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);
    t20 = vfmaq_n_f32(t20,tmp00,alpha2);


    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
    t11 = vfmaq_n_f32(t11,tmp01,alpha1);
    t21 = vfmaq_n_f32(t21,tmp01,alpha2);


    t02 = vfmaq_n_f32(t02,tmp02,alpha0);
    t12 = vfmaq_n_f32(t12,tmp02,alpha1);
    t22 = vfmaq_n_f32(t22,tmp02,alpha2);


    t03 = vfmaq_n_f32(t03,tmp03,alpha0);
    t13 = vfmaq_n_f32(t13,tmp03,alpha1);
    t23 = vfmaq_n_f32(t23,tmp03,alpha2);


  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);
  pC += LANE;
  vst1q_f32(pC,t02);
  pC += LANE;
  vst1q_f32(pC,t03);
  pC += cols - 3*LANE;

  vst1q_f32(pC,t10);
  pC += LANE;
  vst1q_f32(pC,t11);
  pC += LANE;
  vst1q_f32(pC,t12);
  pC += LANE;
  vst1q_f32(pC,t13);
  pC += cols - 3*LANE;

  vst1q_f32(pC,t20);
  pC += LANE;
  vst1q_f32(pC,t21);
  pC += LANE;
  vst1q_f32(pC,t22);
  pC += LANE;
  vst1q_f32(pC,t23);
  pC += cols - 3*LANE;



}

__STATIC_INLINE void kernel_3x12(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01,tmp02;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += LANE;
  vec t02 = vld1q_f32(p);
  p += cols - 2*LANE;

  vec t10 = vld1q_f32(p);
  p += LANE;
  vec t11 = vld1q_f32(p);
  p += LANE;
  vec t12 = vld1q_f32(p);
  p += cols - 2*LANE;

  vec t20 = vld1q_f32(p);
  p += LANE;
  vec t21 = vld1q_f32(p);
  p += LANE;
  vec t22 = vld1q_f32(p);
  p += cols - 2*LANE;

  

  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;
  const float *pAlpha2 = pAlpha1 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;
    const float alpha2 = *pAlpha2++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp02 = vld1q_f32(pB0);
    pB0 += LANE;

  
    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);
    t20 = vfmaq_n_f32(t20,tmp00,alpha2);


    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
    t11 = vfmaq_n_f32(t11,tmp01,alpha1);
    t21 = vfmaq_n_f32(t21,tmp01,alpha2);


    t02 = vfmaq_n_f32(t02,tmp02,alpha0);
    t12 = vfmaq_n_f32(t12,tmp02,alpha1);
    t22 = vfmaq_n_f32(t22,tmp02,alpha2);



  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);
  pC += LANE;
  vst1q_f32(pC,t02);
  pC += cols - 2*LANE;

  vst1q_f32(pC,t10);
  pC += LANE;
  vst1q_f32(pC,t11);
  pC += LANE;
  vst1q_f32(pC,t12);
  pC += cols - 2*LANE;

  vst1q_f32(pC,t20);
  pC += LANE;
  vst1q_f32(pC,t21);
  pC += LANE;
  vst1q_f32(pC,t22);
  pC += cols - 2*LANE;



}

__STATIC_INLINE void kernel_3x8(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += cols - LANE;

  vec t10 = vld1q_f32(p);
  p += LANE;
  vec t11 = vld1q_f32(p);
  p += cols - LANE;

  vec t20 = vld1q_f32(p);
  p += LANE;
  vec t21 = vld1q_f32(p);
  p += cols - LANE;

  

  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;
  const float *pAlpha2 = pAlpha1 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;
    const float alpha2 = *pAlpha2++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

  
    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);
    t20 = vfmaq_n_f32(t20,tmp00,alpha2);


    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
    t11 = vfmaq_n_f32(t11,tmp01,alpha1);
    t21 = vfmaq_n_f32(t21,tmp01,alpha2);


 

  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);
  pC += cols - LANE;

  vst1q_f32(pC,t10);
  pC += LANE;
  vst1q_f32(pC,t11);
  pC += cols - LANE;

  vst1q_f32(pC,t20);
  pC += LANE;
  vst1q_f32(pC,t21);
  pC += cols - LANE;



}

__STATIC_INLINE void kernel_3x4(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += cols ;

  vec t10 = vld1q_f32(p);
  p += cols ;

  vec t20 = vld1q_f32(p);
  p += cols;

  

  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;
  const float *pAlpha2 = pAlpha1 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;
    const float alpha2 = *pAlpha2++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

   
  
    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);
    t20 = vfmaq_n_f32(t20,tmp00,alpha2);


  
 

  }

  vst1q_f32(pC,t00);
  pC += cols ;

  vst1q_f32(pC,t10);
  pC += cols ;

  vst1q_f32(pC,t20);
  pC += cols ;



}

__STATIC_INLINE void kernel_2x16(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01,tmp02,tmp03;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += LANE;
  vec t02 = vld1q_f32(p);
  p += LANE;
  vec t03 = vld1q_f32(p);
  p += cols - 3*LANE;

  vec t10 = vld1q_f32(p);
  p += LANE;
  vec t11 = vld1q_f32(p);
  p += LANE;
  vec t12 = vld1q_f32(p);
  p += LANE;
  vec t13 = vld1q_f32(p);
  p += cols - 3*LANE;

 
  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp02 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp03 = vld1q_f32(pB0);
    pB0 += LANE;
    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);


    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
    t11 = vfmaq_n_f32(t11,tmp01,alpha1);
   

    t02 = vfmaq_n_f32(t02,tmp02,alpha0);
    t12 = vfmaq_n_f32(t12,tmp02,alpha1);
   

    t03 = vfmaq_n_f32(t03,tmp03,alpha0);
    t13 = vfmaq_n_f32(t13,tmp03,alpha1);
    

  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);
  pC += LANE;
  vst1q_f32(pC,t02);
  pC += LANE;
  vst1q_f32(pC,t03);
  pC += cols - 3*LANE;

  vst1q_f32(pC,t10);
  pC += LANE;
  vst1q_f32(pC,t11);
  pC += LANE;
  vst1q_f32(pC,t12);
  pC += LANE;
  vst1q_f32(pC,t13);
  pC += cols - 3*LANE;


}

__STATIC_INLINE void kernel_2x12(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01,tmp02;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += LANE;
  vec t02 = vld1q_f32(p);
  p += cols - 2*LANE;

  vec t10 = vld1q_f32(p);
  p += LANE;
  vec t11 = vld1q_f32(p);
  p += LANE;
  vec t12 = vld1q_f32(p);
  p += cols - 2*LANE;

 
  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp02 = vld1q_f32(pB0);
    pB0 += LANE;

    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);


    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
    t11 = vfmaq_n_f32(t11,tmp01,alpha1);
   

    t02 = vfmaq_n_f32(t02,tmp02,alpha0);
    t12 = vfmaq_n_f32(t12,tmp02,alpha1);
   


  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);
  pC += LANE;
  vst1q_f32(pC,t02);
  pC += cols - 2*LANE;

  vst1q_f32(pC,t10);
  pC += LANE;
  vst1q_f32(pC,t11);
  pC += LANE;
  vst1q_f32(pC,t12);
  pC += cols - 2*LANE;


}

__STATIC_INLINE void kernel_2x8(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += cols - LANE;

  vec t10 = vld1q_f32(p);
  p += LANE;
  vec t11 = vld1q_f32(p);
  p += cols - LANE;

 
  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);


    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
    t11 = vfmaq_n_f32(t11,tmp01,alpha1);
  


  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);
  pC += cols - LANE;

  vst1q_f32(pC,t10);
  pC += LANE;
  vst1q_f32(pC,t11);
  pC += cols - LANE;


}

__STATIC_INLINE void kernel_2x4(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  vec t10 = vld1q_f32(p);
 

 
  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;

  

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;
    const float alpha1 = *pAlpha1++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t10 = vfmaq_n_f32(t10,tmp00,alpha1);



  }

  vst1q_f32(pC,t00);
  pC += cols;

  vst1q_f32(pC,t10);
  pC += cols ;


}

__STATIC_INLINE void kernel_1x16(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01,tmp02,tmp03;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += LANE;
  vec t02 = vld1q_f32(p);
  p += LANE;
  vec t03 = vld1q_f32(p);

  const float *pAlpha0 = &PACKEDA[xp * r];

  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp02 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp03 = vld1q_f32(pB0);
    pB0 += LANE;
    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
    t02 = vfmaq_n_f32(t02,tmp02,alpha0);
    t03 = vfmaq_n_f32(t03,tmp03,alpha0);
    
  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);
  pC += LANE;
  vst1q_f32(pC,t02);
  pC += LANE;
  vst1q_f32(pC,t03);


}

__STATIC_INLINE void kernel_1x12(int cols,float32_t *pC,int xp,int r) {

  const float *pB0 = PACKEDB;
  vec tmp00,tmp01,tmp02;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += LANE;
  vec t02 = vld1q_f32(p);
  p += cols - 2*LANE;

 
  const float *pAlpha0 = &PACKEDA[xp * r];


  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp02 = vld1q_f32(pB0);
    pB0 += LANE;

    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);


    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
   

    t02 = vfmaq_n_f32(t02,tmp02,alpha0);
   

    

  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);
  pC += LANE;
  vst1q_f32(pC,t02);
  pC += cols - 2*LANE;


}

__STATIC_INLINE void kernel_1x8(int cols,float32_t *pC,int xp,int r) {

  vec tmp00,tmp01;
  const float *pB0 = PACKEDB;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);
  p += LANE;
  vec t01 = vld1q_f32(p);
  p += cols - LANE;

 
  const float *pAlpha0 = &PACKEDA[xp * r];


  for (int k=0; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    
    
    t00 = vfmaq_n_f32(t00,tmp00,alpha0);


    t01 = vfmaq_n_f32(t01,tmp01,alpha0);
   

   

    

  }

  vst1q_f32(pC,t00);
  pC += LANE;
  vst1q_f32(pC,t01);


}

__STATIC_INLINE void kernel_1x4(int cols,float32_t *pC,int xp,int r) {

  vec tmp00,tmp01;
  const float *pB0 = PACKEDB;
   
  const float *p = pC;

  vec t00 = vld1q_f32(p);

 
  const float *pAlpha0 = &PACKEDA[xp * r];


  int k=0;
  for (; k <= (r-2) ; k += 2) 
  {
    const float alpha00 = *pAlpha0++;
    const float alpha01 = *pAlpha0++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;
    tmp01 = vld1q_f32(pB0);
    pB0 += LANE;

    t00 = vfmaq_n_f32(t00,tmp00,alpha00);
    t00 = vfmaq_n_f32(t00,tmp01,alpha01);
  }

  for (; k < r ; k ++) 
  {
    const float alpha0 = *pAlpha0++;

    tmp00 = vld1q_f32(pB0);
    pB0 += LANE;

    t00 = vfmaq_n_f32(t00,tmp00,alpha0);
  }

  vst1q_f32(pC,t00);

}

__STATIC_INLINE void kernel_4xc(int cols,float32_t *pC,int xp,int r,int nbc) {

  const float *pB0 = PACKEDB;
  float *p0 = pC;
  float *p1 = p0+cols;
  float *p2 = p1+cols;
  float *p3 = p2+cols;


  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;
  const float *pAlpha2 = pAlpha1 + r;
  const float *pAlpha3 = pAlpha2 + r;

  if (nbc == 1)
  {
      float v0;
      float v1;
      float v2;
      float v3;
         
      v0 = p0[0];
      v1 = p1[0];
      v2 = p2[0];
      v3 = p3[0];
      
      for (int k=0; k < r ; k ++) 
      {
        const float alpha0 = *pAlpha0++;
        const float alpha1 = *pAlpha1++;
        const float alpha2 = *pAlpha2++;
        const float alpha3 = *pAlpha3++;
        
        float b = *pB0++;
            
        v0 += alpha0 * b;
        v1 += alpha1 * b;
        v2 += alpha2 * b;
        v3 += alpha3 * b;
    
      }

      p0[0] = v0;
      p1[0] = v1;
      p2[0] = v2;
      p3[0] = v3;
      
  }
  else if (nbc == 2)
  {
      float v0[2];
      float v1[2];
      float v2[2];
      float v3[2];
      for(int i=0;i<2;i++)
      {
         v0[i] = p0[i];
         v1[i] = p1[i];
         v2[i] = p2[i];
         v3[i] = p3[i];
      }
    
      for (int k=0; k < r ; k ++) 
      {
        const float alpha0 = *pAlpha0++;
        const float alpha1 = *pAlpha1++;
        const float alpha2 = *pAlpha2++;
        const float alpha3 = *pAlpha3++;
        for (int c=0;c<2;c++)
        {
            float b = *pB0++;
            
            v0[c] += alpha0 * b;
            v1[c] += alpha1 * b;
            v2[c] += alpha2 * b;
            v3[c] += alpha3 * b;
        }
    
      }

      for(int i=0;i<2;i++)
      {
         p0[i] = v0[i];
         p1[i] = v1[i];
         p2[i] = v2[i];
         p3[i] = v3[i];
      }
  }
  else
  {
      float v0[3];
      float v1[3];
      float v2[3];
      float v3[3];
      for(int i=0;i<3;i++)
      {
         v0[i] = p0[i];
         v1[i] = p1[i];
         v2[i] = p2[i];
         v3[i] = p3[i];
      }
    
      for (int k=0; k < r ; k ++) 
      {
        const float alpha0 = *pAlpha0++;
        const float alpha1 = *pAlpha1++;
        const float alpha2 = *pAlpha2++;
        const float alpha3 = *pAlpha3++;
        for (int c=0;c<3;c++)
        {
            float b = *pB0++;
            
            v0[c] += alpha0 * b;
            v1[c] += alpha1 * b;
            v2[c] += alpha2 * b;
            v3[c] += alpha3 * b;
        }
    
      }

      for(int i=0;i<3;i++)
      {
         p0[i] = v0[i];
         p1[i] = v1[i];
         p2[i] = v2[i];
         p3[i] = v3[i];
      }
  }
}

__STATIC_INLINE void kernel_3xc(int cols,float32_t *pC,int xp,int r,int nbc) {

  const float *pB0 = PACKEDB;
  float *p0 = pC;
  float *p1 = p0+cols;
  float *p2 = p1+cols;


  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;
  const float *pAlpha2 = pAlpha1 + r;

  if (nbc == 1)
  {
      float v0;
      float v1;
      float v2;
    
      v0 = p0[0];
      v1 = p1[0];
      v2 = p2[0];
      
      for (int k=0; k < r ; k ++) 
      {
        const float alpha0 = *pAlpha0++;
        const float alpha1 = *pAlpha1++;
        const float alpha2 = *pAlpha2++;
        float b = *pB0++;
            
        v0 += alpha0 * b;
        v1 += alpha1 * b;
        v2 += alpha2 * b;
    
      }
    
      p0[0] = v0;
      p1[0] = v1;
      p2[0] = v2;
  }
  else if (nbc == 2)
  {
      float v0[2];
      float v1[2];
      float v2[2];
    
      for(int i=0;i<2;i++)
      {
         v0[i] = p0[i];
         v1[i] = p1[i];
         v2[i] = p2[i];
      }
      
      for (int k=0; k < r ; k ++) 
      {
        const float alpha0 = *pAlpha0++;
        const float alpha1 = *pAlpha1++;
        const float alpha2 = *pAlpha2++;
        for (int c=0;c<2;c++)
        {
            float b = *pB0++;
            
            v0[c] += alpha0 * b;
            v1[c] += alpha1 * b;
            v2[c] += alpha2 * b;
        }
    
      }
    
      for(int i=0;i<2;i++)
      {
         p0[i] = v0[i];
         p1[i] = v1[i];
         p2[i] = v2[i];
      }
  }
  else
  {
      float v0[3];
      float v1[3];
      float v2[3];
    
      for(int i=0;i<3;i++)
      {
         v0[i] = p0[i];
         v1[i] = p1[i];
         v2[i] = p2[i];
      }
      
      for (int k=0; k < r ; k ++) 
      {
        const float alpha0 = *pAlpha0++;
        const float alpha1 = *pAlpha1++;
        const float alpha2 = *pAlpha2++;
        for (int c=0;c<3;c++)
        {
            float b = *pB0++;
            
            v0[c] += alpha0 * b;
            v1[c] += alpha1 * b;
            v2[c] += alpha2 * b;
        }
    
      }
    
      for(int i=0;i<3;i++)
      {
         p0[i] = v0[i];
         p1[i] = v1[i];
         p2[i] = v2[i];
      }
  }


 
}

__STATIC_INLINE void kernel_2xc(int cols,float32_t *pC,int xp,int r,int nbc) {

  const float *pB0 = PACKEDB;
  float *p0 = pC;
  float *p1 = p0+cols;


  const float *pAlpha0 = &PACKEDA[xp * r];
  const float *pAlpha1 = pAlpha0 + r;

  if (nbc == 1)
  {
      float v0;
      float v1;
    
      v0 = p0[0];
      v1 = p1[0];
      
      for (int k=0; k < r ; k ++) 
      {
        const float alpha0 = *pAlpha0++;
        const float alpha1 = *pAlpha1++;
    
        float b = *pB0++;
            
        v0 += alpha0 * b;
        v1 += alpha1 * b;
    
      }
    
      p0[0] = v0;
      p1[0] = v1;
  }
  else if (nbc == 2)
  {
      float v0[2];
      float v1[2];
    
      for(int i=0;i<2;i++)
      {
         v0[i] = p0[i];
         v1[i] = p1[i];
      }
      
      
      for (int k=0; k < r ; k ++) 
      {
        const float alpha0 = *pAlpha0++;
        const float alpha1 = *pAlpha1++;
    
        for (int c=0;c<2;c++)
        {
            float b = *pB0++;
            
            v0[c] += alpha0 * b;
            v1[c] += alpha1 * b;
        }
    
      }
    
      for(int i=0;i<2;i++)
      {
         p0[i] = v0[i];
         p1[i] = v1[i];
      }
  }
  else
  {
      float v0[3];
      float v1[3];
    
      for(int i=0;i<3;i++)
      {
         v0[i] = p0[i];
         v1[i] = p1[i];
      }
      
      
      for (int k=0; k < r ; k ++) 
      {
        const float alpha0 = *pAlpha0++;
        const float alpha1 = *pAlpha1++;
    
        for (int c=0;c<3;c++)
        {
            float b = *pB0++;
            
            v0[c] += alpha0 * b;
            v1[c] += alpha1 * b;
        }
    
      }
    
      for(int i=0;i<3;i++)
      {
         p0[i] = v0[i];
         p1[i] = v1[i];
      }
  }


 
}

__STATIC_INLINE void kernel_1xc(int cols,float32_t *pC,int xp,int r,int nbc) {
  const float *pB0 = PACKEDB;
  float *p0 = pC;


  const float *pAlpha0 = &PACKEDA[xp * r];

  if (nbc == 1)
  {
       float v0;

       v0 = p0[0];
       
       for (int k=0; k < r ; k ++) 
       {
         const float alpha0 = *pAlpha0++;

         float b = *pB0++;
             
          v0 += alpha0 * b;

       }

       p0[0] = v0;
  }
  else if (nbc == 2)
  {
       float v0[2];

       for(int i=0;i<2;i++)
       {
          v0[i] = p0[i];
       }
       
       for (int k=0; k < r ; k ++) 
       {
         const float alpha0 = *pAlpha0++;

         for (int c=0;c<2;c++)
         {
             float b = *pB0++;
             
             v0[c] += alpha0 * b;
         }

       }

       for(int i=0;i<2;i++)
       {
          p0[i] = v0[i];
       }
  }
  else
  {
       float v0[3];

       for(int i=0;i<3;i++)
       {
          v0[i] = p0[i];
       }
       
       for (int k=0; k < r ; k ++) 
       {
         const float alpha0 = *pAlpha0++;

         for (int c=0;c<3;c++)
         {
             float b = *pB0++;
             
             v0[c] += alpha0 * b;
         }

       }

       for(int i=0;i<3;i++)
       {
          p0[i] = v0[i];
       }
  }


}

#define PACK(BUF,PACKED,WIDTH,HEIGHT,RB,CB,ROW,COL)     \
{                                                       \
    const float *p = &(BUF)[(COL)+(ROW)*(WIDTH)];       \
    float *packed=(PACKED);                             \
    const int nb_rows = MIN((HEIGHT),(ROW)+(RB))-(ROW); \
    const int nb_cols = MIN((WIDTH),(COL)+(CB)) - (COL);\
    for(int i=0;i<nb_rows;i++)                          \
    {                                                   \
        int j=0;                                        \
        for(;j<=(nb_cols-LANE);j+=LANE)                 \
        {                                               \
            vec tmp = vld1q_f32(p);                     \
            p += LANE;                                  \
            vst1q_f32(packed,tmp);                      \
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


ARM_DSP_ATTRIBUTE arm_status arm_mat_mult_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
  arm_matrix_instance_f32 * pDst)
{
  const float32_t *a = pSrcA->pData;
  const float32_t *b = pSrcB->pData;
  float32_t *c = pDst->pData;

  int rows,cols,inners;
  rows=pSrcA->numRows;
  inners=pSrcA->numCols;
  cols=pSrcB->numCols;

   memset(c,0,sizeof(float32_t)*cols*rows);

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

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-DC); col += DC)
          {
            int row=0;
            PACK(b,PACKEDB,cols,inners,max_inner,DC,block_inner,block_col+col);

            for (; row <= (max_rows-DR); row += DR)
            {
              kernel_4x16(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }

            int remaining = max_rows - row;

            //for (; row <= (max_rows-HDR); row += HDR)
            if (remaining == 3)
            {
              kernel_3x16(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
            else if (remaining == 2)
            {
              kernel_2x16(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
            else if (remaining == 1)
            //(; row < max_rows; row ++)
            {
              kernel_1x16(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            } // end remaining row
          } // end block col


          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-12); col += 12)
          {
             int row=0;
             PACK(b,PACKEDB,cols,inners,max_inner,12,block_inner,block_col+col);

             for (; row <= (max_rows-DR); row += DR)
             {
               kernel_4x12(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
             }

             int remaining = max_rows - row;

             if (remaining == 3)
             {
               kernel_3x12(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
             }
             else if (remaining == 2)
             {
               kernel_2x12(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
             }
             else if (remaining == 1)
             {
               kernel_1x12(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
             }
          }

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-8); col += 8)
          {
            int row=0;
            PACK(b,PACKEDB,cols,inners,max_inner,8,block_inner,block_col+col);

            for (; row <= (max_rows-DR); row += DR)
            {
              kernel_4x8(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }

            int remaining = max_rows - row;

            if (remaining == 3)
            {
              kernel_3x8(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
            else if (remaining == 2)
            {
              kernel_2x8(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
            else if (remaining == 1)
            {
              kernel_1x8(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
          }

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-4); col += 4)
          {
            int row=0;
            PACK(b,PACKEDB,cols,inners,max_inner,4,block_inner,block_col+col);

            for (; row <= (max_rows-DR); row += DR)
            {
              kernel_4x4(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }

            int remaining = max_rows - row;

            if (remaining == 3)
            {
              kernel_3x4(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
            else if (remaining == 2)
            {
              kernel_2x4(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
            else if (remaining == 1)
            {
              kernel_1x4(cols,c+(block_row+row)*cols + block_col+col,row,max_inner);
            }
          }

          int maxnc = (MIN(0 + COLS_BLOCK, cols - block_col)) - col;
          if (maxnc > 0)
          {
             int row=0;
             PACK(b,PACKEDB,cols,inners,max_inner,maxnc,block_inner,block_col+col);

             for (; row <= (max_rows-DR); row += DR)
             {
               kernel_4xc(cols,c+(block_row+row)*cols + block_col+col,row,max_inner,maxnc);
             }

             int remaining = max_rows - row;

             if (remaining == 3)
             {
               kernel_3xc(cols,c+(block_row+row)*cols + block_col+col,row,max_inner,maxnc);
             }
             else if (remaining == 2)
             {
               kernel_2xc(cols,c+(block_row+row)*cols + block_col+col,row,max_inner,maxnc);
             }
             else if (remaining == 1)
             {
              kernel_1xc(cols,c+(block_row+row)*cols + block_col+col,row,max_inner,maxnc);
             }
          }

         
       } // end inner loop
     } // end row loop
   } // end col loop

   return (ARM_MATH_SUCCESS);
}

#undef DR 
#undef DC
#undef LANE
#undef UNROLL
#undef DCS

