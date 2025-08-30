#include "user.h"
#include "mbedtls/aes.h"
#include "mbedtls/des.h"
#include <stdlib.h>

mbedtls_aes_context aes_ctx;
mbedtls_des_context des_ctx;
#define KEY_LENGTH 128
#ifdef HAL_CRYP_MODULE_ENABLED
extern CRYP_HandleTypeDef hcryp;
typedef union {
	uint8_t uc[KEY_LENGTH / 8];
	uint32_t ul[KEY_LENGTH / 32];
} uBuffer;
__ALIGN_BEGIN static uBuffer pKeyCRYP __ALIGN_END = { .ul = {0x00000000, 0x00000000, 0x00000000, 0x00000000} };
#endif

void User_Init() {
	mbedtls_aes_init(&aes_ctx);
	mbedtls_des_init(&des_ctx);
#ifdef HAL_CRYP_MODULE_ENABLED
	hcryp.Init.pKey = (void*)pKeyCRYP.ul;
#endif
}

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
	Nut_IO_USER(1);
	Nut_IO_1(1);
	int status = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, received_data_ptr, result_buffer_ptr);
	Nut_IO_1(0);
	Nut_IO_USER(0);
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
	Nut_IO_USER(1);
	Nut_IO_1(1);
	int status = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_DECRYPT, received_data_ptr, result_buffer_ptr);
	Nut_IO_1(0);
	Nut_IO_USER(0);
	Nut_LED(0);
	Nut_Unquiet();
	if (status == 0) {
		return NUT_OK;
	} else {
		return NUT_ERROR;
	}
}

NutStatus_e DES_SetEncryptionKey(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 0;
	if (mbedtls_des_setkey_enc(&des_ctx, received_data_ptr))
		return NUT_ERROR;
	return NUT_OK;
}

NutStatus_e DES_SetDecryptionKey(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 0;
	if (mbedtls_des_setkey_dec(&des_ctx, received_data_ptr))
		return NUT_ERROR;
	return NUT_OK;
}

NutStatus_e DES_Encrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 8;
	if (mbedtls_des_crypt_ecb(&des_ctx, received_data_ptr, result_buffer_ptr))
		return NUT_ERROR;
	return NUT_OK;
}

#ifdef HAL_CRYP_MODULE_ENABLED

NutStatus_e HWAES_SetEncryptionKey(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 0;
	uint8_t i;
	HAL_Delay(5);
	if (received_data_length != KEY_LENGTH / 8) {
		*result_length = 0;
		return NUT_ERROR;
	}
	for (i = 0; i < KEY_LENGTH / 8; i++) {
		pKeyCRYP.uc[i] = received_data_ptr[i];
	}
	return NUT_OK;
}

NutStatus_e HWAES_SetDecryptionKey(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = received_data_length;
	// TODO HW AES Decryption Key Set
	HAL_Delay(5);
	return NUT_ERROR;
}

NutStatus_e HWAES_Encrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 16;
	uint8_t i;
	uBuffer in, out;

	for (i = 0; i < KEY_LENGTH / 8; i++) {
		in.uc[i] = received_data_ptr[i];
	}

	HAL_Delay(5);
	Nut_Quiet();
	Nut_LED(1);
	Nut_IO_USER(1);
	Nut_IO_1(1);
#if defined (CRYP)
	HAL_StatusTypeDef status = HAL_CRYP_Encrypt(&hcryp, in.ul, received_data_length, out.ul, 10); // Size is in hcryp.Init.DataWidthUnit, CRYP_DATAWIDTHUNIT_BYTE in this case
#else
	HAL_StatusTypeDef status = HAL_CRYP_AESECB_Encrypt(&hcryp, in.uc, received_data_length, out.uc, 10);
#endif
	Nut_IO_1(0);
	Nut_IO_USER(0);
	Nut_LED(0);
	Nut_Unquiet();

	for (i = 0; i < KEY_LENGTH / 8; i++) {
		result_buffer_ptr[i] = out.uc[i];
	}

	if (status == HAL_OK) {
		return NUT_OK;
	} else {
		return NUT_ERROR;
	}
}

NutStatus_e HWAES_Decrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = received_data_length;
	*result_length = 16;
	uint8_t i;
	uBuffer in, out;

	for (i = 0; i < KEY_LENGTH / 8; i++) {
		in.uc[i] = received_data_ptr[i];
	}

	HAL_Delay(5);
	Nut_Quiet();
	Nut_LED(1);
	Nut_IO_USER(1);
	Nut_IO_1(1);
