#ifndef RSA_H_
#define RSA_H_

#include <stdint.h>

// ����ֵ����
#define RE_DATA 0x0401
#define RE_LEN  0x0406
typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef u64		uint64_t;
//typedef u32		uint32_t;
typedef u16		uint16_t;
typedef u8		uint8_t;
//typedef int 	int32_t;
typedef uint32_t NN_DIGIT;
typedef uint16_t NN_HALF_DIGIT;
/* RSA key lengths.
 */
#define MIN_RSA_MODULUS_BITS 508
/* #define MAX_RSA_MODULUS_BITS 1024 ** linq modify ->>2048 ***/
#define MAX_RSA_MODULUS_BITS 2048
#define MAX_RSA_MODULUS_LEN ((MAX_RSA_MODULUS_BITS + 7) / 8)
#define MAX_RSA_PRIME_BITS ((MAX_RSA_MODULUS_BITS + 1) / 2)
#define MAX_RSA_PRIME_LEN ((MAX_RSA_PRIME_BITS + 7) / 8)

/* Length of digit in bits */
#define NN_DIGIT_BITS 32
#define NN_HALF_DIGIT_BITS 16
/* Length of digit in bytes */
#define NN_DIGIT_LEN (NN_DIGIT_BITS / 8)
/* Maximum length in digits */
#define MAX_NN_DIGITS \
  ((MAX_RSA_MODULUS_LEN + NN_DIGIT_LEN - 1) / NN_DIGIT_LEN + 1)
/* Maximum digits */
#define MAX_NN_DIGIT 0xffffffff
#define MAX_NN_HALF_DIGIT 0xffff

/* Macros.
 */
#define LOW_HALF(x) ((x) & MAX_NN_HALF_DIGIT)
#define HIGH_HALF(x) (((x) >> NN_HALF_DIGIT_BITS) & MAX_NN_HALF_DIGIT)
#define TO_HIGH_HALF(x) (((NN_DIGIT)(x)) << NN_HALF_DIGIT_BITS)
#define DIGIT_MSB(x) (uint32_t)(((x) >> (NN_DIGIT_BITS - 1)) & 1)
#define DIGIT_2MSB(x) (uint32_t)(((x) >> (NN_DIGIT_BITS - 2)) & 3)

#define NN_ASSIGN_DIGIT(a, b, digits) {NN_AssignZero (a, digits); a[0] = b;}
#define NN_EQUAL(a, b, digits) (! NN_Cmp (a, b, digits))
#define NN_EVEN(a, digits) (((digits) == 0) || ! (a[0] & 1))


typedef struct     // ��Կ�ṹ
{
	u32 bits;                   /* length in bits of modulus */
	u8  modulus[256];           /* modulus */
	u8  exponent[4];            /* public exponent */
} R_RSA_PUBLIC_KEY;

typedef struct    // ˽Կ�ṹ
{
	u32 bits;                   /* length in bits of modulus */
	u8  modulus[256];           /* modulus */
	u8  publicExponent[4];      /* public exponent */
	u8  exponent[256];          /* private exponent */
	u8  prime[2][128];          /* prime factors */
	u8  primeExponent[2][128];  /* exponents for CRT */
	u8  coefficient[128];       /* CRT coefficient */
} R_RSA_PRIVATE_KEY;


int32_t  rsa_public(void *outbuf, int32_t *outlen, const void *inbuf, int32_t inlen, const R_RSA_PUBLIC_KEY *pubkey);
int32_t  rsa_private(void *outbuf, int32_t *outlen, const void *inbuf, int32_t inlen, const R_RSA_PRIVATE_KEY *prikey);

#endif

