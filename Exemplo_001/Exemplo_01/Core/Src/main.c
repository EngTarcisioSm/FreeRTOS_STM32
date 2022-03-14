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
// #include "cmsis_os.h"   //removido

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"   //inclusão
#include "task.h"       //inclusão
#include "queue.h"      //inclusão
#include "semphr.h"     //inclusão

#include <stdio.h>      //inclusão
#include <string.h>     //inclusão
#include <stdint.h>     //incluso
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
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

//osThreadId defaultTaskHandle;		//removido
/* USER CODE BEGIN PV */
QueueHandle_t xQueue;
QueueHandle_t xQueue_count;
SemaphoreHandle_t xMutex;

struct AMessage {
	char c_messageID;
	char c_data[20];
};

typedef struct t {
	uint32_t u32_value;
	char c_task_id;
	char *task_name;
} xData_t;
//funções de escrita serial
void vPrintString(char *pc_uartSend_f);
void vUsartLib_Putc(UART_HandleTypeDef *huart, char c_data);
void vUsartLib_Puts(char *c_data);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void const *argument);

/* USER CODE BEGIN PFP */
void vTask_print_q(void *pvParameters);		//efetua envio de dados para fila
void vTask_print(void *pvParameters);		//efetua o recebimento da fila
void vTask_blink(void *pvParameters);		//efetua um toggle

