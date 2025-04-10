
#include "kd32f328_it.h"
#include "includes.h"

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  NVIC_SystemReset();
}


void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    UserVectors[0].intHandle();
}

void USART1_IRQHandler(void)
{
    UserVectors[1].intHandle();
}

void USART2_IRQHandler(void)
{
    UserVectors[2].intHandle();
}

void DMA1_Channel4_5_IRQHandler(void)
{
    UserVectors[3].intHandle(); 
}

