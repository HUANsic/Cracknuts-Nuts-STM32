/*
 * nut_config.h
 *
 *  Created on: Jun 24, 2025
 *      Author: abcde
 */

#ifndef INC_NUTSLIB_CONFIG_H_
#define INC_NUTSLIB_CONFIG_H_

#include "stm32f4xx_hal.h"

/*  DEVICE DEPENDENT SETTINGS  */
// F407VG - 112K(RAM1)+64K(RAM2)+16K(RAM3)+64K(CCM)=256K RAM total
#define		NUT_BUFFER_SIZE		512

/*  PIN DEFINITIONS  */

#define		NUT_LED_PORT		GPIOB
#define		NUT_LED_PIN			GPIO_PIN_7

#define		NUT_IO_USER_PORT	GPIOB
#define		NUT_IO_USER_PIN		GPIO_PIN_6

#define		NUT_IO1_PORT		GPIOD
#define		NUT_IO1_PIN			GPIO_PIN_13

#define		NUT_IO2_PORT		GPIOD
#define		NUT_IO2_PIN			GPIO_PIN_14

#define		NUT_IO3_PORT		GPIOD
#define		NUT_IO3_PIN			GPIO_PIN_15

/*  ALTERNATE FUNCTION DEFINITIONS  */

#define		NUT_I2C				hi2c2

#define		NUT_SPI				hspi1
#define		NUT_SPI_CS_PORT		GPIOA
#define		NUT_SPI_CS_PIN		GPIO_PIN_4

#define		NUT_UART			huart1

#define		NUT_CAN				hcan2

#endif /* INC_NUTSLIB_CONFIG_H_ */
