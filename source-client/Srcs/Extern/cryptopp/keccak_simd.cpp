// keccak_simd.cpp - written and placed in the public domain by Jeffrey Walton.
//
//    This source file uses intrinsics to gain access to SSE and
//    NEON instructions. A separate source file is needed because
//    additional CXXFLAGS are required to enable the appropriate
//    instructions sets in some build configurations.

// The XKCP package is provided by Guido Bertoni, Joan Daemen, Seth Hoffert,
// Michael Peeters, Gilles Van Assche, and Ronny Van Keer. The code was
// placed public domain by the authors.

// KeccakF1600x2_SSE is ParallelHash128. The SSE2 ParallelHash128
// implementation was extracted from XKCP using the following command.
//
// gcc -I lib/common -I lib/low/KeccakP-1600/Optimized
//   -I lib/low/KeccakP-1600-times2/SIMD128/SSE2ufull
//   lib/low/KeccakP-1600-times2/SIMD128/KeccakP-1600-times2-SIMD128.c -E

#include "pch.h"
#include "config.h"
#include "keccak.h"
#include "misc.h"

#if (CRYPTOPP_SSSE3_AVAILABLE)
# include <emmintrin.h>
# include <tmmintrin.h>
#endif

// Squash MS LNK4221 and libtool warnings
extern const char KECCAK_SIMD_FNAME[] = __FILE__;

NAMESPACE_BEGIN(CryptoPP)

#if (CRYPTOPP_SSSE3_AVAILABLE)

// The Keccak ParallelHash128 core function
extern void KeccakF1600x2_SSE(word64 *state);

// The F1600 round constants
extern const word64 KeccakF1600Constants[24];

const word64 rho8[2] = {W64LIT(0x0605040302010007), W64LIT(0x0E0D0C0B0A09080F)};
const word64 rho56[2] = {W64LIT(0x0007060504030201), W64LIT(0x080F0E0D0C0B0A09)};

#define V128 __m128i
#define CV128 const __m128i

#define CONST128(a)         _mm_load_si128((CV128 *)&(a))
#define XOREQ128(a, b)      a = _mm_xor_si128((a), (b))
#define UNPACKL(a, b)       _mm_unpacklo_epi64((a), (b))
#define UNPACKH(a, b)       _mm_unpackhi_epi64((a), (b))

#if defined(__XOP__)
# define ROL64in128(a, o)    _mm_roti_epi64((a), (o))
# define ROL64in128_8(a)     ROL64in128((a), 8)
# define ROL64in128_56(a)    ROL64in128((a), 56)
#else
# define ROL64in128(a, o)    _mm_or_si128(_mm_slli_epi64((a), (o)), _mm_srli_epi64(a, 64-(o)))
# define ROL64in128_8(a)     _mm_shuffle_epi8((a), CONST128(rho8))
# define ROL64in128_56(a)    _mm_shuffle_epi8((a), CONST128(rho56))
#endif

// Damn Visual Studio is missing too many intrinsics...
inline __m128i SPLAT64(const word64 a)
{
#if defined(_MSC_VER)
    double x; std::memcpy(&x, &a, 8);
    return _mm_castpd_si128(_mm_loaddup_pd(&x));
#else
    return _mm_set1_epi64x(a);
#endif
}

