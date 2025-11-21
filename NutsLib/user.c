#include "user.h"

/*		Command Function Template
 *

 NutStatus_e FunctionName(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
 uint32_t result_buffer_MAX_size) {
 *result_length = 0;		// length of responding payload


 // return run result: NUT_OK, NUT_WARNING, NUT_ERROR
 return NUT_OK;
 }

 */

NutStatus_e Echo(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	uint32_t i;
	uint8_t tempu8;
	*result_length = received_data_length;
	for (i = 0; i < received_data_length; i++) {
		tempu8 = received_data_ptr[i];
		result_buffer_ptr[i] = tempu8;
	}
	return NUT_OK;
}

/*********** RSA ***********/

#include "rsa.h"
NN_DIGIT outbuffer[MAX_NN_DIGITS], inbuffer[MAX_NN_DIGITS];

// @formatter:off
R_RSA_PUBLIC_KEY public_key = {
		.bits=1024,
		.modulus={
				0xE1, 0xED, 0x78, 0x00, 0x56, 0x38, 0x6B, 0xCB,
				0xC2, 0x63, 0xAC, 0x48, 0x58, 0xFF, 0x08, 0xFE,
				0x64, 0xDF, 0x62, 0x0F, 0x44, 0x7C, 0x87, 0x45,
				0xBF, 0x21, 0xC4, 0x7B, 0x56, 0x95, 0xF8, 0x67,

				0x4A, 0x25, 0x5B, 0x80, 0xE9, 0x0C, 0xF8, 0x76,
				0xA0, 0x54, 0xF7, 0xE0, 0x0F, 0xEF, 0x0A, 0xD9,
				0x98, 0x35, 0xD1, 0x4E, 0xFF, 0x77, 0x67, 0x6E,
				0x87, 0xDB, 0x51, 0x5B, 0x2E, 0x4E, 0xEF, 0x08,

				0x84, 0x52, 0x5B, 0x9E, 0x73, 0x57, 0x63, 0x7D,
				0xAD, 0x31, 0x38, 0x78, 0x88, 0xE9, 0x41, 0x65,
				0x68, 0x41, 0x2C, 0xC7, 0x76, 0x67, 0xC4, 0x5E,
				0xA1, 0x09, 0xEB, 0x0D, 0x5C, 0xF3, 0xD2, 0x10,

				0x4C, 0xDE, 0xE4, 0x84, 0x75, 0x02, 0x69, 0x87,
				0xD1, 0x8E, 0x1D, 0xD4, 0x68, 0xE0, 0x32, 0x42,
				0x48, 0xF2, 0x87, 0xC2, 0xCB, 0x97, 0x0F, 0x67,
				0x45, 0xA6, 0x0C, 0x93, 0xC0, 0xE7, 0x9B, 0x31
		},
		.exponent={
				0x00, 0x01, 0x00, 0x01
		}
};

