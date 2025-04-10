
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KD32F328XB_CONF_H
#define __KD32F328XB_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Comment the line below to disable peripheral header file inclusion */
#include "KD32f328_adc.h"
//#include "KD32f328_crc.h"
#include "KD32f328_dma.h"
//#include "KD32f328_exti.h"
#include "KD32f328_flash.h"
#include "KD32f328_gpio.h"
//#include "KD32f328_iwdg.h"
//#include "KD32f328_pwr.h"
#include "KD32f328_rcc.h"
#include "KD32f328_spi.h"
#include "KD32f328_tim.h"
#include "KD32f328_usart.h"
//#include "KD32f328_wwdg.h"
#include "KD32f328_misc.h"  

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif

