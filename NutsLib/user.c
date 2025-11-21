#include "user.h"
#include "aes.h"

void User_Init() {
	Nut_LED(1);
	HAL_Delay(200);
	mbedtls_aes_init(&aes_ctx);
	mbedtls_des_init(&des_ctx);
	Nut_LED(0);
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

// Software AES
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

/* User command */
// @formatter:off
NutAction_t command_list[] = {
		{.command=0x0001, .function=Echo},
		// Software AES
		{.command=0x0100, .function=AES_SetEncryptionKey},
		{.command=0x0101, .function=AES_SetDecryptionKey},
		{.command=0x0102, .function=AES_Encrypt},
		{.command=0x0103, .function=AES_Decrypt},
};
// @formatter:on
uint16_t command_count = sizeof(command_list) / sizeof(command_list[0]);