R_RSA_PRIVATE_KEY private_key = {
		.bits = 1024,
		.modulus = {
				0xE1, 0xED, 0x78, 0x00, 0x56, 0x38, 0x6B, 0xCB,
				0xC2, 0x63, 0xAC, 0x48, 0x58, 0xFF, 0x08, 0xFE,
				0x64, 0xDF, 0x62, 0x0F, 0x44, 0x7C, 0x87, 0x45,
				0xBF, 0x21, 0xC4, 0x7B, 0x56, 0x95, 0xF8, 0x67,

				0x4A, 0x25, 0x5B, 0x80, 0xE9, 0x0C, 0xF8, 0x76,
				0xA0, 0x54, 0xF7, 0xE0, 0x0F, 0xEF, 0x0A, 0xD9,
				0x98, 0x35, 0xD1, 0x4E, 0xFF, 0x77, 0x67, 0x6E,
				0x87, 0xDB, 0x51, 0x5B, 0x2E, 0x4E, 0xEF, 0x08,

				0x84, 0x52, 0x5B, 0x9E, 0x73, 0x57, 0x63, 0x7D,
				0xAD, 0x31, 0x38, 0x78, 0x88, 0xE9, 0x41, 0x65,
				0x68, 0x41, 0x2C, 0xC7, 0x76, 0x67, 0xC4, 0x5E,
				0xA1, 0x09, 0xEB, 0x0D, 0x5C, 0xF3, 0xD2, 0x10,

				0x4C, 0xDE, 0xE4, 0x84, 0x75, 0x02, 0x69, 0x87,
				0xD1, 0x8E, 0x1D, 0xD4, 0x68, 0xE0, 0x32, 0x42,
				0x48, 0xF2, 0x87, 0xC2, 0xCB, 0x97, 0x0F, 0x67,
				0x45, 0xA6, 0x0C, 0x93, 0xC0, 0xE7, 0x9B, 0x31
		},
		.publicExponent = {
				0x00, 0x01, 0x00, 0x01
		},
		.exponent = {
				0x71, 0xC6, 0x91, 0x24, 0x3A, 0x47, 0x77, 0xA1,
				0xC9, 0x67, 0x40, 0x9F, 0xBE, 0x91, 0xD1, 0x09,
				0xAB, 0xEB, 0x03, 0x0B, 0x05, 0x46, 0x61, 0x29,
				0x32, 0xB3, 0xE2, 0xAA, 0xEA, 0x53, 0xE4, 0x20,

				0xC0, 0x2A, 0x48, 0x06, 0x87, 0x56, 0xCC, 0xD9,
				0x90, 0xAC, 0xB9, 0x97, 0xD0, 0xEE, 0x64, 0xDC,
				0x67, 0xD6, 0xD2, 0x6D, 0xA2, 0x21, 0x93, 0xA9,
				0x0A, 0x29, 0xDC, 0xB2, 0xBA, 0x83, 0x6D, 0xC4,

				0x3D, 0xF2, 0x57, 0xE2, 0x17, 0x92, 0x56, 0x03,
				0x5C, 0x94, 0x59, 0x1A, 0x1D, 0x45, 0x34, 0x70,
				0x11, 0x96, 0xC0, 0x4B, 0x9D, 0x2E, 0xBF, 0x33,
				0x19, 0x03, 0x11, 0x09, 0xE4, 0xF4, 0xD4, 0x76,

				0x01, 0x24, 0x7B, 0xD0, 0xB7, 0xDD, 0x7C, 0x8E,
				0xD1, 0xED, 0x97, 0x94, 0xEE, 0x93, 0x90, 0xD5,
				0x61, 0xA6, 0xD0, 0x21, 0xEE, 0x82, 0x12, 0xB3,
				0x66, 0x3E, 0x71, 0x71, 0xF6, 0xA3, 0xB9, 0x31
		},
		.prime[0] = {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

				0x07, 0xA5, 0xBC, 0x7C, 0x36, 0x11, 0x64, 0x5E,
				0x9E, 0xCD, 0x7F, 0x3A, 0x6E, 0x14, 0x17, 0x79,
				0xD6, 0xCB, 0xF4, 0xA6, 0x1C, 0x08, 0x0F, 0x46,
				0x18, 0x05, 0x56, 0x65, 0xD8, 0xE5, 0x77, 0x27
		},
		.prime[1] = {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

				0xEA, 0xAB, 0x14, 0x3C, 0xFE, 0x76, 0xF5, 0x46,
				0xB6, 0x94, 0xDB, 0xA8, 0xCB, 0x40, 0xD2, 0xAC,
				0xE4, 0x01, 0xAC, 0x12, 0xFC, 0xDD, 0xD6, 0x93,
				0x24, 0x91, 0x6E, 0x27, 0x04, 0x73, 0xDE, 0xE4
		},
		.primeExponent[0] = {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

				0xED, 0x31, 0xD6, 0x4F, 0x88, 0x72, 0xE9, 0xDF,
				0x02, 0x9F, 0x7A, 0xF2, 0x2C, 0x4B, 0x92, 0xC1,
				0x76, 0x4A, 0x3B, 0x59, 0xD8, 0xE9, 0x02, 0x34,
				0x1D, 0x59, 0x51, 0xED, 0x58, 0xED, 0x2B, 0xD2
		},
		.primeExponent[1] = {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

				0x81, 0xA0, 0x6B, 0xB5, 0xC2, 0x34, 0x78, 0x1D,
				0xC2, 0x34, 0x32, 0x2A, 0x93, 0x01, 0x38, 0x59,
				0x10, 0xC5, 0x0D, 0xA8, 0x78, 0x49, 0xDD, 0x8D,
				0xEA, 0x30, 0x95, 0xB9, 0x3B, 0x51, 0x74, 0xF3
		},
		.coefficient = {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

				0x80, 0xC7, 0x51, 0x2F, 0xAA, 0x6F, 0x54, 0x44,
				0x75, 0xE4, 0xF1, 0x31, 0x4C, 0x0B, 0x80, 0xBF,
				0x8A, 0x28, 0x84, 0x35, 0x1F, 0x4F, 0x8F, 0x2B,
				0xEA, 0xE8, 0x93, 0xDE, 0xB6, 0x0D, 0xF7, 0xA7
		}
};
// @formatter:on

