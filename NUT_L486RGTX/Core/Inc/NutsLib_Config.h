/*
 * nut_config.h
 *
 *  Created on: Jun 24, 2025
 *      Author: abcde
 */

#ifndef INC_NUTSLIB_CONFIG_H_
#define INC_NUTSLIB_CONFIG_H_

#include "stm32l4xx_hal.h"

/*  DEVICE DEPENDENT SETTINGS  */
// F439VG - 128K RAM total
#define		NUT_BUFFER_SIZE		20000	// 20K to be safe

/*  PIN DEFINITIONS  */

#define		NUT_LED_PORT		GPIOB
#define		NUT_LED_PIN			GPIO_PIN_6

#define		NUT_IO_USER_PORT	GPIOC
#define		NUT_IO_USER_PIN		GPIO_PIN_7

#define		NUT_IO1_PORT		GPIOC
#define		NUT_IO1_PIN			GPIO_PIN_7

#define		NUT_IO2_PORT		GPIOC
#define		NUT_IO2_PIN			GPIO_PIN_8

#define		NUT_IO3_PORT		GPIOC
#define		NUT_IO3_PIN			GPIO_PIN_9

/*  ALTERNATE FUNCTION DEFINITIONS  */

#define		NUT_I2C				hi2c2

#define		NUT_SPI				hspi2
#define		NUT_SPI_CS_PORT		GPIOA
#define		NUT_SPI_CS_PIN		GPIO_PIN_1

#define		NUT_UART			huart1

#define		NUT_CAN				hcan1

#endif /* INC_NUTSLIB_CONFIG_H_ */
