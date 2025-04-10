
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KD32F3XX_it_H
#define __KD32F3XX_it_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "kd32f3xx.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);


#ifdef __cplusplus
}
#endif

#endif 

