// oids.h - originally written and placed in the public domain by Wei Dai

/// \file oids.h
/// \brief ASN.1 object identifiers for algorthms and schemes
/// \details Most OIDs can be found at http://www.oid-info.com/. The Chinese OIDs
///   are assigned in GM/T 0006-2012, Cryptographic Application Identifier Criterion
///   Specification. A reproduction can be found at http://gmssl.org/docs/oid.html.
///   There seems to be some confusion between the text of GmSSL's oid.html web page
///   and the actual OID used in the code. We used the same OIDs that were detailed in
///   http://github.com/guanzhi/GmSSL/blob/master/crypto/objects/objects.txt.

#ifndef CRYPTOPP_OIDS_H
#define CRYPTOPP_OIDS_H

#include "asn.h"

NAMESPACE_BEGIN(CryptoPP)

NAMESPACE_BEGIN(ASN1)

#define DEFINE_OID(value, name)	inline OID name() {return value;}

DEFINE_OID(1, iso)
	DEFINE_OID(iso()+2, member_body)
		DEFINE_OID(member_body()+156, iso_cn)
			DEFINE_OID(iso_cn()+10197, oscca)
				DEFINE_OID(oscca()+1, sm_scheme)
					DEFINE_OID(sm_scheme()+104, sms4)
					DEFINE_OID(sm_scheme()+301, sm2p256v1)
						DEFINE_OID(sm2p256v1()+1, sm2sign)
						DEFINE_OID(sm2p256v1()+2, sm2exchange)
						DEFINE_OID(sm2p256v1()+3, sm2encrypt)
							DEFINE_OID(sm2encrypt()+1, sm2encrypt_recommendedParameters)
							DEFINE_OID(sm2encrypt()+2, sm2encrypt_specifiedParameters)
		DEFINE_OID(member_body()+840, iso_us)
			DEFINE_OID(iso_us()+10040, ansi_x9_57)
				DEFINE_OID(ansi_x9_57()+4+1, id_dsa)
			DEFINE_OID(iso_us()+10045, ansi_x9_62)
				DEFINE_OID(ansi_x9_62()+1, id_fieldType)
					DEFINE_OID(id_fieldType()+1, prime_field)
					DEFINE_OID(id_fieldType()+2, characteristic_two_field)
						DEFINE_OID(characteristic_two_field()+3, id_characteristic_two_basis)
							DEFINE_OID(id_characteristic_two_basis()+1, gnBasis)
							DEFINE_OID(id_characteristic_two_basis()+2, tpBasis)
							DEFINE_OID(id_characteristic_two_basis()+3, ppBasis)
				DEFINE_OID(ansi_x9_62()+2, id_publicKeyType)
					DEFINE_OID(id_publicKeyType()+1, id_ecPublicKey)
				DEFINE_OID(ansi_x9_62()+3, ansi_x9_62_curves)
					DEFINE_OID(ansi_x9_62_curves()+1, ansi_x9_62_curves_prime)
						DEFINE_OID(ansi_x9_62_curves_prime()+1, secp192r1)
						DEFINE_OID(ansi_x9_62_curves_prime()+7, secp256r1)
			DEFINE_OID(iso_us()+113549, rsadsi)
				DEFINE_OID(rsadsi()+1, pkcs)
					DEFINE_OID(pkcs()+1, pkcs_1)
						DEFINE_OID(pkcs_1()+1, rsaEncryption)
				DEFINE_OID(rsadsi()+2, rsadsi_digestAlgorithm)
					DEFINE_OID(rsadsi_digestAlgorithm()+2, id_md2)
					DEFINE_OID(rsadsi_digestAlgorithm()+5, id_md5)
	DEFINE_OID(iso()+3, identified_organization)
		// Arc from http://tools.ietf.org/html/draft-josefsson-pkix-newcurves
		DEFINE_OID(identified_organization()+6, dod)
			DEFINE_OID(dod()+1, internet)
				DEFINE_OID(internet()+4, internet_private)
					DEFINE_OID(internet_private()+1, enterprise)
						DEFINE_OID(enterprise()+11591,gnu)
							DEFINE_OID(gnu()+15,ellipticCurve)
								DEFINE_OID(ellipticCurve()+1,curve25519)
								DEFINE_OID(ellipticCurve()+2,curve448)
								DEFINE_OID(ellipticCurve()+3,curve25519ph)
								DEFINE_OID(ellipticCurve()+4,curve448ph)
		DEFINE_OID(identified_organization()+14, oiw)
			DEFINE_OID(oiw()+3, oiw_secsig)
				DEFINE_OID(oiw_secsig()+2, oiw_secsig_algorithms)
					DEFINE_OID(oiw_secsig_algorithms()+26, id_sha1)
		DEFINE_OID(identified_organization()+36, teletrust)
			DEFINE_OID(teletrust()+3, teletrust_algorithm)
				DEFINE_OID(teletrust_algorithm()+2+1, id_ripemd160)
				DEFINE_OID(teletrust_algorithm()+3+2+8+1, teletrust_ellipticCurve)
					DEFINE_OID(teletrust_ellipticCurve()+1+1, brainpoolP160r1)
					DEFINE_OID(teletrust_ellipticCurve()+1+3, brainpoolP192r1)
					DEFINE_OID(teletrust_ellipticCurve()+1+5, brainpoolP224r1)
					DEFINE_OID(teletrust_ellipticCurve()+1+7, brainpoolP256r1)
					DEFINE_OID(teletrust_ellipticCurve()+1+9, brainpoolP320r1)
					DEFINE_OID(teletrust_ellipticCurve()+1+11, brainpoolP384r1)
					DEFINE_OID(teletrust_ellipticCurve()+1+13, brainpoolP512r1)
		// https://tools.ietf.org/html/draft-ietf-curdle-pkix-07
		DEFINE_OID(identified_organization()+101, thawte)
			DEFINE_OID(thawte()+110, X25519)
			DEFINE_OID(thawte()+111, X448)
			DEFINE_OID(thawte()+112, Ed25519)
			DEFINE_OID(thawte()+113, Ed448)
		DEFINE_OID(identified_organization()+132, certicom)
			DEFINE_OID(certicom()+0, certicom_ellipticCurve)
				// these are sorted by curve type and then by OID
				// first curves based on GF(p)
				DEFINE_OID(certicom_ellipticCurve()+6, secp112r1)
				DEFINE_OID(certicom_ellipticCurve()+7, secp112r2)
				DEFINE_OID(certicom_ellipticCurve()+8, secp160r1)
				DEFINE_OID(certicom_ellipticCurve()+9, secp160k1)
				DEFINE_OID(certicom_ellipticCurve()+10, secp256k1)
				DEFINE_OID(certicom_ellipticCurve()+28, secp128r1)
				DEFINE_OID(certicom_ellipticCurve()+29, secp128r2)
				DEFINE_OID(certicom_ellipticCurve()+30, secp160r2)
				DEFINE_OID(certicom_ellipticCurve()+31, secp192k1)
				DEFINE_OID(certicom_ellipticCurve()+32, secp224k1)
				DEFINE_OID(certicom_ellipticCurve()+33, secp224r1)
				DEFINE_OID(certicom_ellipticCurve()+34, secp384r1)
				DEFINE_OID(certicom_ellipticCurve()+35, secp521r1)
				// then curves based on GF(2^n)
				DEFINE_OID(certicom_ellipticCurve()+1, sect163k1)
				DEFINE_OID(certicom_ellipticCurve()+2, sect163r1)
				DEFINE_OID(certicom_ellipticCurve()+3, sect239k1)
				DEFINE_OID(certicom_ellipticCurve()+4, sect113r1)
				DEFINE_OID(certicom_ellipticCurve()+5, sect113r2)
				DEFINE_OID(certicom_ellipticCurve()+15, sect163r2)
				DEFINE_OID(certicom_ellipticCurve()+16, sect283k1)
				DEFINE_OID(certicom_ellipticCurve()+17, sect283r1)
				DEFINE_OID(certicom_ellipticCurve()+22, sect131r1)
				DEFINE_OID(certicom_ellipticCurve()+23, sect131r2)
				DEFINE_OID(certicom_ellipticCurve()+24, sect193r1)
				DEFINE_OID(certicom_ellipticCurve()+25, sect193r2)
				DEFINE_OID(certicom_ellipticCurve()+26, sect233k1)
				DEFINE_OID(certicom_ellipticCurve()+27, sect233r1)
				DEFINE_OID(certicom_ellipticCurve()+36, sect409k1)
				DEFINE_OID(certicom_ellipticCurve()+37, sect409r1)
				DEFINE_OID(certicom_ellipticCurve()+38, sect571k1)
				DEFINE_OID(certicom_ellipticCurve()+39, sect571r1)
