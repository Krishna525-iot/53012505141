/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "globals.h"
#include "lcd.h"
#include "screen1.h"
#include "screen2.h"
#include "button_manager.h"
#include "input_manager.h"
#include "uart.h"
#include "uart_command_handler.h"

#include "FreeRTOS.h"
#include "task.h" // For vTaskDelete
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_D4_Pin GPIO_PIN_0
#define LCD_D4_GPIO_Port GPIOA
#define LCD_D5_Pin GPIO_PIN_1
#define LCD_D5_GPIO_Port GPIOA
#define LCD_EN_Pin GPIO_PIN_4
#define LCD_EN_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_5
#define LCD_RS_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_6
#define BUZZER_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOB
#define STANDBY_Pin GPIO_PIN_2
#define STANDBY_GPIO_Port GPIOB
#define S_PRV_Pin GPIO_PIN_11
#define S_PRV_GPIO_Port GPIOB
#define S_PRV_EXTI_IRQn EXTI4_15_IRQn
#define S_NEXT_Pin GPIO_PIN_12
#define S_NEXT_GPIO_Port GPIOB
#define S_NEXT_EXTI_IRQn EXTI4_15_IRQn
#define DEPTH_Pin GPIO_PIN_13
#define DEPTH_GPIO_Port GPIOB
#define DEPTH_EXTI_IRQn EXTI4_15_IRQn
#define IR_N_Pin GPIO_PIN_14
#define IR_N_GPIO_Port GPIOB
#define CHANGE_N_Pin GPIO_PIN_15
#define CHANGE_N_GPIO_Port GPIOB
#define CHANGE_N_EXTI_IRQn EXTI4_15_IRQn
#define LED2_Pin GPIO_PIN_8
#define LED2_GPIO_Port GPIOA
#define CHANGE_P_Pin GPIO_PIN_10
#define CHANGE_P_GPIO_Port GPIOA
#define CHANGE_P_EXTI_IRQn EXTI4_15_IRQn
#define cam_enable_Pin GPIO_PIN_7
#define cam_enable_GPIO_Port GPIOF
#define LCD_D7_Pin GPIO_PIN_4
#define LCD_D7_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_5
#define LCD_D6_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
