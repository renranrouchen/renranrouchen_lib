/* ----------------------------------------------------------------------   
* Copyright (C) 2010 ARM Limited. All rights reserved.   
*   
* $Date: 14/06/07 1:57a $Revision: 	V1.1.0  
*   
* Project: 	    CMSIS DSP Library   
* Title:	    arm_cfft_radix2_q15.c   
*   
* Description:	Radix-2 Decimation in Frequency CFFT & CIFFT Fixed point processing function   
*   
*   
* Target Processor: Cortex-M4/Cortex-M3/Cortex-M0
*  
* Version 1.1.0 2012/02/15 
*    Updated with more optimizations, bug fixes and minor API changes.  
*   
* Version 0.0.3  2010/03/10    
*    Initial version   
* -------------------------------------------------------------------- */

#include "arm_math.h"

/**   
 * @ingroup groupTransforms   
 */

/**   
 * @defgroup Radix2_CFFT_CIFFT Radix-2 Complex FFT Functions   
 *   
 * \par   
 * Complex Fast Fourier Transform(CFFT) and Complex Inverse Fast Fourier Transform(CIFFT) is an efficient algorithm to compute Discrete Fourier Transform(DFT) and Inverse Discrete Fourier Transform(IDFT).   
 * Computational complexity of CFFT reduces drastically when compared to DFT.    
 */


/**   
 * @addtogroup Radix2_CFFT_CIFFT   
 * @{   
 */

/**   
 * @details   
 * @brief Processing function for the fixed-point CFFT/CIFFT.  
 * @param[in]      *S    points to an instance of the fixed-point CFFT/CIFFT structure.  
 * @param[in, out] *pSrc points to the complex data buffer of size <code>2*fftLen</code>. Processing occurs in-place.  
 * @return none.  
 */

void arm_cfft_radix2_q15(
  const arm_cfft_radix2_instance_q15 * S,
  q15_t * pSrc)
{

  if(S->ifftFlag == 1u)
  {
    arm_radix2_butterfly_inverse_q15(pSrc, S->fftLen,
                                     S->pTwiddle, S->twidCoefModifier);
  }
  else
  {
    arm_radix2_butterfly_q15(pSrc, S->fftLen,
                             S->pTwiddle, S->twidCoefModifier);
  }

  arm_bitreversal_q15(pSrc, S->fftLen, S->bitRevFactor, S->pBitRevTable);
}

/**   
 * @} end of Radix2_CFFT_CIFFT group   
 */

