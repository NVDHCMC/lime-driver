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
#include <litve_adc.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define configIRLF_THRESHOLD 1000
#define IRLF_SPD_1 			0x8000
#define IRLF_SPD_2 			0x0500
#define IRLF_SPD_3 			0x3000

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
	uint8_t initonce = 0;
	for (;;)
	{
		// Update motor status
		MDRIVE1.MOTOR_UPDATE();
		MDRIVE2.MOTOR_UPDATE();
		if (initonce == 0)
		{
			LIME_MPACKET_AddField("PosM1", (uint32_t) MDRIVE1.MOTOR.Encoder, &SERIAL_PACKAGE1);
			LIME_MPACKET_AddField("PosM2", (uint32_t) MDRIVE2.MOTOR.Encoder, &SERIAL_PACKAGE1);
			LIME_MPACKET_AddField("SpdM1", (uint32_t) MDRIVE1.MOTOR.SpeedVal, &SERIAL_PACKAGE1);
			LIME_MPACKET_AddField("SpdM2", (uint32_t) MDRIVE2.MOTOR.SpeedVal, &SERIAL_PACKAGE1);
			LIME_MPACKET_AddField("ADC00", (uint32_t) IRLF_ADC.ADC_channel_value[0], &SERIAL_PACKAGE1);
			LIME_MPACKET_AddField("ADC01", (uint32_t) IRLF_ADC.ADC_channel_value[1], &SERIAL_PACKAGE1);
			LIME_MPACKET_AddField("ADC02", (uint32_t) IRLF_ADC.ADC_channel_value[2], &SERIAL_PACKAGE1);
			LIME_MPACKET_AddField("ADC03", (uint32_t) IRLF_ADC.ADC_channel_value[3], &SERIAL_PACKAGE1);
			LIME_MPACKET_AddField("ADC04", (uint32_t) IRLF_ADC.ADC_channel_value[4], &SERIAL_PACKAGE1);
			LIME_MPACKET_AddField("ADC05", (uint32_t) IRLF_ADC.ADC_channel_value[5], &SERIAL_PACKAGE1);
			LIME_MPACKET_AddField("ADC06", (uint32_t) IRLF_ADC.ADC_channel_value[6], &SERIAL_PACKAGE1);
			initonce = 1;
		}
		else
		{
			SERIAL_PACKAGE1.MesContent[6] 		= (uint8_t) (MDRIVE1.MOTOR.Encoder >> 24);
			SERIAL_PACKAGE1.MesContent[7] 		= (uint8_t) (MDRIVE1.MOTOR.Encoder >> 16);
			SERIAL_PACKAGE1.MesContent[8] 		= (uint8_t) (MDRIVE1.MOTOR.Encoder >> 8);
			SERIAL_PACKAGE1.MesContent[9] 		= (uint8_t) (MDRIVE1.MOTOR.Encoder);
			SERIAL_PACKAGE1.MesContent[17] 		= (uint8_t) (MDRIVE2.MOTOR.Encoder >> 24);
			SERIAL_PACKAGE1.MesContent[18] 		= (uint8_t) (MDRIVE2.MOTOR.Encoder >> 16);
			SERIAL_PACKAGE1.MesContent[19] 		= (uint8_t) (MDRIVE2.MOTOR.Encoder >> 8);
			SERIAL_PACKAGE1.MesContent[20] 		= (uint8_t) (MDRIVE2.MOTOR.Encoder);
			SERIAL_PACKAGE1.MesContent[28] 		= (uint8_t) (MDRIVE1.MOTOR.SpeedVal >> 24);
			SERIAL_PACKAGE1.MesContent[29] 		= (uint8_t) (MDRIVE1.MOTOR.SpeedVal >> 16);
			SERIAL_PACKAGE1.MesContent[30] 		= (uint8_t) (MDRIVE1.MOTOR.SpeedVal >> 8);
			SERIAL_PACKAGE1.MesContent[31] 		= (uint8_t) (MDRIVE1.MOTOR.SpeedVal);
			SERIAL_PACKAGE1.MesContent[39] 		= (uint8_t) (MDRIVE2.MOTOR.SpeedVal >> 24);
			SERIAL_PACKAGE1.MesContent[40] 		= (uint8_t) (MDRIVE2.MOTOR.SpeedVal >> 16);
			SERIAL_PACKAGE1.MesContent[41] 		= (uint8_t) (MDRIVE2.MOTOR.SpeedVal >> 8);
			SERIAL_PACKAGE1.MesContent[42] 		= (uint8_t) (MDRIVE2.MOTOR.SpeedVal);
			SERIAL_PACKAGE1.MesContent[50] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[0] >> 24);
			SERIAL_PACKAGE1.MesContent[51] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[0] >> 16);
			SERIAL_PACKAGE1.MesContent[52] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[0] >> 8);
			SERIAL_PACKAGE1.MesContent[53] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[0]);
			SERIAL_PACKAGE1.MesContent[61] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[1] >> 24);
			SERIAL_PACKAGE1.MesContent[62] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[1] >> 16);
			SERIAL_PACKAGE1.MesContent[63] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[1] >> 8);
			SERIAL_PACKAGE1.MesContent[64] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[1]);
			SERIAL_PACKAGE1.MesContent[72] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[2] >> 24);
			SERIAL_PACKAGE1.MesContent[73] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[2] >> 16);
			SERIAL_PACKAGE1.MesContent[74] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[2] >> 8);
			SERIAL_PACKAGE1.MesContent[75] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[2]);
			SERIAL_PACKAGE1.MesContent[83] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[3] >> 24);
			SERIAL_PACKAGE1.MesContent[84] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[3] >> 16);
			SERIAL_PACKAGE1.MesContent[85] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[3] >> 8);
			SERIAL_PACKAGE1.MesContent[86] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[3]);
			SERIAL_PACKAGE1.MesContent[94] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[4] >> 24);
			SERIAL_PACKAGE1.MesContent[95] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[4] >> 16);
			SERIAL_PACKAGE1.MesContent[96] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[4] >> 8);
			SERIAL_PACKAGE1.MesContent[97] 		= (uint8_t) (IRLF_ADC.ADC_channel_value[4]);
			SERIAL_PACKAGE1.MesContent[105] 	= (uint8_t) (IRLF_ADC.ADC_channel_value[5] >> 24);
			SERIAL_PACKAGE1.MesContent[106] 	= (uint8_t) (IRLF_ADC.ADC_channel_value[5] >> 16);
			SERIAL_PACKAGE1.MesContent[107] 	= (uint8_t) (IRLF_ADC.ADC_channel_value[5] >> 8);
			SERIAL_PACKAGE1.MesContent[108] 	= (uint8_t) (IRLF_ADC.ADC_channel_value[5]);
			SERIAL_PACKAGE1.MesContent[116] 	= (uint8_t) (IRLF_ADC.ADC_channel_value[6] >> 24);
			SERIAL_PACKAGE1.MesContent[117] 	= (uint8_t) (IRLF_ADC.ADC_channel_value[6] >> 16);
			SERIAL_PACKAGE1.MesContent[118] 	= (uint8_t) (IRLF_ADC.ADC_channel_value[6] >> 8);
			SERIAL_PACKAGE1.MesContent[119] 	= (uint8_t) (IRLF_ADC.ADC_channel_value[6]);
		}

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
		DRIVE1_FWD();
		DRIVE2_FWD();
		// IRLF_ADC.EXTRAPOLATE_LINE_CENTER();
		// IRLF_ADC.CALCULATE_REFERENCE_POSTURE();
		// IRLF_ADC.CALCULATE_SPEED
		if ((IRLF_ADC.ADC_channel_value[2] < configIRLF_THRESHOLD) && (IRLF_ADC.ADC_channel_value[4] > configIRLF_THRESHOLD))
		{
			MDRIVE1.PWMTimerHandle.Instance->CCR2 = IRLF_SPD_2;
			MDRIVE2.PWMTimerHandle.Instance->CCR1 = IRLF_SPD_1;
		}
		else if ((IRLF_ADC.ADC_channel_value[4] < configIRLF_THRESHOLD) && (IRLF_ADC.ADC_channel_value[2] > configIRLF_THRESHOLD))
		{
			MDRIVE1.PWMTimerHandle.Instance->CCR2 = IRLF_SPD_1;
			MDRIVE2.PWMTimerHandle.Instance->CCR1 = IRLF_SPD_2;
		}
		else
		{
			MDRIVE1.PWMTimerHandle.Instance->CCR2 = IRLF_SPD_1;
			MDRIVE2.PWMTimerHandle.Instance->CCR1 = IRLF_SPD_1;
		}
		osDelay(5);
	}
}