void vTask_print_count(void *pvParameters);
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	//criação da queue
	//Criado uma fila com 5 espaços e cada espaço possui um tamanho de uint32_t. Função que Cria uma fila
	if ((xQueue = xQueueCreate(5, sizeof(struct AMessage*))) == NULL) {
		vPrintString("Não foi possivel alocar a xQueue\n");
	} else {
		vPrintString("Fila criada com sucesso!\n");
	}

	//Nova fila criada para a struct do tipo xData_t
	if ((xQueue_count = xQueueCreate(5, sizeof(xData_t))) == NULL) {
		vPrintString("Não foi possivel alocar a xQueue\n");
	} else {
		vPrintString("Fila criada com sucesso!\n");
	}
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of defaultTask */
	// osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);   //removido
	// defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);        //removido
	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	//criação da tarefa 1
	if ((xTaskCreate(vTask_print_q, "Task Print Q",
	configMINIMAL_STACK_SIZE * 3, NULL, 2, NULL)) != pdTRUE) {
		vPrintString(
				"Não foi possivel alocar tarefa Task Print Q no escalonador\n");
	} else {
		vPrintString("Tarefa Task Print Q criada com sucesso!\n");
	}

	if ((xTaskCreate(vTask_blink, "Task Blink", configMINIMAL_STACK_SIZE, NULL,
			1, NULL)) != pdTRUE) {
		vPrintString(
				"não foi possivel alocar tarefa Blink vTaskBlink no escalonador\n");
	} else {
		vPrintString("Tarefa Task Blink criada com sucesso!\n");
	}

	if ((xTaskCreate(vTask_print, "Task Print", configMINIMAL_STACK_SIZE * 3,
	NULL, 1, NULL)) != pdTRUE) {
		vPrintString(
				"Não foi possivel alocar tarefa Task Print no escalonador\n");
	} else {
		vPrintString("Tarefa Task Print criada com sucesso!\n");
	}

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

	if ((xTaskCreate(vTask_print_count, "vTask_print_count",
			configMINIMAL_STACK_SIZE * 3, NULL, 1, NULL)) != pdTRUE) {
		vPrintString("Não foi possivel alocar tarefa Task_2 no escalonador\n");
	} else {
		vPrintString("Tarefa Task_2 criada com sucesso!\n");
	}

	/* USER CODE END RTOS_THREADS */

	/* Start scheduler */
	// osKernelStart();    //removido
	vTaskStartScheduler();

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
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {
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
	if (HAL_UART_Init(&huart1) != HAL_OK) {
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
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, DOUT_LED1_Pin | SEL_1_Pin, GPIO_PIN_RESET);

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
//-----------------------------------------------------------------------------
//funções de impressão
void vPrintString(char *pc_uartSend_f) {
	//acionamentos necessários para chavear e acionar o rs485 em modo de transmissão
	HAL_GPIO_WritePin(SEL_0_GPIO_Port, SEL_0_Pin, Bit_RESET);
	HAL_GPIO_WritePin(SEL_1_GPIO_Port, SEL_1_Pin, Bit_RESET);
	HAL_GPIO_WritePin(EN_RX_485_GPIO_Port, EN_RX_485_Pin, Bit_RESET);
//  taskENTER_CRITICAL();	//removido
	xSemaphoreTake(xMutex, portMAX_DELAY);
	{
		vUsartLib_Puts(pc_uartSend_f);
	}
	//acionamentos necessários para chavear e acionar o rs485 em modo de recepção
	HAL_GPIO_WritePin(SEL_0_GPIO_Port, SEL_0_Pin, Bit_SET);
	HAL_GPIO_WritePin(SEL_1_GPIO_Port, SEL_1_Pin, Bit_SET);
	HAL_GPIO_WritePin(EN_RX_485_GPIO_Port, EN_RX_485_Pin, Bit_SET);
	xSemaphoreGive(xMutex);
//  taskEXIT_CRITICAL();		//removido
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
//-----------------------------------------------------------------------------

//funções de tarefas
void vTask_print_q(void *pvParameters) {
	uint32_t u32_status = 0;

	struct AMessage xMessage =
			{ .c_messageID = 2, .c_data = "Curso RTOS STM32" };
	struct AMessage *px_message = &xMessage;

	vPrintString("Entrei na task Queue\n");

	for (;;) {

		if (!HAL_GPIO_ReadPin(DIN_TARA_GPIO_Port, DIN_TARA_Pin)
				&& !u32_status) {
			//Aguarda um periodo para evitar o efeito bounce bottão pressionado
			vTaskDelay(50 / portTICK_PERIOD_MS);

			if (!HAL_GPIO_ReadPin(DIN_TARA_GPIO_Port, DIN_TARA_Pin)
					&& !u32_status) {
				//           xQueueSend(1:nome fila   2:endereço da variavel  3: timeout)
				if ( xQueueSend(xQueue, (void* )&px_message,
						( 50 / portTICK_PERIOD_MS)) == pdPASS) {

					vPrintString(
							"Valor da constante button enviado na queue! \n\n");
				}
			}

			u32_status = 1;

		}

		//checa se o botão foi levantado
		if (HAL_GPIO_ReadPin(DIN_TARA_GPIO_Port, DIN_TARA_Pin) && u32_status) {

			vTaskDelay(50 / portTICK_PERIOD_MS);	//delay para debounce

			if (HAL_GPIO_ReadPin(DIN_TARA_GPIO_Port, DIN_TARA_Pin)
					&& u32_status) {

				vPrintString("Botão foi solto! \n\n");
				u32_status = 0;
			}
		}

		vTaskDelay(50 / portTICK_PERIOD_MS);
	}

	vTaskDelete( NULL); //caso ocorra algo que faça o for sair
}

void vTask_print(void *pvParameters) {
	uint32_t u32_status;
	// char *ps_receve_string;
	struct AMessage *msg;

	vPrintString("Entrei na task Print\n");

	for (;;) {
		//xQueueReceive(1:nome da fila 2:endereço variavel de recebimento, 3:timeout)
		u32_status = xQueueReceive(xQueue, &msg, portMAX_DELAY);
		if (u32_status == pdPASS) {

			//Para entender a necessidade de alocação dinamica de memoria
			//deixo o link abaixo, onde explica porque precisamos fazer um malloc
			//para alocar um buffer ao nosso ponteiro para uso na função sprintf
			//lembrando que iremos usar o pvPortMalloc que é safe thread para o FreeRTOS
			//https://stackoverflow.com/questions/19772667/c-sprintf-array-char-pointers
			char *pc_uartSend = pvPortMalloc(sizeof(msg) + 1);

			sprintf(pc_uartSend,
					"Estrutura recebida : ID = %d , MSG = \" %s \" \n",
					msg->c_messageID, msg->c_data);
			vPrintString(pc_uartSend);
			vPortFree(pc_uartSend);

			msg->c_messageID++;
		}
		vPrintString("Task print \r\n\n");
		//vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	vTaskDelete( NULL);
}

void vTask_blink(void *pvParameters) {
	for (;;) {
		//Altera o estado do led
		HAL_GPIO_TogglePin(DOUT_LED1_GPIO_Port, DOUT_LED1_Pin);
		vTaskDelay(250 / portTICK_PERIOD_MS);
	}
	vTaskDelete( NULL);
}

void vTask_print_count(void *pvParameters) {
	xData_t x_count;

	vPrintString("vTask_Print_Count iniciada!");

	for (;;) {

		/*
		 * Realiza a leitura da fila. Caso tenha algum valor a ser lido
		 * este será armazenado em count;
		 */
		xQueueReceive(xQueue_count, &x_count, portMAX_DELAY);

		char *pc_uartSend = pvPortMalloc(sizeof(x_count) + 1);

		sprintf(pc_uartSend,
				"Estrutura recebida : task_id = %d, task_name = \" %s \", value = %ld \n\n",
				x_count.c_task_id, x_count.task_name, x_count.u32_value);

		vPrintString(pc_uartSend);

		vPortFree(pc_uartSend);

		vTaskDelay( 10 / portTICK_PERIOD_MS );
	}
}

void vTask1(void *pvParameters) {
	xData_t x_count_1 = {
			.u32_value = 0,
			.c_task_id = 1,
			.task_name = "task_1"
	};

	char c_value_char[30];

	vPrintString("vTask_1 iniciada!");

	for(;;) {
		/*
		 * Envia uma cópia do valor de count_1 na fila.
		 * Caso a fila esteja cheia, esta task será bloqueada
		 *
		 * Não é checada se foi enviado pois caso não consiga existe o portMAX_DELAY, a tarefa
		 * se manterá bloqueada até que seja enviada
		 */
		xQueueSend( xQueue_count, &x_count_1, portMAX_DELAY);

		sprintf(c_value_char, "vTask_1 envia: %ld \r\n", x_count_1.u32_value);

		x_count_1.u32_value++;

		vTaskDelay( 1000 / portTICK_PERIOD_MS );
	}
}

void vTask2(void *pvParameters) {
	xData_t x_count_2 = {
			.u32_value = 0,
			.c_task_id = 1,
			.task_name = "task_2"
	};

	char c_value_char[30];

	vPrintString("vTask_2 iniciada!");

	for(;;) {
		/*
		 * Envia uma cópia do valor de count_1 na fila.
		 * Caso a fila esteja cheia, esta task será bloqueada
		 * Não é checada se foi enviado pois caso não consiga existe o portMAX_DELAY, a tarefa
		 * se manterá bloqueada até que seja enviada
		 */
		xQueueSend( xQueue_count, &x_count_2, portMAX_DELAY);

		sprintf(c_value_char, "vTask_2 envia: %ld \r\n", x_count_2.u32_value);

		x_count_2.u32_value++;

		vTaskDelay( 1000 / portTICK_PERIOD_MS );
	}
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM4 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
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
void Error_Handler(void) {
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