void arm_radix2_butterfly_q15(
  q15_t * pSrc,
  uint32_t fftLen,
  q15_t * pCoef,
  uint16_t twidCoefModifier)
{
#ifndef ARM_MATH_CM0

  int i, j, k, l;
  int n1, n2, ia;
  q15_t in;
  q31_t T, S, R;
  q31_t coeff, out1, out2;

  //N = fftLen; 
  n2 = fftLen;

  n1 = n2;
  n2 = n2 >> 1;
  ia = 0;

  // loop for groups 
  for (i = 0; i < n2; i++)
  {
    coeff = _SIMD32_OFFSET(pCoef + (ia * 2u));

    ia = ia + twidCoefModifier;

    l = i + n2;

    T = _SIMD32_OFFSET(pSrc + (2 * i));
    in = ((int16_t) (T & 0xFFFF)) >> 2;
    T = ((T >> 2) & 0xFFFF0000) | (in & 0xFFFF);

    S = _SIMD32_OFFSET(pSrc + (2 * l));
    in = ((int16_t) (S & 0xFFFF)) >> 2;
    S = ((S >> 2) & 0xFFFF0000) | (in & 0xFFFF);

    R = __QSUB16(T, S);

    _SIMD32_OFFSET(pSrc + (2 * i)) = __SHADD16(T, S);

#ifndef ARM_MATH_BIG_ENDIAN

    out1 = __SMUAD(coeff, R) >> 16;
    out2 = __SMUSDX(coeff, R);

#else

    out1 = __SMUSDX(R, coeff) >> 16u;
    out2 = __SMUAD(coeff, R);

#endif //     #ifndef ARM_MATH_BIG_ENDIAN

    _SIMD32_OFFSET(pSrc + (2u * l)) =
      (q31_t) ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);

    coeff = _SIMD32_OFFSET(pCoef + (ia * 2u));

    ia = ia + twidCoefModifier;

    // loop for butterfly 
    i++;
    l++;

    T = _SIMD32_OFFSET(pSrc + (2 * i));
    in = ((int16_t) (T & 0xFFFF)) >> 2;
    T = ((T >> 2) & 0xFFFF0000) | (in & 0xFFFF);

    S = _SIMD32_OFFSET(pSrc + (2 * l));
    in = ((int16_t) (S & 0xFFFF)) >> 2;
    S = ((S >> 2) & 0xFFFF0000) | (in & 0xFFFF);

    R = __QSUB16(T, S);

    _SIMD32_OFFSET(pSrc + (2 * i)) = __SHADD16(T, S);

#ifndef ARM_MATH_BIG_ENDIAN

    out1 = __SMUAD(coeff, R) >> 16;
    out2 = __SMUSDX(coeff, R);

#else

    out1 = __SMUSDX(R, coeff) >> 16u;
    out2 = __SMUAD(coeff, R);

#endif //     #ifndef ARM_MATH_BIG_ENDIAN

    _SIMD32_OFFSET(pSrc + (2u * l)) =
      (q31_t) ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);

  }                             // groups loop end 

  twidCoefModifier = twidCoefModifier << 1u;

  // loop for stage 
  for (k = fftLen / 2; k > 2; k = k >> 1)
  {
    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;

    // loop for groups 
    for (j = 0; j < n2; j++)
    {
      coeff = _SIMD32_OFFSET(pCoef + (ia * 2u));

      ia = ia + twidCoefModifier;

      // loop for butterfly 
      for (i = j; i < fftLen; i += n1)
      {
        l = i + n2;

        T = _SIMD32_OFFSET(pSrc + (2 * i));

        S = _SIMD32_OFFSET(pSrc + (2 * l));

        R = __QSUB16(T, S);

        _SIMD32_OFFSET(pSrc + (2 * i)) = __SHADD16(T, S);

#ifndef ARM_MATH_BIG_ENDIAN

        out1 = __SMUAD(coeff, R) >> 16;
        out2 = __SMUSDX(coeff, R);

#else

        out1 = __SMUSDX(R, coeff) >> 16u;
        out2 = __SMUAD(coeff, R);

#endif //     #ifndef ARM_MATH_BIG_ENDIAN

        _SIMD32_OFFSET(pSrc + (2u * l)) =
          (q31_t) ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);

        i += n1;

        l = i + n2;

        T = _SIMD32_OFFSET(pSrc + (2 * i));

        S = _SIMD32_OFFSET(pSrc + (2 * l));

        R = __QSUB16(T, S);

        _SIMD32_OFFSET(pSrc + (2 * i)) = __SHADD16(T, S);

#ifndef ARM_MATH_BIG_ENDIAN

        out1 = __SMUAD(coeff, R) >> 16;
        out2 = __SMUSDX(coeff, R);

#else

        out1 = __SMUSDX(R, coeff) >> 16u;
        out2 = __SMUAD(coeff, R);

#endif //     #ifndef ARM_MATH_BIG_ENDIAN

        _SIMD32_OFFSET(pSrc + (2u * l)) =
          (q31_t) ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);

      }                         // butterfly loop end 

    }                           // groups loop end 

    twidCoefModifier = twidCoefModifier << 1u;
  }                             // stages loop end 

  n1 = n2;
  n2 = n2 >> 1;
  ia = 0;

  coeff = _SIMD32_OFFSET(pCoef + (ia * 2u));

  ia = ia + twidCoefModifier;

  // loop for butterfly 
  for (i = 0; i < fftLen; i += n1)
  {
    l = i + n2;

    T = _SIMD32_OFFSET(pSrc + (2 * i));

    S = _SIMD32_OFFSET(pSrc + (2 * l));

    R = __QSUB16(T, S);

    _SIMD32_OFFSET(pSrc + (2 * i)) = __QADD16(T, S);

    _SIMD32_OFFSET(pSrc + (2u * l)) = R;

    i += n1;
    l = i + n2;

    T = _SIMD32_OFFSET(pSrc + (2 * i));

    S = _SIMD32_OFFSET(pSrc + (2 * l));

    R = __QSUB16(T, S);

    _SIMD32_OFFSET(pSrc + (2 * i)) = __QADD16(T, S);

    _SIMD32_OFFSET(pSrc + (2u * l)) = R;

  }                             // groups loop end 


