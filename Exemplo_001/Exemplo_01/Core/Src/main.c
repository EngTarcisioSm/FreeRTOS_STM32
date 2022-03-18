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
//#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"   	//inclusão
#include "task.h"       	//inclusão
#include "queue.h"      	//inclusão
#include "semphr.h"     	//inclusão
#include "event_groups.h" 	//incluido

#include <stdio.h>      	//inclusão
#include <string.h>     	//inclusão
#include <stdint.h>     	//incluso
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//criação de enumeração
typedef enum {
	Bit_RESET = 0, Bit_SET
} BitAction;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
#define BIT_3 (1 << 3)
#define BIT_4 (1 << 4)
#define BIT_5 (1 << 5)
#define BIT_6 (1 << 6)
#define BIT_7 (1 << 7)
#define BIT_8 (1 << 8)
#define BIT_9 (1 << 9)
#define BIT_10 (1 << 10)
#define BIT_11 (1 << 11)
#define BIT_12 (1 << 12)
#define BIT_13 (1 << 13)
#define BIT_14 (1 << 14)
#define BIT_15 (1 << 15)
#define BIT_16 (1 << 16)
#define BIT_17 (1 << 17)
#define BIT_18 (1 << 18)
#define BIT_19 (1 << 19)
#define BIT_20 (1 << 20)
#define BIT_21 (1 << 21)
#define BIT_22 (1 << 22)
#define BIT_23 (1 << 23)

#define TASK_0_BIT (1 << 0)
#define TASK_1_BIT (1 << 1)
#define TASK_2_BIT (1 << 2)
#define TASK_3_BIT (1 << 3)

#define ALL_SYNC_BITS ( TASK_0_BIT | TASK_1_BIT | TASK_2_BIT | TASK_3_BIT)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

//osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */
static EventGroupHandle_t xHandle_Event_Group;

//criação de uma variavel que aloca conjunto de queues

SemaphoreHandle_t xMutex;

