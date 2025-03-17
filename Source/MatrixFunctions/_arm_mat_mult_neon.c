



#define DC (LANE*DCS) // Must be multiple of lane
#define DR 4 // Max number rows in kernels (can't be changed)
#define HDR 2 // Max number rows by half
#define DCS 4 // Max number of vectors for columns (can't be changed)

// INNER * DC < L1/2
// INNER * ROWS < L2 
// INNER * COLS < L3
#define INNER_BLOCK ((((ARM_MATH_L1_CACHE_SIZE>>2) >> 1)/DC+DC-1) & ~(DC-1))
#define ROWS_BLOCK ((((ARM_MATH_L2_CACHE_SIZE>>2) >> 0) / INNER_BLOCK + DR - 1) & ~(DR-1) ) 
#define COLS_BLOCK (((ARM_MATH_L3_CACHE_SIZE>>2) / INNER_BLOCK + DC - 1) & ~(DC-1)) 


static __ALIGNED(16) char PACKEDB[4*INNER_BLOCK*DC];
static __ALIGNED(16) char PACKEDA[4*ROWS_BLOCK*INNER_BLOCK];


// 4 row and 4 column VECtors
__STATIC_INLINE void EXT(kernel_4rx4cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;

  VEC tmp00,tmp01,tmp02,tmp03;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += LANE;
  VEC t01 = VLOAD(p);
  p += LANE;
  VEC t02 = VLOAD(p);
  p += LANE;
  VEC t03 = VLOAD(p);
  p += cols - 3*LANE;

  VEC t10 = VLOAD(p);
  p += LANE;
  VEC t11 = VLOAD(p);
  p += LANE;
  VEC t12 = VLOAD(p);
  p += LANE;
  VEC t13 = VLOAD(p);
  p += cols - 3*LANE;

  VEC t20 = VLOAD(p);
  p += LANE;
  VEC t21 = VLOAD(p);
  p += LANE;
  VEC t22 = VLOAD(p);
  p += LANE;
  VEC t23 = VLOAD(p);
  p += cols - 3*LANE;

  VEC t30 = VLOAD(p);
  p += LANE;
  VEC t31 = VLOAD(p);
  p += LANE;
  VEC t32 = VLOAD(p);
  p += LANE;
  VEC t33 = VLOAD(p);
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

  VSTORE(pC,t00);
  pC += LANE;
  VSTORE(pC,t01);
  pC += LANE;
  VSTORE(pC,t02);
  pC += LANE;
  VSTORE(pC,t03);
  pC += cols - 3*LANE;

  VSTORE(pC,t10);
  pC += LANE;
  VSTORE(pC,t11);
  pC += LANE;
  VSTORE(pC,t12);
  pC += LANE;
  VSTORE(pC,t13);
  pC += cols - 3*LANE;

  VSTORE(pC,t20);
  pC += LANE;
  VSTORE(pC,t21);
  pC += LANE;
  VSTORE(pC,t22);
  pC += LANE;
  VSTORE(pC,t23);
  pC += cols - 3*LANE;

  VSTORE(pC,t30);
  pC += LANE;
  VSTORE(pC,t31);
  pC += LANE;
  VSTORE(pC,t32);
  pC += LANE;
  VSTORE(pC,t33);
  pC += cols - 3*LANE;
}

__STATIC_INLINE void EXT(kernel_4rx3cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;

  VEC tmp00,tmp01,tmp02;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += LANE;
  VEC t01 = VLOAD(p);
  p += LANE;
  VEC t02 = VLOAD(p);
  p += cols - 2*LANE;

  VEC t10 = VLOAD(p);
  p += LANE;
  VEC t11 = VLOAD(p);
  p += LANE;
  VEC t12 = VLOAD(p);
  p += cols - 2*LANE;

  VEC t20 = VLOAD(p);
  p += LANE;
  VEC t21 = VLOAD(p);
  p += LANE;
  VEC t22 = VLOAD(p);
  p += cols - 2*LANE;

  VEC t30 = VLOAD(p);
  p += LANE;
  VEC t31 = VLOAD(p);
  p += LANE;
  VEC t32 = VLOAD(p);
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

  VSTORE(pC,t00);
  pC += LANE;
  VSTORE(pC,t01);
  pC += LANE;
  VSTORE(pC,t02);
  pC += cols - 2*LANE;

  VSTORE(pC,t10);
  pC += LANE;
  VSTORE(pC,t11);
  pC += LANE;
  VSTORE(pC,t12);
  pC += cols - 2*LANE;

  VSTORE(pC,t20);
  pC += LANE;
  VSTORE(pC,t21);
  pC += LANE;
  VSTORE(pC,t22);
  pC += cols - 2*LANE;

  VSTORE(pC,t30);
  pC += LANE;
  VSTORE(pC,t31);
  pC += LANE;
  VSTORE(pC,t32);
  pC += cols - 2*LANE;

}

