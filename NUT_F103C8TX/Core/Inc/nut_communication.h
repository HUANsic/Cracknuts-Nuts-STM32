/*
 * nut_communication.h
 *
 *  Created on: Jun 24, 2025
 *      Author: abcde
 */

#ifndef INC_NUT_COMMUNICATION_H_
#define INC_NUT_COMMUNICATION_H_

#include "stm32f1xx_hal.h"

typedef enum {
	NUT_OK = 0,
	NUT_WARNING,
	NUT_ERROR
} NutStatus_e;

typedef enum {
	NUT_ERROR_UNKNOWN = 0,
	NUT_ERROR_TIMEOUT,
	NUT_ERROR_BUSY,
	NUT_ERROR_PAYLOAD_SIZE,
	NUT_ERROR_USER_CODE,
	NUT_ERROR_CMD_UNKNOWN
} NutError_e;

typedef enum {
	NUT_CMD_IGNORE = 0x0000,
	NUT_CMD_ECHO = 0x0001,		// at the end of reception, copy the RX buffer to TX buffer
} NutCommand_e;

void vNut_Init(void);

void vNut_loop(void);

void vNut_Quiet(void);

void vNut_unQuiet(void);

void vNut_LED(uint8_t on);

void vNut_IO_1(uint8_t set);

void vNut_IO_2(uint8_t set);

void vNut_IO_3(uint8_t set);

void vNut_IO_USER(uint8_t set);

#endif /* INC_NUT_COMMUNICATION_H_ */
