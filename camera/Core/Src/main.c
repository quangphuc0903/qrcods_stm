/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mb.h"
#include "mbport.h"
#include "user_mb_app.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t check_messenger(uint8_t *mes,uint8_t len);int32_t Tag_ID;
int32_t x_coor;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t messages_camera[21];

extern uint16_t   usSRegInBuf[];

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
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(DR_uart2_GPIO_Port,DR_uart2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DR_uart3_GPIO_Port,DR_uart3_Pin,GPIO_PIN_RESET);
	
	eMBErrorCode eStatus = eMBInit( MB_RTU, 2, 3, 115200, MB_PAR_EVEN );
  eStatus = eMBEnable();
  /* USER CODE END 2 */
	uint32_t time_stamp = HAL_GetTick();
	uint8_t camera_tran[2] = {0xc8,0x37};
	//HAL_UART_Receive_IT(&huart3,&dat_camera,1);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		/************************************ CONVERT DATA MATRIX TAG RTU *********
		
		dete_code = Input_Register[0]
		Tag ID = Input_Register[2][3][4][5] /256
		Angle  = Input_Register[6] / 40
		Y_Coor = Input_Register[7] /40
		X_Coor = Input_Register[8][9] /2560
		
		
		
		
		
		
		
		
		*/
    /* USER CODE END WHILE */
	eMBPoll();
	//process_message_master_camera();
	while((HAL_GetTick() - time_stamp)>(uint32_t)50)
		{
			HAL_GPIO_WritePin(DR_uart3_GPIO_Port,DR_uart3_Pin,GPIO_PIN_SET);
			HAL_UART_Transmit(&huart3,camera_tran,2,0xff);
			HAL_GPIO_WritePin(DR_uart3_GPIO_Port,DR_uart3_Pin,GPIO_PIN_RESET);
			HAL_UART_Receive_IT(&huart3,messages_camera,21);
			
			if((messages_camera[0] == 0x00) && (check_messenger(messages_camera,20) == messages_camera[20]))
			{
				
				usSRegInBuf[0] = 1;
				usSRegInBuf[1] = messages_camera[1]&0x40;
				
				//ID
				uint64_t Tag_ID = (((uint64_t)messages_camera[8]&0x7F)<<57)|    //TAG_49 _> TAG 55
													(((uint64_t)messages_camera[9]&0x7F)<<50)|		//TAG_42 _> TAG 48
													(((uint64_t)messages_camera[12]&0x7F)<<43)|		//TAG_35 _> TAG 41
													(((uint64_t)messages_camera[13]&0x7F)<<36)|		//TAG_28 _> TAG 34
													(((uint64_t)messages_camera[14]&0x7F)<<29)|		//TAG_21 _> TAG 27
													(((uint64_t)messages_camera[15]&0x7F)<<22)|		//TAG_14 _> TAG 20
													(((uint64_t)messages_camera[16]&0x7F)<<15)|		//TAG_7 _> TAG 13
													(((uint64_t)messages_camera[17]&0x7F)<<8);					//TAG_0 _> TAG 6
				
				
				usSRegInBuf[2] = (Tag_ID&0xffff000000000000)>>48;							// id high
				usSRegInBuf[3] = (Tag_ID&0x0000ffff00000000)>32; 					// id low
				usSRegInBuf[4] = (Tag_ID&0x00000000ffff0000)>>16;							// id high
				usSRegInBuf[5] = (Tag_ID&0x000000000000ffff);
				
				//angle
				usSRegInBuf[6] =	(((uint16_t)messages_camera[10]&0x7f)<<9)|
													(((uint16_t)messages_camera[11]&0x7f)<<2);	// angle
			
				//Y
				uint16_t y_coor = (((int16_t)messages_camera[6]&0x7F)<<9)|
													(((int16_t)messages_camera[7]&0x7F)<<2);		// Y

				usSRegInBuf[7] = y_coor;
				//X
				int32_t x_coor =	((int32_t)((messages_camera[2]&0x7F)<<29)|
									((int32_t)(messages_camera[3]&0x7F)<<22)|
									((int32_t)(messages_camera[4]&0x7f)<<15)|
									((int32_t)messages_camera[5]&0x7f)<<8); //X
		
				
				usSRegInBuf[8] = (x_coor&0xffff0000)>>16; // X high
				usSRegInBuf[9] = (x_coor&0x0000ffff); // X low
			}
			else if(messages_camera[0] == 0x02)
			{
				usSRegInBuf[0] = 0;
			}
			memset(messages_camera,NULL,21);
			time_stamp = HAL_GetTick();
		}
	
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig;
	uint32_t uwPrescalerValue = 0;
	
	
  htim3.Instance = TIM3;
  htim3.Init.Period = 0;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    while(1);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    while(1);
  }
	
//  if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
//  {
//    /* Starting Error */
//    while(1);
//  }
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
  huart2.Init.WordLength = UART_WORDLENGTH_9B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_EVEN;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_9B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_EVEN;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DR_uart2_GPIO_Port, DR_uart2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DR_uart3_GPIO_Port, DR_uart3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : DR_uart2_Pin */
  GPIO_InitStruct.Pin = DR_uart2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DR_uart2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DR_uart3_Pin */
  GPIO_InitStruct.Pin = DR_uart3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DR_uart3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
uint8_t check_messenger(uint8_t *mes,uint8_t len)
{
	uint8_t check_sum =0x00;
	for(int i =0; i<len; i++)
	{
		check_sum ^= mes[i];
	}
	return check_sum;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
