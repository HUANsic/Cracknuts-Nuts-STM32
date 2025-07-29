/*
 * nut_communication.c
 *
 *  Created on: Jun 24, 2025
 *      Author: abcde
 */

#include "nut_communication.h"
#include "nut_config.h"
#include "user.h"

/* Communication Interfaces */
extern CAN_HandleTypeDef NUT_CAN;
extern I2C_HandleTypeDef NUT_I2C;
extern SPI_HandleTypeDef NUT_SPI;
extern UART_HandleTypeDef NUT_UART;

/* System command program declarations/prototypes */
NutStatus_e Echo(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size);

/* User command */
extern uint16_t cmd_list[];
/* User command program, returns result length */
extern NutStatus_e (*cmd_prog_list[])(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size);

/* System command */
uint16_t sys_cmd[] = { NUT_CMD_IGNORE, NUT_CMD_ECHO };
/* System command program */
NutStatus_e (*sys_cmd_prog[])(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size) = {
			0,
			Echo
};

/* Command program to be executed */
NutStatus_e (*cmd_program)(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size);

/* Current status of Nut */
NutStatus_e status;
NutError_e error;

/* Buffers and variables for communication */
uint8_t tx_header[NUT_BUFFER_SIZE + 6], rx_header[NUT_BUFFER_SIZE + 8];
uint8_t *tx_buffer = tx_header + 6;
uint8_t *rx_buffer = rx_header + 8;
uint16_t comm_max_batch_size;

/*  CONSTANTS  */
uint8_t error_header[] = { NUT_ERROR, NUT_ERROR_UNKNOWN, 0, 0, 0, 0 };	// payload size too large
uint8_t response_header[] = { NUT_OK, ~NUT_OK, 0, 0, 0, 0 };

/* Other function prototypes */

/* Stop CLK of peripheral */
void _NutComm_UART_Disable(void);
void _NutComm_SPI_Disable(void);
void _NutComm_I2C_Disable(void);
void _NutComm_CAN_Disable(void);
void _NutComm_UART_Enable(void);
void _NutComm_SPI_Enable(void);
void _NutComm_I2C_Enable(void);
void _NutComm_CAN_Enable(void);

/* Decode header */
uint32_t _NutComm_DecodeHeader() {
	uint32_t length;
	uint8_t i;
	uint16_t command;
	/* Decode the header */
	command = rx_header[0];
	command <<= 8;
	command |= rx_header[1];
	length = rx_header[4];
	length <<= 8;
	length |= rx_header[5];
	length <<= 8;
	length |= rx_header[6];
	length <<= 8;
	length |= rx_header[7];
	/* Parse command */
	cmd_program = 0;
	for (i = 0; i < sizeof(sys_cmd) / 2; i++) {
		if (cmd_program)
			break;
		if (sys_cmd[i] == command)
			cmd_program = sys_cmd_prog[i];	// set the program
	}
	for (i = 0; i < 255; i++) {		// just scan all TODO need to fix this
		if (cmd_program)
			break;
		if (cmd_list[i] == command)
			cmd_program = cmd_prog_list[i];	// set the program
		if (cmd_list[i] == 0)
			break;
	}
	/* If the command is not found, then continue to receive the payload if possible */
	if (!cmd_program) {
		status = NUT_WARNING;
		error = NUT_ERROR_CMD_UNKNOWN;
	}
	if (length > NUT_BUFFER_SIZE) {
		status = NUT_ERROR;
		error = NUT_ERROR_PAYLOAD_SIZE;
	}
	return length;
}

/*  INTERFACE INITIALIZATION  */
void _NutComm_UART_Init() {
	// nothing
}
void _NutComm_SPI_Init() {

}
void _NutComm_I2C_Init() {

}
void _NutComm_CAN_Init() {

}
void _NutComm_Pins_Init() {

}