#else

  int i, j, k, l;
  int n1, n2, ia;
  q15_t xt, yt, cosVal, sinVal;


  //N = fftLen; 
  n2 = fftLen;

  n1 = n2;
  n2 = n2 >> 1;
  ia = 0;

  // loop for groups 
  for (j = 0; j < n2; j++)
  {
    cosVal = pCoef[ia * 2];
    sinVal = pCoef[(ia * 2) + 1];
    ia = ia + twidCoefModifier;

    // loop for butterfly 
    for (i = j; i < fftLen; i += n1)
    {
      l = i + n2;
      xt = (pSrc[2 * i] >> 2u) - (pSrc[2 * l] >> 2u);
      pSrc[2 * i] = ((pSrc[2 * i] >> 2u) + (pSrc[2 * l] >> 2u)) >> 1u;

      yt = (pSrc[2 * i + 1] >> 2u) - (pSrc[2 * l + 1] >> 2u);
      pSrc[2 * i + 1] =
        ((pSrc[2 * l + 1] >> 2u) + (pSrc[2 * i + 1] >> 2u)) >> 1u;

      pSrc[2u * l] = (((int16_t) (((q31_t) xt * cosVal) >> 16)) +
                      ((int16_t) (((q31_t) yt * sinVal) >> 16)));

      pSrc[2u * l + 1u] = (((int16_t) (((q31_t) yt * cosVal) >> 16)) -
                           ((int16_t) (((q31_t) xt * sinVal) >> 16)));

    }                           // butterfly loop end 

  }                             // groups loop end 

  twidCoefModifier = twidCoefModifier << 1u;

  // loop for stage 
  for (k = fftLen / 2; k > 2; k = k >> 1)
  {
    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;

    // loop for groups 
    for (j = 0; j < n2; j++)
    {
      cosVal = pCoef[ia * 2];
      sinVal = pCoef[(ia * 2) + 1];
      ia = ia + twidCoefModifier;

      // loop for butterfly 
      for (i = j; i < fftLen; i += n1)
      {
        l = i + n2;
        xt = pSrc[2 * i] - pSrc[2 * l];
        pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]) >> 1u;

        yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
        pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]) >> 1u;

        pSrc[2u * l] = (((int16_t) (((q31_t) xt * cosVal) >> 16)) +
                        ((int16_t) (((q31_t) yt * sinVal) >> 16)));

        pSrc[2u * l + 1u] = (((int16_t) (((q31_t) yt * cosVal) >> 16)) -
                             ((int16_t) (((q31_t) xt * sinVal) >> 16)));

      }                         // butterfly loop end 

    }                           // groups loop end 

    twidCoefModifier = twidCoefModifier << 1u;
  }                             // stages loop end 

  n1 = n2;
  n2 = n2 >> 1;
  ia = 0;

  // loop for groups 
  for (j = 0; j < n2; j++)
  {
    cosVal = pCoef[ia * 2];
    sinVal = pCoef[(ia * 2) + 1];

    ia = ia + twidCoefModifier;

    // loop for butterfly 
    for (i = j; i < fftLen; i += n1)
    {
      l = i + n2;
      xt = pSrc[2 * i] - pSrc[2 * l];
      pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]);

      yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
      pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]);

      pSrc[2u * l] = xt;

      pSrc[2u * l + 1u] = yt;

    }                           // butterfly loop end 

  }                             // groups loop end 

  twidCoefModifier = twidCoefModifier << 1u;

#endif //             #ifndef ARM_MATH_CM0

}


