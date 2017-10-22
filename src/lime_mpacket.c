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
#include <cmsis_os.h>
#include <stdlib.h>
#include <initsys.h>
#include <lime_mpacket.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
LIME_MPACKET_Signals_Type 	limeSignalList =
{
	1,
	0
};

/* Exported functions --------------------------------------------------------*/
/**
  * @brief
  * @param 	Pointer to LIME_MPACKET_Type
  * @retval LIME_Status
  *
  */
LIME_Status LIME_MPACKET_Init(LIME_MPACKET_Type * limePacket)
{
	uint8_t status = LIME_OK;

	limePacket->MesID 		= malloc(sizeof(limePacket->MesID));
	limePacket->MesContent  = malloc(sizeof(uint8_t)*configLIME_MPACKET_MAXSIZE);
	limePacket->MesLen 		= 0;//(uint8_t) (configLIME_MPACKET_MAXSIZE + configLIME_MPACKET_HEADERS);
	limePacket->MesComposed = malloc(sizeof(uint8_t)*(configLIME_MPACKET_MAXSIZE + configLIME_MPACKET_HEADERS));
	limePacket->MesReceived = malloc(sizeof(uint8_t)*configLIME_MPACKET_RECEIVE_SIZE);

	if ((limePacket->MesID == NULL) || (limePacket->MesContent == NULL) || (limePacket->MesComposed == NULL))
	{
		status 				= LIME_ERROR;
	}

	return status;
}

/**
  * @brief
  * @param 	Pointer to LIME_MPACKET_Type
  * @retval LIME_Status
  *
  */
LIME_Status LIME_MPACKET_Publish(LIME_MPACKET_Type *limePacket, initHandle_struct *initHandles)
{
	uint8_t status = LIME_OK;

	/* Send message packet */
	HAL_UART_Transmit_IT(&initHandles->USART2_Handle, limePacket->MesContent, limePacket->MesLen);
	osSignalWait(limeSignalList.sFINISHED_PUBLISHING, configMAX_PUBLISH_WAIT);
	return status;
}

/**
  * @brief
  * @param 	Pointer to LIME_MPACKET_Type
  * @retval LIME_Status
  *
  */
LIME_Status LIME_MPACKET_Subcribe(LIME_MPACKET_Type *limePacket, initHandle_struct *initHandles)
{
	uint8_t status = LIME_OK;

	return status;
}

/**
  * @brief
  * @param 	Pointer to LIME_MPACKET_Type
  * @retval LIME_Status
  *
  */
LIME_Status LIME_MPACKET_Packing(LIME_MPACKET_Type *limePacket, initHandle_struct *initHandles)
{
	uint8_t status = LIME_OK;

	/* Add header */

	/* Packet body */

	/* Footer */

	return status;
}
