/* Error handlers, in case they are needed */
void _NutComm_SPI_Error() {
	/* Wait until chip not selected */
	while (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_RESET)
		;
	/* Clear all flags */
	__HAL_SPI_CLEAR_OVRFLAG(&NUT_SPI);
	__HAL_SPI_CLEAR_MODFFLAG(&NUT_SPI);
	__HAL_SPI_CLEAR_CRCERRFLAG(&NUT_SPI);
}
void _NutComm_I2C_Error() {
	uint16_t dmy;
	/* Wait until bus idle */
	while (NUT_I2C.Instance->SR2 & (uint32_t) I2C_FLAG_BUSY)
		;
	/* Clear all flags */
	__HAL_I2C_CLEAR_FLAG(&NUT_I2C, 0xFF);	// clear SR1 flags
	dmy = NUT_I2C.Instance->SR2;	// clear SR2 flags immediately to clear
	(void) dmy;
}
void _NutComm_CAN_Error() {
	/* Wait until controller is in SLEEP mode, meaning current TX or RX activity is done */
	NUT_CAN.Instance->MCR |= CAN_MCR_SLEEP;
	while ((NUT_CAN.Instance->MSR & CAN_FLAG_SLAK) == 0)
		;
	/* Clear all error flags, but not bus error flags */
	// bus error should be recovered automatically by hardware according to the CAN bus specification
	__HAL_CAN_CLEAR_FLAG(&NUT_CAN, CAN_FLAG_FF0);
	__HAL_CAN_CLEAR_FLAG(&NUT_CAN, CAN_FLAG_FOV0);
	__HAL_CAN_CLEAR_FLAG(&NUT_CAN, CAN_FLAG_FF1);
	__HAL_CAN_CLEAR_FLAG(&NUT_CAN, CAN_FLAG_FOV1);
	__HAL_CAN_CLEAR_FLAG(&NUT_CAN, CAN_FLAG_ALST0);
	__HAL_CAN_CLEAR_FLAG(&NUT_CAN, CAN_FLAG_TERR0);
	__HAL_CAN_CLEAR_FLAG(&NUT_CAN, CAN_FLAG_ALST1);
	__HAL_CAN_CLEAR_FLAG(&NUT_CAN, CAN_FLAG_TERR1);
	__HAL_CAN_CLEAR_FLAG(&NUT_CAN, CAN_FLAG_ALST2);
	__HAL_CAN_CLEAR_FLAG(&NUT_CAN, CAN_FLAG_TERR2);
	/* Wake up */
	NUT_CAN.Instance->MCR &= ~CAN_MCR_SLEEP;
	// no need to check for wake up
}

/* System command program definitions */
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

/* Initializes communication interfaces */
void _NutComm_Init() {
	/* Constrain the size of each burst */
	comm_max_batch_size = (65535 > NUT_BUFFER_SIZE) ? NUT_BUFFER_SIZE : 65535;

}

