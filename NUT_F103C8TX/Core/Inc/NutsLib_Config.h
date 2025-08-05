/*
 * nut_config.h
 *
 *  Created on: Jun 24, 2025
 *      Author: abcde
 */

#ifndef INC_NUTSLIB_CONFIG_H_
#define INC_NUTSLIB_CONFIG_H_

#include "stm32f1xx_hal.h"

/*  DEVICE DEPENDENT SETTINGS  */
// F103C8 - 20K RAM total
#define		NUT_BUFFER_SIZE		4096

/*  PIN DEFINITIONS  */

#define		NUT_LED_PORT		GPIOB
#define		NUT_LED_PIN			GPIO_PIN_9

#define		NUT_IO_USER_PORT	GPIOB
#define		NUT_IO_USER_PIN		GPIO_PIN_8

#define		NUT_IO1_PORT		GPIOB
#define		NUT_IO1_PIN			GPIO_PIN_0

#define		NUT_IO2_PORT		GPIOB
#define		NUT_IO2_PIN			GPIO_PIN_1

#define		NUT_IO3_PORT		GPIOB
#define		NUT_IO3_PIN			GPIO_PIN_2

/*  ALTERNATE FUNCTION DEFINITIONS  */

#define		NUT_I2C				hi2c1

#define		NUT_SPI				hspi1
#define		NUT_SPI_CS_PORT		GPIOA
#define		NUT_SPI_CS_PIN		GPIO_PIN_4

#define		NUT_UART			huart1

#define		NUT_CAN				hcan

#endif /* INC_NUTSLIB_CONFIG_H_ */
