#include "user.h"
#include "mbedtls/aes.h"
#include "mbedtls/des.h"

mbedtls_aes_context aes_ctx;
mbedtls_des_context des_ctx;
int key_length = 128;

void User_Init() {
	mbedtls_aes_init(&aes_ctx);
	mbedtls_des_init(&des_ctx);
}

NutStatus_e AES_SetEncryptionKey(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 0;
	if (mbedtls_aes_setkey_enc(&aes_ctx, received_data_ptr, key_length))
		return NUT_ERROR;
	return NUT_OK;
}

NutStatus_e AES_SetDecryptionKey(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 0;
	if (mbedtls_aes_setkey_dec(&aes_ctx, received_data_ptr, key_length))
		return NUT_ERROR;
	return NUT_OK;
}

NutStatus_e AES_Encrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 16;
	vNut_Quiet();
	vNut_LED(1);
	vNut_IO_USER(1);
	vNut_IO_1(1);
	if (mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, received_data_ptr, result_buffer_ptr))
		return NUT_ERROR;
	vNut_IO_1(0);
	vNut_IO_USER(0);
	vNut_LED(0);
	vNut_unQuiet();
	return NUT_OK;
}

NutStatus_e AES_Decrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	*result_length = 16;
	vNut_Quiet();
	vNut_LED(1);
	vNut_IO_USER(1);
	vNut_IO_1(1);
	if (mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_DECRYPT, received_data_ptr, result_buffer_ptr))
		return NUT_ERROR;
	vNut_IO_1(0);
	vNut_IO_USER(0);
	vNut_LED(0);
	vNut_unQuiet();
	return NUT_OK;
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

/* User command */
uint16_t user_cmd[] = { 0x0100, 0x0101, 0x0102, 0x0103, 0x0200, 0x0201, 0x0202, 0x0203 };

/* User command program, returns result length */
NutStatus_e (*user_cmd_prog[])(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) = {
			AES_SetEncryptionKey,
			AES_SetDecryptionKey,
			AES_Encrypt,
			AES_Decrypt,
			DES_SetEncryptionKey,
			DES_SetDecryptionKey,
			DES_Encrypt
};