/* Continuously check for signs of communication */
void Nut_loop() {
	HAL_StatusTypeDef retstatus = HAL_OK;
	uint32_t length;
	uint8_t *tempptr;
	uint32_t response_length = 0;
	uint32_t data_length;
	uint32_t i;

	/* Check UART */
	if (NUT_UART.Instance->SR & UART_FLAG_RXNE) {
		/* Record the first byte of header */
		rx_header[0] = (uint8_t) NUT_UART.Instance->DR;
		/* Disable other interfaces */
		_NutComm_SPI_Disable();
		_NutComm_I2C_Disable();
		_NutComm_CAN_Disable();
		/* Finish receiving the header */
		retstatus = HAL_UART_Receive(&NUT_UART, rx_header + 1, 7, 100);
		if (retstatus == HAL_TIMEOUT) {
			status = NUT_WARNING;
			error = NUT_ERROR_TIMEOUT;
			/* Enable other interfaces */
			_NutComm_SPI_Enable();
			_NutComm_I2C_Enable();
			_NutComm_CAN_Enable();
			return;
		}
		if (retstatus == HAL_BUSY) {
			status = NUT_WARNING;
			error = NUT_ERROR_BUSY;
			/* Enable other interfaces */
			_NutComm_SPI_Enable();
			_NutComm_I2C_Enable();
			_NutComm_CAN_Enable();
			return;
		}
		if (retstatus != HAL_OK) {
			status = NUT_WARNING;
			error = NUT_ERROR_UNKNOWN;
			/* Enable other interfaces */
			_NutComm_SPI_Enable();
			_NutComm_I2C_Enable();
			_NutComm_CAN_Enable();
			return;
		}
		/* Decode header */
		length = _NutComm_DecodeHeader();
		/* If ERROR occurred, prepare to send error package */
		if (status == NUT_ERROR) {
			/* Wait until the other side to finish transmission */
			while (length) {
				if (length < comm_max_batch_size) {
					HAL_UART_Receive(&NUT_UART, rx_header, length, 1000);
					length = 0;
				} else {
					if (HAL_UART_Receive(&NUT_UART, rx_header, comm_max_batch_size, 1000) == HAL_TIMEOUT)
						break;	// also break on timeout
					length -= comm_max_batch_size;
				}
			}
			/* Send error package */
			error_header[1] = NUT_ERROR_PAYLOAD_SIZE;
			HAL_UART_Transmit(&NUT_UART, error_header, 6, 100);
			/* And just return */
			/* Enable other interfaces */
			_NutComm_SPI_Enable();
			_NutComm_I2C_Enable();
			_NutComm_CAN_Enable();
			return;
		}
		/* Receive the payload */
		data_length = length;
		tempptr = rx_buffer;
		while (length) {
			if (length < comm_max_batch_size) {
				HAL_UART_Receive(&NUT_UART, tempptr, length, 1000);
				length = 0;
			} else {
				if (HAL_UART_Receive(&NUT_UART, tempptr, comm_max_batch_size, 1000) == HAL_TIMEOUT) {
					status = NUT_ERROR;
					error = NUT_ERROR_TIMEOUT;
					/* Send error package on timeout */
					error_header[1] = NUT_ERROR_TIMEOUT;
					HAL_UART_Transmit(&NUT_UART, error_header, 6, 100);
					/* And just return */
					/* Enable other interfaces */
					_NutComm_SPI_Enable();
					_NutComm_I2C_Enable();
					_NutComm_CAN_Enable();
					return;
				}
				tempptr += comm_max_batch_size;
				length -= comm_max_batch_size;
			}
		}
		/* Process the command and give feedback */
		if (cmd_program) {
			status = cmd_program(rx_buffer, data_length, tx_buffer, &response_length, NUT_BUFFER_SIZE);
			/* Send response package according to response length */
			if (response_length > NUT_BUFFER_SIZE) {
				error_header[1] = NUT_ERROR_PAYLOAD_SIZE;
				HAL_UART_Transmit(&NUT_UART, error_header, 6, 100);
				/* Enable other interfaces */
				_NutComm_SPI_Enable();
				_NutComm_I2C_Enable();
				_NutComm_CAN_Enable();
				return;
			} else {
				/* Prepare header */
				if (status == NUT_OK) {
					tx_header[0] = NUT_OK;
					tx_header[1] = 0;
				} else {
					tx_header[0] = NUT_ERROR;
					tx_header[1] = NUT_ERROR_USER_CODE;
				}
				tx_header[2] = 0x0FF & (response_length >> 24);
				tx_header[3] = 0x0FF & (response_length >> 16);
				tx_header[4] = 0x0FF & (response_length >> 8);
				tx_header[5] = 0x0FF & (response_length);
				/* Send the header */
				HAL_UART_Transmit(&NUT_UART, tx_header, 6, 100);
				/* Then send the payload */
				tempptr = tx_buffer;
				while (response_length) {
					if (response_length < comm_max_batch_size) {
						HAL_UART_Transmit(&NUT_UART, tempptr, response_length, 1000);
						response_length = 0;
					} else {
						HAL_UART_Transmit(&NUT_UART, tempptr, comm_max_batch_size, 1000);
						tempptr += comm_max_batch_size;
						response_length -= comm_max_batch_size;
					}
				}
				/* Enable other interfaces */
				_NutComm_SPI_Enable();
				_NutComm_I2C_Enable();
				_NutComm_CAN_Enable();
				return;
			}
		}
		/* Command not found, return error */
		else {
			error_header[1] = NUT_ERROR_CMD_UNKNOWN;
			HAL_UART_Transmit(&NUT_UART, error_header, 6, 100);
			/* Enable other interfaces */
			_NutComm_SPI_Enable();
			_NutComm_I2C_Enable();
			_NutComm_CAN_Enable();
			return;
		}
	}

	/* Check SPI */
//	else if (0) {
	else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_RESET) {
		/* Clear Internal SS */
		NUT_SPI.Instance->CR1 &= ~SPI_CR1_SSI;
		/* Disable other interfaces */
		_NutComm_UART_Disable();
		_NutComm_I2C_Disable();
		_NutComm_CAN_Disable();
		/* Poll for header */
		for (i = 0; i < 8;) {
			retstatus = HAL_SPI_Receive(&NUT_SPI, rx_header + i, 1, 10);
			if (retstatus == HAL_OK) {
				i++;
			}
			/* if CS is released midway (in case of timeout), block if CS is held low */
			else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
				status = NUT_WARNING;
				error = NUT_ERROR_SPI_ABORTED;
				goto Fuck_SPI;
			}
		}
		/* Decode header */
		length = _NutComm_DecodeHeader();
		/* If ERROR occurred, prepare to send error package */
		if (status == NUT_ERROR) {
			/* Wait until the other side to finish transmission */
			for (i = 0; i < length;) {
				retstatus = HAL_SPI_Receive(&NUT_SPI, rx_buffer, 1, 10);
				if (retstatus == HAL_OK) {
					i++;
				}
				/* if CS is released midway (in case of timeout), block if CS is held low */
				else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
					status = NUT_WARNING;
					error = NUT_ERROR_SPI_ABORTED;
					goto Fuck_SPI;
				}
			}
			/* Send error package */
			error_header[1] = NUT_ERROR_PAYLOAD_SIZE;
			for (i = 0; i < 6;) {
				retstatus = HAL_SPI_Transmit(&NUT_SPI, error_header + i, 1, 10);
				if (retstatus == HAL_OK) {
					i++;
				}
				/* if CS is released midway (in case of timeout), block if CS is held low */
				else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
					status = NUT_WARNING;
					error = NUT_ERROR_SPI_ABORTED;
					goto Fuck_SPI;
				}
			}
			/* And just return */
			goto Fuck_SPI;
		}
		/* Receive the payload */
		data_length = length;
		tempptr = rx_buffer;
		for (i = 0; length;) {
			retstatus = HAL_SPI_Receive(&NUT_SPI, rx_buffer + i, 1, 10);
			if (retstatus == HAL_OK) {
				length--;
				i++;
			}
			/* if CS is released midway (in case of timeout), block if CS is held low */
			else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
				status = NUT_WARNING;
				error = NUT_ERROR_SPI_ABORTED;
				goto Fuck_SPI;
			}
		}
		/* Process the command and give feedback */
		if (cmd_program) {
			status = cmd_program(rx_buffer, data_length, tx_buffer, &response_length, NUT_BUFFER_SIZE);
			/* Send response package according to response length */
			if (response_length > NUT_BUFFER_SIZE) {
				error_header[1] = NUT_ERROR_PAYLOAD_SIZE;
				for (i = 0; i < 6;) {
					retstatus = HAL_SPI_Transmit(&NUT_SPI, error_header + i, 1, 10);
					if (retstatus == HAL_OK) {
						i++;
					}
					/* if CS is released midway (in case of timeout), block if CS is held low */
					else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
						status = NUT_WARNING;
						error = NUT_ERROR_SPI_ABORTED;
						goto Fuck_SPI;
					}
				}
				goto Fuck_SPI;
			} else {
				/* Prepare header */
				if (status == NUT_OK) {
					tx_header[0] = NUT_OK;
					tx_header[1] = 0;
				} else {
					tx_header[0] = NUT_ERROR;
					tx_header[1] = NUT_ERROR_USER_CODE;
				}
				tx_header[2] = 0x0FF & (response_length >> 24);
				tx_header[3] = 0x0FF & (response_length >> 16);
				tx_header[4] = 0x0FF & (response_length >> 8);
				tx_header[5] = 0x0FF & (response_length);
				/* Send the header */
				for (i = 0; i < 6;) {
					retstatus = HAL_SPI_Transmit(&NUT_SPI, tx_header + i, 1, 10);
					if (retstatus == HAL_OK) {
						i++;
					}
					/* if CS is released midway (in case of timeout), block if CS is held low */
					else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
						status = NUT_WARNING;
						error = NUT_ERROR_SPI_ABORTED;
						goto Fuck_SPI;
					}
				}
				/* Then send the payload */
				for (i = 0; i < response_length;) {
					retstatus = HAL_SPI_Transmit(&NUT_SPI, tx_buffer + i, 1, 10);
					if (retstatus == HAL_OK) {
						i++;
					}
					/* if CS is released midway (in case of timeout), block if CS is held low */
					else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
						status = NUT_WARNING;
						error = NUT_ERROR_SPI_ABORTED;
						goto Fuck_SPI;
					}
				}
				goto Fuck_SPI;
			}
		}
		/* Command not found, return error */
		else {
			error_header[1] = NUT_ERROR_CMD_UNKNOWN;
			for (i = 0; i < 6;) {
				retstatus = HAL_SPI_Transmit(&NUT_SPI, error_header + i, 1, 10);
				if (retstatus == HAL_OK) {
					i++;
				}
				/* if CS is released midway (in case of timeout), block if CS is held low */
				else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
					status = NUT_WARNING;
					error = NUT_ERROR_SPI_ABORTED;
					goto Fuck_SPI;
				}
			}
			Fuck_SPI:
			/* Wait until CSn releases */
			while (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_RESET)
				;
			/* Set Internal SS */
			NUT_SPI.Instance->CR1 |= SPI_CR1_SSI;
			/* Enable other interfaces */
			_NutComm_UART_Enable();
			_NutComm_I2C_Enable();
			_NutComm_CAN_Enable();
			return;
		}
	}

	/* Check I2C */
	else if (1) {
		// TODO
	}
}

