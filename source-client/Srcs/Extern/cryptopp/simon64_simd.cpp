// simon-simd.cpp - written and placed in the public domain by Jeffrey Walton
//
//    This source file uses intrinsics and built-ins to gain access to
//    SSSE3, ARM NEON and ARMv8a, and Altivec instructions. A separate
//    source file is needed because additional CXXFLAGS are required to enable
//    the appropriate instructions sets in some build configurations.

#include "pch.h"
#include "config.h"

#include "simon.h"
#include "misc.h"

// Uncomment for benchmarking C++ against SSE or NEON.
// Do so in both simon.cpp and simon-simd.cpp.
// #undef CRYPTOPP_SSE41_AVAILABLE
// #undef CRYPTOPP_ARM_NEON_AVAILABLE

#if (CRYPTOPP_SSSE3_AVAILABLE)
# include "adv_simd.h"
# include <pmmintrin.h>
# include <tmmintrin.h>
#endif

#if (CRYPTOPP_SSE41_AVAILABLE)
# include <smmintrin.h>
#endif

#if defined(__XOP__)
# include <ammintrin.h>
#endif

#if defined(__AVX512F__)
# define CRYPTOPP_AVX512_ROTATE 1
# include <immintrin.h>
#endif

// C1189: error: This header is specific to ARM targets
#if (CRYPTOPP_ARM_NEON_AVAILABLE)
# include "adv_simd.h"
# ifndef _M_ARM64
#  include <arm_neon.h>
# endif
#endif

#if (CRYPTOPP_ARM_ACLE_AVAILABLE)
# include <stdint.h>
# include <arm_acle.h>
#endif

#if defined(CRYPTOPP_ALTIVEC_AVAILABLE)
# include "adv_simd.h"
# include "ppc_simd.h"
#endif

// Squash MS LNK4221 and libtool warnings
extern const char SIMON64_SIMD_FNAME[] = __FILE__;

ANONYMOUS_NAMESPACE_BEGIN

using CryptoPP::byte;
using CryptoPP::word32;
using CryptoPP::word64;
using CryptoPP::vec_swap;  // SunCC

// *************************** ARM NEON ************************** //

#if (CRYPTOPP_ARM_NEON_AVAILABLE)

template <class T>
inline T UnpackHigh32(const T& a, const T& b)
{
    const uint32x2_t x(vget_high_u32((uint32x4_t)a));
    const uint32x2_t y(vget_high_u32((uint32x4_t)b));
    const uint32x2x2_t r = vzip_u32(x, y);
    return (T)vcombine_u32(r.val[0], r.val[1]);
}

template <class T>
inline T UnpackLow32(const T& a, const T& b)
{
    const uint32x2_t x(vget_low_u32((uint32x4_t)a));
    const uint32x2_t y(vget_low_u32((uint32x4_t)b));
    const uint32x2x2_t r = vzip_u32(x, y);
    return (T)vcombine_u32(r.val[0], r.val[1]);
}

template <unsigned int R>
inline uint32x4_t RotateLeft32(const uint32x4_t& val)
{
    const uint32x4_t a(vshlq_n_u32(val, R));
    const uint32x4_t b(vshrq_n_u32(val, 32 - R));
    return vorrq_u32(a, b);
}

template <unsigned int R>
inline uint32x4_t RotateRight32(const uint32x4_t& val)
{
    const uint32x4_t a(vshlq_n_u32(val, 32 - R));
    const uint32x4_t b(vshrq_n_u32(val, R));
    return vorrq_u32(a, b);
}

#if defined(__aarch32__) || defined(__aarch64__)
// Faster than two Shifts and an Or. Thanks to Louis Wingers and Bryan Weeks.
template <>
inline uint32x4_t RotateLeft32<8>(const uint32x4_t& val)
{
    const uint8_t maskb[16] = { 3,0,1,2, 7,4,5,6, 11,8,9,10, 15,12,13,14 };
    const uint8x16_t mask = vld1q_u8(maskb);

    return vreinterpretq_u32_u8(
        vqtbl1q_u8(vreinterpretq_u8_u32(val), mask));
}

// Faster than two Shifts and an Or. Thanks to Louis Wingers and Bryan Weeks.
template <>
inline uint32x4_t RotateRight32<8>(const uint32x4_t& val)
{
    const uint8_t maskb[16] = { 1,2,3,0, 5,6,7,4, 9,10,11,8, 13,14,14,12 };
    const uint8x16_t mask = vld1q_u8(maskb);

    return vreinterpretq_u32_u8(
        vqtbl1q_u8(vreinterpretq_u8_u32(val), mask));
}
#endif

inline uint32x4_t SIMON64_f(const uint32x4_t& val)
{
    return veorq_u32(RotateLeft32<2>(val),
        vandq_u32(RotateLeft32<1>(val), RotateLeft32<8>(val)));
}

inline void SIMON64_Enc_Block(uint32x4_t &block1, uint32x4_t &block0,
    const word32 *subkeys, unsigned int rounds)
{
    // [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 A3 B1 B3][A2 A4 B2 B4] ...
    uint32x4_t x1 = vuzpq_u32(block0, block1).val[1];
    uint32x4_t y1 = vuzpq_u32(block0, block1).val[0];

    for (int i = 0; i < static_cast<int>(rounds & ~1)-1; i += 2)
    {
        const uint32x4_t rk1 = vld1q_dup_u32(subkeys+i);
        y1 = veorq_u32(veorq_u32(y1, SIMON64_f(x1)), rk1);

        const uint32x4_t rk2 = vld1q_dup_u32(subkeys+i+1);
        x1 = veorq_u32(veorq_u32(x1, SIMON64_f(y1)), rk2);
    }

    if (rounds & 1)
    {
        const uint32x4_t rk = vld1q_dup_u32(subkeys+rounds-1);

        y1 = veorq_u32(veorq_u32(y1, SIMON64_f(x1)), rk);
        std::swap(x1, y1);
    }

    // [A1 A3 B1 B3][A2 A4 B2 B4] => [A1 A2 A3 A4][B1 B2 B3 B4]
    block0 = UnpackLow32(y1, x1);
    block1 = UnpackHigh32(y1, x1);
}

