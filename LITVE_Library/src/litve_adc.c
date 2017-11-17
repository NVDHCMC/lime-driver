/**
  ******************************************************************************
  * @file    litve_adc.c
  * @author  Azure
  * @version V0.1.0
  * @date    2nd-November-2017
  * @brief 	 Source code of Line-tracking vehicle ADC data acquisition.
  *
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <cmsis_os.h>
#include <stdlib.h>
#include <initsys.h>
#include <litve_adc.h>

extern void _Error_Handler(void);
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
IRLF_ADC_HandleTypeDef 		IRLF_ADC = {
		{0},
		{0},
		NULL,
		IRLF_ADC_PERIPH_INIT,
		IRLF_DMA_INIT
};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  IRLF_ADC_PERIPH_INIT: Init ADC peripheral
  * @param 	none
  * @retval none
  *
  * @notice unusable if not first initialized main data bus clock
  */
void IRLF_ADC_PERIPH_INIT(void)
{
	ADC_ChannelConfTypeDef sConfig;

	/* Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) */
	IRLF_ADC.ADC_Handle.Instance 					= ADC1;
	IRLF_ADC.ADC_Handle.Init.ClockPrescaler 		= ADC_CLOCK_SYNC_PCLK_DIV2;
	IRLF_ADC.ADC_Handle.Init.Resolution 			= ADC_RESOLUTION_12B;
	IRLF_ADC.ADC_Handle.Init.ScanConvMode 			= ENABLE;
	IRLF_ADC.ADC_Handle.Init.ContinuousConvMode 	= ENABLE;
	IRLF_ADC.ADC_Handle.Init.DiscontinuousConvMode 	= DISABLE;
	IRLF_ADC.ADC_Handle.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
	IRLF_ADC.ADC_Handle.Init.ExternalTrigConv 		= ADC_SOFTWARE_START;
	IRLF_ADC.ADC_Handle.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
	IRLF_ADC.ADC_Handle.Init.NbrOfConversion 		= configNUM_OF_CHANNELs;
	IRLF_ADC.ADC_Handle.Init.DMAContinuousRequests 	= ENABLE;
	IRLF_ADC.ADC_Handle.Init.EOCSelection 			= ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&IRLF_ADC.ADC_Handle) != HAL_OK)
	{
		_Error_Handler();
	}

	/* Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. */
	sConfig.Channel 		= ADC_CHANNEL_8;
	sConfig.Rank 			= 1;
	sConfig.SamplingTime 	= ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&IRLF_ADC.ADC_Handle, &sConfig) != HAL_OK)
	{
		_Error_Handler();
	}

	sConfig.Channel 		= ADC_CHANNEL_9;
	sConfig.Rank 			= 2;
	if (HAL_ADC_ConfigChannel(&IRLF_ADC.ADC_Handle, &sConfig) != HAL_OK)
	{
		_Error_Handler();
	}

	sConfig.Channel 		= ADC_CHANNEL_11;
	sConfig.Rank 			= 3;
	if (HAL_ADC_ConfigChannel(&IRLF_ADC.ADC_Handle, &sConfig) != HAL_OK)
	{
		_Error_Handler();
	}

	sConfig.Channel 		= ADC_CHANNEL_12;
	sConfig.Rank 			= 4;
	if (HAL_ADC_ConfigChannel(&IRLF_ADC.ADC_Handle, &sConfig) != HAL_OK)
	{
		_Error_Handler();
	}

	sConfig.Channel 		= ADC_CHANNEL_13;
	sConfig.Rank 			= 5;
	if (HAL_ADC_ConfigChannel(&IRLF_ADC.ADC_Handle, &sConfig) != HAL_OK)
	{
		_Error_Handler();
	}

	sConfig.Channel 		= ADC_CHANNEL_14;
	sConfig.Rank 			= 6;
	if (HAL_ADC_ConfigChannel(&IRLF_ADC.ADC_Handle, &sConfig) != HAL_OK)
	{
		_Error_Handler();
	}

	sConfig.Channel 		= ADC_CHANNEL_15;
	sConfig.Rank 			= 7;
	if (HAL_ADC_ConfigChannel(&IRLF_ADC.ADC_Handle, &sConfig) != HAL_OK)
	{
		_Error_Handler();
	}

	/**	ADC1 GPIO Configuration
	  * PB0     ------> ADC1_8
	  * PB1 	------> ADC1_9
	  *	PC1     ------> ADC1_11
	  *	PC2 	------> ADC1_12
	  *	PC3 	------> ADC1_13
	  *	PC4 	------> ADC1_14
	  *	PC5 	------> ADC1_15
	  */
	GPIO_InitTypeDef 		GPIO_Handle;

	GPIO_Handle.Pin 		= GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
							  GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_Handle.Mode 		= GPIO_MODE_ANALOG;
	GPIO_Handle.Pull 		= GPIO_NOPULL;
	GPIO_Handle.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;

	HAL_GPIO_Init(GPIOC, &GPIO_Handle);

	GPIO_Handle.Pin 		= GPIO_PIN_0 | GPIO_PIN_1;

	HAL_GPIO_Init(GPIOB, &GPIO_Handle);

	/* Allocating memory slots for ADC channel data*/
#if !defined(USE_FREERTOS)
	IRLF_ADC.ADC_channel_value 		= 	malloc(sizeof(IRLF_ADC_ValueBaseType)*configNUM_OF_CHANNELs);
#else
	IRLF_ADC.ADC_channel_value 		=	pvPortMalloc(sizeof(IRLF_ADC_ValueBaseType)*configNUM_OF_CHANNELs);
#endif
}

void IRLF_DMA_INIT(void)
{
	IRLF_ADC.DMA_Handle.Instance 				= DMA2_Stream0;
	IRLF_ADC.DMA_Handle.Init.Channel 			= DMA_CHANNEL_0;
	IRLF_ADC.DMA_Handle.Init.Direction 			= DMA_PERIPH_TO_MEMORY;
	IRLF_ADC.DMA_Handle.Init.PeriphInc 			= DMA_PINC_DISABLE;
	IRLF_ADC.DMA_Handle.Init.MemInc 			= DMA_MINC_ENABLE;
	IRLF_ADC.DMA_Handle.Init.PeriphDataAlignment= DMA_PDATAALIGN_HALFWORD;
	IRLF_ADC.DMA_Handle.Init.MemDataAlignment 	= DMA_MDATAALIGN_HALFWORD;
	IRLF_ADC.DMA_Handle.Init.Mode 				= DMA_CIRCULAR;
	IRLF_ADC.DMA_Handle.Init.Priority 			= DMA_PRIORITY_HIGH;
	IRLF_ADC.DMA_Handle.Init.FIFOMode 			= DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&IRLF_ADC.DMA_Handle) != HAL_OK)
	{
		_Error_Handler();
	}

	__HAL_LINKDMA(&IRLF_ADC.ADC_Handle,DMA_Handle,IRLF_ADC.DMA_Handle);
}
