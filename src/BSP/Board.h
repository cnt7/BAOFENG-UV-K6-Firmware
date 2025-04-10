#ifndef __BOARD_H
    #define __BOARD_H

/*************************************************************************************/
typedef struct 
{
    void (*intHandle)(void);
}STR_INTFUN;  

extern __IO STR_INTFUN UserVectors[10];
/*************************************************************************************/
extern void GpioModeSwitch(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint8_t mode);
extern void uartSendChar( uint8_t ch );
extern uint8_t UserADC_GetValOfBatt(void);
extern uint8_t UserADC_GetValOfVox(void);
extern void Board_Init(void);


#endif
