/*
 * huansic_rsa32.c
 *
 *  Created on: Nov 21, 2025
 *      Author: abcde
 */

#include "huansic_rsa32.h"

uint32_t rsa_wheel_ModAdd(uint32_t a, uint32_t b, uint32_t modulus) {
	uint32_t result = 0;
	a %= modulus;
	b %= modulus;
	if (a + b < a) {	// overflow
		result = (0xFFFFFFFF % modulus + 1) % modulus;	// ((2^32-1) % m + 1) % m < 2^31
	}
	result += a + b;
	result %= modulus;
	return result;
}

uint32_t rsa_wheel_ModMult(uint32_t a, uint32_t b, uint32_t modulus) {
	uint8_t i;
	uint32_t result = 0;

	// pick the smaller one to loop
	a %= modulus;
	b %= modulus;
	if (b < a) {
		result = a;
		a = b;
		b = result;
		result = 0;
	}

	/*
	 * 	a*b mod m = a^b[0]^1 + a^b[1]^2 + a^b[2]^4 + ...  mod m
	 * 			  = (a^b[0]^1 mod m) + a^b[1]^2 + a^b[2]^4 + ...  mod m
	 * 			  = ((a^b[0]^1 mod m) + a^b[1]^2 mod m) + a^b[2]^4 mod m) + ...  mod m
	 * 			  =
	 */
	// check every single bit for demonstration; exit when i underflows
	for (i = 31; i < 32; i--) {
		// shift and mod result from last round
		result = rsa_wheel_ModAdd(result, result, modulus);
		if (a & (1 << i)) {	// if a bit is not 0
			// add b from this round
			result = rsa_wheel_ModAdd(result, b, modulus);
		}
	}
	/*
	for (i = 31; (i < 32) && (a & (1 << i)); i--)
		;	// skip leading zeros
	for (; i < 32; i--) {
		// shift and mod result from last round
		result = rsa_wheel_ModAdd(result, result, modulus);
		if (a & (1 << i)) {	// if a bit is not 0
			// add b from this round
			result = rsa_wheel_ModAdd(result, b, modulus);
		}
	}
	*/
	return result;
}

uint32_t rsa_wheel_ModExp(uint32_t base, uint32_t exponent, uint32_t modulus) {
	uint8_t i;
	uint32_t result = 1;
	/*
	 * 	b^e mod m = b^e[0]^1 * b^e[1]^2 * b^e[2]^4 * ...  mod m
	 * 			  = (b^e[0]^1 mod m) * b^e[1]^2 * b^e[2]^4 * ...  mod m
	 * 			  = ((b^e[0]^1 mod m) * b^e[1]^2 mod m) * b^e[2]^4 mod m) * ...  mod m
	 * 			  =
	 */
	// check every single bit for demonstration; exit when i underflows
	for (i = 0; i < 32; i++) {
		if (exponent & (1 << i)) {	// if exponent bit is not 0
			// add base from this round
			result = rsa_wheel_ModMult(result, base, modulus);
		}
		// square and mod base for next round
		base = rsa_wheel_ModMult(base, base, modulus);
	}
	return result;
}

uint32_t rsa_encode(uint32_t data, Key *public_key) {
//	return data ^ public_key->exponent % public_key->modulus;
	return rsa_wheel_ModExp(data, public_key->exponent, public_key->modulus);
}

uint32_t rsa_decode(uint32_t cipher, Key *private_key) {
	//	return data ^ private_key->exponent % private_key->modulus;
	return rsa_wheel_ModExp(cipher, private_key->exponent, private_key->modulus);
}

// 4E 75 74 73 // Nuts
