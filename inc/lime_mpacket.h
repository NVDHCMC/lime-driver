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

#ifndef LIME_MPACKET_C_
#define LIME_MPACKET_C_


/* Private typedef -----------------------------------------------------------*/
typedef uint32_t 		LIME_MID_Type;
typedef uint32_t 		LIME_RTOS_Signal_t;

/* Private define ------------------------------------------------------------*/
#define configLIME_MPACKET_MAXSIZE 		128 			// In number of bytes
#define configLIME_MPACKET_HEADERS 		10 				// Number of bytes used for header and stuff
#define configLIME_MPACKET_RECEIVE_SIZE 20 				// Number of bytes expected to receive
#define configMAX_PUBLISH_WAIT 			500 				//

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/** @defgroup MESSAGE_PACKET_Signal_list
  * @{
  */
typedef struct
{
	LIME_RTOS_Signal_t 	sFINISHED_PUBLISHING;
	LIME_RTOS_Signal_t 	sDEBUG;
} LIME_MPACKET_Signals_Type;
/**
  * @}
  */

/** @defgroup MESSAGE_PACKET_Structure
  * @{
  */
typedef struct
{
	LIME_MID_Type 		*MesID;
	uint8_t 			MesLen;
	uint8_t 			*MesContent;
	uint8_t 			*MesComposed;
	uint8_t 			*CHKSUM;
	uint8_t 			*MesReceived;
} LIME_MPACKET_Type;
/**
  * @}
  */
/** @defgroup LIME_Publisher Structure
  * @{
  */

/* Exported functions ---------------------------------------------------------*/
LIME_Status LIME_MPACKET_Init(LIME_MPACKET_Type * limePacket);
LIME_Status LIME_MPACKET_Publish(LIME_MPACKET_Type *limePacket, initHandle_struct *initHandles);
LIME_Status LIME_MPACKET_Subcribe(LIME_MPACKET_Type *limePacket, initHandle_struct *initHandles);

extern LIME_MPACKET_Signals_Type 	limeSignalList;
#endif /* LIME_MPACKET_C_ */