inline void SIMON64_Dec_Block(uint32x4_t &block0, uint32x4_t &block1,
    const word32 *subkeys, unsigned int rounds)
{
    // [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 A3 B1 B3][A2 A4 B2 B4] ...
    uint32x4_t x1 = vuzpq_u32(block0, block1).val[1];
    uint32x4_t y1 = vuzpq_u32(block0, block1).val[0];

    if (rounds & 1)
    {
        std::swap(x1, y1);
        const uint32x4_t rk = vld1q_dup_u32(subkeys + rounds - 1);

        y1 = veorq_u32(veorq_u32(y1, rk), SIMON64_f(x1));
        rounds--;
    }

    for (int i = static_cast<int>(rounds-2); i >= 0; i -= 2)
    {
        const uint32x4_t rk1 = vld1q_dup_u32(subkeys+i+1);
        x1 = veorq_u32(veorq_u32(x1, SIMON64_f(y1)), rk1);

        const uint32x4_t rk2 = vld1q_dup_u32(subkeys+i);
        y1 = veorq_u32(veorq_u32(y1, SIMON64_f(x1)), rk2);
    }

    // [A1 A3 B1 B3][A2 A4 B2 B4] => [A1 A2 A3 A4][B1 B2 B3 B4]
    block0 = UnpackLow32(y1, x1);
    block1 = UnpackHigh32(y1, x1);
}

inline void SIMON64_Enc_6_Blocks(uint32x4_t &block0, uint32x4_t &block1,
    uint32x4_t &block2, uint32x4_t &block3, uint32x4_t &block4, uint32x4_t &block5,
    const word32 *subkeys, unsigned int rounds)
{
    // [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 A3 B1 B3][A2 A4 B2 B4] ...
    uint32x4_t x1 = vuzpq_u32(block0, block1).val[1];
    uint32x4_t y1 = vuzpq_u32(block0, block1).val[0];
    uint32x4_t x2 = vuzpq_u32(block2, block3).val[1];
    uint32x4_t y2 = vuzpq_u32(block2, block3).val[0];
    uint32x4_t x3 = vuzpq_u32(block4, block5).val[1];
    uint32x4_t y3 = vuzpq_u32(block4, block5).val[0];

    for (int i = 0; i < static_cast<int>(rounds & ~1) - 1; i += 2)
    {
        const uint32x4_t rk1 = vld1q_dup_u32(subkeys+i);
        y1 = veorq_u32(veorq_u32(y1, SIMON64_f(x1)), rk1);
        y2 = veorq_u32(veorq_u32(y2, SIMON64_f(x2)), rk1);
        y3 = veorq_u32(veorq_u32(y3, SIMON64_f(x3)), rk1);

        const uint32x4_t rk2 = vld1q_dup_u32(subkeys+i+1);
        x1 = veorq_u32(veorq_u32(x1, SIMON64_f(y1)), rk2);
        x2 = veorq_u32(veorq_u32(x2, SIMON64_f(y2)), rk2);
        x3 = veorq_u32(veorq_u32(x3, SIMON64_f(y3)), rk2);
    }

    if (rounds & 1)
    {
        const uint32x4_t rk = vld1q_dup_u32(subkeys + rounds - 1);

        y1 = veorq_u32(veorq_u32(y1, SIMON64_f(x1)), rk);
        y2 = veorq_u32(veorq_u32(y2, SIMON64_f(x2)), rk);
        y3 = veorq_u32(veorq_u32(y3, SIMON64_f(x3)), rk);
        std::swap(x1, y1); std::swap(x2, y2); std::swap(x3, y3);
    }

    // [A1 A3 B1 B3][A2 A4 B2 B4] => [A1 A2 A3 A4][B1 B2 B3 B4]
    block0 = UnpackLow32(y1, x1);
    block1 = UnpackHigh32(y1, x1);
    block2 = UnpackLow32(y2, x2);
    block3 = UnpackHigh32(y2, x2);
    block4 = UnpackLow32(y3, x3);
    block5 = UnpackHigh32(y3, x3);
}

