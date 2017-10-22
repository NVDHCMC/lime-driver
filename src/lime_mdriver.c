/**
  ******************************************************************************
  * @file    lime_mdriver.c
  * @author  Azure
  * @version V0.2.0
  * @date    8th-October-2017
  * @brief 	 Contain main kernel initialization, OS startup sequence
  * 		 and peripheral interrupt request handlers and callbacks
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <initsys.h>
#include <lime_mdriver.h>

/* Imported functions --------------------------------------------------------*/
extern void _Error_Handler(void);

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* MDRIVER timers for encoder reading and PWM signal generation */
#ifdef LIME_MDRIVER_1
MDRIVE_Handles 					MDRIVE1 =
{
	{0},
	{0},
	{0},
	LIME_MDRIVE_INIT,
	LIME_MDRIVE_START,
	LIME_MDRIVE_MOTOR_UPDATE
};
LIME_MPACKET_Type 				SERIAL_PACKAGE1;
#endif

#ifdef LIME_MDRIVER_2
MDRIVE_Handles 					MDRIVE2;
#endif

/* Exported functions --------------------------------------------------------*/
/**
  * @brief 	MDRIVE1.ENCODER_INIT
  * @param
  * @retval LIME_Status
  */
void MDRIVE1_ENCODER_INIT(void)
{
#ifdef LIME_MDRIVER_1
	TIM_Encoder_InitTypeDef 	MDRIVE1_EncoderTimerConfig;
	/*
	 *  CODE BEGINS
	 *  Encoder timer initialization
	 */
	MDRIVE1.EncoderTimerHandle.Instance 			= DRIVER1_ENCODER_TIMER;
	MDRIVE1.EncoderTimerHandle.Init.ClockDivision 	= TIM_CLOCKDIVISION_DIV1;
	MDRIVE1.EncoderTimerHandle.Init.CounterMode 	= TIM_COUNTERMODE_UP;
	MDRIVE1.EncoderTimerHandle.Init.Period 			= PULSE_PER_REV;
	MDRIVE1.EncoderTimerHandle.Init.Prescaler		= 0;
	MDRIVE1.EncoderTimerHandle.Channel 				= TIM_CHANNEL_1 | TIM_CHANNEL_2;
	MDRIVE1_EncoderTimerConfig.EncoderMode 			= TIM_ENCODERMODE_TI12;
	MDRIVE1_EncoderTimerConfig.IC1Filter 			= 0x07;
	MDRIVE1_EncoderTimerConfig.IC1Polarity  		= TIM_ICPOLARITY_FALLING;
	MDRIVE1_EncoderTimerConfig.IC1Prescaler 		= TIM_ICPSC_DIV1;
	MDRIVE1_EncoderTimerConfig.IC1Selection 		= TIM_ICSELECTION_DIRECTTI;
	MDRIVE1_EncoderTimerConfig.IC2Filter 			= 0x7;
	MDRIVE1_EncoderTimerConfig.IC2Polarity 			= TIM_ICPOLARITY_FALLING;
	MDRIVE1_EncoderTimerConfig.IC2Prescaler			= TIM_ICPSC_DIV1;
	MDRIVE1_EncoderTimerConfig.IC2Selection 		= TIM_ICSELECTION_DIRECTTI;

	if (HAL_TIM_Encoder_Init(&MDRIVE1.EncoderTimerHandle, &MDRIVE1_EncoderTimerConfig) != HAL_OK)
	{
		_Error_Handler();
	}
	/*
	 * MDRIVER_1 Logic pins init
	 */
	GPIO_InitTypeDef GPIO_Handle;

	GPIO_Handle.Pin 	= DRIVER1_LOGIC_PIN_1 | DRIVER1_LOGIC_PIN_2;
	GPIO_Handle.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_Handle.Pull 	= GPIO_NOPULL;
	GPIO_Handle.Speed 	= GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(DRIVER1_LOGIC_PORT, &GPIO_Handle);
	/*
	 * CODE ENDS
	 * Sample timer initialization
	 */
	GPIO_InitTypeDef GPIO_InitStruct;
	/* CODE BEGINS Driver 1 encoder timer Msp_Init */
	if (MDRIVE1.EncoderTimerHandle.Instance == DRIVER1_ENCODER_TIMER)
	{
		GPIO_InitStruct.Pin 	= DRIVER1_ENCODER_PINS;
		GPIO_InitStruct.Mode 	= GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull 	= GPIO_NOPULL;
		GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_VERY_HIGH;
		// This line also needs to be modified if there is a change in the motor's encoder timer
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
		HAL_GPIO_Init(DRIVER1_ENCODER_PORT, &GPIO_InitStruct);
	}

	/* Init encoder's timer entry in NVIC table */
	HAL_NVIC_SetPriority(DRIVER1_ENCODER_NVIC_IRQn, DRIVER1_ENCODER_PRIORITY, 0);
	HAL_NVIC_EnableIRQ(DRIVER1_ENCODER_NVIC_IRQn);
	/* CODE ENDS Driver 1 encoder timer Msp_Init */
#endif
}

