/**
  ******************************************************************************
  * @file    freertos.c
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

/* Includes */
#include "stm32f4xx.h"
#include <initsys.h>
#include <lime_mdriver.h>
#include <lime_mpacket.h>
#include <lime_l3gd20.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
osThreadId 		OPERATION_TaskHandle; 				/* Periodically blink LD15 to indicate operation */
osThreadId 		SERIAL_COM_TaskHandle;				/* Serial communication task to communicate with the computer */
osThreadId		PWM_UPDATE_TaskHandle;				/**/
osThreadId 		ADC_READING_TaskHandle;				/**/

/* Private function prototypes -----------------------------------------------*/
void 			freertos_init(void);

/* Task/Threads functions ----------------------------------------------------*/
/**
  * @brief 	OPERATION_task: Periodically blink LD15 to indicate operation
  * @param 	argument
  * @retval none
  */
void OPERATION_task(void const *argument)
{
	for(;;)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		osDelay(1000);
	}
}

/**
  * @brief 	SERIAL_COM_task : Serial communication task to communicate with the computer
  * @param 	argument
  * @retval none
  */
void SERIAL_COM_task(void const *argument)
{
	uint8_t init_once = 0;
	for (;;)
	{
		if (init_once == 0)
		{
			char * str = "123456789123456789123456789\n";
			SERIAL_PACKAGE1.MesContent 	= (uint8_t *) str;
			SERIAL_PACKAGE1.MesLen 		= 28;
			init_once = 1;
		}
		// Loop forever
		LIME_MPACKET_Publish(&SERIAL_PACKAGE1, &initHandles);
		osSignalWait(limeSignalList.sDEBUG, configMAX_PUBLISH_WAIT);
		limeSignalList.sDEBUG = 1;
		limeSignalList.sFINISHED_PUBLISHING = 1;
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	}
}

/**
  * @brief 	PWM_UPDATE_task : Periodically update PWM value.
  * @param 	argument
  * @retval none
  */
void PWM_UPDATE_task(void const *argument)
{
	for (;;)
	{
		MDRIVE1.PWMTimerHandle.Instance->CCR2 += 0x1000;
		MDRIVE2.PWMTimerHandle.Instance->CCR1 += 0x1000;
		DRIVE1_FWD();
		DRIVE2_FWD();
		osDelay(1000);
		DRIVE1_BWD();
		DRIVE2_BWD();
		osDelay(1000);
	}
}

/**
  * @brief 	PWM_UPDATE_task : Serial communication task to communicate with the computer
  * @param 	argument
  * @retval none
  */
void ADC_READING_task(void const *argument)
{
	for (;;)
	{

		osDelay(10);
	}
}

/**
  * @brief 	freertos_init function: Initialize all the tasks.
  * @param 	none
  * @retval none
  */
void freertos_init(void)
{
	/* Task def and set task priority */
	osThreadDef(OPERATION, OPERATION_task, osPriorityNormal, 0, 128);
	OPERATION_TaskHandle 		= osThreadCreate(osThread(OPERATION), NULL);

	osThreadDef(uart2Com,  SERIAL_COM_task, osPriorityNormal, 0, 200);
	SERIAL_COM_TaskHandle 			= osThreadCreate(osThread(uart2Com), NULL);

	osThreadDef(PWM_UPDATE, PWM_UPDATE_task, osPriorityHigh, 0, 128);
	PWM_UPDATE_TaskHandle 		= osThreadCreate(osThread(PWM_UPDATE), NULL);

	osThreadDef(ADC_READING, ADC_READING_task, osPriorityHigh, 0, 200);
	ADC_READING_TaskHandle 	 	= osThreadCreate(osThread(ADC_READING), NULL);
}
