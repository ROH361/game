// seed.cpp - originally written and placed in the public domain by Wei Dai

#include "pch.h"
#include "seed.h"
#include "misc.h"
#include "stdcpp.h"

ANONYMOUS_NAMESPACE_BEGIN

using CryptoPP::byte;
using CryptoPP::word32;

const word32 s_kc[16] = {
	0x9e3779b9, 0x3c6ef373, 0x78dde6e6, 0xf1bbcdcc, 0xe3779b99, 0xc6ef3733, 0x8dde6e67, 0x1bbcdccf,
	0x3779b99e, 0x6ef3733c, 0xdde6e678, 0xbbcdccf1, 0x779b99e3, 0xef3733c6, 0xde6e678d, 0xbcdccf1b};

const byte s_s0[256] = {
	0xA9, 0x85, 0xD6, 0xD3, 0x54, 0x1D, 0xAC, 0x25, 0x5D, 0x43, 0x18, 0x1E, 0x51, 0xFC, 0xCA, 0x63, 0x28,
	0x44, 0x20, 0x9D, 0xE0, 0xE2, 0xC8, 0x17, 0xA5, 0x8F, 0x03, 0x7B, 0xBB, 0x13, 0xD2, 0xEE, 0x70, 0x8C,
	0x3F, 0xA8, 0x32, 0xDD, 0xF6, 0x74, 0xEC, 0x95, 0x0B, 0x57, 0x5C, 0x5B, 0xBD, 0x01, 0x24, 0x1C, 0x73,
	0x98, 0x10, 0xCC, 0xF2, 0xD9, 0x2C, 0xE7, 0x72, 0x83, 0x9B, 0xD1, 0x86, 0xC9, 0x60, 0x50, 0xA3, 0xEB,
	0x0D, 0xB6, 0x9E, 0x4F, 0xB7, 0x5A, 0xC6, 0x78, 0xA6, 0x12, 0xAF, 0xD5, 0x61, 0xC3, 0xB4, 0x41, 0x52,
	0x7D, 0x8D, 0x08, 0x1F, 0x99, 0x00, 0x19, 0x04, 0x53, 0xF7, 0xE1, 0xFD, 0x76, 0x2F, 0x27, 0xB0, 0x8B,
	0x0E, 0xAB, 0xA2, 0x6E, 0x93, 0x4D, 0x69, 0x7C, 0x09, 0x0A, 0xBF, 0xEF, 0xF3, 0xC5, 0x87, 0x14, 0xFE,
	0x64, 0xDE, 0x2E, 0x4B, 0x1A, 0x06, 0x21, 0x6B, 0x66, 0x02, 0xF5, 0x92, 0x8A, 0x0C, 0xB3, 0x7E, 0xD0,
	0x7A, 0x47, 0x96, 0xE5, 0x26, 0x80, 0xAD, 0xDF, 0xA1, 0x30, 0x37, 0xAE, 0x36, 0x15, 0x22, 0x38, 0xF4,
	0xA7, 0x45, 0x4C, 0x81, 0xE9, 0x84, 0x97, 0x35, 0xCB, 0xCE, 0x3C, 0x71, 0x11, 0xC7, 0x89, 0x75, 0xFB,
	0xDA, 0xF8, 0x94, 0x59, 0x82, 0xC4, 0xFF, 0x49, 0x39, 0x67, 0xC0, 0xCF, 0xD7, 0xB8, 0x0F, 0x8E, 0x42,
	0x23, 0x91, 0x6C, 0xDB, 0xA4, 0x34, 0xF1, 0x48, 0xC2, 0x6F, 0x3D, 0x2D, 0x40, 0xBE, 0x3E, 0xBC, 0xC1,
	0xAA, 0xBA, 0x4E, 0x55, 0x3B, 0xDC, 0x68, 0x7F, 0x9C, 0xD8, 0x4A, 0x56, 0x77, 0xA0, 0xED, 0x46, 0xB5,
	0x2B, 0x65, 0xFA, 0xE3, 0xB9, 0xB1, 0x9F, 0x5E, 0xF9, 0xE6, 0xB2, 0x31, 0xEA, 0x6D, 0x5F, 0xE4, 0xF0,
	0xCD, 0x88, 0x16, 0x3A, 0x58, 0xD4, 0x62, 0x29, 0x07, 0x33, 0xE8, 0x1B, 0x05, 0x79, 0x90, 0x6A, 0x2A,
	0x9A};