inline void SIMON64_Dec_6_Blocks(uint32x4_t &block0, uint32x4_t &block1,
    uint32x4_t &block2, uint32x4_t &block3, uint32x4_t &block4, uint32x4_t &block5,
    const word32 *subkeys, unsigned int rounds)
{
    // [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 A3 B1 B3][A2 A4 B2 B4] ...
    uint32x4_t x1 = vuzpq_u32(block0, block1).val[1];
    uint32x4_t y1 = vuzpq_u32(block0, block1).val[0];
    uint32x4_t x2 = vuzpq_u32(block2, block3).val[1];
    uint32x4_t y2 = vuzpq_u32(block2, block3).val[0];
    uint32x4_t x3 = vuzpq_u32(block4, block5).val[1];
    uint32x4_t y3 = vuzpq_u32(block4, block5).val[0];

    if (rounds & 1)
    {
        std::swap(x1, y1); std::swap(x2, y2); std::swap(x3, y3);
        const uint32x4_t rk = vld1q_dup_u32(subkeys + rounds - 1);

        y1 = veorq_u32(veorq_u32(y1, rk), SIMON64_f(x1));
        y2 = veorq_u32(veorq_u32(y2, rk), SIMON64_f(x2));
        y3 = veorq_u32(veorq_u32(y3, rk), SIMON64_f(x3));
        rounds--;
    }

    for (int i = static_cast<int>(rounds-2); i >= 0; i -= 2)
    {
        const uint32x4_t rk1 = vld1q_dup_u32(subkeys + i + 1);
        x1 = veorq_u32(veorq_u32(x1, SIMON64_f(y1)), rk1);
        x2 = veorq_u32(veorq_u32(x2, SIMON64_f(y2)), rk1);
        x3 = veorq_u32(veorq_u32(x3, SIMON64_f(y3)), rk1);

        const uint32x4_t rk2 = vld1q_dup_u32(subkeys + i);
        y1 = veorq_u32(veorq_u32(y1, SIMON64_f(x1)), rk2);
        y2 = veorq_u32(veorq_u32(y2, SIMON64_f(x2)), rk2);
        y3 = veorq_u32(veorq_u32(y3, SIMON64_f(x3)), rk2);
    }

    // [A1 A3 B1 B3][A2 A4 B2 B4] => [A1 A2 A3 A4][B1 B2 B3 B4]
    block0 = UnpackLow32(y1, x1);
    block1 = UnpackHigh32(y1, x1);
    block2 = UnpackLow32(y2, x2);
    block3 = UnpackHigh32(y2, x2);
    block4 = UnpackLow32(y3, x3);
    block5 = UnpackHigh32(y3, x3);
}

#endif  // CRYPTOPP_ARM_NEON_AVAILABLE

// ***************************** IA-32 ***************************** //

#if defined(CRYPTOPP_SSE41_AVAILABLE)

inline void Swap128(__m128i& a,__m128i& b)
{
#if defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x5120)
    // __m128i is an unsigned long long[2], and support for swapping it was not added until C++11.
    // SunCC 12.1 - 12.3 fail to consume the swap; while SunCC 12.4 consumes it without -std=c++11.
    vec_swap(a, b);
#else
    std::swap(a, b);
#endif
}

template <unsigned int R>
inline __m128i RotateLeft32(const __m128i& val)
{
#if defined(__XOP__)
    return _mm_roti_epi32(val, R);
#else
    return _mm_or_si128(
        _mm_slli_epi32(val, R), _mm_srli_epi32(val, 32-R));
#endif
}

template <unsigned int R>
inline __m128i RotateRight32(const __m128i& val)
{
#if defined(__XOP__)
    return _mm_roti_epi32(val, 32-R);
#else
    return _mm_or_si128(
        _mm_slli_epi32(val, 32-R), _mm_srli_epi32(val, R));
#endif
}

// Faster than two Shifts and an Or. Thanks to Louis Wingers and Bryan Weeks.
template <>
__m128i RotateLeft32<8>(const __m128i& val)
{
#if defined(__XOP__)
    return _mm_roti_epi32(val, 8);
#else
    const __m128i mask = _mm_set_epi8(14,13,12,15, 10,9,8,11, 6,5,4,7, 2,1,0,3);
    return _mm_shuffle_epi8(val, mask);
#endif
}

// Faster than two Shifts and an Or. Thanks to Louis Wingers and Bryan Weeks.
template <>
__m128i RotateRight32<8>(const __m128i& val)
{
#if defined(__XOP__)
    return _mm_roti_epi32(val, 32-8);
#else
    const __m128i mask = _mm_set_epi8(12,15,14,13, 8,11,10,9, 4,7,6,5, 0,3,2,1);
    return _mm_shuffle_epi8(val, mask);
#endif
}

inline __m128i SIMON64_f(const __m128i& v)
{
    return _mm_xor_si128(RotateLeft32<2>(v),
        _mm_and_si128(RotateLeft32<1>(v), RotateLeft32<8>(v)));
}

inline void SIMON64_Enc_Block(__m128i &block0, __m128i &block1,
    const word32 *subkeys, unsigned int rounds)
{
    // [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 A3 B1 B3][A2 A4 B2 B4] ...
    const __m128 t0 = _mm_castsi128_ps(block0);
    const __m128 t1 = _mm_castsi128_ps(block1);
    __m128i x1 = _mm_castps_si128(_mm_shuffle_ps(t0, t1, _MM_SHUFFLE(3,1,3,1)));
    __m128i y1 = _mm_castps_si128(_mm_shuffle_ps(t0, t1, _MM_SHUFFLE(2,0,2,0)));

    for (int i = 0; i < static_cast<int>(rounds & ~1)-1; i += 2)
    {
        const __m128i rk1 = _mm_set1_epi32(subkeys[i]);
        y1 = _mm_xor_si128(_mm_xor_si128(y1, SIMON64_f(x1)), rk1);

        const __m128i rk2 = _mm_set1_epi32(subkeys[i+1]);
        x1 = _mm_xor_si128(_mm_xor_si128(x1, SIMON64_f(y1)), rk2);
    }

    if (rounds & 1)
    {
        const __m128i rk = _mm_set1_epi32(subkeys[rounds-1]);
        y1 = _mm_xor_si128(_mm_xor_si128(y1, SIMON64_f(x1)), rk);
        Swap128(x1, y1);
    }

    // [A1 A3 B1 B3][A2 A4 B2 B4] => [A1 A2 A3 A4][B1 B2 B3 B4]
    block0 = _mm_unpacklo_epi32(y1, x1);
    block1 = _mm_unpackhi_epi32(y1, x1);
}