/**
  * @brief 	LIME_MDRIVE_INIT
  * @param 	none
  * @retval none
  */
void MDRIVE1_PWM_INIT(void)
{
#ifdef LIME_MDRIVER_1
	/*
	 * Timer configuration
	 */
	TIM_OC_InitTypeDef 	MDRIVE1_PWMTimerOCConfig;

	MDRIVE1.PWMTimerHandle.Instance 			= DRIVER1_STEP_TIMER;
	MDRIVE1.PWMTimerHandle.Init.CounterMode 	= TIM_COUNTERMODE_UP;
	MDRIVE1.PWMTimerHandle.Init.ClockDivision 	= TIM_CLOCKDIVISION_DIV1;
	MDRIVE1.PWMTimerHandle.Init.Period 			= 0x7fff;
	MDRIVE1.PWMTimerHandle.Init.Prescaler 		= 0;
	MDRIVE1.PWMTimerHandle.Init.RepetitionCounter = 1;

	if (HAL_TIM_PWM_Init(&MDRIVE1.PWMTimerHandle) != HAL_OK)
	{
		_Error_Handler();
	}

	MDRIVE1_PWMTimerOCConfig.OCMode 			= TIM_OCMODE_PWM1;
	MDRIVE1_PWMTimerOCConfig.Pulse 				= 0;
	MDRIVE1_PWMTimerOCConfig.OCPolarity 	 	= TIM_OCPOLARITY_HIGH;
	MDRIVE1_PWMTimerOCConfig.OCFastMode 		= TIM_OCFAST_DISABLE;

	if (HAL_TIM_PWM_ConfigChannel(&MDRIVE1.PWMTimerHandle, &MDRIVE1_PWMTimerOCConfig, TIM_CHANNEL_1) != HAL_OK)
	{
		_Error_Handler();
	}

	/*
	 * Timer MSP Initialization
	 */

	GPIO_InitTypeDef GPIO_Handle;

	GPIO_Handle.Pin		= DRIVER1_STEP;
	GPIO_Handle.Mode 	= GPIO_MODE_AF_PP;
	GPIO_Handle.Pull 	= GPIO_NOPULL;
	GPIO_Handle.Speed 	= GPIO_SPEED_FREQ_VERY_HIGH;
	// In case of custom configuration, change this into appropriate GPIO pin mode.
	GPIO_Handle.Alternate = GPIO_AF3_TIM10;
	HAL_GPIO_Init(DRIVER1_STEP_PORT, &GPIO_Handle);
#endif
}

/**
  * @brief 	LIME_MDRIVE_INIT
  * @param
  * @retval LIME_Status
  */