NutStatus_e RSA_SetPublicKey(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	uint32_t i, j, tmp;
	uint32_t keyLength;		// in bits
	uint32_t keySize;		// in bytes

	// fetch key length from package
	keyLength = received_data_ptr[0];
	keyLength <<= 8;
	keyLength |= received_data_ptr[1];
	keyLength <<= 8;
	keyLength |= received_data_ptr[2];
	keyLength <<= 8;
	keyLength |= received_data_ptr[3];
	keySize = keyLength / 8;
	if (keySize * 8 != keyLength) {
		*result_length = 0;
		return NUT_ERROR_USER_CODE;
	}
	if (keySize % 32) {
		*result_length = 0;
		return NUT_ERROR_USER_CODE;
	}
	public_key.bits = keyLength;
	private_key.bits = keyLength;

	// check package size: length ~ public exponent ~ modulus
	if (4 + 4 + keySize > received_data_length) {
		*result_length = 0;
		return NUT_ERROR_USER_CODE;
	}

	// copy exponent into buffer
	for (i = 0, j = 4; i < 4; i++, j += 4) {
		tmp = received_data_ptr[j];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 1];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 2];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 3];
		public_key.exponent[i] = tmp;
		private_key.exponent[i] = tmp;
	}

	// copy modulus into buffer
	for (i = 0; i < keySize; i++, j += 4) {
		tmp = received_data_ptr[j];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 1];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 2];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 3];
		public_key.modulus[i] = tmp;
		private_key.modulus[i] = tmp;
	}

	return NUT_OK;
}

NutStatus_e RSA_SetPrivateKey(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	uint32_t i, j, tmp;
	uint32_t keyLength;		// in bits
	uint32_t keySize;		// in bytes

	// fetch key length from package
	keyLength = received_data_ptr[0];
	keyLength <<= 8;
	keyLength |= received_data_ptr[1];
	keyLength <<= 8;
	keyLength |= received_data_ptr[2];
	keyLength <<= 8;
	keyLength |= received_data_ptr[3];
	keySize = keyLength / 8;
	if (keySize * 8 != keyLength) {
		*result_length = 0;
		return NUT_ERROR_USER_CODE;
	}
	if (keySize % 32) {
		*result_length = 0;
		return NUT_ERROR_USER_CODE;
	}
	private_key.bits = keyLength;

	// check package size: length ~ public exponent ~ modulus ~ private exponent ~ prime factor 0 ~ prime factor 1 ~ prime exponent 0 ~ prime exponent 1 ~ coefficient (qInv)
	if (4 + 4 + keySize + keySize / 2 + keySize / 2 + keySize / 2 + keySize / 2 + keySize / 2 > received_data_length) {
		*result_length = 0;
		return NUT_ERROR_USER_CODE;
	}

	// copy exponent into buffer
	for (i = 0, j = 4; i < 4; i++, j += 4) {
		tmp = received_data_ptr[j];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 1];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 2];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 3];
		public_key.exponent[i] = tmp;
		private_key.publicExponent[i] = tmp;
	}

	// copy modulus into buffer
	for (i = 0; i < keySize; i++, j += 4) {
		tmp = received_data_ptr[j];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 1];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 2];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 3];
		public_key.modulus[i] = tmp;
		private_key.modulus[i] = tmp;
	}

	// copy modulus into buffer
	for (i = 0; i < keySize; i++, j += 4) {
		tmp = received_data_ptr[j];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 1];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 2];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 3];
		private_key.exponent[i] = tmp;
	}

	// copy prime factor 0 into buffer
	for (i = 0; i < keySize / 2; i++, j += 4) {
		tmp = received_data_ptr[j];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 1];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 2];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 3];
		private_key.prime[0][i] = tmp;
	}

	// copy prime factor 1 into buffer
	for (i = 0; i < keySize / 2; i++, j += 4) {
		tmp = received_data_ptr[j];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 1];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 2];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 3];
		private_key.prime[1][i] = tmp;
	}

	// copy prime exponent 0 into buffer
	for (i = 0; i < keySize / 2; i++, j += 4) {
		tmp = received_data_ptr[j];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 1];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 2];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 3];
		private_key.primeExponent[0][i] = tmp;
	}

	// copy prime exponent 1 into buffer
	for (i = 0; i < keySize / 2; i++, j += 4) {
		tmp = received_data_ptr[j];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 1];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 2];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 3];
		private_key.primeExponent[1][i] = tmp;
	}

	// copy coefficient into buffer
	for (i = 0; i < keySize / 2; i++, j += 4) {
		tmp = received_data_ptr[j];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 1];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 2];
		tmp <<= 8;
		tmp |= received_data_ptr[j + 3];
		private_key.coefficient[i] = tmp;
	}

	*result_length = 0;
	return NUT_OK;
}

