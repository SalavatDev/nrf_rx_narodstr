/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "NRF24.h"
#include "LCD_HD44780.h"
#include "stepper_motor.h"
#include 	<math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

 
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  
 
	if(GPIO_Pin	== IRQ_PIN)
	{
		
		uint8_t status = NRF24_ReadReg(STATUS);
		DelayMicro(10);
		status = NRF24_ReadReg(STATUS);
		if(status & 0x40)
		{
			
			NRF24_WriteReg(STATUS, 0x40);
			NRF24_Read_Buf(RD_RX_PLOAD,RX_BUF,TX_PLOAD_WIDTH);
			dt = *(uint16_t*)RX_BUF;
		 
			display_send_num((int)(dt & 0x0200), 1, 0, 1);
			display_send_num((int)(dt & 0x1000), 1, 3, 1);
			display_send_num((int)(dt & 0x2000), 1, 7, 1);
	 	 
			dt = dt & 0x01ff;
			dt = ((float)((float)dt / 512) * 360); 		 
			if(pulse_angle.synchr){				
				pulse_angle.synchr = SYNCHR_OFF;
				pulse_angle.angle = (float)dt;
			}
			
			display_send_num((int)dt, 4, 12, 1);
			
		}	
		   
	}else if(GPIO_Pin	== GPIO_PIN_10){
		
		is_rpm_editing = (!is_rpm_editing);
		
		if(is_rpm_editing){
			
			HAL_NVIC_DisableIRQ(EXTI3_IRQn);
			HAL_TIM_Base_Stop_IT(&htim1);

			HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
			
			display_clear_text();
			goto_xy(0, 0);
			display_send_char("RPM");			 			 
			display_send_num((int)rpm_val[cnt_encoder], 3, 0, 1);
			
		}else{
			
		  HAL_TIM_Encoder_Stop_IT(&htim3, TIM_CHANNEL_ALL);
			HAL_NVIC_EnableIRQ(EXTI3_IRQn);
			HAL_TIM_Base_Start_IT(&htim1);

			display_clear_text();
			main_menu(); 
			
		}
 
	}
	 
	
}
 

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start(&htim2);
	
	if(RIGHT_DIR)
		HAL_GPIO_WritePin(DIR_ROTATION_GPIO_Port, DIR_ROTATION_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DIR_ROTATION_GPIO_Port, DIR_ROTATION_Pin, GPIO_PIN_RESET);
	
 
	
	LCD_HD44780_init();
  
	main_menu(); 
	NRF24_ini();

	motor_soft_start();
	
	HAL_Delay(15000);
 
	motor_soft_stop();
 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  
	
  while (1)
  {
		/*
		if(is_rpm_editing){
			
		}*/
	 /*
	 	GPIOB->BSRR = GPIO_PIN_0;
		DelayMicro(156);
		GPIOB->BRR = GPIO_PIN_0;
		DelayMicro(156); 
 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