inline void SIMON64_Dec_Block(__m128i &block0, __m128i &block1,
    const word32 *subkeys, unsigned int rounds)
{
    // [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 A3 B1 B3][A2 A4 B2 B4] ...
    const __m128 t0 = _mm_castsi128_ps(block0);
    const __m128 t1 = _mm_castsi128_ps(block1);
    __m128i x1 = _mm_castps_si128(_mm_shuffle_ps(t0, t1, _MM_SHUFFLE(3,1,3,1)));
    __m128i y1 = _mm_castps_si128(_mm_shuffle_ps(t0, t1, _MM_SHUFFLE(2,0,2,0)));

    if (rounds & 1)
    {
        Swap128(x1, y1);
        const __m128i rk = _mm_set1_epi32(subkeys[rounds-1]);
        y1 = _mm_xor_si128(_mm_xor_si128(y1, rk), SIMON64_f(x1));
        rounds--;
    }

    for (int i = static_cast<int>(rounds-2); i >= 0; i -= 2)
    {
        const __m128i rk1 = _mm_set1_epi32(subkeys[i+1]);
        x1 = _mm_xor_si128(_mm_xor_si128(x1, SIMON64_f(y1)), rk1);

        const __m128i rk2 = _mm_set1_epi32(subkeys[i]);
        y1 = _mm_xor_si128(_mm_xor_si128(y1, SIMON64_f(x1)), rk2);
    }

    // [A1 A3 B1 B3][A2 A4 B2 B4] => [A1 A2 A3 A4][B1 B2 B3 B4]
    block0 = _mm_unpacklo_epi32(y1, x1);
    block1 = _mm_unpackhi_epi32(y1, x1);
}

inline void SIMON64_Enc_6_Blocks(__m128i &block0, __m128i &block1,
    __m128i &block2, __m128i &block3, __m128i &block4, __m128i &block5,
    const word32 *subkeys, unsigned int rounds)
{
    // [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 A3 B1 B3][A2 A4 B2 B4] ...
    const __m128 t0 = _mm_castsi128_ps(block0);
    const __m128 t1 = _mm_castsi128_ps(block1);
    __m128i x1 = _mm_castps_si128(_mm_shuffle_ps(t0, t1, _MM_SHUFFLE(3,1,3,1)));
    __m128i y1 = _mm_castps_si128(_mm_shuffle_ps(t0, t1, _MM_SHUFFLE(2,0,2,0)));

    const __m128 t2 = _mm_castsi128_ps(block2);
    const __m128 t3 = _mm_castsi128_ps(block3);
    __m128i x2 = _mm_castps_si128(_mm_shuffle_ps(t2, t3, _MM_SHUFFLE(3,1,3,1)));
    __m128i y2 = _mm_castps_si128(_mm_shuffle_ps(t2, t3, _MM_SHUFFLE(2,0,2,0)));

    const __m128 t4 = _mm_castsi128_ps(block4);
    const __m128 t5 = _mm_castsi128_ps(block5);
    __m128i x3 = _mm_castps_si128(_mm_shuffle_ps(t4, t5, _MM_SHUFFLE(3,1,3,1)));
    __m128i y3 = _mm_castps_si128(_mm_shuffle_ps(t4, t5, _MM_SHUFFLE(2,0,2,0)));

    for (int i = 0; i < static_cast<int>(rounds & ~1)-1; i += 2)
    {
        const __m128i rk1 = _mm_set1_epi32(subkeys[i]);
        y1 = _mm_xor_si128(_mm_xor_si128(y1, SIMON64_f(x1)), rk1);
        y2 = _mm_xor_si128(_mm_xor_si128(y2, SIMON64_f(x2)), rk1);
        y3 = _mm_xor_si128(_mm_xor_si128(y3, SIMON64_f(x3)), rk1);

        const __m128i rk2 = _mm_set1_epi32(subkeys[i+1]);
        x1 = _mm_xor_si128(_mm_xor_si128(x1, SIMON64_f(y1)), rk2);
        x2 = _mm_xor_si128(_mm_xor_si128(x2, SIMON64_f(y2)), rk2);
        x3 = _mm_xor_si128(_mm_xor_si128(x3, SIMON64_f(y3)), rk2);
    }

    if (rounds & 1)
    {
        const __m128i rk = _mm_set1_epi32(subkeys[rounds-1]);
        y1 = _mm_xor_si128(_mm_xor_si128(y1, SIMON64_f(x1)), rk);
        y2 = _mm_xor_si128(_mm_xor_si128(y2, SIMON64_f(x2)), rk);
        y3 = _mm_xor_si128(_mm_xor_si128(y3, SIMON64_f(x3)), rk);
        Swap128(x1, y1); Swap128(x2, y2); Swap128(x3, y3);
    }

    // [A1 A3 B1 B3][A2 A4 B2 B4] => [A1 A2 A3 A4][B1 B2 B3 B4]
    block0 = _mm_unpacklo_epi32(y1, x1);
    block1 = _mm_unpackhi_epi32(y1, x1);
    block2 = _mm_unpacklo_epi32(y2, x2);
    block3 = _mm_unpackhi_epi32(y2, x2);
    block4 = _mm_unpacklo_epi32(y3, x3);
    block5 = _mm_unpackhi_epi32(y3, x3);
}

