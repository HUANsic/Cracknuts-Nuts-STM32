#include "user.h"

void User_Init() {
	Nut_LED(1);
	HAL_Delay(200);
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

NutStatus_e Encrypt(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) {
	uint32_t i;
	uint8_t tempu8, text;
	uint8_t key = received_data_ptr[0];

	// trigger set
	HAL_Delay(100);
	Nut_Trigger_Set();
	// delay approximately 10us
	for(i = 0; i < 10; i++){
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
	}

	// encrypt
	text = received_data_ptr[1];
	for (i = 2; i < received_data_length; i++) {
		if((0x01 & text) == (0x01 & key)) {
			tempu8 = (0x80 & text);
			text <<= 1;
			text |= tempu8 ? 0x01 : 0x00;
		} else {
			text ^= key;
		}
	}

	// trigger clear
	HAL_Delay(10);
	Nut_Trigger_Clear();

	// return
	result_buffer_ptr[0] = text;
	*result_length = 1;
	return NUT_OK;
}

/* User command */
// @formatter:off
NutAction_t command_list[] = {
		{.command=0x0001, .function=Echo},
		// example
		{.command=0x0002, .function=Encrypt},
};
// @formatter:on
uint16_t command_count = sizeof(command_list) / sizeof(command_list[0]);
