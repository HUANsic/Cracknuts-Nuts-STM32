/*
 * huansic_rsa32.h
 *
 *  Created on: Nov 21, 2025
 *      Author: abcde
 */

#ifndef NUTSLIB_HUANSIC_RSA32_H_
#define NUTSLIB_HUANSIC_RSA32_H_

#include <stdint.h>

typedef struct {
	uint32_t exponent;
	uint32_t modulus;
} Key;

uint32_t rsa_encode(uint32_t data, Key* public_key);

uint32_t rsa_decode(uint32_t cipher, Key* private_key);

#endif /* NUTSLIB_HUANSIC_RSA32_H_ */
