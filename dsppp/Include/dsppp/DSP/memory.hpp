// -*- C++ -*-
/** @file */ 
#pragma once 

#ifdef DOXYGEN
#define ARM_MATH_DSP
#undef ARM_MATH_MVEI
#undef ARM_MATH_MVEF
#undef ARM_MATH_NEON
#endif


namespace arm_cmsis_dsp {


/** \addtogroup DSPALG 
 *  @{
 */

#define __PACKq7(v0,v1,v2,v3) ( (((int32_t)(v0) <<  0) & (int32_t)0x000000FF) | \
                                  (((int32_t)(v1) <<  8) & (int32_t)0x0000FF00) | \
                                  (((int32_t)(v2) << 16) & (int32_t)0x00FF0000) | \
                                  (((int32_t)(v3) << 24) & (int32_t)0xFF000000)  )


__STATIC_FORCEINLINE int32_t read_q15x2 (
  Q15 const * pQ15)
{
  int32_t val;
  const int16_t *p=reinterpret_cast<const int16_t* >(pQ15);

#ifdef __ARM_FEATURE_UNALIGNED
  memcpy (&val, p, 4);
#else
  val = (p[1] << 16) | (p[0] & 0x0FFFF) ;
#endif

  return (val);
};



__STATIC_FORCEINLINE void write_q15x2 (
  Q15 * pQ15,
  int32_t   value)
{
  int32_t val = value;
  int16_t *p=reinterpret_cast<int16_t* >(pQ15);

#ifdef __ARM_FEATURE_UNALIGNED
  memcpy (p, &val, 4);
#else
  p[0] = (int16_t)(val & 0x0FFFF);
  p[1] = (int16_t)(val >> 16);
#endif
};


__STATIC_FORCEINLINE int32_t read_q7x4 (
  Q7 const * pQ7)
{
  int32_t val;
  const int8_t *p=reinterpret_cast<const int8_t*>(pQ7);

#ifdef __ARM_FEATURE_UNALIGNED
  memcpy (&val, p, 4);
#else
  val =((p[3] & 0x0FF) << 24)  | ((p[2] & 0x0FF) << 16)  | ((p[1] & 0x0FF) << 8)  | (p[0] & 0x0FF);
#endif 
  return (val);
};






__STATIC_FORCEINLINE void write_q7x4 (
  Q7 *& pQ7,
  int32_t   value)
{
  int8_t *p=reinterpret_cast<int8_t*>(pQ7);
  int32_t val = value;
#ifdef __ARM_FEATURE_UNALIGNED
  memcpy (p, &val, 4);
#else
  p[0] = (q7_t)(val & 0x0FF);
  p[1] = (q7_t)((val >> 8) & 0x0FF);
  p[2] = (q7_t)((val >> 16) & 0x0FF);
  p[3] = (q7_t)((val >> 24) & 0x0FF);

#endif
};

/*! @} */

}