LIME_Status LIME_MDRIVE_INIT(void)
{
	uint8_t status = LIME_ERROR;

#ifdef LIME_MDRIVER_1
	/* Init motor struct */
	MDRIVE1.MOTOR.DRVStat 	&= (MDRIVE_StatusType) ~(MOTOR_ENABLE | MOTOR_LAPPED_EVENT | MOTOR_UPDATE_EVENT | MOTOR_RESET);
	MDRIVE1.MOTOR.NumRev	= 0;
	MDRIVE1.MOTOR.SpeedRef  = 0;
	MDRIVE1.MOTOR.SpeedVal  = 0;
	MDRIVE1.MOTOR.Encoder 	= 0;
	MDRIVE1.MOTOR.SampleFreq= SAMPLE_FREQ_MEDIUM;

	/* Replace system handle and init function with custom ones */
	//initHandles.TIM10_Handle= MDRIVE1.PWMTimerHandle;
	initHandles.EX0_PPP_INIT= MDRIVE1_PWM_INIT;
	//initHandles.TIM3_Handle = MDRIVE1.EncoderTimerHandle;
	initHandles.EX1_PPP_INIT= MDRIVE1_ENCODER_INIT;
	if (LIME_MPACKET_Init(&SERIAL_PACKAGE1) != LIME_OK)
	{
		status = LIME_ERROR;
	}
	status 					= LIME_OK;
#endif

#ifdef LIME_MDRIVER_2
	/* Init motor struct */
	MDRIVE2.MOTOR.DRVStat 	&= (MDRIVE_StatusType) 0;
	MDRIVE2.MOTOR.NumRev	= 0;
	MDRIVE2.MOTOR.SpeedRef  = 0;
	MDRIVE2.MOTOR.SpeedVal  = 0;
	MDRIVE2.MOTOR.Encoder 	= 0;

	/* Replace system handle and init function with custom ones */
	//initHandles.TIM10_Handle= MDRIVE1.PWMTimerHandle;
	initHandles.EX2_PPP_INIT= MDRIVE2_PWM_INIT;
	//initHandles.TIM3_Handle = MDRIVE1.EncoderTimerHandle;
	initHandles.EX3_PPP_INIT= MDRIVE2_ENCODER_INIT;
	status 					= LIME_OK;
#endif
	return status;
}

/**
  * @brief 	LIME_MDRIVE_MOTOR_UPDATE
  * @param
  * @retval LIME_Status
  */
LIME_Status LIME_MDRIVE_MOTOR_UPDATE(void)
{
	uint8_t status 			= LIME_OK;
#ifdef LIME_MDRIVER_1
	MDRIVE1.MOTOR.Encoder 	= MDRIVE1.EncoderTimerHandle.Instance->CNT;
#endif

#ifdef LIME_MDRIVER_2
	MDRIVE2.MOTOR.Encoder 	= MDRIVE2.EncoderTimerHandle.Instance->CNT;
#endif
	return status;
}

/**
  * @brief 	LIME_MDRIVE_START
  * @param
  * @retval LIME_Status
  */
LIME_Status LIME_MDRIVE_START(void)
{
	uint8_t status 			= LIME_ERROR;
#ifdef LIME_MDRIVER_1
	/* Start the encoder pulse counter. */
	HAL_TIM_Base_Start(&MDRIVE1.EncoderTimerHandle);
	HAL_TIM_Encoder_Start(&MDRIVE1.EncoderTimerHandle, TIM_CHANNEL_1 | TIM_CHANNEL_2);

	/* */
	HAL_TIM_Base_Start_IT(&initHandles.TIM4_Handle);

	/* Start the PWM generation. */
	HAL_TIM_PWM_Start(&MDRIVE1.PWMTimerHandle, TIM_CHANNEL_1);
	/* Stop the motor. */
	HAL_GPIO_WritePin(DRIVER1_LOGIC_PORT, DRIVER1_LOGIC_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRIVER1_LOGIC_PORT, DRIVER1_LOGIC_PIN_2, GPIO_PIN_RESET);
	status 					= LIME_OK;
#endif

#ifdef LIME_MDRIVER_2
	/* Start the encoder pulse counter. */
	HAL_TIM_Base_Start(&MDRIVE2.EncoderTimerHandle);
	HAL_TIM_Encoder_Start(&MDRIVE2.EncoderTimerHandle, TIM_CHANNEL_1 | TIM_CHANNEL_2);

	/* Start the PWM generation. */
	HAL_TIM_PWM_Start(&MDRIVE2.PWMTimerHandle, TIM_CHANNEL_1);
	/* Stop the motor. */
	HAL_GPIO_WritePin(DRIVER2_LOGIC_PORT, DRIVER2_LOGIC_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRIVER2_LOGIC_PORT, DRIVER2_LOGIC_PIN_2, GPIO_PIN_RESET);
	status 					= LIME_OK;
#endif
	return status;
}
