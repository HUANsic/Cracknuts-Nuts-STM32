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

/*********** Software AES ***********/

#include "mbedtls/aes.h"
#include "mbedtls/des.h"
#include <stdlib.h>

#define KEY_LENGTH 128
mbedtls_aes_context aes_ctx;
mbedtls_des_context des_ctx;

NutStatus_e AES_SetEncryptionKey(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 0;
	if (mbedtls_aes_setkey_enc(&aes_ctx, received_data_ptr, KEY_LENGTH))
		return NUT_ERROR;
	return NUT_OK;
}

NutStatus_e AES_SetDecryptionKey(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 0;
	if (mbedtls_aes_setkey_dec(&aes_ctx, received_data_ptr, KEY_LENGTH))
		return NUT_ERROR;
	return NUT_OK;
}

NutStatus_e AES_Encrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 16;
	Nut_Quiet();
	Nut_LED(1);
	Nut_IO_1(1);
	int status = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, received_data_ptr, result_buffer_ptr);
	Nut_IO_1(0);
	Nut_LED(0);
	Nut_Unquiet();
	if (status == 0) {
		return NUT_OK;
	} else {
		return NUT_ERROR;
	}
}

NutStatus_e AES_Decrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 16;
	Nut_Quiet();
	Nut_LED(1);
	Nut_IO_1(1);
	int status = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_DECRYPT, received_data_ptr, result_buffer_ptr);
	Nut_IO_1(0);
	Nut_LED(0);
	Nut_Unquiet();
	if (status == 0) {
		return NUT_OK;
	} else {
		return NUT_ERROR;
	}
}

/*********** RSA ***********/

#include "rsa.h"
NN_DIGIT outbuffer[MAX_NN_DIGITS], inbuffer[MAX_NN_DIGITS];

// @formatter:off
R_RSA_PUBLIC_KEY public_key = {
		.bits = 128,
		.modulus = {
				0xA0, 0x6D, 0xC9, 0x6A, 0x57, 0x2C, 0xC1, 0xFB,
				0x84, 0x91, 0x8F, 0x99, 0x74, 0x9E, 0x6C, 0xA3
		},
		.exponent = {
				0x00, 0x01, 0x00, 0x01
		}
};

R_RSA_PRIVATE_KEY private_key = {
		.bits = 128,
		.modulus = {
				0xA0, 0x6D, 0xC9, 0x6A, 0x57, 0x2C, 0xC1, 0xFB,
				0x84, 0x91, 0x8F, 0x99, 0x74, 0x9E, 0x6C, 0xA3
		},
		.publicExponent = {
				0x00, 0x01, 0x00, 0x01
		},
		.exponent = {
				0x24, 0xD9, 0x32, 0xB3, 0x2F, 0x34, 0xC3, 0x95,
				0x31, 0x9D, 0x85, 0xB4, 0xB6, 0x2B, 0x19, 0xA9
		},
		.prime[0] = {
				0xD6, 0x88, 0x9C, 0x35, 0x7C, 0x26, 0x98, 0xDD
		},
		.prime[1] = {
				0xBF, 0x70, 0x01, 0x50, 0x9E, 0x86, 0x03, 0x7F
		},
		.primeExponent[0] = {
				0xB6, 0xE5, 0x40, 0x47, 0x58, 0xD0, 0x8F, 0x01
		},
		.primeExponent[1] = {
				0x8E, 0x24, 0x47, 0x05, 0xAA, 0x3E, 0xCD, 0x57
		},
		.coefficient = {
				0xAC, 0xB8, 0xE2, 0x8E, 0x21, 0x2B, 0x0E, 0x28
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
	int32_t textLength, cipherLength;

	// debug: turn on LED
	Nut_LED(1);

	// fetch text length from package
	cipherLength = received_data_ptr[0];
	cipherLength <<= 8;
	cipherLength |= received_data_ptr[1];
	cipherLength <<= 8;
	cipherLength |= received_data_ptr[2];
	cipherLength <<= 8;
	cipherLength |= received_data_ptr[3];

	// set trigger signal
	Nut_Trigger_Set();

	// decode
	if (rsa_public(result_buffer_ptr, &textLength, received_data_ptr + 4, cipherLength, &public_key)) {
		*result_length = 0;
		return NUT_ERROR_USER_CODE;
	}

	// clear trigger signal
	Nut_Trigger_Clear();

	// if all good, return the encoded text
	if (textLength > result_buffer_MAX_size) {
		*result_length = 0;
		return NUT_ERROR_PAYLOAD_SIZE;
	}
	*result_length = textLength;

	// debug: turn off LED
	Nut_LED(0);

	return NUT_OK;
}

NutStatus_e RSA_Decrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
//	int32_t i, j, k, tmp;
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

	// set trigger signal
	Nut_Trigger_Set();

	// encode
	if (rsa_private(result_buffer_ptr, &cipherLength, received_data_ptr + 4, textLength, &private_key)) {
		*result_length = 0;
		return NUT_ERROR_USER_CODE;
	}

	// clear trigger signal
	Nut_Trigger_Clear();

	// if all good, return the encoded text
	if (cipherLength > result_buffer_MAX_size) {
		*result_length = 0;
		return NUT_ERROR_PAYLOAD_SIZE;
	}
	*result_length = cipherLength;

	// debug: turn off LED
	Nut_LED(0);

	return NUT_OK;
}