inline void SIMON64_Dec_6_Blocks(__m128i &block0, __m128i &block1,
    __m128i &block2, __m128i &block3, __m128i &block4, __m128i &block5,
    const word32 *subkeys, unsigned int rounds)
{
    // [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 A3 B1 B3][A2 A4 B2 B4] ...
    const __m128 t0 = _mm_castsi128_ps(block0);
    const __m128 t1 = _mm_castsi128_ps(block1);
    __m128i x1 = _mm_castps_si128(_mm_shuffle_ps(t0, t1, _MM_SHUFFLE(3,1,3,1)));
    __m128i y1 = _mm_castps_si128(_mm_shuffle_ps(t0, t1, _MM_SHUFFLE(2,0,2,0)));

    const __m128 t2 = _mm_castsi128_ps(block2);
    const __m128 t3 = _mm_castsi128_ps(block3);
    __m128i x2 = _mm_castps_si128(_mm_shuffle_ps(t2, t3, _MM_SHUFFLE(3,1,3,1)));
    __m128i y2 = _mm_castps_si128(_mm_shuffle_ps(t2, t3, _MM_SHUFFLE(2,0,2,0)));

    const __m128 t4 = _mm_castsi128_ps(block4);
    const __m128 t5 = _mm_castsi128_ps(block5);
    __m128i x3 = _mm_castps_si128(_mm_shuffle_ps(t4, t5, _MM_SHUFFLE(3,1,3,1)));
    __m128i y3 = _mm_castps_si128(_mm_shuffle_ps(t4, t5, _MM_SHUFFLE(2,0,2,0)));

    if (rounds & 1)
    {
        Swap128(x1, y1); Swap128(x2, y2); Swap128(x3, y3);
        const __m128i rk = _mm_set1_epi32(subkeys[rounds-1]);
        y1 = _mm_xor_si128(_mm_xor_si128(y1, rk), SIMON64_f(x1));
        y2 = _mm_xor_si128(_mm_xor_si128(y2, rk), SIMON64_f(x2));
        y3 = _mm_xor_si128(_mm_xor_si128(y3, rk), SIMON64_f(x3));
        rounds--;
    }

    for (int i = static_cast<int>(rounds-2); i >= 0; i -= 2)
    {
        const __m128i rk1 = _mm_set1_epi32(subkeys[i+1]);
        x1 = _mm_xor_si128(_mm_xor_si128(x1, SIMON64_f(y1)), rk1);
        x2 = _mm_xor_si128(_mm_xor_si128(x2, SIMON64_f(y2)), rk1);
        x3 = _mm_xor_si128(_mm_xor_si128(x3, SIMON64_f(y3)), rk1);

        const __m128i rk2 = _mm_set1_epi32(subkeys[i]);
        y1 = _mm_xor_si128(_mm_xor_si128(y1, SIMON64_f(x1)), rk2);
        y2 = _mm_xor_si128(_mm_xor_si128(y2, SIMON64_f(x2)), rk2);
        y3 = _mm_xor_si128(_mm_xor_si128(y3, SIMON64_f(x3)), rk2);
    }

    // [A1 A3 B1 B3][A2 A4 B2 B4] => [A1 A2 A3 A4][B1 B2 B3 B4]
    block0 = _mm_unpacklo_epi32(y1, x1);
    block1 = _mm_unpackhi_epi32(y1, x1);
    block2 = _mm_unpacklo_epi32(y2, x2);
    block3 = _mm_unpackhi_epi32(y2, x2);
    block4 = _mm_unpacklo_epi32(y3, x3);
    block5 = _mm_unpackhi_epi32(y3, x3);
}

#endif  // CRYPTOPP_SSE41_AVAILABLE

// ***************************** Altivec ***************************** //

#if defined(CRYPTOPP_ALTIVEC_AVAILABLE)

using CryptoPP::uint8x16_p;
using CryptoPP::uint32x4_p;

using CryptoPP::VecAnd;
using CryptoPP::VecXor;
using CryptoPP::VecLoad;
using CryptoPP::VecLoadBE;
using CryptoPP::VecPermute;

// Rotate left by bit count
template<unsigned int C>
inline uint32x4_p RotateLeft32(const uint32x4_p val)
{
    const uint32x4_p m = {C, C, C, C};
    return vec_rl(val, m);
}

// Rotate right by bit count
template<unsigned int C>
inline uint32x4_p RotateRight32(const uint32x4_p val)
{
    const uint32x4_p m = {32-C, 32-C, 32-C, 32-C};
    return vec_rl(val, m);
}

inline uint32x4_p SIMON64_f(const uint32x4_p val)
{
    return VecXor(RotateLeft32<2>(val),
        VecAnd(RotateLeft32<1>(val), RotateLeft32<8>(val)));
}

