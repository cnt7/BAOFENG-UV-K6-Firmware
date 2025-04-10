#include "includes.h"

STU_STOPWATCH g_stopWatch; 

extern void EnterStopWatchMode(void)
{
    SpeakerSwitch(OFF);
	Rfic_SetAfout(OFF);
    LedRxSwitch(LED_OFF);

    g_stopWatch.flagStart = 0;
    g_stopWatch.millisecond = 0;
    g_stopWatch.minute = 0;
    g_stopWatch.second = 0;
	
    g_sysRunPara.sysRunMode = MODE_STOPWATCH;

    StopWatchDisplayHome();
}

extern void ExitStopWatchMode(void)
{
    if(g_sysRunPara.sysRunMode != MODE_STOPWATCH)
    {
        return;
    }
    
    g_sysRunPara.sysRunMode = MODE_MAIN;    
   
    //切换为显示主界面
    DisplayHomePage();
}

extern void StopWatchTick(void)
{
    if(g_stopWatch.flagStart == 0)
    {
        return;
    }

    g_stopWatch.millisecond++;
    if(g_stopWatch.millisecond >= 100)
    {
        g_stopWatch.millisecond = 0;
        g_stopWatch.second++;
        if(g_stopWatch.second >= 60)
        {
            g_stopWatch.second = 0;
            g_stopWatch.minute++;
            if(g_stopWatch.minute >= 60)
            {
                g_stopWatch.minute = 0;
            }
        }
    }
}

extern void StopWatch_Start(void)
{
    if(g_stopWatch.flagStart == 0)
    {
        g_stopWatch.flagStart = 0;
        g_stopWatch.millisecond = 0;
        g_stopWatch.minute = 0;
        g_stopWatch.second = 0;
        g_stopWatch.flagStart = 1;
    }
    else
    {
        g_stopWatch.flagStart = 0;
    }
}

extern void StopWatchDisplayHome(void)
{
    String disBuf[16] = {0};

    LCD_ClearWorkArea();

    sprintf(disBuf,"%s",(U8 *)((g_radioInform.language== LANG_CN)?"   秒 表    ":" STOP WATCH "));
    LCD_DisplayText(17, 20,(U8 *)disBuf, FONTSIZE_16x16,LCD_DIS_NORMAL);

    StopWatchDisplayTime();
    
}
extern void StopWatchDisplayTime(void)
{
    String disBuf[16] = {0};

    if(g_sysRunPara.sysRunMode != MODE_STOPWATCH)
    {
        return;
    }

    sprintf(disBuf," %.2d--%.2d--%.2d   ", g_stopWatch.minute, g_stopWatch.second, g_stopWatch.millisecond );
    LCD_DisplayText(35,16,(U8 *)disBuf, FONTSIZE_16x16,LCD_DIS_NORMAL);
    LCD_UpdateWorkAre();	
}

extern void KeyProcess_StopWatch(U8 keyEvent)
{
    switch(keyEvent)
    {
        case KEYID_MENU:
            StopWatch_Start();
            break;
        case KEYID_EXIT:
            ExitStopWatchMode();
            BeepOut(BEEP_EXITMENU);
            break;
            
        case KEYID_SIDEKEY1:
        case KEYID_SIDEKEY2:
        case KEYID_SIDEKEYL1:
            keyEvent = Sidekey_GetRemapEvent(keyEvent);
            if(keyEvent != KEYID_LIGHT)
            {
                keyEvent = KEYID_NONE;
            }
            SideKey_Process(keyEvent);
            break;
            
        default:
            BeepOut(BEEP_NULL);
            break;
    }
}


