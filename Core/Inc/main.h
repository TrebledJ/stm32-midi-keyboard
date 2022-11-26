/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdint.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

#define gpio_set(p)    HAL_GPIO_WritePin(p##_GPIO_Port, p##_Pin, GPIO_PIN_SET)
#define gpio_reset(p)  HAL_GPIO_WritePin(p##_GPIO_Port, p##_Pin, GPIO_PIN_RESET)
#define gpio_read(p)   HAL_GPIO_ReadPin(p##_GPIO_Port, p##_Pin)
#define gpio_toggle(p) HAL_GPIO_TogglePin(p##_GPIO_Port, p##_Pin)
#define delay(t)       HAL_Delay(t)
#define get_ticks()    HAL_GetTick()


/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define col5_Pin            GPIO_PIN_13
#define col5_GPIO_Port      GPIOC
#define row0_Pin            GPIO_PIN_14
#define row0_GPIO_Port      GPIOC
#define col7_Pin            GPIO_PIN_15
#define col7_GPIO_Port      GPIOC
#define TFT_BLK_Pin         GPIO_PIN_2
#define TFT_BLK_GPIO_Port   GPIOC
#define TFT_SDA_Pin         GPIO_PIN_3
#define TFT_SDA_GPIO_Port   GPIOC
#define col3_Pin            GPIO_PIN_0
#define col3_GPIO_Port      GPIOA
#define ADC_vol_Pin         GPIO_PIN_1
#define ADC_vol_GPIO_Port   GPIOA
#define SPEAKER_L_Pin       GPIO_PIN_4
#define SPEAKER_L_GPIO_Port GPIOA
#define SPEAKER_R_Pin       GPIO_PIN_5
#define SPEAKER_R_GPIO_Port GPIOA
#define BUZZER_Pin          GPIO_PIN_6
#define BUZZER_GPIO_Port    GPIOA
#define LED2_Pin            GPIO_PIN_7
#define LED2_GPIO_Port      GPIOA
#define LED1_Pin            GPIO_PIN_4
#define LED1_GPIO_Port      GPIOC
#define LED3_Pin            GPIO_PIN_5
#define LED3_GPIO_Port      GPIOC
#define LED0_Pin            GPIO_PIN_0
#define LED0_GPIO_Port      GPIOB
#define TFT_RES_Pin         GPIO_PIN_1
#define TFT_RES_GPIO_Port   GPIOB
#define TFT_DC_Pin          GPIO_PIN_2
#define TFT_DC_GPIO_Port    GPIOB
#define TFT_CLK_Pin         GPIO_PIN_10
#define TFT_CLK_GPIO_Port   GPIOB
#define TFT_CS_Pin          GPIO_PIN_11
#define TFT_CS_GPIO_Port    GPIOB
#define UART_TX_Pin         GPIO_PIN_9
#define UART_TX_GPIO_Port   GPIOA
#define UART_RX_Pin         GPIO_PIN_10
#define UART_RX_GPIO_Port   GPIOA
#define row7_Pin            GPIO_PIN_15
#define row7_GPIO_Port      GPIOA
#define row6_Pin            GPIO_PIN_10
#define row6_GPIO_Port      GPIOC
#define row5_Pin            GPIO_PIN_11
#define row5_GPIO_Port      GPIOC
#define row4_Pin            GPIO_PIN_12
#define row4_GPIO_Port      GPIOC
#define row3_Pin            GPIO_PIN_2
#define row3_GPIO_Port      GPIOD
#define row2_Pin            GPIO_PIN_3
#define row2_GPIO_Port      GPIOB
#define col2_Pin            GPIO_PIN_4
#define col2_GPIO_Port      GPIOB
#define col4_Pin            GPIO_PIN_5
#define col4_GPIO_Port      GPIOB
#define col0_Pin            GPIO_PIN_6
#define col0_GPIO_Port      GPIOB
#define row1_Pin            GPIO_PIN_7
#define row1_GPIO_Port      GPIOB
#define col1_Pin            GPIO_PIN_8
#define col1_GPIO_Port      GPIOB
#define col6_Pin            GPIO_PIN_9
#define col6_GPIO_Port      GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
