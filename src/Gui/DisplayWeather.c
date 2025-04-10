#include "includes.h"

extern void WeatherDisplayFreq(void)
{
    String disBuf[11]={0};
    
    sprintf(disBuf,"WX-%02d",g_radioInform.weatherNum + 1);
    LCD_DisplayText(30,85,(U8 *)disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
    
    sprintf((String *)(disBuf),"%d.%05d",TAB_WEATHER[g_radioInform.weatherNum]/100000,TAB_WEATHER[g_radioInform.weatherNum]%100000);
    LCD_DisplayText(45,24,(U8 *)disBuf,FONTSIZE_16x16,LCD_DIS_NORMAL);
    
    LCD_UpdateWorkAre();
}

extern void WeatherDisplayHome(void)
{    
    LCD_ClearWorkArea();
    
    //显示天气预报
    LCD_DisplayPicture(14,47,ICON_WORK_WX_SIZEX,ICON_WORK_WX_SIZEY,iconWorkWeather,LCD_DIS_NORMAL);
    
    //显示收音机频率   
    WeatherDisplayFreq();
}


