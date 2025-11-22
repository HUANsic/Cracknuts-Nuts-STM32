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

	// fetch text length from package (I dont care)
//	cipherLength = received_data_ptr[0];
//	cipherLength <<= 8;
//	cipherLength |= received_data_ptr[1];
//	cipherLength <<= 8;
//	cipherLength |= received_data_ptr[2];
//	cipherLength <<= 8;
//	cipherLength |= received_data_ptr[3];

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

		// fetch text length from package (I dont care)
	//	cipherLength = received_data_ptr[0];
	//	cipherLength <<= 8;
	//	cipherLength |= received_data_ptr[1];
	//	cipherLength <<= 8;
	//	cipherLength |= received_data_ptr[2];
	//	cipherLength <<= 8;
	//	cipherLength |= received_data_ptr[3];

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
	Nut_LED(0);
}
