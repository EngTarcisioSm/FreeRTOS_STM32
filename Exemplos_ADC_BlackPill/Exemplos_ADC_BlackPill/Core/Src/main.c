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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
	Bit_RESET = 0, Bit_SET
} BitAction;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ULONG_MAX 0xFFFFFFFFUL

#define TNBIT_1 0x01
#define TNBIT_2 0x02
#define TNBIT_3 0x03
#define TNBIT_4 0x04
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart1;

osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */
SemaphoreHandle_t xMutex = NULL;

TaskHandle_t xTaskHandle_ISR;
TaskHandle_t xTaskHandle_Evt_Ext;
TaskHandle_t xTaskHandle_3;
TaskHandle_t xTaskHandle_Recv_ADC;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void const *argument);

/* USER CODE BEGIN PFP */
void vTask_blink(void *pvParameter);
void vTask_Handle(void *pvParameter);
void vTask_1(void *pvParameter);
void vTask_2(void *pvParameter);
void vTask_3(void *pvParameter);
void vTask_4(void *pvParameter);
void vTask_SEND_ADC(void *pvParameter);
void vTask_Recv_ADC(void *pvParameter);

void vPrintString(char *UartSend_f);
void USARTlib_Putc(UART_HandleTypeDef *huart, char c);
void USARTlib_Puts(char *str);
uint32_t Ain_Read_value();
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
	MX_ADC1_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	xMutex = xSemaphoreCreateMutex();

	if (xMutex != NULL)
		vPrintString("xMutex criada com sucesso!\r\n");
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
	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */

	if ((xTaskCreate(vTask_blink, "blink task", configMINIMAL_STACK_SIZE * 4,
	NULL, 1, NULL)) != pdTRUE) {
		vPrintString(
				"nao foi possivel alocar tarefa vTaskBlink no escalonador");
	}

	if ((xTaskCreate(vTask_Handle, "handle task", configMINIMAL_STACK_SIZE,
	NULL, 2, &xTaskHandle_ISR)) != pdTRUE) {
		vPrintString(
				"nao foi possivel alocar tarefa vTask_Handle no escalonador");
	}

	if ((xTaskCreate(vTask_1, "task 1", configMINIMAL_STACK_SIZE, NULL, 1, NULL))
			!= pdTRUE) {
		vPrintString("nao foi possivel alocar tarefa vTask_1 no escalonador");
	}

	if ((xTaskCreate(vTask_2, "task 2", configMINIMAL_STACK_SIZE, NULL, 1, NULL))
			!= pdTRUE) {
		vPrintString("nao foi possivel alocar tarefa vTask_2 no escalonador");
	}

	if ((xTaskCreate(vTask_3, "task 3", configMINIMAL_STACK_SIZE, NULL, 2,
			&xTaskHandle_3)) != pdTRUE) {
		vPrintString("nao foi possivel alocar tarefa vTask_3 no escalonador");
	}

	if ((xTaskCreate(vTask_4, "task 4", configMINIMAL_STACK_SIZE, NULL, 2,
			&xTaskHandle_Evt_Ext)) != pdTRUE) {
		vPrintString("nao foi possivel alocar tarefa vTask_4 no escalonador");
	}

	if ((xTaskCreate(vTask_SEND_ADC, "taskADC", configMINIMAL_STACK_SIZE, NULL,
			1, NULL)) != pdTRUE) {
		vPrintString("nao foi possivel alocar tarefa vTaskADC no escalonador");
	}

	if ((xTaskCreate(vTask_Recv_ADC, "RECV ADC", configMINIMAL_STACK_SIZE, NULL,
			3, &xTaskHandle_Recv_ADC)) != pdTRUE) {
		vPrintString(
				"nao foi possivel alocar tarefa vTask_Recv_ADC no escalonador");
	}

	vTaskStartScheduler();
	/* USER CODE END RTOS_THREADS */

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
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 100;
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

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void) {

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */
	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

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
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(DOUT_LED1_GPIO_Port, DOUT_LED1_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : DOUT_LED1_Pin */
	GPIO_InitStruct.Pin = DOUT_LED1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DOUT_LED1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : DIN_BUTTON_Pin */
	GPIO_InitStruct.Pin = DIN_BUTTON_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DIN_BUTTON_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void vPrintString(char *UartSend_f) {
	xSemaphoreTake(xMutex, portMAX_DELAY);
	{

		USARTlib_Puts(UartSend_f);

	}
	xSemaphoreGive(xMutex);

}

void USARTlib_Putc(UART_HandleTypeDef *huart, char c) {
	//Envia caracter
	HAL_UART_Transmit(huart, &c, 1, 10);
}

void USARTlib_Puts(char *str) {
	while (*str) {
		USARTlib_Putc(&huart1, *str++);
	}
}

uint32_t Ain_Read_value() {
	uint32_t result;

//	HAL_ADCEx_Calibration_Start(&hadc1);

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1000);
	result = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	return result;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	BaseType_t xHigherPriorityTaskWoken;

	/* O parâmetro xHigherPriorityTaskWoken deve ser inicializado para pdFALSE,
	 * pois será definido como pdTRUE dentro da função API segura contra interrupções,
	 * se for necessária uma alternância de contexto */

	xHigherPriorityTaskWoken = pdFALSE;

	/* Envia uma notificação diretamente para tarefa que esta bloqueada aguardando o Semaforo por TaskNotify */

	vTaskNotifyGiveFromISR( /* O identificador da tarefa para a qual a notificação está sendo enviada.
	 * O identificador foi salvo quando a tarefa foi criada. */
	xTaskHandle_ISR, &xHigherPriorityTaskWoken);

	vTaskNotifyGiveFromISR(xTaskHandle_ISR, &xHigherPriorityTaskWoken);
	vTaskNotifyGiveFromISR(xTaskHandle_ISR, &xHigherPriorityTaskWoken);

	xTaskNotifyFromISR(xTaskHandle_Evt_Ext, //Handle da task a ser notificada
			TNBIT_1,//BIT a ser verificado
			eSetBits,//Setando bit nivel alto
			NULL);//Parametro NULL pois não estou usando Token para acordar esta task, e sim outra.

	/* Passa o token xHigherPriorityTaskWoken para portYIELD_FROM_ISR().
	 * Se xHigherPriorityTaskWoken foi definido como pdTRUE na chamada da função vTaskNotifyGiveFromISR(),
	 *  portYIELD_FROM_ISR() irá solicitar uma troca de contexto para tarefa pendente de notificação.
	 * Se xHigherPriorityTaskWoken for pdFALSE,
	 * portYIELD_FROM_ISR() não tera nenhum efeito no escalonador.
	 */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}

/*********************************** TASKS **************************************/

void vTask_SEND_ADC(void *pvParameter) {
	vPrintString("vTask_SEND_ADC iniciada!\r\n");

	uint32_t analog_pot = 0;

	BaseType_t xResult = pdFAIL;

	for (;;) {
		analog_pot = Ain_Read_value();

		//essa task emula uma chamada por ISR, a unica diferença é que não usamos o xHigherPriorityTaskWoken
		do {
			//enquanto xResult não retornar pdPASS que é informado caso a outra tarefa ja tenha lido o valor, ele irá permancer neste loop tentando executar o envio
			//é importante observar que o parametro eSetValueWithoutOverwrite significa que:
			//so irá modificar o valor passado no parametro value utilizado na variavel analog_pot
			//caso a tarefa notificada tenha lido este valor.
			xResult = xTaskNotify(xTaskHandle_Recv_ADC, analog_pot,
					eSetValueWithoutOverwrite);
		} while (xResult != pdPASS);

		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void vTask_Recv_ADC(void *pvParameter) {
	vPrintString("vTask_Recv_ADC iniciada!\r\n");

	uint32_t ulADCValue = 0;
	BaseType_t xResult = pdFAIL;
	char string_value[20] = { 0 };

	for (;;) {
		xResult = xTaskNotifyWait(0, 0, &ulADCValue, portMAX_DELAY);
		if (xResult == pdPASS) {
			//aqui podemos processar o valor que chegou do adc, podemos fazer algum filtro digital, media movel ou enviar a um display e etc.
			sprintf(string_value, "Value: %lu\r\n", ulADCValue);
			vPrintString(string_value);
		} else {
			vPrintString("Erro ao receber task notify!");
		}
	}
}
void vTask_1(void *pvParameter) {
	vPrintString("vTask_1 iniciada!\r\n");

	for (;;) {
		xTaskNotify(xTaskHandle_3, TNBIT_2, eSetBits);
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}

	vTaskDelete(NULL);
}

void vTask_2(void *pvParameter) {
	vPrintString("vTask_2 iniciada!\r\n");

	for (;;) {
		xTaskNotify(xTaskHandle_3, TNBIT_3, eSetBits);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

	vTaskDelete(NULL);
}

void vTask_3(void *pvParameter) {
	vPrintString("vTask_3 iniciada!\r\n");

	uint32_t ulNotifiedValue = 0;

	for (;;) {
		xTaskNotifyWait( pdFALSE, //pdFALSE = não limpa os bits na entrada da função / pdTRUE = Limpa os bits na entrada da função
				ULONG_MAX, //Limpar todos os BITS na saída, caso queira limpar separadamente usar (TNBIT_3 | TNBIT_2) como exemplo
				&ulNotifiedValue, //Variavel para armazenar o valor do bit notificado
				portMAX_DELAY);

		if (ulNotifiedValue == TNBIT_2) {
			vPrintString("Recebido evento da tarefa 1 no BIT_2! \r\n");
		} else if (ulNotifiedValue == TNBIT_3) {
			vPrintString("Recebido evento da tarefa 2 no BIT_3! \r\n");
		} else {
			//tratar algum erro
			vPrintString("Outro BIT foi sinalizado!");
		}
	}
	vTaskDelete(NULL);
}

void vTask_4(void *pvParameter) {
	vPrintString("vTask_4 iniciada!\r\n");

	uint32_t ulNotifiedValue = 0;

	for (;;) {

		if (xTaskNotifyWait( pdFALSE, //pdFALSE = não limpa os bits na entrada da função / pdTRUE = Limpa os bits na entrada da função
				ULONG_MAX, 					//Limpar todos os BITS na saída
				&ulNotifiedValue, //Variavel para armazenar o valor do bit notificado
				portMAX_DELAY) == pdPASS) {
			if (ulNotifiedValue == TNBIT_1) {
				vPrintString("Recebido evento no BIT_1! \r\n");
			} else {
				vPrintString("Outro BIT foi sinalizado!");
			}
		}
	}

	vTaskDelete(NULL);
}

void vTask_Handle(void *pvParameter) {
	vPrintString("Task gerenciada pelo semaforo iniciada!");

	const TickType_t xMaxExpectedBlockTime = pdMS_TO_TICKS(5000);

	for (;;) {
		/*Aguarda uma notificação da ISR por 500ms. */
		/** pdTRUE Limpa o buffer contador, trabalhando somente como semaforo binario.
		 *	pdFALSE incrementa a variavel ulEvents com o numero de Give gerados na ISR, assim trabalhando como semaforo contador.
		 *	Cada chamada TaskNotify decrementa a quantidade de ISR geradas pelo Give.
		 */

		/*Quando xClearOnExit é pdFALSE,
		 * chamar ulTaskNotifyTake() só poderá diminuir (reduzir em um) o valor de notificação da tarefa de chamada,
		 * em vez de reduzi-lo até zero. A contagem de notificações é a diferença entre o número de eventos que ocorreram e
		 * o número de eventos que foram processados. Simplificando o decremento da notificação dentro da função ulTaskNotifyTake
		 * não precisamos utilizar uma variavel para fazer a contagem
		 */

		if (ulTaskNotifyTake(pdFALSE, xMaxExpectedBlockTime) != 0) {
			// Para chegar aqui, pelo menos um evento deve ter ocorrido..

			vPrintString("Handler task - Processing event.\r\n");
		} else {
			/* Se essa parte da função for atingida,
			 * nenhuma ISR foi recebida no tempo de 500mS
			 * podendo ser executado algum tratamento de erro ou logica qualquer. */
			vPrintString("Time-out TaskNotify.\r\n");
		}
	}
}

void vTask_blink(void *pvParameter) {
	char buff[400];

	//uint16_t size_heap = 0;

	for (;;) {
		vTaskList(buff);

		vPrintString(
				"\r\nTask-------------State-----Prio------Stack---Num\r\n");
		vPrintString(buff);
		vPrintString("\n\n\n");

		memset(buff, 0, sizeof(buff));
		//size_heap = xPortGetMinimumEverFreeHeapSize();
		sprintf(buff, "Free Heap: %d bytes\n\n",
				xPortGetMinimumEverFreeHeapSize());

		vPrintString(buff);

		vTaskDelay(5000 / portTICK_PERIOD_MS);

		HAL_GPIO_TogglePin(DOUT_LED1_GPIO_Port, DOUT_LED1_Pin);

	}

	vTaskDelete(NULL);
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument) {
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END 5 */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM10 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM10) {
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