__STATIC_INLINE void EXT(kernel_4rx2cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += LANE;
  VEC t01 = VLOAD(p);
  p += cols - LANE;

  VEC t10 = VLOAD(p);
  p += LANE;
  VEC t11 = VLOAD(p);
  p += cols - LANE;

  VEC t20 = VLOAD(p);
  p += LANE;
  VEC t21 = VLOAD(p);
  p += cols - LANE;

  VEC t30 = VLOAD(p);
  p += LANE;
  VEC t31 = VLOAD(p);
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

  VSTORE(pC,t00);
  pC += LANE;
  VSTORE(pC,t01);
  pC += cols - LANE;

  VSTORE(pC,t10);
  pC += LANE;
  VSTORE(pC,t11);
  pC += cols - LANE;

  VSTORE(pC,t20);
  pC += LANE;
  VSTORE(pC,t21);
  pC += cols - LANE;

  VSTORE(pC,t30);
  pC += LANE;
  VSTORE(pC,t31);
  pC += cols - LANE;

}

__STATIC_INLINE void EXT(kernel_4rx1cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  VEC t10 = VLOAD(p);
  VEC t20 = VLOAD(p);
  VEC t30 = VLOAD(p);
 

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

  VSTORE(pC,t00);
  pC += cols;
  VSTORE(pC,t10);
  pC += cols;
  VSTORE(pC,t20);
  pC += cols;
  VSTORE(pC,t30);
 

}

__STATIC_INLINE void EXT(kernel_3rx4cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01,tmp02,tmp03;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += LANE;
  VEC t01 = VLOAD(p);
  p += LANE;
  VEC t02 = VLOAD(p);
  p += LANE;
  VEC t03 = VLOAD(p);
  p += cols - 3*LANE;

  VEC t10 = VLOAD(p);
  p += LANE;
  VEC t11 = VLOAD(p);
  p += LANE;
  VEC t12 = VLOAD(p);
  p += LANE;
  VEC t13 = VLOAD(p);
  p += cols - 3*LANE;

  VEC t20 = VLOAD(p);
  p += LANE;
  VEC t21 = VLOAD(p);
  p += LANE;
  VEC t22 = VLOAD(p);
  p += LANE;
  VEC t23 = VLOAD(p);
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

  VSTORE(pC,t00);
  pC += LANE;
  VSTORE(pC,t01);
  pC += LANE;
  VSTORE(pC,t02);
  pC += LANE;
  VSTORE(pC,t03);
  pC += cols - 3*LANE;

  VSTORE(pC,t10);
  pC += LANE;
  VSTORE(pC,t11);
  pC += LANE;
  VSTORE(pC,t12);
  pC += LANE;
  VSTORE(pC,t13);
  pC += cols - 3*LANE;

  VSTORE(pC,t20);
  pC += LANE;
  VSTORE(pC,t21);
  pC += LANE;
  VSTORE(pC,t22);
  pC += LANE;
  VSTORE(pC,t23);
  pC += cols - 3*LANE;



}

__STATIC_INLINE void EXT(kernel_3rx3cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01,tmp02;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += LANE;
  VEC t01 = VLOAD(p);
  p += LANE;
  VEC t02 = VLOAD(p);
  p += cols - 2*LANE;

  VEC t10 = VLOAD(p);
  p += LANE;
  VEC t11 = VLOAD(p);
  p += LANE;
  VEC t12 = VLOAD(p);
  p += cols - 2*LANE;

  VEC t20 = VLOAD(p);
  p += LANE;
  VEC t21 = VLOAD(p);
  p += LANE;
  VEC t22 = VLOAD(p);
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

  VSTORE(pC,t00);
  pC += LANE;
  VSTORE(pC,t01);
  pC += LANE;
  VSTORE(pC,t02);
  pC += cols - 2*LANE;

  VSTORE(pC,t10);
  pC += LANE;
  VSTORE(pC,t11);
  pC += LANE;
  VSTORE(pC,t12);
  pC += cols - 2*LANE;

  VSTORE(pC,t20);
  pC += LANE;
  VSTORE(pC,t21);
  pC += LANE;
  VSTORE(pC,t22);
  pC += cols - 2*LANE;



}

