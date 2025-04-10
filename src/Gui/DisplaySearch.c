#include "includes.h"

extern void SearchFreqModeDisplayDCSData(U8 ctsDcsType, U32 dat, U8 isStandard)
{
    String disBuf[12] = {0};
    memset( disBuf, ' ', 11 );
    LCD_DisplayText(49,37,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
    if( ctsDcsType > SUBAUDIO_CTS )
    {
        if(isStandard == 1)
        {
            sprintf( disBuf, "   D%03oN ", dat );
		    LCD_DisplayText(49,37,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
        }
        else
        {
            sprintf( disBuf, "   %6X", dat );
		    LCD_DisplayText(49,37,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
        }
	}
    else if(ctsDcsType == SUBAUDIO_CTS)
    {
        sprintf( disBuf, "   %3d.%0.1d", dat/10, dat%10 );
		LCD_DisplayText(49,37,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
    }
	else
	{
		sprintf( disBuf, "   NONE "  );
		LCD_DisplayText(49,37,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
	}
	LCD_UpdateWorkAre();
}

extern void SearchFreqModeDisplayStepMsg( ENUM_SEARCHFREQ_STEP step )
{
    String disBuf[13]={" SEEK...  "};

    if( searchFreqImofs.band == FREQ_BAND_UHF )
    {
        LCD_DisplayText(17,108,"UHF",FONTSIZE_12x12,LCD_DIS_NORMAL);
    }
    else if(searchFreqImofs.band == FREQ_BAND_200M)
    {
        LCD_DisplayText(17,108,"200",FONTSIZE_12x12,LCD_DIS_NORMAL);
    }
    
    else if(searchFreqImofs.band == FREQ_BAND_350M)
    {
        LCD_DisplayText(17,108,"350",FONTSIZE_12x12,LCD_DIS_NORMAL);
    }
    else
    {
        LCD_DisplayText(17,108,"VHF",FONTSIZE_12x12,LCD_DIS_NORMAL);
    }

	if( step == STEP_SEEK_FREQ )
	{
        LCD_DisplayText(33,37,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
	
	    memset( disBuf, ' ', 9 );
        LCD_DisplayText(49,37,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
	}
	else 
	{
        LCD_DisplayText(49,37,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
	
        sprintf(disBuf, "%3d.%0.5d", searchFreqImofs.freq/100000, searchFreqImofs.freq%100000 );
        LCD_DisplayText(33,37,(U8 *)&disBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
	}

	LCD_UpdateWorkAre();
}


extern void SearchFreqDisplayHome(void )
{
    U8 posx;
    
    LCD_ClearWorkArea();

    if(g_radioInform.language == LANG_CN)
    {
        posx = 52;
    }
    else
    {
        posx = 46;
    }
    //显示扫频标志
    LCD_DisplayText(17,posx,(U8 *)((g_radioInform.language == LANG_CN)?"扫频":"SEARCH"),FONTSIZE_12x12, LCD_DIS_NORMAL);	
    //显示频率
    SearchFreqModeDisplayStepMsg(STEP_SEEK_FREQ);
}