NutStatus_e RSA_Encrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	uint32_t i, j, k, tmp;
	int32_t textLength, cipherLength;

	// debug: turn on LED
	Nut_LED(1);

	// fetch text length from package
	textLength = received_data_ptr[0];
	textLength <<= 8;
	textLength |= received_data_ptr[1];
	textLength <<= 8;
	textLength |= received_data_ptr[2];
	textLength <<= 8;
	textLength |= received_data_ptr[3];

	// copy text into buffer
	for (i = 0, j = 0; i + 4 <= textLength; i += 4, j++) {
		tmp = received_data_ptr[4 + i];
		tmp <<= 8;
		tmp |= received_data_ptr[4 + i + 1];
		tmp <<= 8;
		tmp |= received_data_ptr[4 + i + 2];
		tmp <<= 8;
		tmp |= received_data_ptr[4 + i + 3];
		inbuffer[j] = tmp;
	}
	// if input data length is not a multiple of 4, then pad the rest with 0
	if (textLength % 4) {
		tmp = received_data_ptr[i];
		for (k = 1; i + k < textLength; k++) {
			tmp <<= 8;
			tmp |= received_data_ptr[4 + i + k];
		}
		tmp <<= 8 * (4 - k);
		inbuffer[j] = tmp;
	}

	// set trigger signal
	Nut_Trigger_Set();

	// encode
	if (rsa_private(outbuffer, &cipherLength, inbuffer, textLength, &private_key)){
		*result_length = 0;
		return NUT_ERROR_USER_CODE;
	}

	// clear trigger signal
	Nut_Trigger_Clear();

	// if all good, return the encoded text
	if (cipherLength * 4 > result_buffer_MAX_size) {
		*result_length = 0;
		return NUT_ERROR_PAYLOAD_SIZE;
	}
	*result_length = cipherLength * 4;
	for (i = 0, j = 0; j < cipherLength; i += 4, j++) {
		result_buffer_ptr[i] = (uint8_t) (outbuffer[j] >> 24);
		result_buffer_ptr[i + 1] = (uint8_t) (outbuffer[j] >> 16);
		result_buffer_ptr[i + 2] = (uint8_t) (outbuffer[j] >> 8);
		result_buffer_ptr[i + 3] = (uint8_t) (outbuffer[j]);
	}

	// debug: turn off LED
	Nut_LED(0);

	return NUT_OK;
}

NutStatus_e RSA_Decrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	uint32_t i, j, k, tmp;
	int32_t textLength, cipherLength;

	// fetch text length from package
	cipherLength = received_data_ptr[0];
	cipherLength <<= 8;
	cipherLength |= received_data_ptr[1];
	cipherLength <<= 8;
	cipherLength |= received_data_ptr[2];
	cipherLength <<= 8;
	cipherLength |= received_data_ptr[3];

	// copy text into buffer
	for (i = 0, j = 0; i + 4 <= cipherLength; i += 4, j++) {
		tmp = received_data_ptr[4 + i];
		tmp <<= 8;
		tmp |= received_data_ptr[4 + i + 1];
		tmp <<= 8;
		tmp |= received_data_ptr[4 + i + 2];
		tmp <<= 8;
		tmp |= received_data_ptr[4 + i + 3];
		inbuffer[j] = tmp;
	}
	// if input data length is not a multiple of 4, then pad the rest with 0
	if (cipherLength % 4) {
		tmp = received_data_ptr[i];
		for (k = 1; i + k < cipherLength; k++) {
			tmp <<= 8;
			tmp |= received_data_ptr[4 + i + k];
		}
		tmp <<= 8 * (4 - k);
		inbuffer[j] = tmp;
	}

	// decode
	if (rsa_public(outbuffer, &textLength, inbuffer, cipherLength, &public_key)){
		*result_length = 0;
		return NUT_ERROR_USER_CODE;
	}

	// if all good, return the encoded text
	if (textLength * 4 > result_buffer_MAX_size) {
		*result_length = 0;
		return NUT_ERROR_PAYLOAD_SIZE;
	}
	*result_length = textLength * 4;
	for (i = 0, j = 0; j < textLength; i += 4, j++) {
		result_buffer_ptr[i] = (uint8_t) (outbuffer[j] >> 24);
		result_buffer_ptr[i + 1] = (uint8_t) (outbuffer[j] >> 16);
		result_buffer_ptr[i + 2] = (uint8_t) (outbuffer[j] >> 8);
		result_buffer_ptr[i + 3] = (uint8_t) (outbuffer[j]);
	}

	return NUT_OK;
}

/* User command */
// @formatter:off
NutAction_t command_list[] = {
		{.command=0x0001, .function=Echo},
// RSA
		{.command=0x0400, .function=RSA_SetPublicKey},
		{.command=0x0401, .function=RSA_SetPrivateKey},
		{.command=0x0402, .function=RSA_Encrypt},
		{.command=0x0403, .function=RSA_Decrypt},
};
// @formatter:on
uint16_t command_count = sizeof(command_list) / sizeof(command_list[0]);

void User_Init() {
	Nut_LED(1);
	HAL_Delay(200);
	Nut_LED(0);
}