__STATIC_INLINE void EXT(kernel_3rx2cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += LANE;
  VEC t01 = VLOAD(p);
  p += cols - LANE;

  VEC t10 = VLOAD(p);
  p += LANE;
  VEC t11 = VLOAD(p);
  p += cols - LANE;

  VEC t20 = VLOAD(p);
  p += LANE;
  VEC t21 = VLOAD(p);
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

  VSTORE(pC,t00);
  pC += LANE;
  VSTORE(pC,t01);
  pC += cols - LANE;

  VSTORE(pC,t10);
  pC += LANE;
  VSTORE(pC,t11);
  pC += cols - LANE;

  VSTORE(pC,t20);
  pC += LANE;
  VSTORE(pC,t21);
  pC += cols - LANE;



}

__STATIC_INLINE void EXT(kernel_3rx1cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += cols ;

  VEC t10 = VLOAD(p);
  p += cols ;

  VEC t20 = VLOAD(p);
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

  VSTORE(pC,t00);
  pC += cols ;

  VSTORE(pC,t10);
  pC += cols ;

  VSTORE(pC,t20);
  pC += cols ;



}

__STATIC_INLINE void EXT(kernel_2rx4cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01,tmp02,tmp03;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += LANE;
  VEC t01 = VLOAD(p);
  p += LANE;
  VEC t02 = VLOAD(p);
  p += LANE;
  VEC t03 = VLOAD(p);
  p += cols - 3*LANE;

  VEC t10 = VLOAD(p);
  p += LANE;
  VEC t11 = VLOAD(p);
  p += LANE;
  VEC t12 = VLOAD(p);
  p += LANE;
  VEC t13 = VLOAD(p);
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

  VSTORE(pC,t00);
  pC += LANE;
  VSTORE(pC,t01);
  pC += LANE;
  VSTORE(pC,t02);
  pC += LANE;
  VSTORE(pC,t03);
  pC += cols - 3*LANE;

  VSTORE(pC,t10);
  pC += LANE;
  VSTORE(pC,t11);
  pC += LANE;
  VSTORE(pC,t12);
  pC += LANE;
  VSTORE(pC,t13);
  pC += cols - 3*LANE;


}

__STATIC_INLINE void EXT(kernel_2rx3cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01,tmp02;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += LANE;
  VEC t01 = VLOAD(p);
  p += LANE;
  VEC t02 = VLOAD(p);
  p += cols - 2*LANE;

  VEC t10 = VLOAD(p);
  p += LANE;
  VEC t11 = VLOAD(p);
  p += LANE;
  VEC t12 = VLOAD(p);
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

  VSTORE(pC,t00);
  pC += LANE;
  VSTORE(pC,t01);
  pC += LANE;
  VSTORE(pC,t02);
  pC += cols - 2*LANE;

  VSTORE(pC,t10);
  pC += LANE;
  VSTORE(pC,t11);
  pC += LANE;
  VSTORE(pC,t12);
  pC += cols - 2*LANE;


}

__STATIC_INLINE void EXT(kernel_2rx2cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += LANE;
  VEC t01 = VLOAD(p);
  p += cols - LANE;

  VEC t10 = VLOAD(p);
  p += LANE;
  VEC t11 = VLOAD(p);
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

  VSTORE(pC,t00);
  pC += LANE;
  VSTORE(pC,t01);
  pC += cols - LANE;

  VSTORE(pC,t10);
  pC += LANE;
  VSTORE(pC,t11);
  pC += cols - LANE;


}

__STATIC_INLINE void EXT(kernel_2rx1cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  VEC t10 = VLOAD(p);
 

 
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

  VSTORE(pC,t00);
  pC += cols;

  VSTORE(pC,t10);
  pC += cols ;


}

__STATIC_INLINE void EXT(kernel_1rx4cv)(int cols,DTYPE *pC,int xp,int r) {

  (void)cols;
  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01,tmp02,tmp03;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += LANE;
  VEC t01 = VLOAD(p);
  p += LANE;
  VEC t02 = VLOAD(p);
  p += LANE;
  VEC t03 = VLOAD(p);

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

  VSTORE(pC,t00);
  pC += LANE;
  VSTORE(pC,t01);
  pC += LANE;
  VSTORE(pC,t02);
  pC += LANE;
  VSTORE(pC,t03);


}

__STATIC_INLINE void EXT(kernel_1rx3cv)(int cols,DTYPE *pC,int xp,int r) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  VEC tmp00,tmp01,tmp02;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += LANE;
  VEC t01 = VLOAD(p);
  p += LANE;
  VEC t02 = VLOAD(p);
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

  VSTORE(pC,t00);
  pC += LANE;
  VSTORE(pC,t01);
  pC += LANE;
  VSTORE(pC,t02);
  pC += cols - 2*LANE;


}

