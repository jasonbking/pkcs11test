// Copyright 2013-2014 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "globals.h"

namespace pkcs11 {
namespace test {

CK_FUNCTION_LIST_PTR g_fns = nullptr;
CK_SLOT_ID g_slot_id = 0;
bool g_verbose = false;
bool g_so_tests = true;
bool g_init_token = false;
CK_FLAGS g_token_flags;
CK_UTF8CHAR g_token_label[32];
const char* g_user_pin = "useruser";
const char* g_reset_user_pin = "12345678";
const char* g_so_pin = "sososo";
const char* g_reset_so_pin = "87654321";

// Algorithm information.
std::map<std::string, HmacInfo> kHmacInfo = {
  {"MD5-HMAC", {CKM_MD5_HMAC,  16}},
  {"SHA1-HMAC", {CKM_SHA_1_HMAC, 20}},
  {"SHA256-HMAC", {CKM_SHA256_HMAC, 256/8}},
  {"SHA384-HMAC", {CKM_SHA384_HMAC, 384/8}},
  {"SHA512-HMAC", {CKM_SHA512_HMAC, 512/8}},
};

std::map<std::string, SignatureInfo> kSignatureInfo = {
  // CKM_RSA_PKCS has restrictions on data sizes (see PKCS#11 s12.1.6 table 37).
  {"RSA", {CKM_RSA_PKCS, 64}},
  {"MD5-RSA", {CKM_MD5_RSA_PKCS, 1024}},
  {"SHA1-RSA", {CKM_SHA1_RSA_PKCS, 1024}},
  {"SHA256-RSA", {CKM_SHA256_RSA_PKCS, 1024}},
  {"SHA384-RSA", {CKM_SHA384_RSA_PKCS, 1024}},
  {"SHA512-RSA", {CKM_SHA512_RSA_PKCS, 1024}},
};

std::map<std::string, CipherInfo> kCipherInfo = {
  {"DES-ECB", {CKK_DES, CKM_DES_KEY_GEN, CKM_DES_ECB, 8, false, -1}},
  {"DES-CBC", {CKK_DES, CKM_DES_KEY_GEN, CKM_DES_CBC, 8, true, -1}},
  {"3DES-ECB", {CKK_DES3, CKM_DES3_KEY_GEN, CKM_DES3_ECB, 8, false, -1}},
  {"3DES-CBC", {CKK_DES3, CKM_DES3_KEY_GEN, CKM_DES3_CBC, 8, true, -1}},
  {"IDEA-ECB", {CKK_IDEA, CKM_IDEA_KEY_GEN, CKM_IDEA_ECB, 8, false, -1}},
  {"IDEA-CBC", {CKK_IDEA, CKM_IDEA_KEY_GEN, CKM_IDEA_CBC, 8, true, -1}},
  {"AES-ECB", {CKK_AES, CKM_AES_KEY_GEN, CKM_AES_ECB, 16, false, 16}},
  {"AES-CBC", {CKK_AES, CKM_AES_KEY_GEN, CKM_AES_CBC, 16, true, 16}},
};

std::map<std::string, DigestInfo> kDigestInfo = {
  {"MD5", {CKM_MD5, 16}},
  {"SHA-1", {CKM_SHA_1, 20}},
  {"SHA-256", {CKM_SHA256, 256/8}},
  {"SHA-384", {CKM_SHA384, 384/8}},
  {"SHA-512", {CKM_SHA512, 512/8}},
};

// PKCS#11 s12 table 34: Mechanisms vs. Functions
std::set<CK_MECHANISM_TYPE> encrypt_decrypt_mechanisms = {
  CKM_RSA_PKCS,
  CKM_RSA_PKCS_OAEP,
  CKM_RSA_X_509,
  CKM_RC2_ECB,
  CKM_RC2_CBC,
  CKM_RC2_CBC_PAD,
  CKM_RC4,
  CKM_RC5_ECB,
  CKM_RC5_CBC,
  CKM_RC5_CBC_PAD,
  CKM_AES_ECB,
  CKM_AES_CBC,
  CKM_AES_CBC_PAD,
  CKM_DES_ECB,
  CKM_DES_CBC,
  CKM_DES_CBC_PAD,
  CKM_DES3_ECB,
  CKM_DES3_CBC,
  CKM_DES3_CBC_PAD,
  CKM_CAST_ECB,
  CKM_CAST_CBC,
  CKM_CAST_CBC_PAD,
  CKM_CAST3_ECB,
  CKM_CAST3_CBC,
  CKM_CAST3_CBC_PAD,
  CKM_CAST128_ECB,
  CKM_CAST128_CBC,
  CKM_CAST128_CBC_PAD,
  CKM_IDEA_ECB,
  CKM_IDEA_CBC,
  CKM_IDEA_CBC_PAD,
  CKM_CDMF_ECB,
  CKM_CDMF_CBC,
  CKM_CDMF_CBC_PAD,
  CKM_SKIPJACK_ECB64,
  CKM_SKIPJACK_CBC64,
  CKM_SKIPJACK_OFB64,
  CKM_SKIPJACK_CFB64,
  CKM_SKIPJACK_CFB32,
  CKM_SKIPJACK_CFB16,
  CKM_SKIPJACK_CFB8,
  CKM_BATON_ECB128,
  CKM_BATON_ECB96,
  CKM_BATON_CBC128,
  CKM_BATON_COUNTER,
  CKM_BATON_SHUFFLE,
  CKM_JUNIPER_ECB128,
  CKM_JUNIPER_CBC128,
  CKM_JUNIPER_COUNTER,
  CKM_JUNIPER_SHUFFLE,
  CKM_BLOWFISH_CBC,
  CKM_TWOFISH_CBC,
};
std::set<CK_MECHANISM_TYPE> sign_verify_mechanisms = {
  CKM_RSA_PKCS,
  CKM_RSA_PKCS_PSS,
  CKM_RSA_9796,
  CKM_RSA_X_509,
  CKM_RSA_X9_31,
  CKM_MD2_RSA_PKCS,
  CKM_MD5_RSA_PKCS,
  CKM_SHA1_RSA_PKCS,
  CKM_SHA256_RSA_PKCS,
  CKM_SHA384_RSA_PKCS,
  CKM_SHA512_RSA_PKCS,
  CKM_RIPEMD128_RSA_PKCS,
  CKM_RIPEMD160_RSA_PKCS,
  CKM_SHA1_RSA_PKCS_PSS,
  CKM_SHA256_RSA_PKCS_PSS,
  CKM_SHA384_RSA_PKCS_PSS,
  CKM_SHA512_RSA_PKCS_PSS,
  CKM_SHA1_RSA_X9_31,
  CKM_DSA,
  CKM_DSA_SHA1,
  CKM_FORTEZZA_TIMESTAMP,
  CKM_ECDSA,
  CKM_ECDSA_SHA1,
  CKM_RC2_MAC_GENERAL,
  CKM_RC2_MAC,
  CKM_RC5_MAC_GENERAL,
  CKM_RC5_MAC,
  CKM_AES_MAC_GENERAL,
  CKM_AES_MAC,
  CKM_DES_MAC_GENERAL,
  CKM_DES_MAC,
  CKM_DES3_MAC_GENERAL,
  CKM_DES3_MAC,
  CKM_CAST_MAC_GENERAL,
  CKM_CAST_MAC,
  CKM_CAST3_MAC_GENERAL,
  CKM_CAST3_MAC,
  CKM_CAST128_MAC_GENERAL,
  CKM_CAST128_MAC,
  CKM_IDEA_MAC_GENERAL,
  CKM_IDEA_MAC,
  CKM_CDMF_MAC_GENERAL,
  CKM_CDMF_MAC,
  CKM_MD2_HMAC_GENERAL,
  CKM_MD2_HMAC,
  CKM_MD5_HMAC_GENERAL,
  CKM_MD5_HMAC,
  CKM_SHA_1_HMAC_GENERAL,
  CKM_SHA_1_HMAC,
  CKM_SHA256_HMAC_GENERAL,
  CKM_SHA256_HMAC,
  CKM_SHA384_HMAC_GENERAL,
  CKM_SHA384_HMAC,
  CKM_SHA512_HMAC_GENERAL,
  CKM_SHA512_HMAC,
  CKM_RIPEMD128_HMAC_GENERAL,
  CKM_RIPEMD128_HMAC,
  CKM_RIPEMD160_HMAC_GENERAL,
  CKM_RIPEMD160_HMAC,
  CKM_SSL3_MD5_MAC,
  CKM_SSL3_SHA1_MAC,
  CKM_CMS_SIG,
};
std::set<CK_MECHANISM_TYPE> sign_verify_recover_mechanisms = {
  CKM_RSA_PKCS,
  CKM_RSA_9796,
  CKM_RSA_X_509,
  CKM_CMS_SIG,
};
std::set<CK_MECHANISM_TYPE> digest_mechanisms = {
  CKM_MD2,
  CKM_MD5,
  CKM_SHA_1,
  CKM_SHA256,
  CKM_SHA384,
  CKM_SHA512,
  CKM_RIPEMD128,
  CKM_RIPEMD160,
  CKM_FASTHASH,
};
std::set<CK_MECHANISM_TYPE> generate_mechanisms = {
  CKM_RSA_PKCS_KEY_PAIR_GEN,
  CKM_RSA_X9_31_KEY_PAIR_GEN,
  CKM_DSA_KEY_PAIR_GEN,
  CKM_DSA_PARAMETER_GEN,
  CKM_EC_KEY_PAIR_GEN,
  CKM_DH_PKCS_KEY_PAIR_GEN,
  CKM_DH_PKCS_PARAMETER_GEN,
  CKM_X9_42_DH_KEY_PAIR_GEN,
  CKM_X9_42_DH_PARAMETER_GEN,
  CKM_KEA_KEY_PAIR_GEN,
  CKM_GENERIC_SECRET_KEY_GEN,
  CKM_RC2_KEY_GEN,
  CKM_RC4_KEY_GEN,
  CKM_RC5_KEY_GEN,
  CKM_AES_KEY_GEN,
  CKM_DES_KEY_GEN,
  CKM_DES2_KEY_GEN,
  CKM_DES3_KEY_GEN,
  CKM_CAST_KEY_GEN,
  CKM_CAST3_KEY_GEN,
  CKM_CAST128_KEY_GEN,
  CKM_IDEA_KEY_GEN,
  CKM_CDMF_KEY_GEN,
  CKM_SKIPJACK_KEY_GEN,
  CKM_BATON_KEY_GEN,
  CKM_JUNIPER_KEY_GEN,
  CKM_PBE_MD2_DES_CBC,
  CKM_PBE_MD5_DES_CBC,
  CKM_PBE_MD5_CAST_CBC,
  CKM_PBE_MD5_CAST3_CBC,
  CKM_PBE_MD5_CAST128_CBC,
  CKM_PBE_SHA1_CAST128_CBC,
  CKM_PBE_SHA1_RC4_128,
  CKM_PBE_SHA1_RC4_40,
  CKM_PBE_SHA1_DES3_EDE_CBC,
  CKM_PBE_SHA1_DES2_EDE_CBC,
  CKM_PBE_SHA1_RC2_128_CBC,
  CKM_PBE_SHA1_RC2_40_CBC,
  CKM_PBA_SHA1_WITH_SHA1_HMAC,
  CKM_PKCS5_PBKD2,
  CKM_SSL3_PRE_MASTER_KEY_GEN,
  CKM_TLS_PRE_MASTER_KEY_GEN,
  CKM_WTLS_PRE_MASTER_KEY_GEN,
  CKM_BLOWFISH_KEY_GEN,
  CKM_TWOFISH_KEY_GEN,
};
std::set<CK_MECHANISM_TYPE> wrap_unwrap_mechanisms = {
  CKM_RSA_PKCS,
  CKM_RSA_PKCS_OAEP,
  CKM_RSA_X_509,
  CKM_RC2_ECB,
  CKM_RC2_CBC,
  CKM_RC2_CBC_PAD,
  CKM_RC5_ECB,
  CKM_RC5_CBC,
  CKM_RC5_CBC_PAD,
  CKM_AES_ECB,
  CKM_AES_CBC,
  CKM_AES_CBC_PAD,
  CKM_DES_ECB,
  CKM_DES_CBC,
  CKM_DES_CBC_PAD,
  CKM_DES3_ECB,
  CKM_DES3_CBC,
  CKM_DES3_CBC_PAD,
  CKM_CAST_ECB,
  CKM_CAST_CBC,
  CKM_CAST_CBC_PAD,
  CKM_CAST3_ECB,
  CKM_CAST3_CBC,
  CKM_CAST3_CBC_PAD,
  CKM_CAST128_ECB,
  CKM_CAST128_CBC,
  CKM_CAST128_CBC_PAD,
  CKM_IDEA_ECB,
  CKM_IDEA_CBC,
  CKM_IDEA_CBC_PAD,
  CKM_CDMF_ECB,
  CKM_CDMF_CBC,
  CKM_CDMF_CBC_PAD,
  CKM_SKIPJACK_WRAP,
  CKM_SKIPJACK_PRIVATE_WRAP,
  CKM_SKIPJACK_RELAYX,
  CKM_BATON_WRAP,
  CKM_JUNIPER_WRAP,
  CKM_KEY_WRAP_SET_OAEP,
  CKM_KEY_WRAP_LYNKS,
  CKM_DES_OFB64,
  CKM_DES_OFB8,
  CKM_DES_CFB64,
  CKM_DES_CFB8,
  CKM_BLOWFISH_CBC,
  CKM_TWOFISH_CBC,
};
std::set<CK_MECHANISM_TYPE> derive_mechanisms = {
  CKM_ECDH1_DERIVE,
  CKM_ECDH1_COFACTOR_DERIVE,
  CKM_ECMQV_DERIVE,
  CKM_DH_PKCS_DERIVE,
  CKM_X9_42_DH_DERIVE,
  CKM_X9_42_DH_HYBRID_DERIVE,
  CKM_X9_42_MQV_DERIVE,
  CKM_KEA_KEY_DERIVE,
  CKM_DES_ECB_ENCRYPT_DATA,
  CKM_DES_CBC_ENCRYPT_DATA,
  CKM_DES3_ECB_ENCRYPT_DATA,
  CKM_DES3_CBC_ENCRYPT_DATA,
  CKM_AES_ECB_ENCRYPT_DATA,
  CKM_AES_CBC_ENCRYPT_DATA,
  CKM_MD2_KEY_DERIVATION,
  CKM_MD5_KEY_DERIVATION,
  CKM_SHA1_KEY_DERIVATION,
  CKM_SHA256_KEY_DERIVATION,
  CKM_SHA384_KEY_DERIVATION,
  CKM_SHA512_KEY_DERIVATION,
  CKM_SSL3_MASTER_KEY_DERIVE,
  CKM_SSL3_MASTER_KEY_DERIVE_DH,
  CKM_SSL3_KEY_AND_MAC_DERIVE,
  CKM_TLS_MASTER_KEY_DERIVE,
  CKM_TLS_MASTER_KEY_DERIVE_DH,
  CKM_TLS_KEY_AND_MAC_DERIVE,
  CKM_TLS_PRF,
  CKM_WTLS_MASTER_KEY_DERIVE,
  CKM_WTLS_MASTER_KEY_DERIVE_DH_ECC,
  CKM_WTLS_SERVER_KEY_AND_MAC_DERIVE,
  CKM_WTLS_CLIENT_KEY_AND_MAC_DERIVE,
  CKM_WTLS_PRF,
  CKM_CONCATENATE_BASE_AND_KEY,
  CKM_CONCATENATE_BASE_AND_DATA,
  CKM_CONCATENATE_DATA_AND_BASE,
  CKM_XOR_BASE_AND_DATA,
  CKM_EXTRACT_KEY_FROM_KEY,
};

CK_BBOOL g_ck_false = CK_FALSE;
CK_BBOOL g_ck_true = CK_TRUE;

const char* g_label = "pkcs11test object";
CK_ULONG g_label_len = 17;  // = strlen(g_label)

}  // namespace test
}  // namespace pkcs11
