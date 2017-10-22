/**
  ******************************************************************************
  * @file    main.c
  * @author  Azure
  * @version V0.2.0
  * @date    8th-October-2017
  * @brief 	 Contain main kernel initialization, OS startup sequence
  * 		 and peripheral interrupt request handlers and callbacks
  *
  * @info 	Handlers: 	Universal Error Handler
  * 					EXTI0 IRQ Handler function
  * 					USART2 IRQ Handler function
  * 					TIM2 IRQ Handler function
  * 					TIM3 IRQ Handler function
  * 					TIM5 IRQ Handler function
  * 					SPI1 IRQ Handler function
  *
  * 		Callbacks: 	UART Receive Complete callback functions
  * 					TIMers Period Elapsed callback
  * 					Input Capture callback
  * 					SPI Receive Complete callback
  *
  *
  ******************************************************************************
*/


/* Includes --------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include <initsys.h>
#include <lime_mdriver.h>
#include <lime_mpacket.h>
#include <lime_l3gd20.h>

/* Private function prototypes -------------------------------------------------*/
extern void 				freertos_init(void);

/* Private variables -----------------------------------------------------------*/
//extern osThreadId 			OPERATION_TaskHandle; 				/* Periodically blink LD15 to indicate operation */
extern osThreadId 			SERIAL_COM_TaskHandle;				/* Serial communication task to communicate with the computer */


/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Universal Error Handler: handling common errors.
  * @param  none
  * @retval none
  *
  * @notice GPIOD port needs to be initialized for the error handling.
  */
void _Error_Handler()
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /*
   * User can add his own implementation to report the HAL error return state
   */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
	while(1)
	{
		/* Flash all 4 LEDs */
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
		HAL_Delay(500);
	}
  /* USER CODE END Error_Handler_Debug */
}

/* Special functions --------------------------------------------------------*/
/** @defgroup MAIN_Callbacks_and IRQs
  * @{
  */

/** @defgroup MAIN_IRQs
  * @brief    Interrupt request functions for various ISRs
  *
@verbatim
!!! Need to be fixed.
 ===============================================================================
                 ##### Extended features functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Start conversion of injected channel.
      (+) Stop conversion of injected channel.
      (+) Start multimode and enable DMA transfer.
      (+) Stop multimode and disable DMA transfer.
      (+) Get result of injected channel conversion.
      (+) Get result of multimode conversion.
      (+) Configure injected channels.
      (+) Configure multimode.

@endverbatim
  * @{
  */

/** @brief	EXTI0 IRQ Handler function: handling External Interrupt Request line #0
  * @param 	none
  * @retval none
  *
  * @notice unusable if not first initialized in NVIC.
  */
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
}

/** @brief	USART2 IRQ Handler function: handling USART Interrupt Request line #2
  * @param 	none
  * @retval none
  *
  * @notice unusable if not first initialized in NVIC.
  */
void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&initHandles.USART2_Handle);
}

/** @brief 	TIM2 IRQ Handler function: handling TIMER Interrupt Request device #2
  * @param 	none
  * @retval none
  *
  * @notice unusable if not first initialized in NVIC.
  */
/*void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&sampleTimer1Handle);
}*/

/** @brief 	TIM3 IRQ Handler function: handling TIMER Interrupt Request device #3
  * @param 	none
  * @retval none
  *
  * @notice unusable if not first initialized in NVIC.
  */
/*void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&encoderTimer1Handle);
}*/

/** @brief 	TIM5 IRQ Handler function
  * @param 	none
  * @retval none
  *
  * @notice unusable if not first initialized in NVIC.
  */
void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&initHandles.TIM4_Handle);
}

/** @brief 	TIM5 IRQ Handler function
  * @param 	none
  * @retval none
  *
  * @notice unusable if not first initialized in NVIC.
  */
void TIM5_IRQHandler(void)
{
	//HAL_TIM_IRQHandler(&sampleTimer1Handle); // Wrong timer
}

/** @brief 	SPI1 IRQ Handler function: handling SPI Interrupt request line #1
  * @param 	none
  * @retval none
  *
  * @notice unusable if not first initialized in NVIC.
  */
/*void SPI1_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&SPI1_Handle);
}*/
/**
  * @}
  */