__STATIC_INLINE void EXT(kernel_1rx2cv)(int cols,DTYPE *pC,int xp,int r) {

  VEC tmp00,tmp01;
  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);
  p += LANE;
  VEC t01 = VLOAD(p);
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

  VSTORE(pC,t00);
  pC += LANE;
  VSTORE(pC,t01);


}

__STATIC_INLINE void EXT(kernel_1rx1cv)(int cols,DTYPE *pC,int xp,int r) {

  (void)cols;
  VEC tmp00,tmp01;
  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
   
  const DTYPE *p = pC;

  VEC t00 = VLOAD(p);

 
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

  VSTORE(pC,t00);

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

  if (nbc == 1)
  {
      DTYPE v0;
      DTYPE v1;
      DTYPE v2;
      DTYPE v3;
         
      v0 = p0[0];
      v1 = p1[0];
      v2 = p2[0];
      v3 = p3[0];
      
      for (int k=0; k < r ; k ++) 
      {
        const DTYPE alpha0 = *pAlpha0++;
        const DTYPE alpha1 = *pAlpha1++;
        const DTYPE alpha2 = *pAlpha2++;
        const DTYPE alpha3 = *pAlpha3++;
        
        DTYPE b = *pB0++;
            
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
      DTYPE v0[2];
      DTYPE v1[2];
      DTYPE v2[2];
      DTYPE v3[2];
      for(int i=0;i<2;i++)
      {
         v0[i] = p0[i];
         v1[i] = p1[i];
         v2[i] = p2[i];
         v3[i] = p3[i];
      }
    
      for (int k=0; k < r ; k ++) 
      {
        const DTYPE alpha0 = *pAlpha0++;
        const DTYPE alpha1 = *pAlpha1++;
        const DTYPE alpha2 = *pAlpha2++;
        const DTYPE alpha3 = *pAlpha3++;
        for (int c=0;c<2;c++)
        {
            DTYPE b = *pB0++;
            
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
      DTYPE v0[3];
      DTYPE v1[3];
      DTYPE v2[3];
      DTYPE v3[3];
      for(int i=0;i<3;i++)
      {
         v0[i] = p0[i];
         v1[i] = p1[i];
         v2[i] = p2[i];
         v3[i] = p3[i];
      }
    
      for (int k=0; k < r ; k ++) 
      {
        const DTYPE alpha0 = *pAlpha0++;
        const DTYPE alpha1 = *pAlpha1++;
        const DTYPE alpha2 = *pAlpha2++;
        const DTYPE alpha3 = *pAlpha3++;
        for (int c=0;c<3;c++)
        {
            DTYPE b = *pB0++;
            
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

__STATIC_INLINE void EXT(kernel_3rxc)(int cols,DTYPE *pC,int xp,int r,int nbc) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  DTYPE *p0 = pC;
  DTYPE *p1 = p0+cols;
  DTYPE *p2 = p1+cols;


  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;
  const DTYPE *pAlpha2 = pAlpha1 + r;

  if (nbc == 1)
  {
      DTYPE v0;
      DTYPE v1;
      DTYPE v2;
    
      v0 = p0[0];
      v1 = p1[0];
      v2 = p2[0];
      
      for (int k=0; k < r ; k ++) 
      {
        const DTYPE alpha0 = *pAlpha0++;
        const DTYPE alpha1 = *pAlpha1++;
        const DTYPE alpha2 = *pAlpha2++;
        DTYPE b = *pB0++;
            
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
      DTYPE v0[2];
      DTYPE v1[2];
      DTYPE v2[2];
    
      for(int i=0;i<2;i++)
      {
         v0[i] = p0[i];
         v1[i] = p1[i];
         v2[i] = p2[i];
      }
      
      for (int k=0; k < r ; k ++) 
      {
        const DTYPE alpha0 = *pAlpha0++;
        const DTYPE alpha1 = *pAlpha1++;
        const DTYPE alpha2 = *pAlpha2++;
        for (int c=0;c<2;c++)
        {
            DTYPE b = *pB0++;
            
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
      DTYPE v0[3];
      DTYPE v1[3];
      DTYPE v2[3];
    
      for(int i=0;i<3;i++)
      {
         v0[i] = p0[i];
         v1[i] = p1[i];
         v2[i] = p2[i];
      }
      
      for (int k=0; k < r ; k ++) 
      {
        const DTYPE alpha0 = *pAlpha0++;
        const DTYPE alpha1 = *pAlpha1++;
        const DTYPE alpha2 = *pAlpha2++;
        for (int c=0;c<3;c++)
        {
            DTYPE b = *pB0++;
            
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

__STATIC_INLINE void EXT(kernel_2rxc)(int cols,DTYPE *pC,int xp,int r,int nbc) {

  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  DTYPE *p0 = pC;
  DTYPE *p1 = p0+cols;


  const DTYPE *pAlpha0 = &packedA[xp * r];
  const DTYPE *pAlpha1 = pAlpha0 + r;

  if (nbc == 1)
  {
      DTYPE v0;
      DTYPE v1;
    
      v0 = p0[0];
      v1 = p1[0];
      
      for (int k=0; k < r ; k ++) 
      {
        const DTYPE alpha0 = *pAlpha0++;
        const DTYPE alpha1 = *pAlpha1++;
    
        DTYPE b = *pB0++;
            
        v0 += alpha0 * b;
        v1 += alpha1 * b;
    
      }
    
      p0[0] = v0;
      p1[0] = v1;
  }
  else if (nbc == 2)
  {
      DTYPE v0[2];
      DTYPE v1[2];
    
      for(int i=0;i<2;i++)
      {
         v0[i] = p0[i];
         v1[i] = p1[i];
      }
      
      
      for (int k=0; k < r ; k ++) 
      {
        const DTYPE alpha0 = *pAlpha0++;
        const DTYPE alpha1 = *pAlpha1++;
    
        for (int c=0;c<2;c++)
        {
            DTYPE b = *pB0++;
            
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
      DTYPE v0[3];
      DTYPE v1[3];
    
      for(int i=0;i<3;i++)
      {
         v0[i] = p0[i];
         v1[i] = p1[i];
      }
      
      
      for (int k=0; k < r ; k ++) 
      {
        const DTYPE alpha0 = *pAlpha0++;
        const DTYPE alpha1 = *pAlpha1++;
    
        for (int c=0;c<3;c++)
        {
            DTYPE b = *pB0++;
            
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

__STATIC_INLINE void EXT(kernel_1rxc)(int cols,DTYPE *pC,int xp,int r,int nbc) {
  (void)cols;
  const DTYPE *pB0 = (DTYPE*)PACKEDB;
  const DTYPE *packedA = (DTYPE*)PACKEDA;
  DTYPE *p0 = pC;


  const DTYPE *pAlpha0 = &packedA[xp * r];

  if (nbc == 1)
  {
       DTYPE v0;

       v0 = p0[0];
       
       for (int k=0; k < r ; k ++) 
       {
         const DTYPE alpha0 = *pAlpha0++;

         DTYPE b = *pB0++;
             
          v0 += alpha0 * b;

       }

       p0[0] = v0;
  }
  else if (nbc == 2)
  {
       DTYPE v0[2];

       for(int i=0;i<2;i++)
       {
          v0[i] = p0[i];
       }
       
       for (int k=0; k < r ; k ++) 
       {
         const DTYPE alpha0 = *pAlpha0++;

         for (int c=0;c<2;c++)
         {
             DTYPE b = *pB0++;
             
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
       DTYPE v0[3];

       for(int i=0;i<3;i++)
       {
          v0[i] = p0[i];
       }
       
       for (int k=0; k < r ; k ++) 
       {
         const DTYPE alpha0 = *pAlpha0++;

         for (int c=0;c<3;c++)
         {
             DTYPE b = *pB0++;
             
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
  const MATTYPE * pSrcA,
  const MATTYPE * pSrcB,
  MATTYPE * pDst)
{
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

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-DC); col += DC)
          {
            int row=0;
            PACK(b,PACKEDB,cols,inners,max_inner,DC,block_inner,block_col+col);

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


          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-12); col += 12)
          {
             int row=0;
             PACK(b,PACKEDB,cols,inners,max_inner,12,block_inner,block_col+col);

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

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-8); col += 8)
          {
            int row=0;
            PACK(b,PACKEDB,cols,inners,max_inner,8,block_inner,block_col+col);

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

          for (; col <= (MIN(0 + COLS_BLOCK, cols - block_col)-4); col += 4)
          {
            int row=0;
            PACK(b,PACKEDB,cols,inners,max_inner,4,block_inner,block_col+col);

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
#undef HVEC 
#undef VLOAD
#undef VSTORE
#undef VMAC_N
#undef MATTYPE 
#undef EXT

#undef DC 
#undef DR 
#undef HDR 
#undef DCS 


#undef INNER_BLOCK
#undef ROWS_BLOCK
#undef COLS_BLOCK 


