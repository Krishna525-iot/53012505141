/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>      // Needed for printf
#include "gesture.h"  // Ensure this header is included
//#include "inactivity_manager.h"

//#include "gesture.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* Definitions for GestureManager */
osThreadId_t GestureManagerHandle;
const osThreadAttr_t GestureManager_attributes = {
  .name = "GestureManager",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for InputManager */
osThreadId_t InputManagerHandle;
const osThreadAttr_t InputManager_attributes = {
  .name = "InputManager",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* USER CODE BEGIN PV */

#define EEPRO_LONG_PRESS_DURATION 20000   // 20 seconds in ms
#define LONG_PRESS_DURATION 2500
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
void GestureManagerTask(void *argument);
void InputManagerTask(void *argument);

/* USER CODE BEGIN PFP */


void CheckButtonLongPress(void);
void sensor_status_update(void);
void short_press_action(void);
void depth_long_press(void);
void depth_short_press(void);
void MyGestureEventHandler(InputAction event);
void MyButtonEventHandler(ButtonEvent *event);
void ResetUART(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* Global variable to track system uptime */
volatile uint32_t systemTimeSec = 0;
#define LONG_PRESS_DURATION 2500  // Long press duration in milliseconds
uint32_t buttonPressStartTime = 0;
uint8_t buttonPressed = 0;


void sensor_status_update(void)
{
	InputEvent input_event = { .source = INPUT_SOURCE_BUTTON };

	input_event.action = INPUT_LONG_MOVE_UP;

	enqueue_event(input_event); //
}

/* Function to Check Long Press */



void short_press_action()
{
	InputEvent input_event = { .source = INPUT_SOURCE_BUTTON };

	input_event.action = INPUT_MOVE_UP;

	enqueue_event(input_event);

}



uint8_t ges_type = 0;
void depth_long_press() {
    // Call the system uptime function or relevant action on depth long press
    systemTimeSec++;
    SaveUptimeToEEPROM(systemTimeSec);
    systemup_time();

}



void depth_short_press()
{
	InputEvent input_event = { .source = INPUT_SOURCE_BUTTON };

	input_event.action = INPUT_DEPTH;

	enqueue_event(input_event);

}
uint8_t buttonId = 0;
void CheckButtonLongPress(void) {
    if (HAL_GPIO_ReadPin(S_PRV_GPIO_Port, S_PRV_Pin) == GPIO_PIN_RESET) {
        if (buttonPressed == 0) {
            buttonPressed = 1;
            buttonId = 1;
            buttonPressStartTime = HAL_GetTick();
        } else if (buttonPressed == 1) {
            if ((HAL_GetTick() - buttonPressStartTime) >= LONG_PRESS_DURATION) {
                sensor_status_update();
                buttonPressed = 2;
            }
        }
    }
    else if (HAL_GPIO_ReadPin(S_NEXT_GPIO_Port, S_NEXT_Pin) == GPIO_PIN_RESET) {
        if (buttonPressed == 0) {
            buttonPressed = 1;
            buttonId = 1;
            buttonPressStartTime = HAL_GetTick();
        } else if (buttonPressed == 1) {
            if ((HAL_GetTick() - buttonPressStartTime) >= EEPRO_LONG_PRESS_DURATION) {
            	ResetSystemUptime();
                buttonPressed = 2;
            }
        }
    }
    else if (HAL_GPIO_ReadPin(DEPTH_GPIO_Port, DEPTH_Pin) == GPIO_PIN_RESET) {
        if (buttonPressed == 0) {
            buttonPressed = 1;
            buttonId = 2;
            buttonPressStartTime = HAL_GetTick();
        } else if (buttonPressed == 1) {
            if ((HAL_GetTick() - buttonPressStartTime) >= LONG_PRESS_DURATION) {
                depth_long_press();
                buttonPressed = 2;
            }
        }
    }
    else if (HAL_GPIO_ReadPin(CHANGE_P_GPIO_Port, CHANGE_P_Pin) == GPIO_PIN_RESET) {
        if (buttonPressed == 0) {
            buttonPressed = 1;
            buttonId = 3;
            buttonPressStartTime = HAL_GetTick();
        } else if (buttonPressed == 1) {
            if ((HAL_GetTick() - buttonPressStartTime) >= LONG_PRESS_DURATION) {
                buttonPressed = 2;
            }
        }
    }
    else {
        if (buttonPressed == 1) {
            if ((HAL_GetTick() - buttonPressStartTime) < LONG_PRESS_DURATION) {
                if (buttonId == 1) short_press_action();
                else if (buttonId == 3) {
                    InputEvent e = { .source = INPUT_SOURCE_BUTTON, .action = INPUT_CHANGE_P };
                    enqueue_event(e);
                }
            }
        }
        buttonPressed = 0;
        buttonId = 0;
    }
}
void MyGestureEventHandler(InputAction event) {
    InputEvent input_event = { .source = INPUT_SOURCE_GESTURE };

    switch (event) {
        case INPUT_MOVE_DOWN:
            input_event.action = INPUT_MOVE_DOWN;
            ges_type = 1;
            break;
        case INPUT_MOVE_UP:
            input_event.action = INPUT_MOVE_UP;
            ges_type = 0;
            break;
        case INPUT_BACK:
            input_event.action = INPUT_BACK;
            ges_type = 2;
            break;
        case INPUT_SELECT:
            input_event.action = INPUT_SELECT;
            ges_type = 3;
            break;
        default:
            return;
    }
    enqueue_event(input_event);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    ButtonManager_HandleInterrupt(GPIO_Pin);
}
void MyButtonEventHandler(ButtonEvent *event) {
    InputEvent e = { .source = INPUT_SOURCE_BUTTON, .action = INPUT_UNKNOWN };

    switch (event->action) {
    case BUTTON_ACTION_SHORT_PRESS:
        e.action = (event->button_id == 1) ? INPUT_MOVE_DOWN :
                   (event->button_id == 2) ? INPUT_SELECT   :
                   (event->button_id == 3) ? INPUT_BACK     :
                   (event->button_id == 4) ? INPUT_DEPTH    :
                   (event->button_id == 5) ? INPUT_CHANGE_P : INPUT_UNKNOWN;
        break;
    case BUTTON_ACTION_LONG_PRESS:
        if (event->button_id == 4) {
            e.action = INPUT_LONG_DEPTH;
        } else if (event->button_id == 5) {
            e.action = INPUT_LONG_CHANGE_P;
        }
        break;
    default: break;
    }
    if (e.action != INPUT_UNKNOWN) enqueue_event(e);
}
int __io_putchar(int ch) {
    return ch;
}
extern volatile uint8_t newData;
void ResetUART(void) {
	 HAL_UART_DeInit(&huart2);
	 MX_USART2_UART_Init();
	  HAL_UART_Receive_IT(&huart2, getRxBuffer(), 1);
}
void ResetSystemUptime(void) {
    systemTimeSec = 0;
    SaveUptimeToEEPROM(systemTimeSec);
    systemup_time();
}
uint8_t I2C_Scanner(void)
{
    char msg[20];
    uint8_t found = 0;
    LCD_Clear();
    LCD_WriteString("Scanning...");
    HAL_Delay(500);
    for (uint8_t addr = 1; addr < 128; addr++)
    {
        if (HAL_I2C_IsDeviceReady(&hi2c1, addr << 1, 2, 10) == HAL_OK)
        {
            sprintf(msg, "Found:0x%02X", addr);
            LCD_Clear();
            LCD_WriteString(msg);
            HAL_Delay(1000);
            found = addr;
        }
    }
    LCD_Clear();
    if (found)
    {
        sprintf(msg, "Dev:0x%02X", found);
        LCD_WriteString(msg);
    }
    else
    {
        LCD_WriteString("No device!");
    }
    HAL_Delay(1000);
    return found;
}
void EEPROM_Debug_Test(void)
{
    char msg[20];
    uint8_t dev_addr = I2C_Scanner();
    if (dev_addr == 0)
    {
        LCD_Clear();
        LCD_WriteString("No I2C Dev Found");
        return;
    }
    LCD_Clear();
    LCD_WriteString("EEPROM Debug");
    HAL_Delay(1000);
    uint8_t val = 99;
    uint8_t val_read = 0;
    HAL_StatusTypeDef status;
    for (uint8_t block = 0; block < 2; block++)
    {
        uint8_t device = 0x50 | block;
        uint8_t memAddr = 0x10;
        LCD_Clear();
        sprintf(msg, "W:B%d Addr0x%02X", block, device);
        LCD_WriteString(msg);
        HAL_Delay(500);
        status = HAL_I2C_Mem_Write(&hi2c1,
                                   (device << 1),
                                   memAddr,
                                   I2C_MEMADD_SIZE_8BIT,
                                   &val,
                                   1,
                                   1000);
        if (status != HAL_OK)
        {
            uint32_t err = HAL_I2C_GetError(&hi2c1);
            LCD_Clear();
            sprintf(msg, "WErr B%d:%lu", block, err);
            LCD_WriteString(msg);
            HAL_Delay(1500);
            continue;
        }
        HAL_Delay(10);
        val_read = 0;
        status = HAL_I2C_Mem_Read(&hi2c1,
                                  (device << 1),
                                  memAddr,
                                  I2C_MEMADD_SIZE_8BIT,
                                  &val_read,
                                  1,
                                  1000);
        LCD_Clear();
        if (status == HAL_OK)
        {
            if (val_read == val)
            {
                sprintf(msg, "B%d OK:%d", block, val_read);
            }
            else
            {
                sprintf(msg, "B%d MISM:%d", block, val_read);
            }
        }
        else
        {
            uint32_t err = HAL_I2C_GetError(&hi2c1);
            sprintf(msg, "R ERR:%lu", err);
        }
        LCD_WriteString(msg);
        HAL_Delay(1500);
    }
    LCD_Clear();
    LCD_WriteString("EEPROM OK");
    HAL_Delay(2000);
}
void EEPROM_Uptime_Test(void)
{
    LCD_Clear();
    LCD_WriteString("Uptime Test...");
    HAL_Delay(1000);
    HAL_StatusTypeDef status;
    uint32_t uptime_write = 0x12345678;
    uint32_t uptime_read  = 0;
    status = SaveUptimeToEEPROM(uptime_write);
    HAL_Delay(20);
    LCD_Clear();
    if (status == HAL_OK)
    {
        LCD_WriteString("Write: OK");
    }
    else
    {
        char msg[16];
        sprintf(msg, "W ERR:%lu", HAL_I2C_GetError(&hi2c1));
        LCD_WriteString(msg);
        return;
    }
    HAL_Delay(1000);
    status = LoadUptimeFromEEPROM(&uptime_read);
    LCD_Clear();
    if (status == HAL_OK)
    {
        char msg[20];
        sprintf(msg, "Read: %08lX", uptime_read);
        LCD_WriteString(msg);
    }
    else
    {
        char msg[16];
        sprintf(msg, "R ERR:%lu", HAL_I2C_GetError(&hi2c1));
        LCD_WriteString(msg);
    }

    HAL_Delay(3000);
    LCD_Clear();
    if (uptime_read == uptime_write)
        LCD_WriteString("✅ MATCH OK");
    else
    {
        char msg[20];
        sprintf(msg, "❌ MISM:%08lX", uptime_read);
        LCD_WriteString(msg);
    }
    HAL_Delay(3000);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
//  InactivityManager_CheckAndAutoOff();
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of GestureManager */
  GestureManagerHandle = osThreadNew(GestureManagerTask, NULL, &GestureManager_attributes);

  /* creation of InputManager */
  InputManagerHandle = osThreadNew(InputManagerTask, NULL, &InputManager_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	ButtonManager_Init(MyButtonEventHandler);

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */

	 UART_Packet_Init(&huart2);
//	 initI2CSemaphore();  // Create the semaphore
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10805D88;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_5_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_D4_Pin|LCD_D5_Pin|LCD_EN_Pin|LCD_RS_Pin
                          |BUZZER_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED1_Pin|GPIO_PIN_1|LCD_D7_Pin|LCD_D6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_D4_Pin LCD_D5_Pin LCD_EN_Pin LCD_RS_Pin
                           BUZZER_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LCD_D4_Pin|LCD_D5_Pin|LCD_EN_Pin|LCD_RS_Pin
                          |BUZZER_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin PB1 LCD_D7_Pin LCD_D6_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|GPIO_PIN_1|LCD_D7_Pin|LCD_D6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : STANDBY_Pin S_PRV_Pin S_NEXT_Pin DEPTH_Pin
                           CHANGE_N_Pin */
  GPIO_InitStruct.Pin = STANDBY_Pin|S_PRV_Pin|S_NEXT_Pin|DEPTH_Pin
                          |CHANGE_N_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : IR_N_Pin */
  GPIO_InitStruct.Pin = IR_N_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IR_N_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CHANGE_P_Pin */
  GPIO_InitStruct.Pin = CHANGE_P_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CHANGE_P_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : cam_enable_Pin */
  GPIO_InitStruct.Pin = cam_enable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(cam_enable_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/* USER CODE BEGIN Header_GestureManagerTask */


int8_t mylastgesture = 0;
uint8_t loadscr = 0;
/**
 * @brief  Function implementing the GestureManager thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_GestureManagerTask */
void GestureManagerTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	/* Infinite loop */
//	LCD_Init();
//	init_screen1(0);
	//----------------
	init_gesture();
	InactivityManager_Init();
//	uint8_t data_1[5];


//	sensor_state = 1;
	for (;;) {
		CheckButtonLongPress();
		InactivityManager_CheckAndAutoOff();

		if (isDataReady()) {
			uint8_t *rxData = getRxBuffer(); // Retrieve the buffer pointer
//			strncpy(data_1, rxData, sizeof(data_1) - 1);
			if(rxData[1] == 'F' || rxData[1] == 'M')
			{
				if( rxData[3] == '2' || rxData[3] == '3' )
				{

				}
				else
				{
					loadscr = 1; //
				}

			}
			else
			{
				loadscr = 0;
			}

			if(screen_state != loadscr)
			{
				if(loadscr)
				{
					init_second_screen();
//						load_focus_options();
						screen_state =1;
				}

				else
				{
							LCD_Init();
							init_screen1(0);
							update_screen1_with_current_values();
							screen_state =0;
				}

			}

			if( rxData[3] != '2' || rxData[3] != '3' )
			{
				process_uart_command((const char *)rxData);
			}

			resetDataReady();

		}
		process_input_events();
		//------------------------
		init_gesture();
		if (sensor_state == 1)
		{
			gestureAvailable();
		       if(newData )
		       {

		    	   mylastgesture = readGesture();
		    	   MyGestureEventHandler(mylastgesture);
		    	   newData = 0;
		       }
		}


		osDelay(pdMS_TO_TICKS(5));
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_InputManagerTask */
/**
 * @brief Function implementing the InputManager thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_InputManagerTask */
void InputManagerTask(void *argument)
{
  /* USER CODE BEGIN InputManagerTask */
	LCD_Init();
	init_screen1(0);

	uint16_t count =0;

	 ResetUART();
	/* Infinite loop */
	for (;;) {

		systemTimeSec++;
		 count++;
		 if(count > 60000)
		 {
			 count=0;
			 ResetUART();
		 }
		osDelay(pdMS_TO_TICKS(1000)); // Task delay

	}
  /* USER CODE END InputManagerTask */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
