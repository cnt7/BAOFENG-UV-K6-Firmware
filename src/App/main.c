#include "includes.h"
#include "stdio.h"

void BeepPowerOn(void)
{
    if(g_radioInform.OpFlag1.Bit.b2 == 1)
    {
        BeepOut(BEEP_FMSW2);
    }
    else if(g_radioInform.OpFlag1.Bit.b2 == 2)
    {
        Audio_PlayVoiceLock(vo_Welcome);
    }
    else
    {
        DelayMs(300);
    }
}

int main(void)
{   
    Board_Init();    
    RadioConfig_Init();
    UI_DisplayPowerOn();
    Rfic_Init();
    ChannelCheckActiveAll();
    BeepPowerOn();
    BatteryInitLevel();

    App_CheckPowerOnPassword();

    //CheckHiddenParaSet(); 

    if(GetKeyCode() == KEYID_8)
    {
        DisplaySoftVersion();
    }
    RadioVfoInfo_Init();

    ResetTimeKeyLockAndPowerSave();
    ResetInputBuf();

    //初始化写频模式
    ProgromInit();
    LCD_BackLightSetOn();

    g_rfState = RF_RX;
    g_rfTxState = TX_READY;
    g_rfRxState = RX_READY;
    g_scanInfo.state = SCAN_IDLE;
    g_sysRunPara.sysRunMode = MODE_MAIN;
    g_keyScan.keyEvent = KEYID_NONE;
    
    while(1)
    {
        //10ms运行一次
        if(g_10msFlag)
        {
            App_10msTask();
        }
        
        if(g_50msFlag)
        {
            App_50msTask();
        }
        
        //100ms运行一次
        if(g_100msFlag)
        {
            App_100msTask();
        }

        //500ms运行一次
        if(g_500msFlag)
        {
            App_500msTask();
        }
        
        AppRunTask();
        AlarmTask();
    }
}

