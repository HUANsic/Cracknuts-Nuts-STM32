/*
 * nut_communication.c
 *
 *  Created on: Jun 24, 2025
 *      Author: abcde
 */

#include "NutsLib.h"
#include "user.h"

/* Communication Interfaces */
extern CAN_HandleTypeDef NUT_CAN;
extern I2C_HandleTypeDef NUT_I2C;
extern SPI_HandleTypeDef NUT_SPI;
extern UART_HandleTypeDef NUT_UART;

/* Command */
extern uint16_t cmd_list[];
/* Command program, returns result status */
extern NutStatus_e (*cmd_prog_list[])(uint8_t *received_data_ptr, uint32_t received_data_length, uint8_t *result_buffer_ptr, uint32_t *result_length,
		uint32_t result_buffer_MAX_size);

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

/*  CONSTANTS  */
uint8_t error_header[] = { NUT_ERROR, NUT_ERROR_UNKNOWN, 0, 0, 0, 0 };	// payload size too large
uint8_t response_header[] = { NUT_OK, ~NUT_OK, 0, 0, 0, 0 };

/* DEBUG */
uint8_t success_cmd_count = 0;
uint8_t received_any_count = 0;

/* Stop CLK of peripheral without needing to re-initialize */
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

/* Last things to do before next communucation */
void _NutComm_UART_Quit() {
	/* Empty buffer */
	// uint8_t tmp = NUT_UART.Instance->DR;
	// (void)tmp;
	/* Enable other interfaces */
	_NutComm_SPI_Enable();
	_NutComm_I2C_Enable();
	_NutComm_CAN_Enable();
}
void _NutComm_SPI_Quit() {
	/* Wait until CSn releases */
	while (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_RESET)
		;
	/* Set Internal SS */
	NUT_SPI.Instance->CR1 |= SPI_CR1_SSI;
	/* Enable other interfaces */
	_NutComm_UART_Enable();
	_NutComm_I2C_Enable();
	_NutComm_CAN_Enable();
}
void _NutComm_I2C_Quit() {

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

/* Initializes communication interfaces */
void _NutComm_Init() {

}

/* Continuously check for signs of communication */
void Nut_loop() {
	HAL_StatusTypeDef retstatus = HAL_OK;
	uint32_t length;
	uint32_t response_length = 0;
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
		for (i = 1; i < 8;) {
			retstatus = HAL_UART_Receive(&NUT_UART, rx_header + i, 1, 100);
			if (retstatus == HAL_OK) {
				i++;
			}
			/* in case of timeout and error, must exit because UART is special and must not get stuck */
			else if (retstatus == HAL_TIMEOUT) {
				status = NUT_WARNING;
				error = NUT_ERROR_TIMEOUT;
				_NutComm_UART_Quit();
				return;
			} else if (retstatus != HAL_OK) {
				status = NUT_ERROR;
				error = NUT_ERROR_UNKNOWN;
				_NutComm_UART_Quit();
				return;
			}
		}

		/* Decode header */
		status = NUT_OK;
		length = _NutComm_DecodeHeader();
		/* If ERROR occurred, prepare to send error package */
		if (status == NUT_ERROR) {
			/* Wait until the other side to finish transmission */
			for (i = 0; i < length;) {
				retstatus = HAL_UART_Receive(&NUT_UART, rx_buffer, 1, 100);
				if (retstatus == HAL_OK) {
					i++;
				}
				/* in case of timeout and error, must exit because UART is special and must not get stuck */
				else if (retstatus == HAL_TIMEOUT) {
					status = NUT_WARNING;
					error = NUT_ERROR_TIMEOUT;
					_NutComm_UART_Quit();
					return;
				} else if (retstatus != HAL_OK) {
					status = NUT_ERROR;
					error = NUT_ERROR_UNKNOWN;
					_NutComm_UART_Quit();
					return;
				}
			}
			/* Send error package */
			tx_header[0] = NUT_ERROR;
			tx_header[1] = NUT_ERROR_PAYLOAD_SIZE;
			tx_header[2] = 0;
			tx_header[3] = 0;
			tx_header[4] = 0;
			tx_header[5] = 0;
			HAL_UART_Transmit(&NUT_UART, tx_header, 6, 100);	// no need to check whether it is successful
			_NutComm_UART_Quit();
			return;
		}
		/* Receive the payload */
		for (i = 0; i < length;) {
			retstatus = HAL_UART_Receive(&NUT_UART, rx_buffer + i, 1, 100);
			if (retstatus == HAL_OK) {
				i++;
			}
			/* in case of timeout and error, must exit because UART is special and must not get stuck */
			else if (retstatus == HAL_TIMEOUT) {
				status = NUT_WARNING;
				error = NUT_ERROR_TIMEOUT;
				_NutComm_UART_Quit();
				return;
			} else if (retstatus != HAL_OK) {
				status = NUT_ERROR;
				error = NUT_ERROR_UNKNOWN;
				_NutComm_UART_Quit();
				return;
			}
		}
		/* Process the command and give feedback */
		if (cmd_program) {
			status = cmd_program(rx_buffer, length, tx_buffer, &response_length, NUT_BUFFER_SIZE);
			/* Send response package according to response length */
			if (response_length > NUT_BUFFER_SIZE) {
				tx_header[0] = NUT_ERROR;
				tx_header[1] = NUT_ERROR_PAYLOAD_SIZE;
				tx_header[2] = 0;
				tx_header[3] = 0;
				tx_header[4] = 0;
				tx_header[5] = 0;
				HAL_UART_Transmit(&NUT_UART, tx_header, 6, 100);	// no need to check whether it is successful
				_NutComm_UART_Quit();
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
				for (i = 0; i < response_length;) {
					retstatus = HAL_UART_Transmit(&NUT_UART, tx_buffer + i, 1, 100);
					if (retstatus == HAL_OK) {
						i++;
					}
					/* in case of timeout and error, must exit because UART is special and must not get stuck */
					else if (retstatus == HAL_TIMEOUT) {
						status = NUT_WARNING;
						error = NUT_ERROR_TIMEOUT;
						_NutComm_UART_Quit();
						return;
					} else if (retstatus != HAL_OK) {
						status = NUT_ERROR;
						error = NUT_ERROR_UNKNOWN;
						_NutComm_UART_Quit();
						return;
					}
				}
				_NutComm_UART_Quit();
				return;
			}
		}
		/* Command not found, return error */
		else {
			tx_header[0] = NUT_ERROR;
			tx_header[1] = NUT_ERROR_CMD_UNKNOWN;
			tx_header[2] = 0;
			tx_header[3] = 0;
			tx_header[4] = 0;
			tx_header[5] = 0;
			HAL_UART_Transmit(&NUT_UART, tx_header, 6, 100);	// no need to check whether it is successful
			_NutComm_UART_Quit();
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
			retstatus = HAL_SPI_Receive(&NUT_SPI, rx_header + i, 1, 100);
			if (retstatus == HAL_OK) {
				i++;
			}
			/* if CS is released midway (in case of timeout), block if CS is held low */
			else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
				status = NUT_WARNING;
				error = NUT_ERROR_SPI_ABORTED;
				_NutComm_SPI_Quit();
				return;
			}
		}
		/* Decode header */
		length = _NutComm_DecodeHeader();
		/* If ERROR occurred, prepare to send error package */
		if (status == NUT_ERROR) {
			/* Wait until the other side to finish transmission */
			for (i = 0; i < length;) {
				retstatus = HAL_SPI_Receive(&NUT_SPI, rx_buffer, 1, 100);
				if (retstatus == HAL_OK) {
					i++;
				}
				/* if CS is released midway (in case of timeout), block if CS is held low */
				else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
					status = NUT_WARNING;
					error = NUT_ERROR_SPI_ABORTED;
					_NutComm_SPI_Quit();
					return;
				}
			}
			/* Send error package */
			tx_header[0] = NUT_ERROR;
			tx_header[1] = NUT_ERROR_PAYLOAD_SIZE;
			tx_header[2] = 0;
			tx_header[3] = 0;
			tx_header[4] = 0;
			tx_header[5] = 0;
			for (i = 0; i < 6;) {
				retstatus = HAL_SPI_Transmit(&NUT_SPI, tx_header + i, 1, 100);
				if (retstatus == HAL_OK) {
					i++;
				}
				/* if CS is released midway (in case of timeout), block if CS is held low */
				else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
					status = NUT_WARNING;
					error = NUT_ERROR_SPI_ABORTED;
					_NutComm_SPI_Quit();
					return;
				}
			}
			/* And just return */
			_NutComm_SPI_Quit();
			return;
		}
		/* Receive the payload */
		for (i = 0; i < length;) {
			retstatus = HAL_SPI_Receive(&NUT_SPI, rx_buffer + i, 1, 100);
			if (retstatus == HAL_OK) {
				i++;
			}
			/* if CS is released midway (in case of timeout), block if CS is held low */
			else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
				status = NUT_WARNING;
				error = NUT_ERROR_SPI_ABORTED;
				_NutComm_SPI_Quit();
				return;
			}
		}
		/* Process the command and give feedback */
		if (cmd_program) {
			status = cmd_program(rx_buffer, length, tx_buffer, &response_length, NUT_BUFFER_SIZE);
			/* Send response package according to response length */
			if (response_length > NUT_BUFFER_SIZE) {
				error_header[1] = NUT_ERROR_PAYLOAD_SIZE;
				for (i = 0; i < 6;) {
					retstatus = HAL_SPI_Transmit(&NUT_SPI, error_header + i, 1, 100);
					if (retstatus == HAL_OK) {
						i++;
					}
					/* if CS is released midway (in case of timeout), block if CS is held low */
					else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
						status = NUT_WARNING;
						error = NUT_ERROR_SPI_ABORTED;
						_NutComm_SPI_Quit();
						return;
					}
				}
				_NutComm_SPI_Quit();
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
				for (i = 0; i < 6;) {
					retstatus = HAL_SPI_Transmit(&NUT_SPI, tx_header + i, 1, 100);
					if (retstatus == HAL_OK) {
						i++;
					}
					/* if CS is released midway (in case of timeout), block if CS is held low */
					else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
						status = NUT_WARNING;
						error = NUT_ERROR_SPI_ABORTED;
						_NutComm_SPI_Quit();
						return;
					}
				}
				/* Then send the payload */
				for (i = 0; i < response_length;) {
					retstatus = HAL_SPI_Transmit(&NUT_SPI, tx_buffer + i, 1, 100);
					if (retstatus == HAL_OK) {
						i++;
					}
					/* if CS is released midway (in case of timeout), block if CS is held low */
					else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
						status = NUT_WARNING;
						error = NUT_ERROR_SPI_ABORTED;
						_NutComm_SPI_Quit();
						return;
					}
				}
				_NutComm_SPI_Quit();
				return;
			}
		}
		/* Command not found, return error */
		else {
			error_header[1] = NUT_ERROR_CMD_UNKNOWN;
			for (i = 0; i < 6;) {
				retstatus = HAL_SPI_Transmit(&NUT_SPI, error_header + i, 1, 100);
				if (retstatus == HAL_OK) {
					i++;
				}
				/* if CS is released midway (in case of timeout), block if CS is held low */
				else if (HAL_GPIO_ReadPin(NUT_SPI_CS_PORT, NUT_SPI_CS_PIN) == GPIO_PIN_SET) {
					status = NUT_WARNING;
					error = NUT_ERROR_SPI_ABORTED;
					_NutComm_SPI_Quit();
					return;
				}
			}
			_NutComm_SPI_Quit();
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
	User_Init();
	Nut_LED(0);
}

void Nut_Quiet() {
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;	// disable SysTick
}

void Nut_unQuiet() {
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;	// enable SysTick
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
