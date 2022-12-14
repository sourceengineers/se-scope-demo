/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    UART/UART_HyperTerminal_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use UART HAL and LL APIs to transmit
  *          and receive a data buffer with a communication process based on IT;
  *          The communication is done with the Hyperterminal PC application;
  *          HAL driver is used to perform UART configuration, 
  *          then TX/RX transfers procedures are based on LL APIs use
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics. 
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
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "UartDriver.h"
#include "RtosMutex.h"
#include "RtosApplication.h"

#include "Scope/Builders/ScopeFramedStack.h"
#include "se-lib-c/stream/BufferedByteStream.h"
#include "se-lib-c/stream/ThreadSafeByteStream.h"

#include <stdlib.h>
#include <assert.h>

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

/* USER CODE BEGIN PV */

/* UART handler declaration */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// global timestamp
uint32_t timestamp = 0;

// scope variables
const float frequency = 10.0f;
float sinus = 0.0f;
float cosinus = 0.0f;
float leistung = 0.0f;
float sum = 0.0f;
int flipflop = -1;
uint8_t toggle = 0;

// global instances TODO passs to thread/task
ScopeFramedStackHandle scopeStack;
IByteStreamHandle logger;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */
		/* STM32G0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Low Level Initialization
     */
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
    MX_ADC1_Init();
    /* USER CODE BEGIN 2 */

		// scope configuration
    ScopeFramedStackConfig config = {
            .addressesInAddressAnnouncer = 6,
            .callback = &transmit_data,
            .timestamp = &timestamp,
            .amountOfChannels = 3,
            .timebase = 0.001f,
            .sizeOfChannels = 50
    };
		
		// Stack mutexes
		RtosMutexHandle dataMutex = RtosMutex_create();
    RtosMutexHandle configMutex = RtosMutex_create();
		RtosMutexHandle loggergMutex = RtosMutex_create();
		
    ScopeFramedStackMutex mutexes = {
      .configMutex = RtosMutex_getIMutex(configMutex),
      .dataMutex = RtosMutex_getIMutex(dataMutex),
			.logBufferMutex = RtosMutex_getIMutex(loggergMutex)
    };

		// Logger 
		RtosMutexHandle bufferMutex = RtosMutex_create();
		BufferedByteStreamHandle stream = BufferedByteStream_create(512);
		ThreadSafeByteStreamHandle bufferedStream = ThreadSafeByteStream_create(RtosMutex_getIMutex(bufferMutex), BufferedByteStream_getIByteStream(stream));
		logger = ThreadSafeByteStream_getIByteStream(bufferedStream);
		
		ScopeFramedStackLogOptions scopeLogOptions = {
			.logByteStream = logger
		};

		// Priorities
		Message_Priorities msgPrios = {
			.data = MEDIUM,
			.log = MEDIUM,
			.stream = MEDIUM
		};
	
		// Scope instantiation
    scopeStack = ScopeFramedStack_createThreadSafe(config, mutexes, scopeLogOptions, msgPrios);
		
    UartDriver_init();
		
    // Variable announcement
    AnnounceStorageHandle addressStorage = ScopeFramedStack_getAnnounceStorage(scopeStack);
    AnnounceStorage_addAnnounceAddress(addressStorage, "sinus", &sinus, SE_FLOAT);
    AnnounceStorage_addAnnounceAddress(addressStorage, "cosinus", &cosinus, SE_FLOAT);
    AnnounceStorage_addAnnounceAddress(addressStorage, "leistung", &leistung, SE_FLOAT);
    AnnounceStorage_addAnnounceAddress(addressStorage, "sum", &sum, SE_FLOAT);
    AnnounceStorage_addAnnounceAddress(addressStorage, "flipflop", &flipflop, SE_INT8);
		AnnounceStorage_addAnnounceAddress(addressStorage, "10s Pulse", &toggle, SE_UINT8);
		
    /* USER CODE END 2 */

    /* Call init function for freertos objects (in freertos.c) */
    MX_FREERTOS_Init();

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
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
    RCC_OscInitStruct.PLL.PLLN = 70;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV10;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV5;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV5;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

// The scope library calls this function when an assert triggered
void __assert_func (const char * file, int line, const char * func, const char * expr)
{
	  assert(false);
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM1) {
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
    /* Turn LED4 to off for error */
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

void __aeabi_assert(const char *expr, const char *file, int line) {
    while (1) {
    }
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