/*********** RSA32 ***********/

#include "huansic_rsa32.h"

// @formatter:off
Key public = {
		.exponent = 0x00010001,
		.modulus  = 0x56C4EB69
};
Key private = {
		.exponent = 0x096B68AD,
		.modulus  = 0x56C4EB69
};
// @formatter:on

NutStatus_e RSA32_SetPublicKey(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 0;
	return NUT_OK;
}

NutStatus_e RSA32_SetPrivateKey(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 0;
	return NUT_OK;
}

NutStatus_e RSA32_Encrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	uint32_t temp;

	// debug: turn on LED
	Nut_LED(1);

	// turn text into one word
	temp = received_data_ptr[4];
	temp <<= 8;
	temp |= received_data_ptr[5];
	temp <<= 8;
	temp |= received_data_ptr[6];
	temp <<= 8;
	temp |= received_data_ptr[7];

	// set trigger signal
	HAL_Delay(50);
	Nut_Trigger_Set();
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	Nut_Trigger_Clear();

	// encode
	temp = rsa_encode(temp, &public);

	// clear trigger signal
	Nut_Trigger_Clear();

	// return the encoded text
	result_buffer_ptr[0] = (temp >> 24) & 0x0FF;
	result_buffer_ptr[1] = (temp >> 16) & 0x0FF;
	result_buffer_ptr[2] = (temp >> 8) & 0x0FF;
	result_buffer_ptr[3] = (temp >> 0) & 0x0FF;
	*result_length = 4;

	// debug: turn off LED
	Nut_LED(0);

	return NUT_OK;
}

NutStatus_e RSA32_Decrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	uint32_t temp;

	// debug: turn on LED
	Nut_LED(1);

	// turn ciphered text into one word
	temp = received_data_ptr[4];
	temp <<= 8;
	temp |= received_data_ptr[5];
	temp <<= 8;
	temp |= received_data_ptr[6];
	temp <<= 8;
	temp |= received_data_ptr[7];

	// set trigger signal
	HAL_Delay(50);
	Nut_Trigger_Set();
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	Nut_Trigger_Clear();

	// encode
	temp = rsa_decode(temp, &private);

	// clear trigger signal
	Nut_Trigger_Clear();

	// return the decoded text
	result_buffer_ptr[0] = (temp >> 24) & 0x0FF;
	result_buffer_ptr[1] = (temp >> 16) & 0x0FF;
	result_buffer_ptr[2] = (temp >> 8) & 0x0FF;
	result_buffer_ptr[3] = (temp >> 0) & 0x0FF;
	*result_length = 4;

	// debug: turn off LED
	Nut_LED(0);

	return NUT_OK;
}

/* User command */
// @formatter:off
NutAction_t command_list[] = {
		{.command=0x0001, .function=Echo},
// Software AES
		{.command=0x0100, .function=AES_SetEncryptionKey},
		{.command=0x0101, .function=AES_SetDecryptionKey},
		{.command=0x0102, .function=AES_Encrypt},
		{.command=0x0103, .function=AES_Decrypt},
// RSA
		{.command=0x0400, .function=RSA_SetPublicKey},
		{.command=0x0401, .function=RSA_SetPrivateKey},
		{.command=0x0402, .function=RSA_Encrypt},
		{.command=0x0403, .function=RSA_Decrypt},
// RSA32
		{.command=0x7F00, .function=RSA32_SetPublicKey},
		{.command=0x7F01, .function=RSA32_SetPrivateKey},
		{.command=0x7F02, .function=RSA32_Encrypt},
		{.command=0x7F03, .function=RSA32_Decrypt},
};
// @formatter:on
uint16_t command_count = sizeof(command_list) / sizeof(command_list[0]);

void User_Init() {
	Nut_LED(1);
	HAL_Delay(200);
	mbedtls_aes_init(&aes_ctx);
	mbedtls_des_init(&des_ctx);
	Nut_LED(0);
}