inline void SIMON64_Enc_Block(uint32x4_p &block0, uint32x4_p &block1,
    const word32 *subkeys, unsigned int rounds)
{
#if (CRYPTOPP_BIG_ENDIAN)
    const uint8x16_p m1 = {7,6,5,4, 15,14,13,12, 23,22,21,20, 31,30,29,28};
    const uint8x16_p m2 = {3,2,1,0, 11,10,9,8, 19,18,17,16, 27,26,25,24};
#else
    const uint8x16_p m1 = {3,2,1,0, 11,10,9,8, 19,18,17,16, 27,26,25,24};
    const uint8x16_p m2 = {7,6,5,4, 15,14,13,12, 23,22,21,20, 31,30,29,28};
#endif

    // [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 A3 B1 B3][A2 A4 B2 B4] ...
    uint32x4_p x1 = VecPermute(block0, block1, m1);
    uint32x4_p y1 = VecPermute(block0, block1, m2);

    for (int i = 0; i < static_cast<int>(rounds & ~1)-1; i += 2)
    {
#if CRYPTOPP_POWER8_AVAILABLE
        const uint32x4_p rk1 = vec_splats(subkeys[i]);
        const uint32x4_p rk2 = vec_splats(subkeys[i+1]);
#else
        const uint8x16_p m = {0,1,2,3, 0,1,2,3, 0,1,2,3, 0,1,2,3};
        uint32x4_p rk1 = VecLoad(subkeys+i);
        uint32x4_p rk2 = VecLoad(subkeys+i+1);
        rk1 = VecPermute(rk1, rk1, m);
        rk2 = VecPermute(rk2, rk2, m);
#endif
        y1 = VecXor(VecXor(y1, SIMON64_f(x1)), rk1);
        x1 = VecXor(VecXor(x1, SIMON64_f(y1)), rk2);
    }

    if (rounds & 1)
    {
#if CRYPTOPP_POWER8_AVAILABLE
        const uint32x4_p rk = vec_splats(subkeys[rounds-1]);
#else
        const uint8x16_p m = {0,1,2,3, 0,1,2,3, 0,1,2,3, 0,1,2,3};
        uint32x4_p rk = VecLoad(subkeys+rounds-1);
        rk = VecPermute(rk, rk, m);
#endif
        y1 = VecXor(VecXor(y1, SIMON64_f(x1)), rk);
        std::swap(x1, y1);
    }

#if (CRYPTOPP_BIG_ENDIAN)
    const uint8x16_p m3 = {19,18,17,16, 3,2,1,0, 23,22,21,20, 7,6,5,4};
    const uint8x16_p m4 = {27,26,25,24, 11,10,9,8, 31,30,29,28, 15,14,13,12};
#else
    const uint8x16_p m3 = {3,2,1,0, 19,18,17,16, 7,6,5,4, 23,22,21,20};
    const uint8x16_p m4 = {11,10,9,8, 27,26,25,24, 15,14,13,12, 31,30,29,28};
#endif

    // [A1 A3 B1 B3][A2 A4 B2 B4] => [A1 A2 A3 A4][B1 B2 B3 B4]
    block0 = (uint32x4_p)VecPermute(x1, y1, m3);
    block1 = (uint32x4_p)VecPermute(x1, y1, m4);
}

inline void SIMON64_Dec_Block(uint32x4_p &block0, uint32x4_p &block1,
    const word32 *subkeys, unsigned int rounds)
{
#if (CRYPTOPP_BIG_ENDIAN)
    const uint8x16_p m1 = {7,6,5,4, 15,14,13,12, 23,22,21,20, 31,30,29,28};
    const uint8x16_p m2 = {3,2,1,0, 11,10,9,8, 19,18,17,16, 27,26,25,24};
#else
    const uint8x16_p m1 = {3,2,1,0, 11,10,9,8, 19,18,17,16, 27,26,25,24};
    const uint8x16_p m2 = {7,6,5,4, 15,14,13,12, 23,22,21,20, 31,30,29,28};
#endif

    // [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 A3 B1 B3][A2 A4 B2 B4] ...
    uint32x4_p x1 = VecPermute(block0, block1, m1);
    uint32x4_p y1 = VecPermute(block0, block1, m2);

    if (rounds & 1)
    {
        std::swap(x1, y1);
#if CRYPTOPP_POWER8_AVAILABLE
        const uint32x4_p rk = vec_splats(subkeys[rounds-1]);
#else
        const uint8x16_p m = {0,1,2,3, 0,1,2,3, 0,1,2,3, 0,1,2,3};
        uint32x4_p rk = VecLoad(subkeys+rounds-1);
        rk = VecPermute(rk, rk, m);
#endif
        y1 = VecXor(VecXor(y1, rk), SIMON64_f(x1));
        rounds--;
    }

    for (int i = static_cast<int>(rounds-2); i >= 0; i -= 2)
    {
#if CRYPTOPP_POWER8_AVAILABLE
        const uint32x4_p rk1 = vec_splats(subkeys[i+1]);
        const uint32x4_p rk2 = vec_splats(subkeys[i]);
#else
        const uint8x16_p m = {0,1,2,3, 0,1,2,3, 0,1,2,3, 0,1,2,3};
        uint32x4_p rk1 = VecLoad(subkeys+i+1);
        uint32x4_p rk2 = VecLoad(subkeys+i);
        rk1 = VecPermute(rk1, rk1, m);
        rk2 = VecPermute(rk2, rk2, m);
#endif
        x1 = VecXor(VecXor(x1, SIMON64_f(y1)), rk1);
        y1 = VecXor(VecXor(y1, SIMON64_f(x1)), rk2);
    }

#if (CRYPTOPP_BIG_ENDIAN)
    const uint8x16_p m3 = {19,18,17,16, 3,2,1,0, 23,22,21,20, 7,6,5,4};
    const uint8x16_p m4 = {27,26,25,24, 11,10,9,8, 31,30,29,28, 15,14,13,12};
#else
    const uint8x16_p m3 = {3,2,1,0, 19,18,17,16, 7,6,5,4, 23,22,21,20};
    const uint8x16_p m4 = {11,10,9,8, 27,26,25,24, 15,14,13,12, 31,30,29,28};
#endif

    // [A1 A3 B1 B3][A2 A4 B2 B4] => [A1 A2 A3 A4][B1 B2 B3 B4]
    block0 = (uint32x4_p)VecPermute(x1, y1, m3);
    block1 = (uint32x4_p)VecPermute(x1, y1, m4);
}

