#include "includes.h"

extern void QTScanDisplay(U8 ctsDcsType, U32 dat, U8 isStandard)
{
    String disBuf[15] = {0};
    memset( disBuf, ' ', 14 );
    LCD_DisplayText(49,28,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
    if( ctsDcsType > SUBAUDIO_CTS )
    {
        if(isStandard == 1)
        {
            sprintf( disBuf, "   D%03oN ", dat );
		    LCD_DisplayText(49,28,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
        }
        else
        {
            sprintf( disBuf, "   %6X", dat );
		    LCD_DisplayText(49,28,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
        }
	}
    else if(ctsDcsType == SUBAUDIO_CTS)
    {
        sprintf( disBuf, "   %3d.%0.1d", dat/10, dat%10 );
		LCD_DisplayText(49,28,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
    }
	else
	{
		sprintf( disBuf, "   NONE "  );
		LCD_DisplayText(49,28,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
	}

	LCD_UpdateWorkAre();
}

extern void QTScanDisplayStatus(U8 status)
{
    String disBuf[13] = {0};
    memset( disBuf, ' ', 13 );
    sprintf( disBuf, "   SCANQT   "  );
    LCD_DisplayText(17,28,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);

    if(status == 0)
    {
        sprintf( disBuf, "    STOP    "  );
    }
    else
    {
        sprintf( disBuf, "  RUNNING   "  );
    }
    LCD_DisplayText(49,28,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);

    LCD_UpdateWorkAre();
}

extern void QTScanDisplayHome(void)
{
    String disBuf[13] = {0};
    
    LCD_ClearWorkArea();

    sprintf(disBuf, "%3d.%0.5d", g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency/100000, g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency%100000 );
    LCD_DisplayText(30,28,(U8 *)&disBuf,FONTSIZE_16x16,LCD_DIS_NORMAL);
    
    QTScanDisplayStatus(0);
}