static const byte s_s1[256] = {
	0x38, 0xE8, 0x2D, 0xA6, 0xCF, 0xDE, 0xB3, 0xB8, 0xAF, 0x60, 0x55, 0xC7, 0x44, 0x6F, 0x6B, 0x5B, 0xC3,
	0x62, 0x33, 0xB5, 0x29, 0xA0, 0xE2, 0xA7, 0xD3, 0x91, 0x11, 0x06, 0x1C, 0xBC, 0x36, 0x4B, 0xEF, 0x88,
	0x6C, 0xA8, 0x17, 0xC4, 0x16, 0xF4, 0xC2, 0x45, 0xE1, 0xD6, 0x3F, 0x3D, 0x8E, 0x98, 0x28, 0x4E, 0xF6,
	0x3E, 0xA5, 0xF9, 0x0D, 0xDF, 0xD8, 0x2B, 0x66, 0x7A, 0x27, 0x2F, 0xF1, 0x72, 0x42, 0xD4, 0x41, 0xC0,
	0x73, 0x67, 0xAC, 0x8B, 0xF7, 0xAD, 0x80, 0x1F, 0xCA, 0x2C, 0xAA, 0x34, 0xD2, 0x0B, 0xEE, 0xE9, 0x5D,
	0x94, 0x18, 0xF8, 0x57, 0xAE, 0x08, 0xC5, 0x13, 0xCD, 0x86, 0xB9, 0xFF, 0x7D, 0xC1, 0x31, 0xF5, 0x8A,
	0x6A, 0xB1, 0xD1, 0x20, 0xD7, 0x02, 0x22, 0x04, 0x68, 0x71, 0x07, 0xDB, 0x9D, 0x99, 0x61, 0xBE, 0xE6,
	0x59, 0xDD, 0x51, 0x90, 0xDC, 0x9A, 0xA3, 0xAB, 0xD0, 0x81, 0x0F, 0x47, 0x1A, 0xE3, 0xEC, 0x8D, 0xBF,
	0x96, 0x7B, 0x5C, 0xA2, 0xA1, 0x63, 0x23, 0x4D, 0xC8, 0x9E, 0x9C, 0x3A, 0x0C, 0x2E, 0xBA, 0x6E, 0x9F,
	0x5A, 0xF2, 0x92, 0xF3, 0x49, 0x78, 0xCC, 0x15, 0xFB, 0x70, 0x75, 0x7F, 0x35, 0x10, 0x03, 0x64, 0x6D,
	0xC6, 0x74, 0xD5, 0xB4, 0xEA, 0x09, 0x76, 0x19, 0xFE, 0x40, 0x12, 0xE0, 0xBD, 0x05, 0xFA, 0x01, 0xF0,
	0x2A, 0x5E, 0xA9, 0x56, 0x43, 0x85, 0x14, 0x89, 0x9B, 0xB0, 0xE5, 0x48, 0x79, 0x97, 0xFC, 0x1E, 0x82,
	0x21, 0x8C, 0x1B, 0x5F, 0x77, 0x54, 0xB2, 0x1D, 0x25, 0x4F, 0x00, 0x46, 0xED, 0x58, 0x52, 0xEB, 0x7E,
	0xDA, 0xC9, 0xFD, 0x30, 0x95, 0x65, 0x3C, 0xB6, 0xE4, 0xBB, 0x7C, 0x0E, 0x50, 0x39, 0x26, 0x32, 0x84,
	0x69, 0x93, 0x37, 0xE7, 0x24, 0xA4, 0xCB, 0x53, 0x0A, 0x87, 0xD9, 0x4C, 0x83, 0x8F, 0xCE, 0x3B, 0x4A,
	0xB7};

#define SS0(x) ((s_s0[x]*0x01010101UL) & 0x3FCFF3FC)
#define SS1(x) ((s_s1[x]*0x01010101UL) & 0xFC3FCFF3)
#define SS2(x) ((s_s0[x]*0x01010101UL) & 0xF3FC3FCF)
#define SS3(x) ((s_s1[x]*0x01010101UL) & 0xCFF3FC3F)
#define G(x) (SS0(GETBYTE(x, 0)) ^ SS1(GETBYTE(x, 1)) ^ SS2(GETBYTE(x, 2)) ^ SS3(GETBYTE(x, 3)))

ANONYMOUS_NAMESPACE_END

NAMESPACE_BEGIN(CryptoPP)

void SEED::Base::UncheckedSetKey(const byte *userKey, unsigned int length, const NameValuePairs& /*params*/)
{
	AssertValidKeyLength(length);

	word64 key01, key23;
	GetBlock<word64, BigEndian> get(userKey);
	get(key01)(key23);
	word32 *k = m_k;
	ptrdiff_t kInc = 2;

	if (!IsForwardTransformation())
	{
		k = k+30;
		kInc = 0-kInc;
	}

	for (unsigned int i=0; i<ROUNDS; i++)
	{
		word32 t0 = word32(key01>>32) + word32(key23>>32) - s_kc[i];
		word32 t1 = word32(key01) - word32(key23) + s_kc[i];
		k[0] = G(t0);
		k[1] = G(t1);
		k+=kInc;
		if (i&1)
			key23 = rotlConstant<8,word64>(key23);
		else
			key01 = rotrConstant<8,word64>(key01);
	}
}

void SEED::Base::ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const
{
	typedef BlockGetAndPut<word32, BigEndian> Block;
	word32 a0, a1, b0, b1, t0, t1;
	Block::Get(inBlock)(a0)(a1)(b0)(b1);

	for (int i=0; i<ROUNDS; i+=2)
	{
		t0 = b0 ^ m_k[2*i+0]; t1 = b1 ^ m_k[2*i+1] ^ t0;
		t1 = G(t1); t0 += t1; t0 = G(t0); t1 += t0; t1 = G(t1);
		a0 ^= t0 + t1; a1 ^= t1;

		t0 = a0 ^ m_k[2*i+2]; t1 = a1 ^ m_k[2*i+3] ^ t0;
		t1 = G(t1); t0 += t1; t0 = G(t0); t1 += t0; t1 = G(t1);
		b0 ^= t0 + t1; b1 ^= t1;
	}

	Block::Put(xorBlock, outBlock)(b0)(b1)(a0)(a1);
}

NAMESPACE_END
