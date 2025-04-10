#include "includes.h"

extern void App_10msTask(void)
{
    g_10msFlag = FALSE;

    RF_Task();
    KEY_ScanTask();
    PTT_ScanTask();
    FmTaskFunc();
}

extern void App_50msTask(void)
{
    g_50msFlag = FALSE;

    ScanTask();
    WeatherScanTask();
    DualStandbyTask();
    StopWatchDisplayTime();
    LCD_CheckBackLight();
}

extern void App_100msTask(void)
{
    g_100msFlag = FALSE;
    SearchFreqTask();
    TaskRemoteScanQT();
    CheckExitMenu();
    CheckPowerOff();
    CheckAutoKeyLockTask();
    LightFlashTask();
    VoxCheckTask();
}

extern void App_500msTask(void)
{
    g_500msFlag = FALSE;
    BatteryCheckTask();
    CalculateSqlLevel();
    CheckSjTimeout();
}

extern void AppRunTask(void)
{
    U8 keyEvent;
    
    switch(g_rfState)
    {
        case RF_TX:
            Radio_TxKeyTone(g_keyScan.keyEvent,g_keyScan.keyPara);
            break;
        case RF_RX:
        default:
           if(g_keyScan.keyEvent != KEYID_NONE)
           {
               keyEvent = Key_GetRealEvent();

               if(alarmDat.alarmStates)
               {
                   if(keyEvent == KEYID_SIDEKEY1 || keyEvent == KEYID_SIDEKEY2 || keyEvent == KEYID_SIDEKEYL1)
                   {
                       keyEvent = Sidekey_GetRemapEvent(keyEvent);

                       if(keyEvent == KEYID_SOS)
                       {
                           AlarmFuncSwitch(OFF);
                       }
                   }
                   break;
               }
               
               switch(g_sysRunPara.sysRunMode)
               {
                   case MODE_MENU:
                       KeyProcess_Menu(keyEvent);
                       break;
                   case MODE_FM:
                       KeyProcess_Fm(keyEvent);
                       break;  
                   case MODE_MONI:
                       KeyProcess_Moni(keyEvent);
                       break;   
                   case MODE_SCAN:
                       KeyProcess_Scan(keyEvent);
                       break; 
                   case MODE_SEARCH:
                       KeyProcess_Search(keyEvent);
                       break;  
                   case MODE_SCAN_QT:
                       KeyProcess_ScanQt(keyEvent);
                       break;     
                   case MODE_WEATHER:
                       KeyProcess_Weather(keyEvent);
                       break;  
                   case MODE_STOPWATCH:
                       KeyProcess_StopWatch(keyEvent);
                       break;    
                   case MODE_DTMF:
                       KeyProcess_DtmfInput(keyEvent);
                       break;
                   case MODE_MAIN:
                   default:
                       KeyProcess_Main(keyEvent);
                       break;
               }
           }
           Audio_PlayTask();
           DtmfReceiveTask();
           break;
    }

    if(g_sysRunPara.sysRunMode == MODE_PROGRAM)
    {
        EnterProgromMode();
    }

    if(g_sysRunPara.sysRunMode == MODE_FLASH_PROGRAM)
    {
        EnterFlashProgromMode();
    }
}