#if defined (CRYP)
	HAL_StatusTypeDef status = HAL_CRYP_Decrypt(&hcryp, in.ul, received_data_length, out.ul, 10); // Size is in hcryp.Init.DataWidthUnit, CRYP_DATAWIDTHUNIT_BYTE in this case
#else
	HAL_StatusTypeDef status = HAL_CRYP_AESECB_Decrypt(&hcryp, in.uc, received_data_length, out.uc, 10);
#endif
	Nut_IO_1(0);
	Nut_IO_USER(0);
	Nut_LED(0);
	Nut_Unquiet();

	for (i = 0; i < KEY_LENGTH / 8; i++) {
		result_buffer_ptr[i] = out.uc[i];
	}

	if (status == HAL_OK) {
		return NUT_OK;
	} else {
		return NUT_ERROR;
	}
}

#endif  // defined HAL_CRYP_MODULE_ENABLED

uint32_t rambuff_u32[256];
uint16_t *rambuff_u16 = (uint16_t*) rambuff_u32;
uint8_t *rambuff_u8 = (uint8_t*) rambuff_u32;
NutStatus_e SPA_UpdateData(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 0;		// length of responding payload

	uint32_t aligned_data_u32[256];
	uint16_t *aligned_data_u16 = (uint16_t*) aligned_data_u32;
	uint8_t *aligned_data_u8 = (uint8_t*) aligned_data_u32;
	uint32_t i;
	uint8_t data_size, data_length;

	// decode package
	data_size = received_data_ptr[0];
	data_length = received_data_ptr[1];

	// align data
	for (i = 0; i < received_data_length - 2; i++) {
		aligned_data_u8[i] = received_data_ptr[i + 2];
	}

	// trigger
	Nut_Quiet();
	Nut_LED(1);
	Nut_IO_USER(1);
	Nut_IO_1(1);
	if (data_size == 8) {
		// spa_update_data_u8
		for (i = 0; i < data_length; i++) {
			rambuff_u8[i] = aligned_data_u8[i];
		}
	} else if (data_size == 16) {
		// spa_update_data_u16
		for (i = 0; i < data_length; i++) {
			rambuff_u16[i] = aligned_data_u16[i];
		}
	} else if (data_size == 32) {
		// spa_update_data_u32
		for (i = 0; i < data_length; i++) {
			rambuff_u32[i] = aligned_data_u32[i];
		}
	} else {
		// error
	}
	// clear trigger
	Nut_IO_1(0);
	Nut_IO_USER(0);
	Nut_LED(0);
	Nut_Unquiet();

	// return run result: NUT_OK, NUT_WARNING, NUT_ERROR
	return NUT_OK;
}

NutStatus_e SPA_ArithmeticCompute(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 0;		// length of responding payload

	// return run result: NUT_OK, NUT_WARNING, NUT_ERROR
	return NUT_OK;
}

NutStatus_e SPA_ConditionJump(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 0;		// length of responding payload

	// return run result: NUT_OK, NUT_WARNING, NUT_ERROR
	return NUT_OK;
}

/* User command */
// @formatter:off

uint16_t cmd_list[] = {
		// echo
		0x0001,

		// software AES
		0x0100,
		0x0101,
		0x0102,
		0x0103,

		// software DES
		0x0200,
		0x0201,
		0x0202,
		0x0203,

#ifdef HAL_CRYP_MODULE_ENABLED
		// hardware AES
		0x0300,
		0x0301,
		0x0302,
		0x0303,
#endif  // defined HAL_CRYP_MODULE_ENABLED

		// SPA
		0x0800,
		0x0801,
		0x0802,

0 };	// must end with 0

/* User command program, returns result length */
NutStatus_e (*cmd_prog_list[])(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length, uint32_t result_buffer_MAX_size) = {
		// echo
		Echo,

		// software AES
		AES_SetEncryptionKey,
		AES_SetDecryptionKey,
		AES_Encrypt,
		AES_Decrypt,

		// software DES
		DES_SetEncryptionKey,
		DES_SetDecryptionKey,
		DES_Encrypt,
		0,

#ifdef HAL_CRYP_MODULE_ENABLED
		// hardware AES
		HWAES_SetEncryptionKey,
		HWAES_SetEncryptionKey,
		HWAES_Encrypt,
		HWAES_Decrypt,
#endif  // defined HAL_CRYP_MODULE_ENABLED

		// SPA
		SPA_UpdateData,
		SPA_ArithmeticCompute,
		SPA_ConditionJump
};

// @formatter:on