//funções de escrita serial
void vPrintString(char *pc_uartSend_f);
void vUsartLib_Putc(UART_HandleTypeDef *huart, char c_data);
void vUsartLib_Puts(char *c_data);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */
void vTask_check_event(void *pvParameters);
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);
void vTask_blink(void *pvParameters);		//efetua um toggle

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
	xHandle_Event_Group = xEventGroupCreate();

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
//  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
//  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	//criação da tarefa 1
	if ((xTaskCreate(vTask1, "task_1", configMINIMAL_STACK_SIZE, NULL, 1, NULL))
			!= pdTRUE) {
		vPrintString("Não foi possivel alocar tarefa Task_1 no escalonador\n");
	} else {
		vPrintString("Tarefa Task_1 criada com sucesso!\n");
	}

	if ((xTaskCreate(vTask2, "task_2", configMINIMAL_STACK_SIZE, NULL, 1, NULL))
			!= pdTRUE) {
		vPrintString("Não foi possivel alocar tarefa Task_2 no escalonador\n");
	} else {
		vPrintString("Tarefa Task_2 criada com sucesso!\n");
	}

	if ((xTaskCreate(vTask3, "task_3", configMINIMAL_STACK_SIZE, NULL, 1, NULL))
			!= pdTRUE) {
		vPrintString("Não foi possivel alocar tarefa Task_3 no escalonador\n");
	} else {
		vPrintString("Tarefa Task_3 criada com sucesso!\n");
	}

	if ((xTaskCreate(vTask_check_event, "check_event", configMINIMAL_STACK_SIZE, NULL, 2, NULL)) != pdTRUE) {
		vPrintString("Não foi possivel alocar tarefa vTask_Print_count no escalonador.");
	} else {
		vPrintString("Tarefa check_event criada com sucesso!\n");
	}

	if ((xTaskCreate(vTask_blink, "Task Blink", configMINIMAL_STACK_SIZE * 2, NULL,
			1, NULL)) != pdTRUE) {
		vPrintString(
				"não foi possivel alocar tarefa Blink vTaskBlink no escalonador\n");
	} else {
		vPrintString("Tarefa Task Blink criada com sucesso!\n");
	}


	vTaskStartScheduler();
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
//  osKernelStart();

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
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
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
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

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
//-----------------------------------------------------------------------------
//funções de impressão
void vPrintString(char *pc_uartSend_f) {
	//acionamentos necessários para chavear e acionar o rs485 em modo de transmissão
	HAL_GPIO_WritePin(SEL_0_GPIO_Port, SEL_0_Pin, Bit_RESET);
	HAL_GPIO_WritePin(SEL_1_GPIO_Port, SEL_1_Pin, Bit_RESET);
	HAL_GPIO_WritePin(EN_RX_485_GPIO_Port, EN_RX_485_Pin, Bit_RESET);
	taskENTER_CRITICAL();	//removido
	//xSemaphoreTake(xMutex, portMAX_DELAY);
	{
		vUsartLib_Puts(pc_uartSend_f);
	}
	taskEXIT_CRITICAL();		//removido
	//acionamentos necessários para chavear e acionar o rs485 em modo de recepção
	//xSemaphoreGive(xMutex);
	HAL_GPIO_WritePin(SEL_0_GPIO_Port, SEL_0_Pin, Bit_SET);
	HAL_GPIO_WritePin(SEL_1_GPIO_Port, SEL_1_Pin, Bit_SET);
	HAL_GPIO_WritePin(EN_RX_485_GPIO_Port, EN_RX_485_Pin, Bit_SET);
}
void vUsartLib_Putc(UART_HandleTypeDef *huart, char c_data) {
	//envia um unico caractere
	HAL_UART_Transmit(huart, &c_data, 1, 10);
}
void vUsartLib_Puts(char *c_data) {
	//roda todo o buffer até encontrar 0x00
	while (*c_data) {
		vUsartLib_Putc(&huart1, *c_data++);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

}
//-----------------------------------------------------------------------------

//funções de tarefas
void vTask_check_event(void *pvParameters) {

	vPrintString("vTask_check_event iniciada! \r\n");

	EventBits_t uxReturn;
	TickType_t xTicksToWait = 10000 / portTICK_PERIOD_MS;

	for(;;){

		/* É definido o bit 0 no grupo de eventos na chamada xEventGroupSync para observar que esta tarefa atingiu o
			ponto de sincronização. As outras duas tarefas definirão os outros dois bits definidos
			por ALL_SYNC_BITS. Todas as três tarefas atingiram a sincronização no momento em que é
			apontado todos os ALL_SYNC_BITS como nivel alto. Aguarde no máximo 500ms para isso acontecer. */

		uxReturn = xEventGroupSync(
				xHandle_Event_Group,
				TASK_0_BIT,
				ALL_SYNC_BITS,
				xTicksToWait);

		if ((uxReturn & ALL_SYNC_BITS) == ALL_SYNC_BITS){
			/* Todas as três tarefas atingiram o ponto de sincronização antes da chamada
				para xEventGroupSync () atingir o tempo limite. */
			vPrintString("Todos os bits foram sincronizados! \r\n");
		} else {
			vPrintString("Sincronização não executada! \r\n");
		}
	}
	vTaskDelete(NULL);
}

void vTask1(void *pvParameters) {

	vPrintString("vTask_1 iniciada! \r\n");


	for(;;){

		/* É definido o bit 1 no grupo de eventos na chamada xEventGroupSync para observar que esta tarefa atingiu o
			ponto de sincronização. As outras duas tarefas definirão os outros dois bits definidos
			por ALL_SYNC_BITS. Todas as três tarefas atingiram a sincronização no momento em que é
			apontado todos os ALL_SYNC_BITS como nivel alto.*/
		xEventGroupSync(xHandle_Event_Group, TASK_1_BIT, ALL_SYNC_BITS, portMAX_DELAY);

		vPrintString("BIT_1 Setado e sincronizado !\r\n");

		vTaskDelay( 5000 / portTICK_PERIOD_MS );
	}
	vTaskDelete(NULL);
}

void vTask2(void *pvParameters) {

	vPrintString("vTask_2 inicada!\r\n");

	for(;;){

		/* É definido o bit 1 no grupo de eventos na chamada xEventGroupSync para observar que esta tarefa atingiu o
			ponto de sincronização. As outras duas tarefas definirão os outros dois bits definidos
			por ALL_SYNC_BITS. Todas as três tarefas atingiram a sincronização no momento em que é
			apontado todos os ALL_SYNC_BITS como nivel alto.*/
		xEventGroupSync(xHandle_Event_Group, TASK_2_BIT, ALL_SYNC_BITS, portMAX_DELAY);

		vPrintString("BIT_2 Setado e sincronizado !\r\n");

		vTaskDelay( 5000 / portTICK_PERIOD_MS );
	}
	vTaskDelete(NULL);
}

void vTask3(void *pvParameters) {

	vPrintString("vTask_3 inicada!\r\n");

	for(;;){
		/* É definido o bit 1 no grupo de eventos na chamada xEventGroupSync para observar que esta tarefa atingiu o
			ponto de sincronização. As outras duas tarefas definirão os outros dois bits definidos
			por ALL_SYNC_BITS. Todas as três tarefas atingiram a sincronização no momento em que é
			apontado todos os ALL_SYNC_BITS como nivel alto.*/
		xEventGroupSync(xHandle_Event_Group, TASK_3_BIT, ALL_SYNC_BITS, portMAX_DELAY);

		vPrintString("BIT_3 Setado e sincronizado !\r\n");

		vTaskDelay( 5000 / portTICK_PERIOD_MS );
	}
	vTaskDelete(NULL);
}


void vTask_blink(void *pvParameters) {

	char  c_buff[200];

	vPrintString("Entrando da Task de debug");

	for (;;) {
		//Altera o estado do led
		HAL_GPIO_TogglePin(DOUT_LED1_GPIO_Port, DOUT_LED1_Pin);

		vTaskList(c_buff);

		vPrintString("\n\r\nTask-------------State-----Prio------Stack---Num\r\n");
		vPrintString(c_buff);
		vPrintString("\n");

		memset(c_buff, 0, sizeof(c_buff));

		sprintf(c_buff, "Free Heap: %d bytes \n\n", xPortGetMinimumEverFreeHeapSize());
		vPrintString(c_buff);

		memset(c_buff, 0, sizeof(c_buff));

		vPrintString("\n\n\n");
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
	vTaskDelete( NULL);
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

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
	while (1) {
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
