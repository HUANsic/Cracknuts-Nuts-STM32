/*
 * nut_communication.h
 *
 *  Created on: Jun 24, 2025
 *      Author: abcde
 */

#ifndef INC_NUT_COMMUNICATION_H_
#define INC_NUT_COMMUNICATION_H_

#include "NutsLib_Config.h"

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
	NUT_ERROR_SPI_ABORTED,
	NUT_ERROR_CMD_UNKNOWN
} NutError_e;

void Nut_Init(void);

void Nut_loop(void);

void Nut_Quiet(void);

void Nut_unQuiet(void);

void Nut_LED(uint8_t on);

#define Nut_Trigger_Set() Nut_IO_1(1)
#define Nut_Trigger_Clear() Nut_IO_1(0)

void Nut_IO_1(uint8_t set);

void Nut_IO_2(uint8_t set);

void Nut_IO_3(uint8_t set);

void Nut_IO_USER(uint8_t set);

#endif /* INC_NUT_COMMUNICATION_H_ */