void arm_radix2_butterfly_inverse_q15(
  q15_t * pSrc,
  uint32_t fftLen,
  q15_t * pCoef,
  uint16_t twidCoefModifier)
{
#ifndef ARM_MATH_CM0

  int i, j, k, l;
  int n1, n2, ia;
  q15_t in;
  q31_t T, S, R;
  q31_t coeff, out1, out2;

  //N = fftLen; 
  n2 = fftLen;

  n1 = n2;
  n2 = n2 >> 1;
  ia = 0;

  // loop for groups 
  for (i = 0; i < n2; i++)
  {
    coeff = _SIMD32_OFFSET(pCoef + (ia * 2u));

    ia = ia + twidCoefModifier;

    l = i + n2;

    T = _SIMD32_OFFSET(pSrc + (2 * i));
    in = ((int16_t) (T & 0xFFFF)) >> 2;
    T = ((T >> 2) & 0xFFFF0000) | (in & 0xFFFF);

    S = _SIMD32_OFFSET(pSrc + (2 * l));
    in = ((int16_t) (S & 0xFFFF)) >> 2;
    S = ((S >> 2) & 0xFFFF0000) | (in & 0xFFFF);

    R = __QSUB16(T, S);

    _SIMD32_OFFSET(pSrc + (2 * i)) = __SHADD16(T, S);

#ifndef ARM_MATH_BIG_ENDIAN

    out1 = __SMUSD(coeff, R) >> 16;
    out2 = __SMUADX(coeff, R);
#else

    out1 = __SMUADX(R, coeff) >> 16u;
    out2 = __SMUSD(__QSUB(0, coeff), R);

#endif //     #ifndef ARM_MATH_BIG_ENDIAN

    _SIMD32_OFFSET(pSrc + (2u * l)) =
      (q31_t) ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);

    coeff = _SIMD32_OFFSET(pCoef + (ia * 2u));

    ia = ia + twidCoefModifier;

    // loop for butterfly 
    i++;
    l++;

    T = _SIMD32_OFFSET(pSrc + (2 * i));
    in = ((int16_t) (T & 0xFFFF)) >> 2;
    T = ((T >> 2) & 0xFFFF0000) | (in & 0xFFFF);

    S = _SIMD32_OFFSET(pSrc + (2 * l));
    in = ((int16_t) (S & 0xFFFF)) >> 2;
    S = ((S >> 2) & 0xFFFF0000) | (in & 0xFFFF);

    R = __QSUB16(T, S);

    _SIMD32_OFFSET(pSrc + (2 * i)) = __SHADD16(T, S);

#ifndef ARM_MATH_BIG_ENDIAN

    out1 = __SMUSD(coeff, R) >> 16;
    out2 = __SMUADX(coeff, R);
#else

    out1 = __SMUADX(R, coeff) >> 16u;
    out2 = __SMUSD(__QSUB(0, coeff), R);

#endif //     #ifndef ARM_MATH_BIG_ENDIAN

    _SIMD32_OFFSET(pSrc + (2u * l)) =
      (q31_t) ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);

  }                             // groups loop end 

  twidCoefModifier = twidCoefModifier << 1u;

  // loop for stage 
  for (k = fftLen / 2; k > 2; k = k >> 1)
  {
    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;

    // loop for groups 
    for (j = 0; j < n2; j++)
    {
      coeff = _SIMD32_OFFSET(pCoef + (ia * 2u));

      ia = ia + twidCoefModifier;

      // loop for butterfly 
      for (i = j; i < fftLen; i += n1)
      {
        l = i + n2;

        T = _SIMD32_OFFSET(pSrc + (2 * i));

        S = _SIMD32_OFFSET(pSrc + (2 * l));

        R = __QSUB16(T, S);

        _SIMD32_OFFSET(pSrc + (2 * i)) = __SHADD16(T, S);

#ifndef ARM_MATH_BIG_ENDIAN

        out1 = __SMUSD(coeff, R) >> 16;
        out2 = __SMUADX(coeff, R);

#else

        out1 = __SMUADX(R, coeff) >> 16u;
        out2 = __SMUSD(__QSUB(0, coeff), R);

#endif //     #ifndef ARM_MATH_BIG_ENDIAN

        _SIMD32_OFFSET(pSrc + (2u * l)) =
          (q31_t) ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);

        i += n1;

        l = i + n2;

        T = _SIMD32_OFFSET(pSrc + (2 * i));

        S = _SIMD32_OFFSET(pSrc + (2 * l));

        R = __QSUB16(T, S);

        _SIMD32_OFFSET(pSrc + (2 * i)) = __SHADD16(T, S);

#ifndef ARM_MATH_BIG_ENDIAN

        out1 = __SMUSD(coeff, R) >> 16;
        out2 = __SMUADX(coeff, R);
#else

        out1 = __SMUADX(R, coeff) >> 16u;
        out2 = __SMUSD(__QSUB(0, coeff), R);

#endif //     #ifndef ARM_MATH_BIG_ENDIAN

        _SIMD32_OFFSET(pSrc + (2u * l)) =
          (q31_t) ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);

      }                         // butterfly loop end 

    }                           // groups loop end 

    twidCoefModifier = twidCoefModifier << 1u;
  }                             // stages loop end 

  n1 = n2;
  n2 = n2 >> 1;
  ia = 0;

  // loop for groups 
  for (j = 0; j < n2; j++)
  {
    coeff = _SIMD32_OFFSET(pCoef + (ia * 2u));

    ia = ia + twidCoefModifier;

    // loop for butterfly 
    for (i = j; i < fftLen; i += n1)
    {
      l = i + n2;

      T = _SIMD32_OFFSET(pSrc + (2 * i));

      S = _SIMD32_OFFSET(pSrc + (2 * l));

      R = __QSUB16(T, S);

      _SIMD32_OFFSET(pSrc + (2 * i)) = __QADD16(T, S);

      _SIMD32_OFFSET(pSrc + (2u * l)) = R;

    }                           // butterfly loop end 

  }                             // groups loop end 

  twidCoefModifier = twidCoefModifier << 1u;

