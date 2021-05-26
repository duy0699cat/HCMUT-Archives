/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "DHT.h"
#include <stdio.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct sTask{

void (*pTask)(void); 	// Pointer to the task (must be a 'void (void)' function)

uint32_t Delay, Period/*,Sum_Period*/ ; // Delay (ticks) until the function will (next) be run
						// And Interval (ticks) between subsequent runs

uint8_t  RunMe /*,PFlag*/;			 // Incremented (by scheduler) when task is due to execute
}sTask;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SCH_MAX_TASKS 7
#define ERROR_SCH_TOO_MANY_TASKS 2
#define ERROR_SCH_CANNOT_DELETE_TASK 3
#define RETURN_ERROR 1
#define RETURN_NORMAL 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
sTask SCH_tasks_G[SCH_MAX_TASKS];
uint32_t Return_code;
uint8_t Error_code_G = 0;
int systime;
unsigned char counter[60]={ '\0' };
int fan1, fan2, fan3, heater, pump;

DHT_DataTypedef DHT11_Data;
	float Temperature, Humidity;
	char pData[60]={ '\0' };

void Task1(){
DHT_GetData(&DHT11_Data);
	  	  Temperature = DHT11_Data.Temperature;
		  Humidity = DHT11_Data.Humidity;
}
void Task2(){

sprintf(pData,"\r temp:%.2f, humid:%.2f, fan1:%d, fan2:%d, heater:%d ", Temperature, Humidity,fan1,fan2,heater);
HAL_UART_Transmit(&huart2, (uint8_t*) pData, sizeof(pData), 500);
}

void Task4(){
if(Temperature<31){heater=1;
fan2=1;}
else{heater=0;fan2=0;}

if(Humidity>90){fan1=3;}
else if(Humidity>80&&Humidity<=90){fan1=2;}
else if(Humidity>65&&Humidity<=80){fan1=1;}
else fan1=0;

}
void Task5(){
sprintf(counter,"\r \n **Task 5 : %02lu  ",HAL_GetTick());
HAL_UART_Transmit(&huart2,counter,sizeof(counter),100);
}

//////////////////

uint32_t SCH_Add_Task();
void SCH_Delete_Task();
void SCH_Dispatch_Tasks();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// The array of tasks

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
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */
  sprintf(pData,"\r \n hi  ");
    HAL_UART_Transmit(&huart2, (uint8_t*) pData, sizeof(pData), 500);
      /* USER CODE END WHILE */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    SCH_Add_Task(*Task1, 00, 1000);
    SCH_Add_Task(*Task2, 100, 200);
    SCH_Add_Task(*Task4, 50, 200);
  while (1)
  {
    /* USER CODE END WHILE */

	//	  HAL_Delay(1000);

		  SCH_Dispatch_Tasks();
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 1 ;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 41999 ;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, sen1_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : sen1_Pin LD2_Pin */
  GPIO_InitStruct.Pin = sen1_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

uint32_t SCH_Add_Task(void(*pFunction)(), const uint32_t DELAY, const uint32_t PERIOD)
{
uint32_t Index = 0;
// First find a gap in the array (if there is one)
while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) {
Index++;
}
if (Index == SCH_MAX_TASKS) {// Have we reached the end of the list?
// Task list is full, Set the global error variable
Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
return SCH_MAX_TASKS; // Also return an error code
}
// If we're here, there is a space in the task array
SCH_tasks_G[Index].pTask = pFunction;
SCH_tasks_G[Index].Delay = DELAY;
SCH_tasks_G[Index].Period = PERIOD;
SCH_tasks_G[Index].RunMe = 0;
return Index; // return position of task (to allow later deletion)
}

/////////////////////////////////////////////
void SCH_Update(void) {
uint32_t Index;
// NOTE: calculations are in *TICKS* (not milliseconds)
for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
	// Check if there is a task at this location
	if (SCH_tasks_G[Index].pTask) {
		if (SCH_tasks_G[Index].Delay == 0) {
			// The task is due to run
			SCH_tasks_G[Index].RunMe += 1; // Inc. the 'RunMe' flag
	//		SCH_Update_Period(Index);

			if (SCH_tasks_G[Index].Period) {
				// Schedule periodic tasks to run again
				SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
				}
		}
		else {
			// Not yet ready to run: just decrement the delay
			SCH_tasks_G[Index].Delay -= 1;

		}
	}
}
}

/*------------------------------------------------------------------*/
void SCH_Dispatch_Tasks(void)
   {
	uint32_t Index;

   // Dispatches (runs) the next task (if one is ready)
   for (Index = 0; Index < SCH_MAX_TASKS; Index++)
      {
      if (SCH_tasks_G[Index].RunMe > 0)
         {
            (*SCH_tasks_G[Index].pTask)();  // Run the task

         //   SCH_tasks_G[Index].Sum_Period += SCH_tasks_G[Index].Period;

            SCH_tasks_G[Index].RunMe -= 1;   // Reset / reduce RunMe flag
           // tasks will automatically run again
            // - if this is a 'one shot' task, remove it from the array
             if (SCH_tasks_G[Index].Period <= 0/*||SCH_tasks_G[Index].Sum_Period>=MAX_TIME*/)
                              {
                                  SCH_Delete_Task(Index);
                              }
         }

      }
  // SCH_Report_Status(); // Report system status
  // SCH_Go_To_Sleep();
   }

   ////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////

/*------------------------------------------------------------------*/
void SCH_Delete_Task(const uint32_t Index)
   {

	if (!SCH_tasks_G[Index].pTask)
	{
	Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK; 	// Set the global error variable
	Return_code = 1;
	}
	else
	{ Return_code = 0;  }
	SCH_tasks_G[Index].pTask = 0;
	SCH_tasks_G[Index].Delay = 0;
	SCH_tasks_G[Index].Period = 0;
	SCH_tasks_G[Index].RunMe = 0;

	}
/////////////////////////////
/*
 * void SCH_Update_Period(const uint32_t Index) {

		if (SCH_tasks_G[Index].PFlag==1) {
			// low delay, ++ period:
			SCH_tasks_G[Index].Period = SCH_tasks_G[Index].Period+SCH_tasks_G[Index].Delay;
		}
		if (SCH_tasks_G[Index].PFlag==2) {
					// high delay, -- period:
					SCH_tasks_G[Index].Period = SCH_tasks_G[Index].Period-SCH_tasks_G[Index].Delay;
				}

		}

*/


//////////////////////////////////////////////////////////////////////////////

void SCH_Report_Status(void) {
#ifdef SCH_REPORT_ERRORS // ONLY APPLIES IF WE ARE REPORTING ERRORS
// Check for a new error code
if (Error_code_G != Last_error_code_G) {
// Negative logic on LEDs assumed
Error_port = 255 - Error_code_G;
Last_error_code_G = Error_code_G;
if (Error_code_G != 0) {
Error_tick_count_G = 60000;
} else {
Error_tick_count_G = 0;
}
} else {
if (Error_tick_count_G != 0) {
if (--Error_tick_count_G == 0) {
Error_code_G = 0; // Reset error code
}
}
}
#endif
}
//INTERRUPT
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim == (&htim3)) {
 //systime++;
	SCH_Update();
	}
//if(systime==100) {Task5();
//systime=0;}

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