/**
  * @brief 	PWM_UPDATE_task : Serial communication task to communicate with the computer
  * @param 	argument
  * @retval none
  */
void ADC_READING_task(void const *argument)
{
	int initonce = 0;
	for (;;)
	{
		if (initonce == 0)
		{
			HAL_ADC_Start_DMA(&IRLF_ADC.ADC_Handle, (uint32_t *)IRLF_ADC.ADC_channel_value, configNUM_OF_CHANNELs);
			initonce = 1;
		}
		//osSignalWait(limeSignalList.sADC_DONE, configMAX_PUBLISH_WAIT);
		osDelay(1000);
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

	osThreadDef(uart2Com,  SERIAL_COM_task, osPriorityHigh, 0, 200);
	SERIAL_COM_TaskHandle 			= osThreadCreate(osThread(uart2Com), NULL);

	osThreadDef(PWM_UPDATE, PWM_UPDATE_task, osPriorityNormal, 0, 128);
	PWM_UPDATE_TaskHandle 		= osThreadCreate(osThread(PWM_UPDATE), NULL);

	osThreadDef(ADC_READING, ADC_READING_task, osPriorityNormal, 0, 200);
	ADC_READING_TaskHandle 	 	= osThreadCreate(osThread(ADC_READING), NULL);

	/* Allocate package structure memory */

}