inline void SIMON64_Enc_6_Blocks(uint32x4_p &block0, uint32x4_p &block1,
            uint32x4_p &block2, uint32x4_p &block3, uint32x4_p &block4,
            uint32x4_p &block5, const word32 *subkeys, unsigned int rounds)
{
#if (CRYPTOPP_BIG_ENDIAN)
    const uint8x16_p m1 = {7,6,5,4, 15,14,13,12, 23,22,21,20, 31,30,29,28};
    const uint8x16_p m2 = {3,2,1,0, 11,10,9,8, 19,18,17,16, 27,26,25,24};
#else
    const uint8x16_p m1 = {3,2,1,0, 11,10,9,8, 19,18,17,16, 27,26,25,24};
    const uint8x16_p m2 = {7,6,5,4, 15,14,13,12, 23,22,21,20, 31,30,29,28};
#endif

    // [A1 A2][B1 B2] ... => [A1 B1][A2 B2] ...
    uint32x4_p x1 = (uint32x4_p)VecPermute(block0, block1, m1);
    uint32x4_p y1 = (uint32x4_p)VecPermute(block0, block1, m2);
    uint32x4_p x2 = (uint32x4_p)VecPermute(block2, block3, m1);
    uint32x4_p y2 = (uint32x4_p)VecPermute(block2, block3, m2);
    uint32x4_p x3 = (uint32x4_p)VecPermute(block4, block5, m1);
    uint32x4_p y3 = (uint32x4_p)VecPermute(block4, block5, m2);

    for (int i = 0; i < static_cast<int>(rounds & ~1)-1; i += 2)
    {
#if CRYPTOPP_POWER8_AVAILABLE
        const uint32x4_p rk1 = vec_splats(subkeys[i]);
        const uint32x4_p rk2 = vec_splats(subkeys[i+1]);
#else
        const uint8x16_p m = {0,1,2,3, 0,1,2,3, 0,1,2,3, 0,1,2,3};
        uint32x4_p rk1 = VecLoad(subkeys+i);
        uint32x4_p rk2 = VecLoad(subkeys+i+1);
        rk1 = VecPermute(rk1, rk1, m);
        rk2 = VecPermute(rk2, rk2, m);
#endif
        y1 = VecXor(VecXor(y1, SIMON64_f(x1)), rk1);
        y2 = VecXor(VecXor(y2, SIMON64_f(x2)), rk1);
        y3 = VecXor(VecXor(y3, SIMON64_f(x3)), rk1);

        x1 = VecXor(VecXor(x1, SIMON64_f(y1)), rk2);
        x2 = VecXor(VecXor(x2, SIMON64_f(y2)), rk2);
        x3 = VecXor(VecXor(x3, SIMON64_f(y3)), rk2);
    }

    if (rounds & 1)
    {
#if CRYPTOPP_POWER8_AVAILABLE
        const uint32x4_p rk = vec_splats(subkeys[rounds-1]);
#else
        const uint8x16_p m = {0,1,2,3, 0,1,2,3, 0,1,2,3, 0,1,2,3};
        uint32x4_p rk = VecLoad(subkeys+rounds-1);
        rk = VecPermute(rk, rk, m);
#endif
        y1 = VecXor(VecXor(y1, SIMON64_f(x1)), rk);
        y2 = VecXor(VecXor(y2, SIMON64_f(x2)), rk);
        y3 = VecXor(VecXor(y3, SIMON64_f(x3)), rk);
        std::swap(x1, y1); std::swap(x2, y2); std::swap(x3, y3);
    }

#if (CRYPTOPP_BIG_ENDIAN)
    const uint8x16_p m3 = {19,18,17,16, 3,2,1,0, 23,22,21,20, 7,6,5,4};
    const uint8x16_p m4 = {27,26,25,24, 11,10,9,8, 31,30,29,28, 15,14,13,12};
#else
    const uint8x16_p m3 = {3,2,1,0, 19,18,17,16, 7,6,5,4, 23,22,21,20};
    const uint8x16_p m4 = {11,10,9,8, 27,26,25,24, 15,14,13,12, 31,30,29,28};
#endif

    // [A1 B1][A2 B2] ... => [A1 A2][B1 B2] ...
    block0 = (uint32x4_p)VecPermute(x1, y1, m3);
    block1 = (uint32x4_p)VecPermute(x1, y1, m4);
    block2 = (uint32x4_p)VecPermute(x2, y2, m3);
    block3 = (uint32x4_p)VecPermute(x2, y2, m4);
    block4 = (uint32x4_p)VecPermute(x3, y3, m3);
    block5 = (uint32x4_p)VecPermute(x3, y3, m4);
}

