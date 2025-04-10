#include "includes.h"

extern void FmDisplayInputFreq(void)
{
    U8 i,j,poin,xpos=36;
    String buf[9] = {0};
    
    if(g_inputbuf.maxLen == 3)
    {
        xpos += 4;
        memset(buf,'-',6);
        buf[2] = '.';
        poin = 2;
    }
    else
    {
        memset(buf,'-',7);
        buf[3] = '.';
        poin = 3;
    }

    j = 0;
    for(i=0;i<g_inputbuf.len;i++)
    {
        if(i == poin)
        {
           j++;
        }
        buf[j++] = g_inputbuf.buf[i];
    }
    LCD_ClearArea(35,0,128,16);
    LCD_DisplayText(35,xpos,(U8 *)buf,FONTSIZE_16x16,LCD_DIS_NORMAL);
    LCD_UpdateWorkAre();
}

extern void FmDisplayInputChNum(void)
{
    U8 i;
    String buf[3] = {0};
    
    memset(buf,'-',2);
    for(i=0;i<g_inputbuf.len;i++)
    {
        buf[i] = g_inputbuf.buf[i];
    }

    LCD_DisplayNumber(42,112,(U8 *)buf,2);

    LCD_UpdateWorkAre();
}

extern void FmDisplaySeek(void)
{
    LCD_ClearWorkArea();
    LCD_DisplayText(17,56,"FM",FONTSIZE_16x16, LCD_DIS_NORMAL);
    LCD_DisplayText(35,36,"SEEK...",FONTSIZE_16x16,LCD_DIS_NORMAL);
    LCD_UpdateWorkAre();
}

extern void FmDisplayChNum(void)
{
    U8 buf[3] = {0};
    
    if(g_FMInform.fmChVfo == CHAN_MODE)
    {
    
        sprintf((String *)buf,"%02d",g_FMInform.fmChNum+1);
    }
    else
    {
        sprintf((String *)buf,"  ");
    }

    LCD_DisplayNumber(42,112,(U8 *)buf,2);
}


extern void FmDisplayFreq(void)
{
    String disBuf[10] = {0};
    U8 len = 0,xpos=36;
    
    len = sprintf(disBuf,"%d.%d00",fmInfo.freq/10,fmInfo.freq%10);
    if(len == 6)
    {
        xpos += 4; 
    }
    LCD_ClearArea(35,0,128,16);
    LCD_DisplayText(35,xpos,(U8 *)disBuf,FONTSIZE_16x16,LCD_DIS_NORMAL);

    //显示信道号
    FmDisplayChNum();

    LCD_UpdateWorkAre();
}

extern void FmDisplayHome(void)
{
    LCD_ClearWorkArea();

    LCD_DisplayText(17,56,"FM",FONTSIZE_16x16, LCD_DIS_NORMAL);
    FmDisplayFreq();
}

