#ifndef __BOARDFUN_H
    #define __BOARDFUN_H

typedef enum 
{
    LED_OFF=0,LED_ON,LED_FLASH
}ENUM_LED;

typedef enum 
{
    PWR_OFF=0, PWR_RXON,PWR_TXON
}ENUM_RFPWR;   

extern void CheckPowerOff(void);
extern void LcdBackLightSwitch(ENUM_LED flag);
extern void LCD_CheckBackLight(void);
extern void LedTxSwitch(ENUM_LED flag);
extern void LedRxSwitch(ENUM_LED flag);
extern void LightSwitch(U8 flag);
extern void LightFlashTask(void);
extern void LCD_BackLightSetOn(void);
extern void RF_PowerSet(U8 band,ENUM_RFPWR flag);
extern void SpeakerSwitch(ENUM_ONOFF flag);

#endif
