/**
  ******************************************************************************
  * @file    litve_adc.h
  * @author  Azure
  * @version V0.1.0
  * @date    2nd-November-2017
  * @brief 	 Configuration file for Line-tracking vehicle ADC data acquisition.
  *
  *
  ******************************************************************************
*/

#ifndef INCLUDE_LITVE_ADC_H_
#define INCLUDE_LITVE_ADC_H_

/* Private typedef -----------------------------------------------------------*/
typedef uint16_t IRLF_ADC_ValueBaseType;

/* Private define ------------------------------------------------------------*/
#define configUSE_LITVE_LIB
#define configNUM_OF_CHANNELs 			(uint32_t) 7

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/** @defgroup 	IRLF_ADC_Handle structure
  * @{
  */
typedef struct
{
	ADC_HandleTypeDef					ADC_Handle;
	DMA_HandleTypeDef 					DMA_Handle;
	IRLF_ADC_ValueBaseType				*ADC_channel_value;
	void 								(*ADC_INIT)(void);
	void 								(*DMA_INIT)(void);
} IRLF_ADC_HandleTypeDef;

/* Exported variables ---------------------------------------------------------*/
extern IRLF_ADC_HandleTypeDef 			IRLF_ADC;

/* Exported function prototypes ----------------------------------------------*/
void 					IRLF_ADC_PERIPH_INIT(void);
void 					IRLF_DMA_INIT(void);
#endif /* INCLUDE_LITVE_ADC_H_ */