inline void SIMON64_Dec_6_Blocks(uint32x4_p &block0, uint32x4_p &block1,
            uint32x4_p &block2, uint32x4_p &block3, uint32x4_p &block4,
            uint32x4_p &block5, const word32 *subkeys, unsigned int rounds)
{
#if (CRYPTOPP_BIG_ENDIAN)
    const uint8x16_p m1 = {7,6,5,4, 15,14,13,12, 23,22,21,20, 31,30,29,28};
    const uint8x16_p m2 = {3,2,1,0, 11,10,9,8, 19,18,17,16, 27,26,25,24};
#else
    const uint8x16_p m1 = {3,2,1,0, 11,10,9,8, 19,18,17,16, 27,26,25,24};
    const uint8x16_p m2 = {7,6,5,4, 15,14,13,12, 23,22,21,20, 31,30,29,28};
#endif

    // [A1 A2][B1 B2] ... => [A1 B1][A2 B2] ...
    uint32x4_p x1 = (uint32x4_p)VecPermute(block0, block1, m1);
    uint32x4_p y1 = (uint32x4_p)VecPermute(block0, block1, m2);
    uint32x4_p x2 = (uint32x4_p)VecPermute(block2, block3, m1);
    uint32x4_p y2 = (uint32x4_p)VecPermute(block2, block3, m2);
    uint32x4_p x3 = (uint32x4_p)VecPermute(block4, block5, m1);
    uint32x4_p y3 = (uint32x4_p)VecPermute(block4, block5, m2);

    if (rounds & 1)
    {
        std::swap(x1, y1); std::swap(x2, y2); std::swap(x3, y3);

#if CRYPTOPP_POWER8_AVAILABLE
        const uint32x4_p rk = vec_splats(subkeys[rounds-1]);
#else
        const uint8x16_p m = {0,1,2,3, 0,1,2,3, 0,1,2,3, 0,1,2,3};
        uint32x4_p rk = VecLoad(subkeys+rounds-1);
        rk = VecPermute(rk, rk, m);
#endif
        y1 = VecXor(VecXor(y1, rk), SIMON64_f(x1));
        y2 = VecXor(VecXor(y2, rk), SIMON64_f(x2));
        y3 = VecXor(VecXor(y3, rk), SIMON64_f(x3));
        rounds--;
    }

    for (int i = static_cast<int>(rounds-2); i >= 0; i -= 2)
    {
#if CRYPTOPP_POWER8_AVAILABLE
        const uint32x4_p rk1 = vec_splats(subkeys[i+1]);
        const uint32x4_p rk2 = vec_splats(subkeys[i]);
#else
        const uint8x16_p m = {0,1,2,3, 0,1,2,3, 0,1,2,3, 0,1,2,3};
        uint32x4_p rk1 = VecLoad(subkeys+i+1);
        uint32x4_p rk2 = VecLoad(subkeys+i);
        rk1 = VecPermute(rk1, rk1, m);
        rk2 = VecPermute(rk2, rk2, m);
#endif
        x1 = VecXor(VecXor(x1, SIMON64_f(y1)), rk1);
        x2 = VecXor(VecXor(x2, SIMON64_f(y2)), rk1);
        x3 = VecXor(VecXor(x3, SIMON64_f(y3)), rk1);

        y1 = VecXor(VecXor(y1, SIMON64_f(x1)), rk2);
        y2 = VecXor(VecXor(y2, SIMON64_f(x2)), rk2);
        y3 = VecXor(VecXor(y3, SIMON64_f(x3)), rk2);
    }

#if (CRYPTOPP_BIG_ENDIAN)
    const uint8x16_p m3 = {19,18,17,16, 3,2,1,0, 23,22,21,20, 7,6,5,4};
    const uint8x16_p m4 = {27,26,25,24, 11,10,9,8, 31,30,29,28, 15,14,13,12};
#else
    const uint8x16_p m3 = {3,2,1,0, 19,18,17,16, 7,6,5,4, 23,22,21,20};
    const uint8x16_p m4 = {11,10,9,8, 27,26,25,24, 15,14,13,12, 31,30,29,28};
#endif

    // [A1 B1][A2 B2] ... => [A1 A2][B1 B2] ...
    block0 = (uint32x4_p)VecPermute(x1, y1, m3);
    block1 = (uint32x4_p)VecPermute(x1, y1, m4);
    block2 = (uint32x4_p)VecPermute(x2, y2, m3);
    block3 = (uint32x4_p)VecPermute(x2, y2, m4);
    block4 = (uint32x4_p)VecPermute(x3, y3, m3);
    block5 = (uint32x4_p)VecPermute(x3, y3, m4);
}

#endif  // CRYPTOPP_ALTIVEC_AVAILABLE

ANONYMOUS_NAMESPACE_END

///////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(CryptoPP)

// *************************** ARM NEON **************************** //

#if (CRYPTOPP_ARM_NEON_AVAILABLE)
size_t SIMON64_Enc_AdvancedProcessBlocks_NEON(const word32* subKeys, size_t rounds,
    const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags)
{
    return AdvancedProcessBlocks64_6x2_NEON(SIMON64_Enc_Block, SIMON64_Enc_6_Blocks,
        subKeys, rounds, inBlocks, xorBlocks, outBlocks, length, flags);
}

size_t SIMON64_Dec_AdvancedProcessBlocks_NEON(const word32* subKeys, size_t rounds,
    const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags)
{
    return AdvancedProcessBlocks64_6x2_NEON(SIMON64_Dec_Block, SIMON64_Dec_6_Blocks,
        subKeys, rounds, inBlocks, xorBlocks, outBlocks, length, flags);
}
#endif  // CRYPTOPP_ARM_NEON_AVAILABLE

// ***************************** IA-32 ***************************** //

#if defined(CRYPTOPP_SSE41_AVAILABLE)
size_t SIMON64_Enc_AdvancedProcessBlocks_SSE41(const word32* subKeys, size_t rounds,
    const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags)
{
    return AdvancedProcessBlocks64_6x2_SSE(SIMON64_Enc_Block, SIMON64_Enc_6_Blocks,
        subKeys, rounds, inBlocks, xorBlocks, outBlocks, length, flags);
}

size_t SIMON64_Dec_AdvancedProcessBlocks_SSE41(const word32* subKeys, size_t rounds,
    const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags)
{
    return AdvancedProcessBlocks64_6x2_SSE(SIMON64_Dec_Block, SIMON64_Dec_6_Blocks,
        subKeys, rounds, inBlocks, xorBlocks, outBlocks, length, flags);
}
#endif

// ***************************** Altivec ***************************** //

#if defined(CRYPTOPP_ALTIVEC_AVAILABLE)
size_t SIMON64_Enc_AdvancedProcessBlocks_ALTIVEC(const word32* subKeys, size_t rounds,
    const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags)
{
    return AdvancedProcessBlocks64_6x2_ALTIVEC(SIMON64_Enc_Block, SIMON64_Enc_6_Blocks,
        subKeys, rounds, inBlocks, xorBlocks, outBlocks, length, flags);
}

size_t SIMON64_Dec_AdvancedProcessBlocks_ALTIVEC(const word32* subKeys, size_t rounds,
    const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags)
{
    return AdvancedProcessBlocks64_6x2_ALTIVEC(SIMON64_Dec_Block, SIMON64_Dec_6_Blocks,
        subKeys, rounds, inBlocks, xorBlocks, outBlocks, length, flags);
}
#endif

NAMESPACE_END