// The Keccak ParallelHash128 core function
void KeccakF1600x2_SSE(word64 *state)
{
    V128 *statesAsLanes = (V128 *)state;

    V128 Aba, Abe, Abi, Abo, Abu;
    V128 Aga, Age, Agi, Ago, Agu;
    V128 Aka, Ake, Aki, Ako, Aku;
    V128 Ama, Ame, Ami, Amo, Amu;
    V128 Asa, Ase, Asi, Aso, Asu;
    V128 Bba, Bbe, Bbi, Bbo, Bbu;
    V128 Bga, Bge, Bgi, Bgo, Bgu;
    V128 Bka, Bke, Bki, Bko, Bku;
    V128 Bma, Bme, Bmi, Bmo, Bmu;
    V128 Bsa, Bse, Bsi, Bso, Bsu;
    V128 Ca, Ce, Ci, Co, Cu;
    V128 Da, De, Di, Do, Du;
    V128 Eba, Ebe, Ebi, Ebo, Ebu;
    V128 Ega, Ege, Egi, Ego, Egu;
    V128 Eka, Eke, Eki, Eko, Eku;
    V128 Ema, Eme, Emi, Emo, Emu;
    V128 Esa, Ese, Esi, Eso, Esu;

    Aba = _mm_loadu_si128((CV128 *)&(statesAsLanes[ 0]));
    Abe = _mm_loadu_si128((CV128 *)&(statesAsLanes[ 1]));
    Abi = _mm_loadu_si128((CV128 *)&(statesAsLanes[ 2]));
    Abo = _mm_loadu_si128((CV128 *)&(statesAsLanes[ 3]));
    Abu = _mm_loadu_si128((CV128 *)&(statesAsLanes[ 4]));
    Aga = _mm_loadu_si128((CV128 *)&(statesAsLanes[ 5]));
    Age = _mm_loadu_si128((CV128 *)&(statesAsLanes[ 6]));
    Agi = _mm_loadu_si128((CV128 *)&(statesAsLanes[ 7]));
    Ago = _mm_loadu_si128((CV128 *)&(statesAsLanes[ 8]));
    Agu = _mm_loadu_si128((CV128 *)&(statesAsLanes[ 9]));
    Aka = _mm_loadu_si128((CV128 *)&(statesAsLanes[10]));
    Ake = _mm_loadu_si128((CV128 *)&(statesAsLanes[11]));
    Aki = _mm_loadu_si128((CV128 *)&(statesAsLanes[12]));
    Ako = _mm_loadu_si128((CV128 *)&(statesAsLanes[13]));
    Aku = _mm_loadu_si128((CV128 *)&(statesAsLanes[14]));
    Ama = _mm_loadu_si128((CV128 *)&(statesAsLanes[15]));
    Ame = _mm_loadu_si128((CV128 *)&(statesAsLanes[16]));
    Ami = _mm_loadu_si128((CV128 *)&(statesAsLanes[17]));
    Amo = _mm_loadu_si128((CV128 *)&(statesAsLanes[18]));
    Amu = _mm_loadu_si128((CV128 *)&(statesAsLanes[19]));
    Asa = _mm_loadu_si128((CV128 *)&(statesAsLanes[20]));
    Ase = _mm_loadu_si128((CV128 *)&(statesAsLanes[21]));
    Asi = _mm_loadu_si128((CV128 *)&(statesAsLanes[22]));
    Aso = _mm_loadu_si128((CV128 *)&(statesAsLanes[23]));
    Asu = _mm_loadu_si128((CV128 *)&(statesAsLanes[24]));

    Ca = _mm_xor_si128(Aba, _mm_xor_si128(Aga, _mm_xor_si128(Aka, _mm_xor_si128(Ama, Asa))));
    Ce = _mm_xor_si128(Abe, _mm_xor_si128(Age, _mm_xor_si128(Ake, _mm_xor_si128(Ame, Ase))));
    Ci = _mm_xor_si128(Abi, _mm_xor_si128(Agi, _mm_xor_si128(Aki, _mm_xor_si128(Ami, Asi))));
    Co = _mm_xor_si128(Abo, _mm_xor_si128(Ago, _mm_xor_si128(Ako, _mm_xor_si128(Amo, Aso))));
    Cu = _mm_xor_si128(Abu, _mm_xor_si128(Agu, _mm_xor_si128(Aku, _mm_xor_si128(Amu, Asu))));
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));

    Aba = _mm_xor_si128(Aba, Da);
    Bba = Aba;
    Age = _mm_xor_si128(Age, De);
    Bbe = ROL64in128(Age, 44);
    Aki = _mm_xor_si128(Aki, Di);
    Bbi = ROL64in128(Aki, 43);
    Eba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Eba = _mm_xor_si128(Eba, SPLAT64(KeccakF1600Constants[0]));
    Ca = Eba;
    Amo = _mm_xor_si128(Amo, Do);
    Bbo = ROL64in128(Amo, 21);
    Ebe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Ebe;
    Asu = _mm_xor_si128(Asu, Du);
    Bbu = ROL64in128(Asu, 14);
    Ebi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Ebi;
    Ebo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Ebo;
    Ebu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Ebu;
    Abo = _mm_xor_si128(Abo, Do);
    Bga = ROL64in128(Abo, 28);
    Agu = _mm_xor_si128(Agu, Du);
    Bge = ROL64in128(Agu, 20);
    Aka = _mm_xor_si128(Aka, Da);
    Bgi = ROL64in128(Aka, 3);
    Ega = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Ega);
    Ame = _mm_xor_si128(Ame, De);
    Bgo = ROL64in128(Ame, 45);
    Ege = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Ege);
    Asi = _mm_xor_si128(Asi, Di);
    Bgu = ROL64in128(Asi, 61);
    Egi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Egi);
    Ego = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ego);
    Egu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Egu);
    Abe = _mm_xor_si128(Abe, De);
    Bka = ROL64in128(Abe, 1);
    Agi = _mm_xor_si128(Agi, Di);
    Bke = ROL64in128(Agi, 6);
    Ako = _mm_xor_si128(Ako, Do);
    Bki = ROL64in128(Ako, 25);
    Eka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Eka);
    Amu = _mm_xor_si128(Amu, Du);
    Bko = ROL64in128_8(Amu);
    Eke = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Eke);
    Asa = _mm_xor_si128(Asa, Da);
    Bku = ROL64in128(Asa, 18);
    Eki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Eki);
    Eko = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Eko);
    Eku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Eku);
    Abu = _mm_xor_si128(Abu, Du);
    Bma = ROL64in128(Abu, 27);
    Aga = _mm_xor_si128(Aga, Da);
    Bme = ROL64in128(Aga, 36);
    Ake = _mm_xor_si128(Ake, De);
    Bmi = ROL64in128(Ake, 10);
    Ema = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ema);
    Ami = _mm_xor_si128(Ami, Di);
    Bmo = ROL64in128(Ami, 15);
    Eme = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Eme);
    Aso = _mm_xor_si128(Aso, Do);
    Bmu = ROL64in128_56(Aso);
    Emi = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Emi);
    Emo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Emo);
    Emu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Emu);
    Abi = _mm_xor_si128(Abi, Di);
    Bsa = ROL64in128(Abi, 62);
    Ago = _mm_xor_si128(Ago, Do);
    Bse = ROL64in128(Ago, 55);
    Aku = _mm_xor_si128(Aku, Du);
    Bsi = ROL64in128(Aku, 39);
    Esa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Esa);
    Ama = _mm_xor_si128(Ama, Da);
    Bso = ROL64in128(Ama, 41);
    Ese = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ese);
    Ase = _mm_xor_si128(Ase, De);
    Bsu = ROL64in128(Ase, 2);
    Esi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Esi);
    Eso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Eso);
    Esu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Esu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Eba = _mm_xor_si128(Eba, Da);
    Bba = Eba;
    Ege = _mm_xor_si128(Ege, De);
    Bbe = ROL64in128(Ege, 44);
    Eki = _mm_xor_si128(Eki, Di);
    Bbi = ROL64in128(Eki, 43);
    Aba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Aba = _mm_xor_si128(Aba, SPLAT64(KeccakF1600Constants[1]));
    Ca = Aba;
    Emo = _mm_xor_si128(Emo, Do);
    Bbo = ROL64in128(Emo, 21);
    Abe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Abe;
    Esu = _mm_xor_si128(Esu, Du);
    Bbu = ROL64in128(Esu, 14);
    Abi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Abi;
    Abo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Abo;
    Abu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Abu;
    Ebo = _mm_xor_si128(Ebo, Do);
    Bga = ROL64in128(Ebo, 28);
    Egu = _mm_xor_si128(Egu, Du);
    Bge = ROL64in128(Egu, 20);
    Eka = _mm_xor_si128(Eka, Da);
    Bgi = ROL64in128(Eka, 3);
    Aga = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Aga);
    Eme = _mm_xor_si128(Eme, De);
    Bgo = ROL64in128(Eme, 45);
    Age = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Age);
    Esi = _mm_xor_si128(Esi, Di);
    Bgu = ROL64in128(Esi, 61);
    Agi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Agi);
    Ago = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ago);
    Agu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Agu);
    Ebe = _mm_xor_si128(Ebe, De);
    Bka = ROL64in128(Ebe, 1);
    Egi = _mm_xor_si128(Egi, Di);
    Bke = ROL64in128(Egi, 6);
    Eko = _mm_xor_si128(Eko, Do);
    Bki = ROL64in128(Eko, 25);
    Aka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Aka);
    Emu = _mm_xor_si128(Emu, Du);
    Bko = ROL64in128_8(Emu);
    Ake = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Ake);
    Esa = _mm_xor_si128(Esa, Da);
    Bku = ROL64in128(Esa, 18);
    Aki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Aki);
    Ako = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Ako);
    Aku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Aku);
    Ebu = _mm_xor_si128(Ebu, Du);
    Bma = ROL64in128(Ebu, 27);
    Ega = _mm_xor_si128(Ega, Da);
    Bme = ROL64in128(Ega, 36);
    Eke = _mm_xor_si128(Eke, De);
    Bmi = ROL64in128(Eke, 10);
    Ama = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ama);
    Emi = _mm_xor_si128(Emi, Di);
    Bmo = ROL64in128(Emi, 15);
    Ame = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Ame);
    Eso = _mm_xor_si128(Eso, Do);
    Bmu = ROL64in128_56(Eso);
    Ami = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Ami);
    Amo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Amo);
    Amu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Amu);
    Ebi = _mm_xor_si128(Ebi, Di);
    Bsa = ROL64in128(Ebi, 62);
    Ego = _mm_xor_si128(Ego, Do);
    Bse = ROL64in128(Ego, 55);
    Eku = _mm_xor_si128(Eku, Du);
    Bsi = ROL64in128(Eku, 39);
    Asa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Asa);
    Ema = _mm_xor_si128(Ema, Da);
    Bso = ROL64in128(Ema, 41);
    Ase = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ase);
    Ese = _mm_xor_si128(Ese, De);
    Bsu = ROL64in128(Ese, 2);
    Asi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Asi);
    Aso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Aso);
    Asu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Asu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Aba = _mm_xor_si128(Aba, Da);
    Bba = Aba;
    Age = _mm_xor_si128(Age, De);
    Bbe = ROL64in128(Age, 44);
    Aki = _mm_xor_si128(Aki, Di);
    Bbi = ROL64in128(Aki, 43);
    Eba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Eba = _mm_xor_si128(Eba, SPLAT64(KeccakF1600Constants[2]));
    Ca = Eba;
    Amo = _mm_xor_si128(Amo, Do);
    Bbo = ROL64in128(Amo, 21);
    Ebe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Ebe;
    Asu = _mm_xor_si128(Asu, Du);
    Bbu = ROL64in128(Asu, 14);
    Ebi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Ebi;
    Ebo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Ebo;
    Ebu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Ebu;
    Abo = _mm_xor_si128(Abo, Do);
    Bga = ROL64in128(Abo, 28);
    Agu = _mm_xor_si128(Agu, Du);
    Bge = ROL64in128(Agu, 20);
    Aka = _mm_xor_si128(Aka, Da);
    Bgi = ROL64in128(Aka, 3);
    Ega = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Ega);
    Ame = _mm_xor_si128(Ame, De);
    Bgo = ROL64in128(Ame, 45);
    Ege = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Ege);
    Asi = _mm_xor_si128(Asi, Di);
    Bgu = ROL64in128(Asi, 61);
    Egi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Egi);
    Ego = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ego);
    Egu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Egu);
    Abe = _mm_xor_si128(Abe, De);
    Bka = ROL64in128(Abe, 1);
    Agi = _mm_xor_si128(Agi, Di);
    Bke = ROL64in128(Agi, 6);
    Ako = _mm_xor_si128(Ako, Do);
    Bki = ROL64in128(Ako, 25);
    Eka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Eka);
    Amu = _mm_xor_si128(Amu, Du);
    Bko = ROL64in128_8(Amu);
    Eke = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Eke);
    Asa = _mm_xor_si128(Asa, Da);
    Bku = ROL64in128(Asa, 18);
    Eki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Eki);
    Eko = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Eko);
    Eku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Eku);
    Abu = _mm_xor_si128(Abu, Du);
    Bma = ROL64in128(Abu, 27);
    Aga = _mm_xor_si128(Aga, Da);
    Bme = ROL64in128(Aga, 36);
    Ake = _mm_xor_si128(Ake, De);
    Bmi = ROL64in128(Ake, 10);
    Ema = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ema);
    Ami = _mm_xor_si128(Ami, Di);
    Bmo = ROL64in128(Ami, 15);
    Eme = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Eme);
    Aso = _mm_xor_si128(Aso, Do);
    Bmu = ROL64in128_56(Aso);
    Emi = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Emi);
    Emo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Emo);
    Emu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Emu);
    Abi = _mm_xor_si128(Abi, Di);
    Bsa = ROL64in128(Abi, 62);
    Ago = _mm_xor_si128(Ago, Do);
    Bse = ROL64in128(Ago, 55);
    Aku = _mm_xor_si128(Aku, Du);
    Bsi = ROL64in128(Aku, 39);
    Esa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Esa);
    Ama = _mm_xor_si128(Ama, Da);
    Bso = ROL64in128(Ama, 41);
    Ese = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ese);
    Ase = _mm_xor_si128(Ase, De);
    Bsu = ROL64in128(Ase, 2);
    Esi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Esi);
    Eso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Eso);
    Esu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Esu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Eba = _mm_xor_si128(Eba, Da);
    Bba = Eba;
    Ege = _mm_xor_si128(Ege, De);
    Bbe = ROL64in128(Ege, 44);
    Eki = _mm_xor_si128(Eki, Di);
    Bbi = ROL64in128(Eki, 43);
    Aba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Aba = _mm_xor_si128(Aba, SPLAT64(KeccakF1600Constants[3]));
    Ca = Aba;
    Emo = _mm_xor_si128(Emo, Do);
    Bbo = ROL64in128(Emo, 21);
    Abe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Abe;
    Esu = _mm_xor_si128(Esu, Du);
    Bbu = ROL64in128(Esu, 14);
    Abi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Abi;
    Abo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Abo;
    Abu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Abu;
    Ebo = _mm_xor_si128(Ebo, Do);
    Bga = ROL64in128(Ebo, 28);
    Egu = _mm_xor_si128(Egu, Du);
    Bge = ROL64in128(Egu, 20);
    Eka = _mm_xor_si128(Eka, Da);
    Bgi = ROL64in128(Eka, 3);
    Aga = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Aga);
    Eme = _mm_xor_si128(Eme, De);
    Bgo = ROL64in128(Eme, 45);
    Age = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Age);
    Esi = _mm_xor_si128(Esi, Di);
    Bgu = ROL64in128(Esi, 61);
    Agi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Agi);
    Ago = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ago);
    Agu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Agu);
    Ebe = _mm_xor_si128(Ebe, De);
    Bka = ROL64in128(Ebe, 1);
    Egi = _mm_xor_si128(Egi, Di);
    Bke = ROL64in128(Egi, 6);
    Eko = _mm_xor_si128(Eko, Do);
    Bki = ROL64in128(Eko, 25);
    Aka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Aka);
    Emu = _mm_xor_si128(Emu, Du);
    Bko = ROL64in128_8(Emu);
    Ake = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Ake);
    Esa = _mm_xor_si128(Esa, Da);
    Bku = ROL64in128(Esa, 18);
    Aki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Aki);
    Ako = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Ako);
    Aku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Aku);
    Ebu = _mm_xor_si128(Ebu, Du);
    Bma = ROL64in128(Ebu, 27);
    Ega = _mm_xor_si128(Ega, Da);
    Bme = ROL64in128(Ega, 36);
    Eke = _mm_xor_si128(Eke, De);
    Bmi = ROL64in128(Eke, 10);
    Ama = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ama);
    Emi = _mm_xor_si128(Emi, Di);
    Bmo = ROL64in128(Emi, 15);
    Ame = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Ame);
    Eso = _mm_xor_si128(Eso, Do);
    Bmu = ROL64in128_56(Eso);
    Ami = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Ami);
    Amo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Amo);
    Amu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Amu);
    Ebi = _mm_xor_si128(Ebi, Di);
    Bsa = ROL64in128(Ebi, 62);
    Ego = _mm_xor_si128(Ego, Do);
    Bse = ROL64in128(Ego, 55);
    Eku = _mm_xor_si128(Eku, Du);
    Bsi = ROL64in128(Eku, 39);
    Asa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Asa);
    Ema = _mm_xor_si128(Ema, Da);
    Bso = ROL64in128(Ema, 41);
    Ase = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ase);
    Ese = _mm_xor_si128(Ese, De);
    Bsu = ROL64in128(Ese, 2);
    Asi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Asi);
    Aso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Aso);
    Asu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Asu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Aba = _mm_xor_si128(Aba, Da);
    Bba = Aba;
    Age = _mm_xor_si128(Age, De);
    Bbe = ROL64in128(Age, 44);
    Aki = _mm_xor_si128(Aki, Di);
    Bbi = ROL64in128(Aki, 43);
    Eba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Eba = _mm_xor_si128(Eba, SPLAT64(KeccakF1600Constants[4]));
    Ca = Eba;
    Amo = _mm_xor_si128(Amo, Do);
    Bbo = ROL64in128(Amo, 21);
    Ebe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Ebe;
    Asu = _mm_xor_si128(Asu, Du);
    Bbu = ROL64in128(Asu, 14);
    Ebi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Ebi;
    Ebo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Ebo;
    Ebu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Ebu;
    Abo = _mm_xor_si128(Abo, Do);
    Bga = ROL64in128(Abo, 28);
    Agu = _mm_xor_si128(Agu, Du);
    Bge = ROL64in128(Agu, 20);
    Aka = _mm_xor_si128(Aka, Da);
    Bgi = ROL64in128(Aka, 3);
    Ega = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Ega);
    Ame = _mm_xor_si128(Ame, De);
    Bgo = ROL64in128(Ame, 45);
    Ege = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Ege);
    Asi = _mm_xor_si128(Asi, Di);
    Bgu = ROL64in128(Asi, 61);
    Egi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Egi);
    Ego = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ego);
    Egu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Egu);
    Abe = _mm_xor_si128(Abe, De);
    Bka = ROL64in128(Abe, 1);
    Agi = _mm_xor_si128(Agi, Di);
    Bke = ROL64in128(Agi, 6);
    Ako = _mm_xor_si128(Ako, Do);
    Bki = ROL64in128(Ako, 25);
    Eka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Eka);
    Amu = _mm_xor_si128(Amu, Du);
    Bko = ROL64in128_8(Amu);
    Eke = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Eke);
    Asa = _mm_xor_si128(Asa, Da);
    Bku = ROL64in128(Asa, 18);
    Eki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Eki);
    Eko = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Eko);
    Eku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Eku);
    Abu = _mm_xor_si128(Abu, Du);
    Bma = ROL64in128(Abu, 27);
    Aga = _mm_xor_si128(Aga, Da);
    Bme = ROL64in128(Aga, 36);
    Ake = _mm_xor_si128(Ake, De);
    Bmi = ROL64in128(Ake, 10);
    Ema = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ema);
    Ami = _mm_xor_si128(Ami, Di);
    Bmo = ROL64in128(Ami, 15);
    Eme = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Eme);
    Aso = _mm_xor_si128(Aso, Do);
    Bmu = ROL64in128_56(Aso);
    Emi = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Emi);
    Emo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Emo);
    Emu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Emu);
    Abi = _mm_xor_si128(Abi, Di);
    Bsa = ROL64in128(Abi, 62);
    Ago = _mm_xor_si128(Ago, Do);
    Bse = ROL64in128(Ago, 55);
    Aku = _mm_xor_si128(Aku, Du);
    Bsi = ROL64in128(Aku, 39);
    Esa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Esa);
    Ama = _mm_xor_si128(Ama, Da);
    Bso = ROL64in128(Ama, 41);
    Ese = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ese);
    Ase = _mm_xor_si128(Ase, De);
    Bsu = ROL64in128(Ase, 2);
    Esi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Esi);
    Eso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Eso);
    Esu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Esu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Eba = _mm_xor_si128(Eba, Da);
    Bba = Eba;
    Ege = _mm_xor_si128(Ege, De);
    Bbe = ROL64in128(Ege, 44);
    Eki = _mm_xor_si128(Eki, Di);
    Bbi = ROL64in128(Eki, 43);
    Aba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Aba = _mm_xor_si128(Aba, SPLAT64(KeccakF1600Constants[5]));
    Ca = Aba;
    Emo = _mm_xor_si128(Emo, Do);
    Bbo = ROL64in128(Emo, 21);
    Abe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Abe;
    Esu = _mm_xor_si128(Esu, Du);
    Bbu = ROL64in128(Esu, 14);
    Abi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Abi;
    Abo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Abo;
    Abu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Abu;
    Ebo = _mm_xor_si128(Ebo, Do);
    Bga = ROL64in128(Ebo, 28);
    Egu = _mm_xor_si128(Egu, Du);
    Bge = ROL64in128(Egu, 20);
    Eka = _mm_xor_si128(Eka, Da);
    Bgi = ROL64in128(Eka, 3);
    Aga = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Aga);
    Eme = _mm_xor_si128(Eme, De);
    Bgo = ROL64in128(Eme, 45);
    Age = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Age);
    Esi = _mm_xor_si128(Esi, Di);
    Bgu = ROL64in128(Esi, 61);
    Agi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Agi);
    Ago = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ago);
    Agu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Agu);
    Ebe = _mm_xor_si128(Ebe, De);
    Bka = ROL64in128(Ebe, 1);
    Egi = _mm_xor_si128(Egi, Di);
    Bke = ROL64in128(Egi, 6);
    Eko = _mm_xor_si128(Eko, Do);
    Bki = ROL64in128(Eko, 25);
    Aka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Aka);
    Emu = _mm_xor_si128(Emu, Du);
    Bko = ROL64in128_8(Emu);
    Ake = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Ake);
    Esa = _mm_xor_si128(Esa, Da);
    Bku = ROL64in128(Esa, 18);
    Aki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Aki);
    Ako = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Ako);
    Aku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Aku);
    Ebu = _mm_xor_si128(Ebu, Du);
    Bma = ROL64in128(Ebu, 27);
    Ega = _mm_xor_si128(Ega, Da);
    Bme = ROL64in128(Ega, 36);
    Eke = _mm_xor_si128(Eke, De);
    Bmi = ROL64in128(Eke, 10);
    Ama = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ama);
    Emi = _mm_xor_si128(Emi, Di);
    Bmo = ROL64in128(Emi, 15);
    Ame = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Ame);
    Eso = _mm_xor_si128(Eso, Do);
    Bmu = ROL64in128_56(Eso);
    Ami = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Ami);
    Amo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Amo);
    Amu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Amu);
    Ebi = _mm_xor_si128(Ebi, Di);
    Bsa = ROL64in128(Ebi, 62);
    Ego = _mm_xor_si128(Ego, Do);
    Bse = ROL64in128(Ego, 55);
    Eku = _mm_xor_si128(Eku, Du);
    Bsi = ROL64in128(Eku, 39);
    Asa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Asa);
    Ema = _mm_xor_si128(Ema, Da);
    Bso = ROL64in128(Ema, 41);
    Ase = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ase);
    Ese = _mm_xor_si128(Ese, De);
    Bsu = ROL64in128(Ese, 2);
    Asi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Asi);
    Aso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Aso);
    Asu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Asu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Aba = _mm_xor_si128(Aba, Da);
    Bba = Aba;
    Age = _mm_xor_si128(Age, De);
    Bbe = ROL64in128(Age, 44);
    Aki = _mm_xor_si128(Aki, Di);
    Bbi = ROL64in128(Aki, 43);
    Eba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Eba = _mm_xor_si128(Eba, SPLAT64(KeccakF1600Constants[6]));
    Ca = Eba;
    Amo = _mm_xor_si128(Amo, Do);
    Bbo = ROL64in128(Amo, 21);
    Ebe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Ebe;
    Asu = _mm_xor_si128(Asu, Du);
    Bbu = ROL64in128(Asu, 14);
    Ebi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Ebi;
    Ebo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Ebo;
    Ebu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Ebu;
    Abo = _mm_xor_si128(Abo, Do);
    Bga = ROL64in128(Abo, 28);
    Agu = _mm_xor_si128(Agu, Du);
    Bge = ROL64in128(Agu, 20);
    Aka = _mm_xor_si128(Aka, Da);
    Bgi = ROL64in128(Aka, 3);
    Ega = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Ega);
    Ame = _mm_xor_si128(Ame, De);
    Bgo = ROL64in128(Ame, 45);
    Ege = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Ege);
    Asi = _mm_xor_si128(Asi, Di);
    Bgu = ROL64in128(Asi, 61);
    Egi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Egi);
    Ego = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ego);
    Egu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Egu);
    Abe = _mm_xor_si128(Abe, De);
    Bka = ROL64in128(Abe, 1);
    Agi = _mm_xor_si128(Agi, Di);
    Bke = ROL64in128(Agi, 6);
    Ako = _mm_xor_si128(Ako, Do);
    Bki = ROL64in128(Ako, 25);
    Eka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Eka);
    Amu = _mm_xor_si128(Amu, Du);
    Bko = ROL64in128_8(Amu);
    Eke = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Eke);
    Asa = _mm_xor_si128(Asa, Da);
    Bku = ROL64in128(Asa, 18);
    Eki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Eki);
    Eko = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Eko);
    Eku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Eku);
    Abu = _mm_xor_si128(Abu, Du);
    Bma = ROL64in128(Abu, 27);
    Aga = _mm_xor_si128(Aga, Da);
    Bme = ROL64in128(Aga, 36);
    Ake = _mm_xor_si128(Ake, De);
    Bmi = ROL64in128(Ake, 10);
    Ema = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ema);
    Ami = _mm_xor_si128(Ami, Di);
    Bmo = ROL64in128(Ami, 15);
    Eme = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Eme);
    Aso = _mm_xor_si128(Aso, Do);
    Bmu = ROL64in128_56(Aso);
    Emi = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Emi);
    Emo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Emo);
    Emu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Emu);
    Abi = _mm_xor_si128(Abi, Di);
    Bsa = ROL64in128(Abi, 62);
    Ago = _mm_xor_si128(Ago, Do);
    Bse = ROL64in128(Ago, 55);
    Aku = _mm_xor_si128(Aku, Du);
    Bsi = ROL64in128(Aku, 39);
    Esa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Esa);
    Ama = _mm_xor_si128(Ama, Da);
    Bso = ROL64in128(Ama, 41);
    Ese = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ese);
    Ase = _mm_xor_si128(Ase, De);
    Bsu = ROL64in128(Ase, 2);
    Esi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Esi);
    Eso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Eso);
    Esu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Esu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Eba = _mm_xor_si128(Eba, Da);
    Bba = Eba;
    Ege = _mm_xor_si128(Ege, De);
    Bbe = ROL64in128(Ege, 44);
    Eki = _mm_xor_si128(Eki, Di);
    Bbi = ROL64in128(Eki, 43);
    Aba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Aba = _mm_xor_si128(Aba, SPLAT64(KeccakF1600Constants[7]));
    Ca = Aba;
    Emo = _mm_xor_si128(Emo, Do);
    Bbo = ROL64in128(Emo, 21);
    Abe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Abe;
    Esu = _mm_xor_si128(Esu, Du);
    Bbu = ROL64in128(Esu, 14);
    Abi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Abi;
    Abo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Abo;
    Abu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Abu;
    Ebo = _mm_xor_si128(Ebo, Do);
    Bga = ROL64in128(Ebo, 28);
    Egu = _mm_xor_si128(Egu, Du);
    Bge = ROL64in128(Egu, 20);
    Eka = _mm_xor_si128(Eka, Da);
    Bgi = ROL64in128(Eka, 3);
    Aga = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Aga);
    Eme = _mm_xor_si128(Eme, De);
    Bgo = ROL64in128(Eme, 45);
    Age = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Age);
    Esi = _mm_xor_si128(Esi, Di);
    Bgu = ROL64in128(Esi, 61);
    Agi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Agi);
    Ago = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ago);
    Agu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Agu);
    Ebe = _mm_xor_si128(Ebe, De);
    Bka = ROL64in128(Ebe, 1);
    Egi = _mm_xor_si128(Egi, Di);
    Bke = ROL64in128(Egi, 6);
    Eko = _mm_xor_si128(Eko, Do);
    Bki = ROL64in128(Eko, 25);
    Aka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Aka);
    Emu = _mm_xor_si128(Emu, Du);
    Bko = ROL64in128_8(Emu);
    Ake = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Ake);
    Esa = _mm_xor_si128(Esa, Da);
    Bku = ROL64in128(Esa, 18);
    Aki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Aki);
    Ako = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Ako);
    Aku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Aku);
    Ebu = _mm_xor_si128(Ebu, Du);
    Bma = ROL64in128(Ebu, 27);
    Ega = _mm_xor_si128(Ega, Da);
    Bme = ROL64in128(Ega, 36);
    Eke = _mm_xor_si128(Eke, De);
    Bmi = ROL64in128(Eke, 10);
    Ama = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ama);
    Emi = _mm_xor_si128(Emi, Di);
    Bmo = ROL64in128(Emi, 15);
    Ame = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Ame);
    Eso = _mm_xor_si128(Eso, Do);
    Bmu = ROL64in128_56(Eso);
    Ami = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Ami);
    Amo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Amo);
    Amu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Amu);
    Ebi = _mm_xor_si128(Ebi, Di);
    Bsa = ROL64in128(Ebi, 62);
    Ego = _mm_xor_si128(Ego, Do);
    Bse = ROL64in128(Ego, 55);
    Eku = _mm_xor_si128(Eku, Du);
    Bsi = ROL64in128(Eku, 39);
    Asa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Asa);
    Ema = _mm_xor_si128(Ema, Da);
    Bso = ROL64in128(Ema, 41);
    Ase = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ase);
    Ese = _mm_xor_si128(Ese, De);
    Bsu = ROL64in128(Ese, 2);
    Asi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Asi);
    Aso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Aso);
    Asu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Asu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Aba = _mm_xor_si128(Aba, Da);
    Bba = Aba;
    Age = _mm_xor_si128(Age, De);
    Bbe = ROL64in128(Age, 44);
    Aki = _mm_xor_si128(Aki, Di);
    Bbi = ROL64in128(Aki, 43);
    Eba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Eba = _mm_xor_si128(Eba, SPLAT64(KeccakF1600Constants[8]));
    Ca = Eba;
    Amo = _mm_xor_si128(Amo, Do);
    Bbo = ROL64in128(Amo, 21);
    Ebe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Ebe;
    Asu = _mm_xor_si128(Asu, Du);
    Bbu = ROL64in128(Asu, 14);
    Ebi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Ebi;
    Ebo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Ebo;
    Ebu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Ebu;
    Abo = _mm_xor_si128(Abo, Do);
    Bga = ROL64in128(Abo, 28);
    Agu = _mm_xor_si128(Agu, Du);
    Bge = ROL64in128(Agu, 20);
    Aka = _mm_xor_si128(Aka, Da);
    Bgi = ROL64in128(Aka, 3);
    Ega = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Ega);
    Ame = _mm_xor_si128(Ame, De);
    Bgo = ROL64in128(Ame, 45);
    Ege = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Ege);
    Asi = _mm_xor_si128(Asi, Di);
    Bgu = ROL64in128(Asi, 61);
    Egi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Egi);
    Ego = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ego);
    Egu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Egu);
    Abe = _mm_xor_si128(Abe, De);
    Bka = ROL64in128(Abe, 1);
    Agi = _mm_xor_si128(Agi, Di);
    Bke = ROL64in128(Agi, 6);
    Ako = _mm_xor_si128(Ako, Do);
    Bki = ROL64in128(Ako, 25);
    Eka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Eka);
    Amu = _mm_xor_si128(Amu, Du);
    Bko = ROL64in128_8(Amu);
    Eke = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Eke);
    Asa = _mm_xor_si128(Asa, Da);
    Bku = ROL64in128(Asa, 18);
    Eki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Eki);
    Eko = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Eko);
    Eku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Eku);
    Abu = _mm_xor_si128(Abu, Du);
    Bma = ROL64in128(Abu, 27);
    Aga = _mm_xor_si128(Aga, Da);
    Bme = ROL64in128(Aga, 36);
    Ake = _mm_xor_si128(Ake, De);
    Bmi = ROL64in128(Ake, 10);
    Ema = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ema);
    Ami = _mm_xor_si128(Ami, Di);
    Bmo = ROL64in128(Ami, 15);
    Eme = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Eme);
    Aso = _mm_xor_si128(Aso, Do);
    Bmu = ROL64in128_56(Aso);
    Emi = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Emi);
    Emo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Emo);
    Emu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Emu);
    Abi = _mm_xor_si128(Abi, Di);
    Bsa = ROL64in128(Abi, 62);
    Ago = _mm_xor_si128(Ago, Do);
    Bse = ROL64in128(Ago, 55);
    Aku = _mm_xor_si128(Aku, Du);
    Bsi = ROL64in128(Aku, 39);
    Esa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Esa);
    Ama = _mm_xor_si128(Ama, Da);
    Bso = ROL64in128(Ama, 41);
    Ese = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ese);
    Ase = _mm_xor_si128(Ase, De);
    Bsu = ROL64in128(Ase, 2);
    Esi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Esi);
    Eso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Eso);
    Esu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Esu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Eba = _mm_xor_si128(Eba, Da);
    Bba = Eba;
    Ege = _mm_xor_si128(Ege, De);
    Bbe = ROL64in128(Ege, 44);
    Eki = _mm_xor_si128(Eki, Di);
    Bbi = ROL64in128(Eki, 43);
    Aba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Aba = _mm_xor_si128(Aba, SPLAT64(KeccakF1600Constants[9]));
    Ca = Aba;
    Emo = _mm_xor_si128(Emo, Do);
    Bbo = ROL64in128(Emo, 21);
    Abe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Abe;
    Esu = _mm_xor_si128(Esu, Du);
    Bbu = ROL64in128(Esu, 14);
    Abi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Abi;
    Abo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Abo;
    Abu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Abu;
    Ebo = _mm_xor_si128(Ebo, Do);
    Bga = ROL64in128(Ebo, 28);
    Egu = _mm_xor_si128(Egu, Du);
    Bge = ROL64in128(Egu, 20);
    Eka = _mm_xor_si128(Eka, Da);
    Bgi = ROL64in128(Eka, 3);
    Aga = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Aga);
    Eme = _mm_xor_si128(Eme, De);
    Bgo = ROL64in128(Eme, 45);
    Age = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Age);
    Esi = _mm_xor_si128(Esi, Di);
    Bgu = ROL64in128(Esi, 61);
    Agi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Agi);
    Ago = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ago);
    Agu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Agu);
    Ebe = _mm_xor_si128(Ebe, De);
    Bka = ROL64in128(Ebe, 1);
    Egi = _mm_xor_si128(Egi, Di);
    Bke = ROL64in128(Egi, 6);
    Eko = _mm_xor_si128(Eko, Do);
    Bki = ROL64in128(Eko, 25);
    Aka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Aka);
    Emu = _mm_xor_si128(Emu, Du);
    Bko = ROL64in128_8(Emu);
    Ake = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Ake);
    Esa = _mm_xor_si128(Esa, Da);
    Bku = ROL64in128(Esa, 18);
    Aki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Aki);
    Ako = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Ako);
    Aku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Aku);
    Ebu = _mm_xor_si128(Ebu, Du);
    Bma = ROL64in128(Ebu, 27);
    Ega = _mm_xor_si128(Ega, Da);
    Bme = ROL64in128(Ega, 36);
    Eke = _mm_xor_si128(Eke, De);
    Bmi = ROL64in128(Eke, 10);
    Ama = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ama);
    Emi = _mm_xor_si128(Emi, Di);
    Bmo = ROL64in128(Emi, 15);
    Ame = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Ame);
    Eso = _mm_xor_si128(Eso, Do);
    Bmu = ROL64in128_56(Eso);
    Ami = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Ami);
    Amo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Amo);
    Amu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Amu);
    Ebi = _mm_xor_si128(Ebi, Di);
    Bsa = ROL64in128(Ebi, 62);
    Ego = _mm_xor_si128(Ego, Do);
    Bse = ROL64in128(Ego, 55);
    Eku = _mm_xor_si128(Eku, Du);
    Bsi = ROL64in128(Eku, 39);
    Asa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Asa);
    Ema = _mm_xor_si128(Ema, Da);
    Bso = ROL64in128(Ema, 41);
    Ase = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ase);
    Ese = _mm_xor_si128(Ese, De);
    Bsu = ROL64in128(Ese, 2);
    Asi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Asi);
    Aso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Aso);
    Asu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Asu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Aba = _mm_xor_si128(Aba, Da);
    Bba = Aba;
    Age = _mm_xor_si128(Age, De);
    Bbe = ROL64in128(Age, 44);
    Aki = _mm_xor_si128(Aki, Di);
    Bbi = ROL64in128(Aki, 43);
    Eba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Eba = _mm_xor_si128(Eba, SPLAT64(KeccakF1600Constants[10]));
    Ca = Eba;
    Amo = _mm_xor_si128(Amo, Do);
    Bbo = ROL64in128(Amo, 21);
    Ebe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Ebe;
    Asu = _mm_xor_si128(Asu, Du);
    Bbu = ROL64in128(Asu, 14);
    Ebi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Ebi;
    Ebo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Ebo;
    Ebu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Ebu;
    Abo = _mm_xor_si128(Abo, Do);
    Bga = ROL64in128(Abo, 28);
    Agu = _mm_xor_si128(Agu, Du);
    Bge = ROL64in128(Agu, 20);
    Aka = _mm_xor_si128(Aka, Da);
    Bgi = ROL64in128(Aka, 3);
    Ega = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Ega);
    Ame = _mm_xor_si128(Ame, De);
    Bgo = ROL64in128(Ame, 45);
    Ege = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Ege);
    Asi = _mm_xor_si128(Asi, Di);
    Bgu = ROL64in128(Asi, 61);
    Egi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Egi);
    Ego = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ego);
    Egu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Egu);
    Abe = _mm_xor_si128(Abe, De);
    Bka = ROL64in128(Abe, 1);
    Agi = _mm_xor_si128(Agi, Di);
    Bke = ROL64in128(Agi, 6);
    Ako = _mm_xor_si128(Ako, Do);
    Bki = ROL64in128(Ako, 25);
    Eka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Eka);
    Amu = _mm_xor_si128(Amu, Du);
    Bko = ROL64in128_8(Amu);
    Eke = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Eke);
    Asa = _mm_xor_si128(Asa, Da);
    Bku = ROL64in128(Asa, 18);
    Eki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Eki);
    Eko = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Eko);
    Eku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Eku);
    Abu = _mm_xor_si128(Abu, Du);
    Bma = ROL64in128(Abu, 27);
    Aga = _mm_xor_si128(Aga, Da);
    Bme = ROL64in128(Aga, 36);
    Ake = _mm_xor_si128(Ake, De);
    Bmi = ROL64in128(Ake, 10);
    Ema = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ema);
    Ami = _mm_xor_si128(Ami, Di);
    Bmo = ROL64in128(Ami, 15);
    Eme = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Eme);
    Aso = _mm_xor_si128(Aso, Do);
    Bmu = ROL64in128_56(Aso);
    Emi = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Emi);
    Emo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Emo);
    Emu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Emu);
    Abi = _mm_xor_si128(Abi, Di);
    Bsa = ROL64in128(Abi, 62);
    Ago = _mm_xor_si128(Ago, Do);
    Bse = ROL64in128(Ago, 55);
    Aku = _mm_xor_si128(Aku, Du);
    Bsi = ROL64in128(Aku, 39);
    Esa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Esa);
    Ama = _mm_xor_si128(Ama, Da);
    Bso = ROL64in128(Ama, 41);
    Ese = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ese);
    Ase = _mm_xor_si128(Ase, De);
    Bsu = ROL64in128(Ase, 2);
    Esi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Esi);
    Eso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Eso);
    Esu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Esu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Eba = _mm_xor_si128(Eba, Da);
    Bba = Eba;
    Ege = _mm_xor_si128(Ege, De);
    Bbe = ROL64in128(Ege, 44);
    Eki = _mm_xor_si128(Eki, Di);
    Bbi = ROL64in128(Eki, 43);
    Aba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Aba = _mm_xor_si128(Aba, SPLAT64(KeccakF1600Constants[11]));
    Ca = Aba;
    Emo = _mm_xor_si128(Emo, Do);
    Bbo = ROL64in128(Emo, 21);
    Abe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Abe;
    Esu = _mm_xor_si128(Esu, Du);
    Bbu = ROL64in128(Esu, 14);
    Abi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Abi;
    Abo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Abo;
    Abu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Abu;
    Ebo = _mm_xor_si128(Ebo, Do);
    Bga = ROL64in128(Ebo, 28);
    Egu = _mm_xor_si128(Egu, Du);
    Bge = ROL64in128(Egu, 20);
    Eka = _mm_xor_si128(Eka, Da);
    Bgi = ROL64in128(Eka, 3);
    Aga = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Aga);
    Eme = _mm_xor_si128(Eme, De);
    Bgo = ROL64in128(Eme, 45);
    Age = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Age);
    Esi = _mm_xor_si128(Esi, Di);
    Bgu = ROL64in128(Esi, 61);
    Agi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Agi);
    Ago = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ago);
    Agu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Agu);
    Ebe = _mm_xor_si128(Ebe, De);
    Bka = ROL64in128(Ebe, 1);
    Egi = _mm_xor_si128(Egi, Di);
    Bke = ROL64in128(Egi, 6);
    Eko = _mm_xor_si128(Eko, Do);
    Bki = ROL64in128(Eko, 25);
    Aka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Aka);
    Emu = _mm_xor_si128(Emu, Du);
    Bko = ROL64in128_8(Emu);
    Ake = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Ake);
    Esa = _mm_xor_si128(Esa, Da);
    Bku = ROL64in128(Esa, 18);
    Aki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Aki);
    Ako = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Ako);
    Aku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Aku);
    Ebu = _mm_xor_si128(Ebu, Du);
    Bma = ROL64in128(Ebu, 27);
    Ega = _mm_xor_si128(Ega, Da);
    Bme = ROL64in128(Ega, 36);
    Eke = _mm_xor_si128(Eke, De);
    Bmi = ROL64in128(Eke, 10);
    Ama = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ama);
    Emi = _mm_xor_si128(Emi, Di);
    Bmo = ROL64in128(Emi, 15);
    Ame = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Ame);
    Eso = _mm_xor_si128(Eso, Do);
    Bmu = ROL64in128_56(Eso);
    Ami = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Ami);
    Amo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Amo);
    Amu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Amu);
    Ebi = _mm_xor_si128(Ebi, Di);
    Bsa = ROL64in128(Ebi, 62);
    Ego = _mm_xor_si128(Ego, Do);
    Bse = ROL64in128(Ego, 55);
    Eku = _mm_xor_si128(Eku, Du);
    Bsi = ROL64in128(Eku, 39);
    Asa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Asa);
    Ema = _mm_xor_si128(Ema, Da);
    Bso = ROL64in128(Ema, 41);
    Ase = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ase);
    Ese = _mm_xor_si128(Ese, De);
    Bsu = ROL64in128(Ese, 2);
    Asi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Asi);
    Aso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Aso);
    Asu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Asu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Aba = _mm_xor_si128(Aba, Da);
    Bba = Aba;
    Age = _mm_xor_si128(Age, De);
    Bbe = ROL64in128(Age, 44);
    Aki = _mm_xor_si128(Aki, Di);
    Bbi = ROL64in128(Aki, 43);
    Eba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Eba = _mm_xor_si128(Eba, SPLAT64(KeccakF1600Constants[12]));
    Ca = Eba;
    Amo = _mm_xor_si128(Amo, Do);
    Bbo = ROL64in128(Amo, 21);
    Ebe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Ebe;
    Asu = _mm_xor_si128(Asu, Du);
    Bbu = ROL64in128(Asu, 14);
    Ebi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Ebi;
    Ebo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Ebo;
    Ebu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Ebu;
    Abo = _mm_xor_si128(Abo, Do);
    Bga = ROL64in128(Abo, 28);
    Agu = _mm_xor_si128(Agu, Du);
    Bge = ROL64in128(Agu, 20);
    Aka = _mm_xor_si128(Aka, Da);
    Bgi = ROL64in128(Aka, 3);
    Ega = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Ega);
    Ame = _mm_xor_si128(Ame, De);
    Bgo = ROL64in128(Ame, 45);
    Ege = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Ege);
    Asi = _mm_xor_si128(Asi, Di);
    Bgu = ROL64in128(Asi, 61);
    Egi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Egi);
    Ego = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ego);
    Egu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Egu);
    Abe = _mm_xor_si128(Abe, De);
    Bka = ROL64in128(Abe, 1);
    Agi = _mm_xor_si128(Agi, Di);
    Bke = ROL64in128(Agi, 6);
    Ako = _mm_xor_si128(Ako, Do);
    Bki = ROL64in128(Ako, 25);
    Eka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Eka);
    Amu = _mm_xor_si128(Amu, Du);
    Bko = ROL64in128_8(Amu);
    Eke = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Eke);
    Asa = _mm_xor_si128(Asa, Da);
    Bku = ROL64in128(Asa, 18);
    Eki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Eki);
    Eko = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Eko);
    Eku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Eku);
    Abu = _mm_xor_si128(Abu, Du);
    Bma = ROL64in128(Abu, 27);
    Aga = _mm_xor_si128(Aga, Da);
    Bme = ROL64in128(Aga, 36);
    Ake = _mm_xor_si128(Ake, De);
    Bmi = ROL64in128(Ake, 10);
    Ema = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ema);
    Ami = _mm_xor_si128(Ami, Di);
    Bmo = ROL64in128(Ami, 15);
    Eme = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Eme);
    Aso = _mm_xor_si128(Aso, Do);
    Bmu = ROL64in128_56(Aso);
    Emi = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Emi);
    Emo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Emo);
    Emu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Emu);
    Abi = _mm_xor_si128(Abi, Di);
    Bsa = ROL64in128(Abi, 62);
    Ago = _mm_xor_si128(Ago, Do);
    Bse = ROL64in128(Ago, 55);
    Aku = _mm_xor_si128(Aku, Du);
    Bsi = ROL64in128(Aku, 39);
    Esa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Esa);
    Ama = _mm_xor_si128(Ama, Da);
    Bso = ROL64in128(Ama, 41);
    Ese = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ese);
    Ase = _mm_xor_si128(Ase, De);
    Bsu = ROL64in128(Ase, 2);
    Esi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Esi);
    Eso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Eso);
    Esu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Esu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Eba = _mm_xor_si128(Eba, Da);
    Bba = Eba;
    Ege = _mm_xor_si128(Ege, De);
    Bbe = ROL64in128(Ege, 44);
    Eki = _mm_xor_si128(Eki, Di);
    Bbi = ROL64in128(Eki, 43);
    Aba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Aba = _mm_xor_si128(Aba, SPLAT64(KeccakF1600Constants[13]));
    Ca = Aba;
    Emo = _mm_xor_si128(Emo, Do);
    Bbo = ROL64in128(Emo, 21);
    Abe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Abe;
    Esu = _mm_xor_si128(Esu, Du);
    Bbu = ROL64in128(Esu, 14);
    Abi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Abi;
    Abo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Abo;
    Abu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Abu;
    Ebo = _mm_xor_si128(Ebo, Do);
    Bga = ROL64in128(Ebo, 28);
    Egu = _mm_xor_si128(Egu, Du);
    Bge = ROL64in128(Egu, 20);
    Eka = _mm_xor_si128(Eka, Da);
    Bgi = ROL64in128(Eka, 3);
    Aga = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Aga);
    Eme = _mm_xor_si128(Eme, De);
    Bgo = ROL64in128(Eme, 45);
    Age = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Age);
    Esi = _mm_xor_si128(Esi, Di);
    Bgu = ROL64in128(Esi, 61);
    Agi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Agi);
    Ago = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ago);
    Agu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Agu);
    Ebe = _mm_xor_si128(Ebe, De);
    Bka = ROL64in128(Ebe, 1);
    Egi = _mm_xor_si128(Egi, Di);
    Bke = ROL64in128(Egi, 6);
    Eko = _mm_xor_si128(Eko, Do);
    Bki = ROL64in128(Eko, 25);
    Aka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Aka);
    Emu = _mm_xor_si128(Emu, Du);
    Bko = ROL64in128_8(Emu);
    Ake = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Ake);
    Esa = _mm_xor_si128(Esa, Da);
    Bku = ROL64in128(Esa, 18);
    Aki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Aki);
    Ako = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Ako);
    Aku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Aku);
    Ebu = _mm_xor_si128(Ebu, Du);
    Bma = ROL64in128(Ebu, 27);
    Ega = _mm_xor_si128(Ega, Da);
    Bme = ROL64in128(Ega, 36);
    Eke = _mm_xor_si128(Eke, De);
    Bmi = ROL64in128(Eke, 10);
    Ama = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ama);
    Emi = _mm_xor_si128(Emi, Di);
    Bmo = ROL64in128(Emi, 15);
    Ame = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Ame);
    Eso = _mm_xor_si128(Eso, Do);
    Bmu = ROL64in128_56(Eso);
    Ami = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Ami);
    Amo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Amo);
    Amu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Amu);
    Ebi = _mm_xor_si128(Ebi, Di);
    Bsa = ROL64in128(Ebi, 62);
    Ego = _mm_xor_si128(Ego, Do);
    Bse = ROL64in128(Ego, 55);
    Eku = _mm_xor_si128(Eku, Du);
    Bsi = ROL64in128(Eku, 39);
    Asa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Asa);
    Ema = _mm_xor_si128(Ema, Da);
    Bso = ROL64in128(Ema, 41);
    Ase = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ase);
    Ese = _mm_xor_si128(Ese, De);
    Bsu = ROL64in128(Ese, 2);
    Asi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Asi);
    Aso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Aso);
    Asu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Asu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Aba = _mm_xor_si128(Aba, Da);
    Bba = Aba;
    Age = _mm_xor_si128(Age, De);
    Bbe = ROL64in128(Age, 44);
    Aki = _mm_xor_si128(Aki, Di);
    Bbi = ROL64in128(Aki, 43);
    Eba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Eba = _mm_xor_si128(Eba, SPLAT64(KeccakF1600Constants[14]));
    Ca = Eba;
    Amo = _mm_xor_si128(Amo, Do);
    Bbo = ROL64in128(Amo, 21);
    Ebe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Ebe;
    Asu = _mm_xor_si128(Asu, Du);
    Bbu = ROL64in128(Asu, 14);
    Ebi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Ebi;
    Ebo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Ebo;
    Ebu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Ebu;
    Abo = _mm_xor_si128(Abo, Do);
    Bga = ROL64in128(Abo, 28);
    Agu = _mm_xor_si128(Agu, Du);
    Bge = ROL64in128(Agu, 20);
    Aka = _mm_xor_si128(Aka, Da);
    Bgi = ROL64in128(Aka, 3);
    Ega = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Ega);
    Ame = _mm_xor_si128(Ame, De);
    Bgo = ROL64in128(Ame, 45);
    Ege = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Ege);
    Asi = _mm_xor_si128(Asi, Di);
    Bgu = ROL64in128(Asi, 61);
    Egi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Egi);
    Ego = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ego);
    Egu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Egu);
    Abe = _mm_xor_si128(Abe, De);
    Bka = ROL64in128(Abe, 1);
    Agi = _mm_xor_si128(Agi, Di);
    Bke = ROL64in128(Agi, 6);
    Ako = _mm_xor_si128(Ako, Do);
    Bki = ROL64in128(Ako, 25);
    Eka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Eka);
    Amu = _mm_xor_si128(Amu, Du);
    Bko = ROL64in128_8(Amu);
    Eke = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Eke);
    Asa = _mm_xor_si128(Asa, Da);
    Bku = ROL64in128(Asa, 18);
    Eki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Eki);
    Eko = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Eko);
    Eku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Eku);
    Abu = _mm_xor_si128(Abu, Du);
    Bma = ROL64in128(Abu, 27);
    Aga = _mm_xor_si128(Aga, Da);
    Bme = ROL64in128(Aga, 36);
    Ake = _mm_xor_si128(Ake, De);
    Bmi = ROL64in128(Ake, 10);
    Ema = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ema);
    Ami = _mm_xor_si128(Ami, Di);
    Bmo = ROL64in128(Ami, 15);
    Eme = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Eme);
    Aso = _mm_xor_si128(Aso, Do);
    Bmu = ROL64in128_56(Aso);
    Emi = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Emi);
    Emo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Emo);
    Emu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Emu);
    Abi = _mm_xor_si128(Abi, Di);
    Bsa = ROL64in128(Abi, 62);
    Ago = _mm_xor_si128(Ago, Do);
    Bse = ROL64in128(Ago, 55);
    Aku = _mm_xor_si128(Aku, Du);
    Bsi = ROL64in128(Aku, 39);
    Esa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Esa);
    Ama = _mm_xor_si128(Ama, Da);
    Bso = ROL64in128(Ama, 41);
    Ese = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ese);
    Ase = _mm_xor_si128(Ase, De);
    Bsu = ROL64in128(Ase, 2);
    Esi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Esi);
    Eso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Eso);
    Esu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Esu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Eba = _mm_xor_si128(Eba, Da);
    Bba = Eba;
    Ege = _mm_xor_si128(Ege, De);
    Bbe = ROL64in128(Ege, 44);
    Eki = _mm_xor_si128(Eki, Di);
    Bbi = ROL64in128(Eki, 43);
    Aba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Aba = _mm_xor_si128(Aba, SPLAT64(KeccakF1600Constants[15]));
    Ca = Aba;
    Emo = _mm_xor_si128(Emo, Do);
    Bbo = ROL64in128(Emo, 21);
    Abe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Abe;
    Esu = _mm_xor_si128(Esu, Du);
    Bbu = ROL64in128(Esu, 14);
    Abi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Abi;
    Abo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Abo;
    Abu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Abu;
    Ebo = _mm_xor_si128(Ebo, Do);
    Bga = ROL64in128(Ebo, 28);
    Egu = _mm_xor_si128(Egu, Du);
    Bge = ROL64in128(Egu, 20);
    Eka = _mm_xor_si128(Eka, Da);
    Bgi = ROL64in128(Eka, 3);
    Aga = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Aga);
    Eme = _mm_xor_si128(Eme, De);
    Bgo = ROL64in128(Eme, 45);
    Age = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Age);
    Esi = _mm_xor_si128(Esi, Di);
    Bgu = ROL64in128(Esi, 61);
    Agi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Agi);
    Ago = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ago);
    Agu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Agu);
    Ebe = _mm_xor_si128(Ebe, De);
    Bka = ROL64in128(Ebe, 1);
    Egi = _mm_xor_si128(Egi, Di);
    Bke = ROL64in128(Egi, 6);
    Eko = _mm_xor_si128(Eko, Do);
    Bki = ROL64in128(Eko, 25);
    Aka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Aka);
    Emu = _mm_xor_si128(Emu, Du);
    Bko = ROL64in128_8(Emu);
    Ake = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Ake);
    Esa = _mm_xor_si128(Esa, Da);
    Bku = ROL64in128(Esa, 18);
    Aki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Aki);
    Ako = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Ako);
    Aku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Aku);
    Ebu = _mm_xor_si128(Ebu, Du);
    Bma = ROL64in128(Ebu, 27);
    Ega = _mm_xor_si128(Ega, Da);
    Bme = ROL64in128(Ega, 36);
    Eke = _mm_xor_si128(Eke, De);
    Bmi = ROL64in128(Eke, 10);
    Ama = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ama);
    Emi = _mm_xor_si128(Emi, Di);
    Bmo = ROL64in128(Emi, 15);
    Ame = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Ame);
    Eso = _mm_xor_si128(Eso, Do);
    Bmu = ROL64in128_56(Eso);
    Ami = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Ami);
    Amo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Amo);
    Amu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Amu);
    Ebi = _mm_xor_si128(Ebi, Di);
    Bsa = ROL64in128(Ebi, 62);
    Ego = _mm_xor_si128(Ego, Do);
    Bse = ROL64in128(Ego, 55);
    Eku = _mm_xor_si128(Eku, Du);
    Bsi = ROL64in128(Eku, 39);
    Asa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Asa);
    Ema = _mm_xor_si128(Ema, Da);
    Bso = ROL64in128(Ema, 41);
    Ase = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ase);
    Ese = _mm_xor_si128(Ese, De);
    Bsu = ROL64in128(Ese, 2);
    Asi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Asi);
    Aso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Aso);
    Asu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Asu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Aba = _mm_xor_si128(Aba, Da);
    Bba = Aba;
    Age = _mm_xor_si128(Age, De);
    Bbe = ROL64in128(Age, 44);
    Aki = _mm_xor_si128(Aki, Di);
    Bbi = ROL64in128(Aki, 43);
    Eba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Eba = _mm_xor_si128(Eba, SPLAT64(KeccakF1600Constants[16]));
    Ca = Eba;
    Amo = _mm_xor_si128(Amo, Do);
    Bbo = ROL64in128(Amo, 21);
    Ebe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Ebe;
    Asu = _mm_xor_si128(Asu, Du);
    Bbu = ROL64in128(Asu, 14);
    Ebi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Ebi;
    Ebo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Ebo;
    Ebu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Ebu;
    Abo = _mm_xor_si128(Abo, Do);
    Bga = ROL64in128(Abo, 28);
    Agu = _mm_xor_si128(Agu, Du);
    Bge = ROL64in128(Agu, 20);
    Aka = _mm_xor_si128(Aka, Da);
    Bgi = ROL64in128(Aka, 3);
    Ega = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Ega);
    Ame = _mm_xor_si128(Ame, De);
    Bgo = ROL64in128(Ame, 45);
    Ege = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Ege);
    Asi = _mm_xor_si128(Asi, Di);
    Bgu = ROL64in128(Asi, 61);
    Egi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Egi);
    Ego = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ego);
    Egu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Egu);
    Abe = _mm_xor_si128(Abe, De);
    Bka = ROL64in128(Abe, 1);
    Agi = _mm_xor_si128(Agi, Di);
    Bke = ROL64in128(Agi, 6);
    Ako = _mm_xor_si128(Ako, Do);
    Bki = ROL64in128(Ako, 25);
    Eka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Eka);
    Amu = _mm_xor_si128(Amu, Du);
    Bko = ROL64in128_8(Amu);
    Eke = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Eke);
    Asa = _mm_xor_si128(Asa, Da);
    Bku = ROL64in128(Asa, 18);
    Eki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Eki);
    Eko = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Eko);
    Eku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Eku);
    Abu = _mm_xor_si128(Abu, Du);
    Bma = ROL64in128(Abu, 27);
    Aga = _mm_xor_si128(Aga, Da);
    Bme = ROL64in128(Aga, 36);
    Ake = _mm_xor_si128(Ake, De);
    Bmi = ROL64in128(Ake, 10);
    Ema = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ema);
    Ami = _mm_xor_si128(Ami, Di);
    Bmo = ROL64in128(Ami, 15);
    Eme = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Eme);
    Aso = _mm_xor_si128(Aso, Do);
    Bmu = ROL64in128_56(Aso);
    Emi = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Emi);
    Emo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Emo);
    Emu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Emu);
    Abi = _mm_xor_si128(Abi, Di);
    Bsa = ROL64in128(Abi, 62);
    Ago = _mm_xor_si128(Ago, Do);
    Bse = ROL64in128(Ago, 55);
    Aku = _mm_xor_si128(Aku, Du);
    Bsi = ROL64in128(Aku, 39);
    Esa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Esa);
    Ama = _mm_xor_si128(Ama, Da);
    Bso = ROL64in128(Ama, 41);
    Ese = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ese);
    Ase = _mm_xor_si128(Ase, De);
    Bsu = ROL64in128(Ase, 2);
    Esi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Esi);
    Eso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Eso);
    Esu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Esu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Eba = _mm_xor_si128(Eba, Da);
    Bba = Eba;
    Ege = _mm_xor_si128(Ege, De);
    Bbe = ROL64in128(Ege, 44);
    Eki = _mm_xor_si128(Eki, Di);
    Bbi = ROL64in128(Eki, 43);
    Aba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Aba = _mm_xor_si128(Aba, SPLAT64(KeccakF1600Constants[17]));
    Ca = Aba;
    Emo = _mm_xor_si128(Emo, Do);
    Bbo = ROL64in128(Emo, 21);
    Abe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Abe;
    Esu = _mm_xor_si128(Esu, Du);
    Bbu = ROL64in128(Esu, 14);
    Abi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Abi;
    Abo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Abo;
    Abu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Abu;
    Ebo = _mm_xor_si128(Ebo, Do);
    Bga = ROL64in128(Ebo, 28);
    Egu = _mm_xor_si128(Egu, Du);
    Bge = ROL64in128(Egu, 20);
    Eka = _mm_xor_si128(Eka, Da);
    Bgi = ROL64in128(Eka, 3);
    Aga = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Aga);
    Eme = _mm_xor_si128(Eme, De);
    Bgo = ROL64in128(Eme, 45);
    Age = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Age);
    Esi = _mm_xor_si128(Esi, Di);
    Bgu = ROL64in128(Esi, 61);
    Agi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Agi);
    Ago = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ago);
    Agu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Agu);
    Ebe = _mm_xor_si128(Ebe, De);
    Bka = ROL64in128(Ebe, 1);
    Egi = _mm_xor_si128(Egi, Di);
    Bke = ROL64in128(Egi, 6);
    Eko = _mm_xor_si128(Eko, Do);
    Bki = ROL64in128(Eko, 25);
    Aka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Aka);
    Emu = _mm_xor_si128(Emu, Du);
    Bko = ROL64in128_8(Emu);
    Ake = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Ake);
    Esa = _mm_xor_si128(Esa, Da);
    Bku = ROL64in128(Esa, 18);
    Aki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Aki);
    Ako = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Ako);
    Aku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Aku);
    Ebu = _mm_xor_si128(Ebu, Du);
    Bma = ROL64in128(Ebu, 27);
    Ega = _mm_xor_si128(Ega, Da);
    Bme = ROL64in128(Ega, 36);
    Eke = _mm_xor_si128(Eke, De);
    Bmi = ROL64in128(Eke, 10);
    Ama = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ama);
    Emi = _mm_xor_si128(Emi, Di);
    Bmo = ROL64in128(Emi, 15);
    Ame = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Ame);
    Eso = _mm_xor_si128(Eso, Do);
    Bmu = ROL64in128_56(Eso);
    Ami = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Ami);
    Amo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Amo);
    Amu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Amu);
    Ebi = _mm_xor_si128(Ebi, Di);
    Bsa = ROL64in128(Ebi, 62);
    Ego = _mm_xor_si128(Ego, Do);
    Bse = ROL64in128(Ego, 55);
    Eku = _mm_xor_si128(Eku, Du);
    Bsi = ROL64in128(Eku, 39);
    Asa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Asa);
    Ema = _mm_xor_si128(Ema, Da);
    Bso = ROL64in128(Ema, 41);
    Ase = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ase);
    Ese = _mm_xor_si128(Ese, De);
    Bsu = ROL64in128(Ese, 2);
    Asi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Asi);
    Aso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Aso);
    Asu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Asu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Aba = _mm_xor_si128(Aba, Da);
    Bba = Aba;
    Age = _mm_xor_si128(Age, De);
    Bbe = ROL64in128(Age, 44);
    Aki = _mm_xor_si128(Aki, Di);
    Bbi = ROL64in128(Aki, 43);
    Eba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Eba = _mm_xor_si128(Eba, SPLAT64(KeccakF1600Constants[18]));
    Ca = Eba;
    Amo = _mm_xor_si128(Amo, Do);
    Bbo = ROL64in128(Amo, 21);
    Ebe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Ebe;
    Asu = _mm_xor_si128(Asu, Du);
    Bbu = ROL64in128(Asu, 14);
    Ebi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Ebi;
    Ebo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Ebo;
    Ebu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Ebu;
    Abo = _mm_xor_si128(Abo, Do);
    Bga = ROL64in128(Abo, 28);
    Agu = _mm_xor_si128(Agu, Du);
    Bge = ROL64in128(Agu, 20);
    Aka = _mm_xor_si128(Aka, Da);
    Bgi = ROL64in128(Aka, 3);
    Ega = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Ega);
    Ame = _mm_xor_si128(Ame, De);
    Bgo = ROL64in128(Ame, 45);
    Ege = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Ege);
    Asi = _mm_xor_si128(Asi, Di);
    Bgu = ROL64in128(Asi, 61);
    Egi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Egi);
    Ego = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ego);
    Egu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Egu);
    Abe = _mm_xor_si128(Abe, De);
    Bka = ROL64in128(Abe, 1);
    Agi = _mm_xor_si128(Agi, Di);
    Bke = ROL64in128(Agi, 6);
    Ako = _mm_xor_si128(Ako, Do);
    Bki = ROL64in128(Ako, 25);
    Eka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Eka);
    Amu = _mm_xor_si128(Amu, Du);
    Bko = ROL64in128_8(Amu);
    Eke = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Eke);
    Asa = _mm_xor_si128(Asa, Da);
    Bku = ROL64in128(Asa, 18);
    Eki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Eki);
    Eko = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Eko);
    Eku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Eku);
    Abu = _mm_xor_si128(Abu, Du);
    Bma = ROL64in128(Abu, 27);
    Aga = _mm_xor_si128(Aga, Da);
    Bme = ROL64in128(Aga, 36);
    Ake = _mm_xor_si128(Ake, De);
    Bmi = ROL64in128(Ake, 10);
    Ema = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ema);
    Ami = _mm_xor_si128(Ami, Di);
    Bmo = ROL64in128(Ami, 15);
    Eme = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Eme);
    Aso = _mm_xor_si128(Aso, Do);
    Bmu = ROL64in128_56(Aso);
    Emi = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Emi);
    Emo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Emo);
    Emu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Emu);
    Abi = _mm_xor_si128(Abi, Di);
    Bsa = ROL64in128(Abi, 62);
    Ago = _mm_xor_si128(Ago, Do);
    Bse = ROL64in128(Ago, 55);
    Aku = _mm_xor_si128(Aku, Du);
    Bsi = ROL64in128(Aku, 39);
    Esa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Esa);
    Ama = _mm_xor_si128(Ama, Da);
    Bso = ROL64in128(Ama, 41);
    Ese = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ese);
    Ase = _mm_xor_si128(Ase, De);
    Bsu = ROL64in128(Ase, 2);
    Esi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Esi);
    Eso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Eso);
    Esu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Esu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Eba = _mm_xor_si128(Eba, Da);
    Bba = Eba;
    Ege = _mm_xor_si128(Ege, De);
    Bbe = ROL64in128(Ege, 44);
    Eki = _mm_xor_si128(Eki, Di);
    Bbi = ROL64in128(Eki, 43);
    Aba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Aba = _mm_xor_si128(Aba, SPLAT64(KeccakF1600Constants[19]));
    Ca = Aba;
    Emo = _mm_xor_si128(Emo, Do);
    Bbo = ROL64in128(Emo, 21);
    Abe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Abe;
    Esu = _mm_xor_si128(Esu, Du);
    Bbu = ROL64in128(Esu, 14);
    Abi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Abi;
    Abo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Abo;
    Abu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Abu;
    Ebo = _mm_xor_si128(Ebo, Do);
    Bga = ROL64in128(Ebo, 28);
    Egu = _mm_xor_si128(Egu, Du);
    Bge = ROL64in128(Egu, 20);
    Eka = _mm_xor_si128(Eka, Da);
    Bgi = ROL64in128(Eka, 3);
    Aga = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Aga);
    Eme = _mm_xor_si128(Eme, De);
    Bgo = ROL64in128(Eme, 45);
    Age = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Age);
    Esi = _mm_xor_si128(Esi, Di);
    Bgu = ROL64in128(Esi, 61);
    Agi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Agi);
    Ago = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ago);
    Agu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Agu);
    Ebe = _mm_xor_si128(Ebe, De);
    Bka = ROL64in128(Ebe, 1);
    Egi = _mm_xor_si128(Egi, Di);
    Bke = ROL64in128(Egi, 6);
    Eko = _mm_xor_si128(Eko, Do);
    Bki = ROL64in128(Eko, 25);
    Aka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Aka);
    Emu = _mm_xor_si128(Emu, Du);
    Bko = ROL64in128_8(Emu);
    Ake = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Ake);
    Esa = _mm_xor_si128(Esa, Da);
    Bku = ROL64in128(Esa, 18);
    Aki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Aki);
    Ako = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Ako);
    Aku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Aku);
    Ebu = _mm_xor_si128(Ebu, Du);
    Bma = ROL64in128(Ebu, 27);
    Ega = _mm_xor_si128(Ega, Da);
    Bme = ROL64in128(Ega, 36);
    Eke = _mm_xor_si128(Eke, De);
    Bmi = ROL64in128(Eke, 10);
    Ama = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ama);
    Emi = _mm_xor_si128(Emi, Di);
    Bmo = ROL64in128(Emi, 15);
    Ame = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Ame);
    Eso = _mm_xor_si128(Eso, Do);
    Bmu = ROL64in128_56(Eso);
    Ami = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Ami);
    Amo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Amo);
    Amu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Amu);
    Ebi = _mm_xor_si128(Ebi, Di);
    Bsa = ROL64in128(Ebi, 62);
    Ego = _mm_xor_si128(Ego, Do);
    Bse = ROL64in128(Ego, 55);
    Eku = _mm_xor_si128(Eku, Du);
    Bsi = ROL64in128(Eku, 39);
    Asa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Asa);
    Ema = _mm_xor_si128(Ema, Da);
    Bso = ROL64in128(Ema, 41);
    Ase = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ase);
    Ese = _mm_xor_si128(Ese, De);
    Bsu = ROL64in128(Ese, 2);
    Asi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Asi);
    Aso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Aso);
    Asu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Asu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Aba = _mm_xor_si128(Aba, Da);
    Bba = Aba;
    Age = _mm_xor_si128(Age, De);
    Bbe = ROL64in128(Age, 44);
    Aki = _mm_xor_si128(Aki, Di);
    Bbi = ROL64in128(Aki, 43);
    Eba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Eba = _mm_xor_si128(Eba, SPLAT64(KeccakF1600Constants[20]));
    Ca = Eba;
    Amo = _mm_xor_si128(Amo, Do);
    Bbo = ROL64in128(Amo, 21);
    Ebe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Ebe;
    Asu = _mm_xor_si128(Asu, Du);
    Bbu = ROL64in128(Asu, 14);
    Ebi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Ebi;
    Ebo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Ebo;
    Ebu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Ebu;
    Abo = _mm_xor_si128(Abo, Do);
    Bga = ROL64in128(Abo, 28);
    Agu = _mm_xor_si128(Agu, Du);
    Bge = ROL64in128(Agu, 20);
    Aka = _mm_xor_si128(Aka, Da);
    Bgi = ROL64in128(Aka, 3);
    Ega = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Ega);
    Ame = _mm_xor_si128(Ame, De);
    Bgo = ROL64in128(Ame, 45);
    Ege = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Ege);
    Asi = _mm_xor_si128(Asi, Di);
    Bgu = ROL64in128(Asi, 61);
    Egi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Egi);
    Ego = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ego);
    Egu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Egu);
    Abe = _mm_xor_si128(Abe, De);
    Bka = ROL64in128(Abe, 1);
    Agi = _mm_xor_si128(Agi, Di);
    Bke = ROL64in128(Agi, 6);
    Ako = _mm_xor_si128(Ako, Do);
    Bki = ROL64in128(Ako, 25);
    Eka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Eka);
    Amu = _mm_xor_si128(Amu, Du);
    Bko = ROL64in128_8(Amu);
    Eke = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Eke);
    Asa = _mm_xor_si128(Asa, Da);
    Bku = ROL64in128(Asa, 18);
    Eki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Eki);
    Eko = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Eko);
    Eku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Eku);
    Abu = _mm_xor_si128(Abu, Du);
    Bma = ROL64in128(Abu, 27);
    Aga = _mm_xor_si128(Aga, Da);
    Bme = ROL64in128(Aga, 36);
    Ake = _mm_xor_si128(Ake, De);
    Bmi = ROL64in128(Ake, 10);
    Ema = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ema);
    Ami = _mm_xor_si128(Ami, Di);
    Bmo = ROL64in128(Ami, 15);
    Eme = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Eme);
    Aso = _mm_xor_si128(Aso, Do);
    Bmu = ROL64in128_56(Aso);
    Emi = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Emi);
    Emo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Emo);
    Emu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Emu);
    Abi = _mm_xor_si128(Abi, Di);
    Bsa = ROL64in128(Abi, 62);
    Ago = _mm_xor_si128(Ago, Do);
    Bse = ROL64in128(Ago, 55);
    Aku = _mm_xor_si128(Aku, Du);
    Bsi = ROL64in128(Aku, 39);
    Esa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Esa);
    Ama = _mm_xor_si128(Ama, Da);
    Bso = ROL64in128(Ama, 41);
    Ese = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ese);
    Ase = _mm_xor_si128(Ase, De);
    Bsu = ROL64in128(Ase, 2);
    Esi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Esi);
    Eso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Eso);
    Esu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Esu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Eba = _mm_xor_si128(Eba, Da);
    Bba = Eba;
    Ege = _mm_xor_si128(Ege, De);
    Bbe = ROL64in128(Ege, 44);
    Eki = _mm_xor_si128(Eki, Di);
    Bbi = ROL64in128(Eki, 43);
    Aba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Aba = _mm_xor_si128(Aba, SPLAT64(KeccakF1600Constants[21]));
    Ca = Aba;
    Emo = _mm_xor_si128(Emo, Do);
    Bbo = ROL64in128(Emo, 21);
    Abe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Abe;
    Esu = _mm_xor_si128(Esu, Du);
    Bbu = ROL64in128(Esu, 14);
    Abi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Abi;
    Abo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Abo;
    Abu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Abu;
    Ebo = _mm_xor_si128(Ebo, Do);
    Bga = ROL64in128(Ebo, 28);
    Egu = _mm_xor_si128(Egu, Du);
    Bge = ROL64in128(Egu, 20);
    Eka = _mm_xor_si128(Eka, Da);
    Bgi = ROL64in128(Eka, 3);
    Aga = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Aga);
    Eme = _mm_xor_si128(Eme, De);
    Bgo = ROL64in128(Eme, 45);
    Age = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Age);
    Esi = _mm_xor_si128(Esi, Di);
    Bgu = ROL64in128(Esi, 61);
    Agi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Agi);
    Ago = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ago);
    Agu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Agu);
    Ebe = _mm_xor_si128(Ebe, De);
    Bka = ROL64in128(Ebe, 1);
    Egi = _mm_xor_si128(Egi, Di);
    Bke = ROL64in128(Egi, 6);
    Eko = _mm_xor_si128(Eko, Do);
    Bki = ROL64in128(Eko, 25);
    Aka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Aka);
    Emu = _mm_xor_si128(Emu, Du);
    Bko = ROL64in128_8(Emu);
    Ake = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Ake);
    Esa = _mm_xor_si128(Esa, Da);
    Bku = ROL64in128(Esa, 18);
    Aki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Aki);
    Ako = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Ako);
    Aku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Aku);
    Ebu = _mm_xor_si128(Ebu, Du);
    Bma = ROL64in128(Ebu, 27);
    Ega = _mm_xor_si128(Ega, Da);
    Bme = ROL64in128(Ega, 36);
    Eke = _mm_xor_si128(Eke, De);
    Bmi = ROL64in128(Eke, 10);
    Ama = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ama);
    Emi = _mm_xor_si128(Emi, Di);
    Bmo = ROL64in128(Emi, 15);
    Ame = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Ame);
    Eso = _mm_xor_si128(Eso, Do);
    Bmu = ROL64in128_56(Eso);
    Ami = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Ami);
    Amo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Amo);
    Amu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Amu);
    Ebi = _mm_xor_si128(Ebi, Di);
    Bsa = ROL64in128(Ebi, 62);
    Ego = _mm_xor_si128(Ego, Do);
    Bse = ROL64in128(Ego, 55);
    Eku = _mm_xor_si128(Eku, Du);
    Bsi = ROL64in128(Eku, 39);
    Asa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Asa);
    Ema = _mm_xor_si128(Ema, Da);
    Bso = ROL64in128(Ema, 41);
    Ase = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ase);
    Ese = _mm_xor_si128(Ese, De);
    Bsu = ROL64in128(Ese, 2);
    Asi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Asi);
    Aso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Aso);
    Asu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Asu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Aba = _mm_xor_si128(Aba, Da);
    Bba = Aba;
    Age = _mm_xor_si128(Age, De);
    Bbe = ROL64in128(Age, 44);
    Aki = _mm_xor_si128(Aki, Di);
    Bbi = ROL64in128(Aki, 43);
    Eba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Eba = _mm_xor_si128(Eba, SPLAT64(KeccakF1600Constants[22]));
    Ca = Eba;
    Amo = _mm_xor_si128(Amo, Do);
    Bbo = ROL64in128(Amo, 21);
    Ebe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Ce = Ebe;
    Asu = _mm_xor_si128(Asu, Du);
    Bbu = ROL64in128(Asu, 14);
    Ebi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Ci = Ebi;
    Ebo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Co = Ebo;
    Ebu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Cu = Ebu;
    Abo = _mm_xor_si128(Abo, Do);
    Bga = ROL64in128(Abo, 28);
    Agu = _mm_xor_si128(Agu, Du);
    Bge = ROL64in128(Agu, 20);
    Aka = _mm_xor_si128(Aka, Da);
    Bgi = ROL64in128(Aka, 3);
    Ega = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Ca = _mm_xor_si128(Ca, Ega);
    Ame = _mm_xor_si128(Ame, De);
    Bgo = ROL64in128(Ame, 45);
    Ege = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Ce = _mm_xor_si128(Ce, Ege);
    Asi = _mm_xor_si128(Asi, Di);
    Bgu = ROL64in128(Asi, 61);
    Egi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ci = _mm_xor_si128(Ci, Egi);
    Ego = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Co = _mm_xor_si128(Co, Ego);
    Egu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Cu = _mm_xor_si128(Cu, Egu);
    Abe = _mm_xor_si128(Abe, De);
    Bka = ROL64in128(Abe, 1);
    Agi = _mm_xor_si128(Agi, Di);
    Bke = ROL64in128(Agi, 6);
    Ako = _mm_xor_si128(Ako, Do);
    Bki = ROL64in128(Ako, 25);
    Eka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Ca = _mm_xor_si128(Ca, Eka);
    Amu = _mm_xor_si128(Amu, Du);
    Bko = ROL64in128_8(Amu);
    Eke = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Ce = _mm_xor_si128(Ce, Eke);
    Asa = _mm_xor_si128(Asa, Da);
    Bku = ROL64in128(Asa, 18);
    Eki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ci = _mm_xor_si128(Ci, Eki);
    Eko = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Co = _mm_xor_si128(Co, Eko);
    Eku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Cu = _mm_xor_si128(Cu, Eku);
    Abu = _mm_xor_si128(Abu, Du);
    Bma = ROL64in128(Abu, 27);
    Aga = _mm_xor_si128(Aga, Da);
    Bme = ROL64in128(Aga, 36);
    Ake = _mm_xor_si128(Ake, De);
    Bmi = ROL64in128(Ake, 10);
    Ema = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Ca = _mm_xor_si128(Ca, Ema);
    Ami = _mm_xor_si128(Ami, Di);
    Bmo = ROL64in128(Ami, 15);
    Eme = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Ce = _mm_xor_si128(Ce, Eme);
    Aso = _mm_xor_si128(Aso, Do);
    Bmu = ROL64in128_56(Aso);
    Emi = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Ci = _mm_xor_si128(Ci, Emi);
    Emo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Co = _mm_xor_si128(Co, Emo);
    Emu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Cu = _mm_xor_si128(Cu, Emu);
    Abi = _mm_xor_si128(Abi, Di);
    Bsa = ROL64in128(Abi, 62);
    Ago = _mm_xor_si128(Ago, Do);
    Bse = ROL64in128(Ago, 55);
    Aku = _mm_xor_si128(Aku, Du);
    Bsi = ROL64in128(Aku, 39);
    Esa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ca = _mm_xor_si128(Ca, Esa);
    Ama = _mm_xor_si128(Ama, Da);
    Bso = ROL64in128(Ama, 41);
    Ese = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ce = _mm_xor_si128(Ce, Ese);
    Ase = _mm_xor_si128(Ase, De);
    Bsu = ROL64in128(Ase, 2);
    Esi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Ci = _mm_xor_si128(Ci, Esi);
    Eso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Co = _mm_xor_si128(Co, Eso);
    Esu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));
    Cu = _mm_xor_si128(Cu, Esu);
    Da = _mm_xor_si128(Cu, ROL64in128(Ce, 1));
    De = _mm_xor_si128(Ca, ROL64in128(Ci, 1));
    Di = _mm_xor_si128(Ce, ROL64in128(Co, 1));
    Do = _mm_xor_si128(Ci, ROL64in128(Cu, 1));
    Du = _mm_xor_si128(Co, ROL64in128(Ca, 1));
    Eba = _mm_xor_si128(Eba, Da);
    Bba = Eba;
    Ege = _mm_xor_si128(Ege, De);
    Bbe = ROL64in128(Ege, 44);
    Eki = _mm_xor_si128(Eki, Di);
    Bbi = ROL64in128(Eki, 43);
    Aba = _mm_xor_si128(Bba, _mm_andnot_si128(Bbe, Bbi));
    Aba = _mm_xor_si128(Aba, SPLAT64(KeccakF1600Constants[23]));
    Emo = _mm_xor_si128(Emo, Do);
    Bbo = ROL64in128(Emo, 21);
    Abe = _mm_xor_si128(Bbe, _mm_andnot_si128(Bbi, Bbo));
    Esu = _mm_xor_si128(Esu, Du);
    Bbu = ROL64in128(Esu, 14);
    Abi = _mm_xor_si128(Bbi, _mm_andnot_si128(Bbo, Bbu));
    Abo = _mm_xor_si128(Bbo, _mm_andnot_si128(Bbu, Bba));
    Abu = _mm_xor_si128(Bbu, _mm_andnot_si128(Bba, Bbe));
    Ebo = _mm_xor_si128(Ebo, Do);
    Bga = ROL64in128(Ebo, 28);
    Egu = _mm_xor_si128(Egu, Du);
    Bge = ROL64in128(Egu, 20);
    Eka = _mm_xor_si128(Eka, Da);
    Bgi = ROL64in128(Eka, 3);
    Aga = _mm_xor_si128(Bga, _mm_andnot_si128(Bge, Bgi));
    Eme = _mm_xor_si128(Eme, De);
    Bgo = ROL64in128(Eme, 45);
    Age = _mm_xor_si128(Bge, _mm_andnot_si128(Bgi, Bgo));
    Esi = _mm_xor_si128(Esi, Di);
    Bgu = ROL64in128(Esi, 61);
    Agi = _mm_xor_si128(Bgi, _mm_andnot_si128(Bgo, Bgu));
    Ago = _mm_xor_si128(Bgo, _mm_andnot_si128(Bgu, Bga));
    Agu = _mm_xor_si128(Bgu, _mm_andnot_si128(Bga, Bge));
    Ebe = _mm_xor_si128(Ebe, De);
    Bka = ROL64in128(Ebe, 1);
    Egi = _mm_xor_si128(Egi, Di);
    Bke = ROL64in128(Egi, 6);
    Eko = _mm_xor_si128(Eko, Do);
    Bki = ROL64in128(Eko, 25);
    Aka = _mm_xor_si128(Bka, _mm_andnot_si128(Bke, Bki));
    Emu = _mm_xor_si128(Emu, Du);
    Bko = ROL64in128_8(Emu);
    Ake = _mm_xor_si128(Bke, _mm_andnot_si128(Bki, Bko));
    Esa = _mm_xor_si128(Esa, Da);
    Bku = ROL64in128(Esa, 18);
    Aki = _mm_xor_si128(Bki, _mm_andnot_si128(Bko, Bku));
    Ako = _mm_xor_si128(Bko, _mm_andnot_si128(Bku, Bka));
    Aku = _mm_xor_si128(Bku, _mm_andnot_si128(Bka, Bke));
    Ebu = _mm_xor_si128(Ebu, Du);
    Bma = ROL64in128(Ebu, 27);
    Ega = _mm_xor_si128(Ega, Da);
    Bme = ROL64in128(Ega, 36);
    Eke = _mm_xor_si128(Eke, De);
    Bmi = ROL64in128(Eke, 10);
    Ama = _mm_xor_si128(Bma, _mm_andnot_si128(Bme, Bmi));
    Emi = _mm_xor_si128(Emi, Di);
    Bmo = ROL64in128(Emi, 15);
    Ame = _mm_xor_si128(Bme, _mm_andnot_si128(Bmi, Bmo));
    Eso = _mm_xor_si128(Eso, Do);
    Bmu = ROL64in128_56(Eso);
    Ami = _mm_xor_si128(Bmi, _mm_andnot_si128(Bmo, Bmu));
    Amo = _mm_xor_si128(Bmo, _mm_andnot_si128(Bmu, Bma));
    Amu = _mm_xor_si128(Bmu, _mm_andnot_si128(Bma, Bme));
    Ebi = _mm_xor_si128(Ebi, Di);
    Bsa = ROL64in128(Ebi, 62);
    Ego = _mm_xor_si128(Ego, Do);
    Bse = ROL64in128(Ego, 55);
    Eku = _mm_xor_si128(Eku, Du);
    Bsi = ROL64in128(Eku, 39);
    Asa = _mm_xor_si128(Bsa, _mm_andnot_si128(Bse, Bsi));
    Ema = _mm_xor_si128(Ema, Da);
    Bso = ROL64in128(Ema, 41);
    Ase = _mm_xor_si128(Bse, _mm_andnot_si128(Bsi, Bso));
    Ese = _mm_xor_si128(Ese, De);
    Bsu = ROL64in128(Ese, 2);
    Asi = _mm_xor_si128(Bsi, _mm_andnot_si128(Bso, Bsu));
    Aso = _mm_xor_si128(Bso, _mm_andnot_si128(Bsu, Bsa));
    Asu = _mm_xor_si128(Bsu, _mm_andnot_si128(Bsa, Bse));

    _mm_storeu_si128((V128 *)&(statesAsLanes[ 0]), Aba);
    _mm_storeu_si128((V128 *)&(statesAsLanes[ 1]), Abe);
    _mm_storeu_si128((V128 *)&(statesAsLanes[ 2]), Abi);
    _mm_storeu_si128((V128 *)&(statesAsLanes[ 3]), Abo);
    _mm_storeu_si128((V128 *)&(statesAsLanes[ 4]), Abu);
    _mm_storeu_si128((V128 *)&(statesAsLanes[ 5]), Aga);
    _mm_storeu_si128((V128 *)&(statesAsLanes[ 6]), Age);
    _mm_storeu_si128((V128 *)&(statesAsLanes[ 7]), Agi);
    _mm_storeu_si128((V128 *)&(statesAsLanes[ 8]), Ago);
    _mm_storeu_si128((V128 *)&(statesAsLanes[ 9]), Agu);
    _mm_storeu_si128((V128 *)&(statesAsLanes[10]), Aka);
    _mm_storeu_si128((V128 *)&(statesAsLanes[11]), Ake);
    _mm_storeu_si128((V128 *)&(statesAsLanes[12]), Aki);
    _mm_storeu_si128((V128 *)&(statesAsLanes[13]), Ako);
    _mm_storeu_si128((V128 *)&(statesAsLanes[14]), Aku);
    _mm_storeu_si128((V128 *)&(statesAsLanes[15]), Ama);
    _mm_storeu_si128((V128 *)&(statesAsLanes[16]), Ame);
    _mm_storeu_si128((V128 *)&(statesAsLanes[17]), Ami);
    _mm_storeu_si128((V128 *)&(statesAsLanes[18]), Amo);
    _mm_storeu_si128((V128 *)&(statesAsLanes[19]), Amu);
    _mm_storeu_si128((V128 *)&(statesAsLanes[20]), Asa);
    _mm_storeu_si128((V128 *)&(statesAsLanes[21]), Ase);
    _mm_storeu_si128((V128 *)&(statesAsLanes[22]), Asi);
    _mm_storeu_si128((V128 *)&(statesAsLanes[23]), Aso);
    _mm_storeu_si128((V128 *)&(statesAsLanes[24]), Asu);
}

#endif

NAMESPACE_END