DEFINE_OID(2, joint_iso_ccitt)
	DEFINE_OID(joint_iso_ccitt()+16, country)
		DEFINE_OID(country()+840, joint_iso_ccitt_us)
			DEFINE_OID(joint_iso_ccitt_us()+1, us_organization)
				DEFINE_OID(us_organization()+101, us_gov)
					DEFINE_OID(us_gov()+3, csor)
						DEFINE_OID(csor()+4, nistalgorithms)
							DEFINE_OID(nistalgorithms()+1, aes)
								DEFINE_OID(aes()+1, id_aes128_ECB)
								DEFINE_OID(aes()+2, id_aes128_cbc)
								DEFINE_OID(aes()+3, id_aes128_ofb)
								DEFINE_OID(aes()+4, id_aes128_cfb)
								DEFINE_OID(aes()+21, id_aes192_ECB)
								DEFINE_OID(aes()+22, id_aes192_cbc)
								DEFINE_OID(aes()+23, id_aes192_ofb)
								DEFINE_OID(aes()+24, id_aes192_cfb)
								DEFINE_OID(aes()+41, id_aes256_ECB)
								DEFINE_OID(aes()+42, id_aes256_cbc)
								DEFINE_OID(aes()+43, id_aes256_ofb)
								DEFINE_OID(aes()+44, id_aes256_cfb)
							DEFINE_OID(nistalgorithms()+2, nist_hashalgs)
								DEFINE_OID(nist_hashalgs()+1, id_sha256)
								DEFINE_OID(nist_hashalgs()+2, id_sha384)
								DEFINE_OID(nist_hashalgs()+3, id_sha512)
								DEFINE_OID(nist_hashalgs()+4, id_sha224)
								DEFINE_OID(nist_hashalgs()+7, id_sha3_224)
								DEFINE_OID(nist_hashalgs()+8, id_sha3_256)
								DEFINE_OID(nist_hashalgs()+9, id_sha3_384)
								DEFINE_OID(nist_hashalgs()+10, id_sha3_512)

NAMESPACE_END

NAMESPACE_END

#endif
