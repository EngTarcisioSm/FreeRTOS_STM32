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
// #include "cmsis_os.h" //removido

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"   //inclusão
#include "task.h"       //inclusão
#include "queue.h"      //inclusão
#include "semphr.h"     //inclusão

#include <stdio.h>      //inclusão
#include <string.h>     //inclusão
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//criação de enumeração
typedef enum
{
  Bit_RESET = 0,
  Bit_SET
}BitAction;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

//osThreadId defaultTaskHandle;   //removido
/* USER CODE BEGIN PV */
SemaphoreHandle_t xMutex;

char c_uartSend[100];   //buffer de escrita serial

//funções de escrita serial
void vPrintString(char *pc_uartSend_f);
void vUsartLib_Putc(UART_HandleTypeDef *huart, char c_data);
void vUsartLib_Puts(char *c_data);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void const * argument); //REMOVIDO

/* USER CODE BEGIN PFP */
//PROTOTIPO DA PRIMEIRA TAREFA
void vTaskOne(void *pvParameters);
//PROTOTIPO DA SEGUNDA TAREFA
void vTaskTwo(void *pvParameters);


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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  xMutex = xSemaphoreCreateMutex();
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
  /* definition and creation of defaultTask */
  // osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);    //removido
  // defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);         //removido

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  //criação de tarefa 1
  if ((xTaskCreate(vTaskOne, "TASK_ONE_NAME", configMINIMAL_STACK_SIZE, NULL, 1, NULL)) != pdTRUE )
  {
    vPrintString("Não foi possivel alocar tarefa vTaskOne no escalonador");
  }

  //criação de tarefa 2
  if ((xTaskCreate(vTaskTwo, "TASK_TWO_NAME", configMINIMAL_STACK_SIZE, NULL, 1, NULL)) != pdTRUE )
  {
    vPrintString("Não foi possivel alocar tarefa vTaskTwo no escalonador");
  }
  
  
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  // osKernelStart();    //Remover
  vTaskStartScheduler();
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DOUT_LED1_Pin|SEL_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SEL_0_GPIO_Port, SEL_0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(EN_RX_485_GPIO_Port, EN_RX_485_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : DOUT_LED1_Pin */
  GPIO_InitStruct.Pin = DOUT_LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DOUT_LED1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SEL_0_Pin */
  GPIO_InitStruct.Pin = SEL_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SEL_0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DIN_CFIG_Pin */
  GPIO_InitStruct.Pin = DIN_CFIG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIN_CFIG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : EN_RX_485_Pin */
  GPIO_InitStruct.Pin = EN_RX_485_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EN_RX_485_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SEL_1_Pin */
  GPIO_InitStruct.Pin = SEL_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SEL_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DIN_TARA_Pin */
  GPIO_InitStruct.Pin = DIN_TARA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIN_TARA_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//funções de impressão
void vPrintString(char *pc_uartSend_f) {
  //acionamentos necessários para chavear e acionar o rs485 em modo de transmissão
  HAL_GPIO_WritePin(SEL_0_GPIO_Port, SEL_0_Pin, Bit_RESET);
  HAL_GPIO_WritePin(SEL_1_GPIO_Port, SEL_1_Pin, Bit_RESET);
  HAL_GPIO_WritePin(EN_RX_485_GPIO_Port, EN_RX_485_Pin, Bit_RESET);
  taskENTER_CRITICAL();
  {
    vUsartLib_Puts(pc_uartSend_f);
  }
  //acionamentos necessários para chavear e acionar o rs485 em modo de recepção
  HAL_GPIO_WritePin(SEL_0_GPIO_Port, SEL_0_Pin, Bit_SET);
  HAL_GPIO_WritePin(SEL_1_GPIO_Port, SEL_1_Pin, Bit_SET);
  HAL_GPIO_WritePin(EN_RX_485_GPIO_Port, EN_RX_485_Pin, Bit_SET);
  taskEXIT_CRITICAL();
}
void vUsartLib_Putc(UART_HandleTypeDef *huart, char c_data) {
  //envia um unico caractere
  HAL_UART_Transmit(huart, &c_data, 1, 10);
}
void vUsartLib_Puts(char *c_data) {
  //roda todo o buffer até encontrar 0x00
  while(*c_data){
    vUsartLib_Putc(&huart1, *c_data++);
  }
}

//funções de tarefas
void vTaskOne(void *pvParameters) {
  for (;;) {
    vPrintString("Testando task_1...\n");
    vTaskDelay( 1000 / portTICK_PERIOD_MS);
  }
  //o comando abaixo não deveria ser executado...Caso algo estranho ocorra, esta task é deletada
  //primeiro parametro NULLinforma que é para desalocar da memória esta task
  vTaskDelete( NULL );
}
void vTaskTwo(void *pvParameters) { 
  while(1){
    vPrintString("Testando task_2...\n");
    vTaskDelay( 1000 / portTICK_PERIOD_MS);
  }
  vTaskDelete( NULL );
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
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