/** @defgroup MAIN_Callbacks
  * @brief    Callback functions for various ISRs
  *
@verbatim
!!! Need to be fixed.
 ===============================================================================
                 ##### Extended features functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Start conversion of injected channel.
      (+) Stop conversion of injected channel.
      (+) Start multimode and enable DMA transfer.
      (+) Stop multimode and disable DMA transfer.
      (+) Get result of injected channel conversion.
      (+) Get result of multimode conversion.
      (+) Configure injected channels.
      (+) Configure multimode.

@endverbatim
  * @{
  */

/** @brief 	HAL_UART_TxCpltCallback
  * @param 	UART_HandleTypeDef
  * @retval none
  *
  * @notice unusable if not first initialized in NVIC.
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	{
		osSignalSet(SERIAL_COM_TaskHandle, limeSignalList.sFINISHED_PUBLISHING);
	}
}

/*void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (uart2Mode == uartNormal)
	{
		if (strncmp((const char *) uart2ComHandle.uartRecvMess.mesContent, "$start", 6) == 0)
		{
			uart2Mode 		= uartAuthenticate;
		}
		uart2ComHandle.uartRecvMess.listening 	= 0;
	}
	else if (uart2Mode == uartAuthenticate)
	{
		if (strncmp((const char *) uart2ComHandle.uartRecvMess.mesContent, ROOT.password, ROOT.pass_len) == 0)
			uart2Mode 		= uartProgram;
		else
		{
			uart2Mode 		= uartNormal;
			uart2ComHandle.uartRecvMess.listening 	= 1;
		}
	}
	else if (uart2Mode == uartProgram)
	{
		motor1.status 		|= MOTOR_SPEED_UPDATE;
		uart2ComHandle.uartRecvMess.listening 	= 0;
	}
}*/

/** @brief 	TIMers Period Elapsed callback
  * @{
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == initHandles.TIM4_Handle.Instance)
	{
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
		osSignalSet(SERIAL_COM_TaskHandle, limeSignalList.sDEBUG);
	}
#endif  /* INCLUDE_xTaskGetSchedulerState */
	}
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
}

/** @brief 	Input Capture callback
  * @{
  */
/*void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == DRIVER1_SAMPLE_TIMER)
	{
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			motor1.channel1_value 		= __HAL_TIM_GET_COMPARE(&sampleTimer1Handle, TIM_CHANNEL_1);
			if (motor1.status && MOTOR_SPEED_UPDATE)
			{
				if (motor1.status && MOTOR_LAPPED_EVENT)
				{
					motor1.speed_value 		= (0xffffffff - motor1.channel2_value) + motor1.channel1_value;
					motor1.status 			&= ~MOTOR_LAPPED_EVENT;
				}
				else
				{
					motor1.speed_value 			= motor1.channel1_value - motor1.channel2_value;
				}
			}
		}

		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			motor1.channel2_value 		= __HAL_TIM_GET_COMPARE(&sampleTimer1Handle, TIM_CHANNEL_2);
			if (motor1.status && MOTOR_SPEED_UPDATE)
			{
				if (motor1.status && MOTOR_LAPPED_EVENT)
				{
					motor1.speed_value 		= (0xffffffff - motor1.channel1_value) + motor1.channel2_value;
					motor1.status 			&= ~MOTOR_LAPPED_EVENT;
				}
				else
				{
					motor1.speed_value 			= motor1.channel2_value - motor1.channel1_value;
				}
			}
		}

		motor1.status 					|= MOTOR_SPEED_UPDATE;
	}
	else if (htim->Instance == DRIVER1_ENCODER_TIMER)
	{

	}
}
*/

/** @brief 	SPI Receive Complete callback
  * @{
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	}
}
/**
  * @}
  */

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	}
}
/**
  * @}
  */

/**
  * @}
  */

/** @brief 	MAIN: to initialize Low level driver for peripheral devices and the OS system kernel.
  * @param	none
  * @retval none
  */
int main(void)
{
	/* HAL Module Initializations */
	HAL_Init();

	/**/
	MDRIVE1.MOTOR_INIT();

	/**/
	LIME_INIT_SYSTEM();

	/**/
	L3GD20DRIVE.INIT();

	/**/
	MDRIVE1.MOTOR_START();

	/* FreeRTOS task init */
	freertos_init();

	/* Start the scheduler */
	osKernelStart();

	//char * str = "absd";
	/* FreeRTOS Initialization */
	//freeRTOS_Init();

	/* Start scheduler */
	//osKernelStart();

	for(;;)
	{
		//HAL_UART_Transmit_IT(&initHandles.USART2_Handle, (uint8_t *) str, 4);
		//HAL_Delay(1000);
		// The microprocessor should never reach here
	}
}