void Nut_Init() {
	_NutComm_Init();
	Nut_LED(1);
	HAL_Delay(200);
	Nut_LED(0);
	HAL_Delay(200);
	User_Init();
}

void Nut_Quiet() {
	_NutComm_UART_Disable();
	_NutComm_SPI_Disable();
	_NutComm_I2C_Disable();
	_NutComm_CAN_Disable();
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;	// disable SysTick
}

void Nut_unQuiet() {
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;	// enable SysTick
	_NutComm_UART_Enable();
	_NutComm_SPI_Enable();
	_NutComm_I2C_Enable();
	_NutComm_CAN_Enable();
}

/* Pin Manipulation */

void Nut_LED(uint8_t on) {
	HAL_GPIO_WritePin(NUT_LED_PORT, NUT_LED_PIN, on ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void Nut_IO_1(uint8_t set) {
	HAL_GPIO_WritePin(NUT_IO1_PORT, NUT_IO1_PIN, set ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Nut_IO_2(uint8_t set) {
	HAL_GPIO_WritePin(NUT_IO2_PORT, NUT_IO2_PIN, set ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Nut_IO_3(uint8_t set) {
	HAL_GPIO_WritePin(NUT_IO3_PORT, NUT_IO3_PIN, set ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Nut_IO_USER(uint8_t set) {
	HAL_GPIO_WritePin(NUT_IO_USER_PORT, NUT_IO_USER_PIN, set ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* Stop CLK of peripheral */
inline void _NutComm_UART_Disable() {
	__HAL_RCC_USART1_CLK_DISABLE();
}
inline void _NutComm_SPI_Disable() {
	__HAL_RCC_SPI1_CLK_DISABLE();
}
inline void _NutComm_I2C_Disable() {
	__HAL_RCC_I2C1_CLK_DISABLE();
}
inline void _NutComm_CAN_Disable() {
	__HAL_RCC_CAN1_CLK_DISABLE();
}

/* Resume CLK of peripheral */
inline void _NutComm_UART_Enable() {
	__HAL_RCC_USART1_CLK_ENABLE();
}
inline void _NutComm_SPI_Enable() {
	__HAL_RCC_SPI1_CLK_ENABLE();
}
inline void _NutComm_I2C_Enable() {
	__HAL_RCC_I2C1_CLK_ENABLE();
}
inline void _NutComm_CAN_Enable() {
	__HAL_RCC_CAN1_CLK_ENABLE();
}