#else


  int i, j, k, l;
  int n1, n2, ia;
  q15_t xt, yt, cosVal, sinVal;

  //N = fftLen; 
  n2 = fftLen;

  n1 = n2;
  n2 = n2 >> 1;
  ia = 0;

  // loop for groups 
  for (j = 0; j < n2; j++)
  {
    cosVal = pCoef[ia * 2];
    sinVal = pCoef[(ia * 2) + 1];
    ia = ia + twidCoefModifier;

    // loop for butterfly 
    for (i = j; i < fftLen; i += n1)
    {
      l = i + n2;
      xt = (pSrc[2 * i] >> 2u) - (pSrc[2 * l] >> 2u);
      pSrc[2 * i] = ((pSrc[2 * i] >> 2u) + (pSrc[2 * l] >> 2u)) >> 1u;

      yt = (pSrc[2 * i + 1] >> 2u) - (pSrc[2 * l + 1] >> 2u);
      pSrc[2 * i + 1] =
        ((pSrc[2 * l + 1] >> 2u) + (pSrc[2 * i + 1] >> 2u)) >> 1u;

      pSrc[2u * l] = (((int16_t) (((q31_t) xt * cosVal) >> 16)) -
                      ((int16_t) (((q31_t) yt * sinVal) >> 16)));

      pSrc[2u * l + 1u] = (((int16_t) (((q31_t) yt * cosVal) >> 16)) +
                           ((int16_t) (((q31_t) xt * sinVal) >> 16)));

    }                           // butterfly loop end 

  }                             // groups loop end 

  twidCoefModifier = twidCoefModifier << 1u;

  // loop for stage 
  for (k = fftLen / 2; k > 2; k = k >> 1)
  {
    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;

    // loop for groups 
    for (j = 0; j < n2; j++)
    {
      cosVal = pCoef[ia * 2];
      sinVal = pCoef[(ia * 2) + 1];
      ia = ia + twidCoefModifier;

      // loop for butterfly 
      for (i = j; i < fftLen; i += n1)
      {
        l = i + n2;
        xt = pSrc[2 * i] - pSrc[2 * l];
        pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]) >> 1u;

        yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
        pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]) >> 1u;

        pSrc[2u * l] = (((int16_t) (((q31_t) xt * cosVal) >> 16)) -
                        ((int16_t) (((q31_t) yt * sinVal) >> 16)));

        pSrc[2u * l + 1u] = (((int16_t) (((q31_t) yt * cosVal) >> 16)) +
                             ((int16_t) (((q31_t) xt * sinVal) >> 16)));

      }                         // butterfly loop end 

    }                           // groups loop end 

    twidCoefModifier = twidCoefModifier << 1u;
  }                             // stages loop end 

  n1 = n2;
  n2 = n2 >> 1;
  ia = 0;

  cosVal = pCoef[ia * 2];
  sinVal = pCoef[(ia * 2) + 1];

  ia = ia + twidCoefModifier;

  // loop for butterfly 
  for (i = 0; i < fftLen; i += n1)
  {
    l = i + n2;
    xt = pSrc[2 * i] - pSrc[2 * l];
    pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]);

    yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
    pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]);

    pSrc[2u * l] = xt;

    pSrc[2u * l + 1u] = yt;

  }                             // groups loop end 


#endif //             #ifndef ARM_MATH_CM0

}
