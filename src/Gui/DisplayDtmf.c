#include "includes.h"


extern void DisplayDtmfEditHome(void)
{
    if(g_ChannelVfoInfo.switchAB || (g_radioInform.dualRxFlag == 0))
    {
        LCD_ClearArea(12, 0, 128,17);
        
        LCD_DisplayPicture(24,1,ICON_RIGHT_SIZEX,ICON_RIGHT_SIZEY,iconRight,LCD_DIS_NORMAL);
        LCD_DisplayText(20,9,"---------------",FONTSIZE_16x16,LCD_DIS_NORMAL);
    }
    else
    {        
        LCD_ClearArea(40, 0, 128,21);
        
        LCD_DisplayPicture(52,1,ICON_RIGHT_SIZEX,ICON_RIGHT_SIZEY,iconRight,LCD_DIS_NORMAL);
        LCD_DisplayText(48,9,"---------------",FONTSIZE_16x16,LCD_DIS_NORMAL);
    }
    LCD_UpdateWorkAre();
}

extern void DisplayInputDtmf(void)
{
    U8 buf[16];

    memset(buf,'-',15);

    memcpy(buf,g_inputbuf.buf,g_inputbuf.len);

    buf[15] = 0;

    if(g_ChannelVfoInfo.switchAB || (g_radioInform.dualRxFlag == 0))
    {
        LCD_DisplayText(20,9,buf,FONTSIZE_16x16,LCD_DIS_NORMAL);
    }
    else
    {
        LCD_DisplayText(48,9,buf,FONTSIZE_16x16,LCD_DIS_NORMAL);
    }
    LCD_UpdateWorkAre();
}

