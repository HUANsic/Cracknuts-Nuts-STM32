/*
 * nut_communication.h
 *
 *  Created on: Jun 24, 2025
 *      Author: abcde
 */

#ifndef INC_NUTSLIB_H_
#define INC_NUTSLIB_H_

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

typedef struct {
	uint16_t command;
	uint16_t reserved;
	NutStatus_e (*function)(uint8_t *, uint32_t, uint8_t *, uint32_t *, uint32_t);
} NutAction_t;

void Nut_Init(void);

void Nut_Loop(void);

void Nut_Quiet(void);

void Nut_Unquiet(void);

void Nut_LED(uint8_t on);

void Nut_IO_1(uint8_t set);

void Nut_IO_2(uint8_t set);

void Nut_IO_3(uint8_t set);

void Nut_Trigger_Set(void);

void Nut_Trigger_Clear(void);

uint8_t Nut_IO_USER(void);

#endif /* INC_NUTSLIB_H_ */
