#include "includes.h"

#define MAX_WEATHER_CHANNEL       10
STR_FREQINFO Weatherfreq;


const U32 TAB_WEATHER[] = {
        16255000, 16240000, 16247500, 16242500, 16245000, 16250000, 16252500, 16165000, 16177500, 16327500
};

extern U32 Weather_GetChFreq(U8 num)
{
    return TAB_WEATHER[num];
}

extern void WeatherInit(U8 num)
{
    Weatherfreq.frequency = TAB_WEATHER[num];
    Weatherfreq.dcsCtsType = SUBAUDIO_NONE;

    g_CurrentVfo->rx = &Weatherfreq;
    Rfic_ConfigRxMode();
}


void WeatherChannelUp(U8 isScan)
{
    g_radioInform.weatherNum = (g_radioInform.weatherNum+1)%MAX_WEATHER_CHANNEL;
    g_rfRxState = RX_READY;   
    
    WeatherDisplayFreq();

    if(isScan == 0)
    {
        BeepOut(BEEP_FMUP);
    }
}

void WeatherChannelDown(U8 isScan)
{
    if(g_radioInform.weatherNum > 0)
    {
        g_radioInform.weatherNum--;
    }
    else
    {
        g_radioInform.weatherNum = MAX_WEATHER_CHANNEL - 1;
    }
    g_rfRxState = RX_READY; 

    WeatherDisplayFreq();

    if(isScan == 0)
    {
        BeepOut(BEEP_FMUP);
    }
}

extern void EnterWeatherMode(void)
{
    //进入天气预报时关闭双守
    DualStandbyWorkOFF();

    //如果在菜单模式，则退出菜单
    if(g_sysRunPara.sysRunMode == MODE_MENU)
    {
        Menu_ExitMode();
    }
    ResetInputBuf();

    if(g_radioInform.weatherNum >= MAX_WEATHER_CHANNEL)
    {
        g_radioInform.weatherNum = 0;
    }

    g_sysRunPara.sysRunMode = MODE_WEATHER;
    g_rfRxState = RX_READY;

    WeatherDisplayHome();
    
    VoiceBroadcastWithBeepLock(vo_ON,BEEP_FASTSW);
}

extern void ExitWeatherMode(void)
{
    if(g_sysRunPara.sysRunMode != MODE_WEATHER)
    {
        return;
    }

    g_CurrentVfo->rx = &g_CurrentVfo->freqRx;
    g_sysRunPara.sysRunMode = MODE_MAIN;
    ResetInputBuf();
    DualStandbyWorkOFF();
    
    //切换为显示主界面
    DisplayRadioHome();

    VoiceBroadcastWithBeepLock(vo_OFF,BEEP_EXITMENU);
}

extern void WeatherScanStart(void)
{
    DualStandbyWorkOFF();

    ResetInputBuf();
    g_scanInfo.state = SCANNING;
    g_scanInfo.scanTime = 4;
    g_scanInfo.direction = SCAN_UP;

    VoiceBroadcastWithBeepLock(vo_scanbegin,BEEP_FMSW1);  
}

extern void WeatherScanOff(void)
{
    g_scanInfo.state = SCAN_IDLE;
    g_scanInfo.scanTime = 0;
    g_rfRxState = RX_READY;
    WeatherDisplayFreq();
    VoiceBroadcastWithBeepLock(vo_scanstop,BEEP_FASTSW);
}


extern void WeatherScanNextChannel(void)
{
    if(g_scanInfo.scanTime)
    {
        return;
    }

    if(g_scanInfo.direction == SCAN_UP)
    {
        WeatherChannelUp(1);  
    }
    else
    {
        WeatherChannelDown(1);   
    }
    //ResetSqlLevel();
    ResetTimeKeyLockAndPowerSave();
    g_scanInfo.state = SCANNING;
    g_scanInfo.scanTime = 2;
}

extern void WeatherScanTask(void)
{
    static U8 sqCnt = 0;

    if(g_sysRunPara.sysRunMode != MODE_WEATHER)
    {//不在扫描模式直接返回
        return;
    }
    
    switch(g_scanInfo.state)
    {
        case SCANNING:
            if(Rfic_GetSQLinkState() == TRUE)
            {
                if(sqCnt < 3)
                {
                    sqCnt++;
                    return;
                }
            
                switch(g_radioInform.scanMode)
                {
                    case TIME_MODE:
                        g_scanInfo.state = WAIT_TIME;
                        g_scanInfo.scanTime = SCAN_LOST_TIME;
                        break;
                    case FREQ_MODE:
                        g_scanInfo.state = WAIT_LOST;
                        g_scanInfo.scanTime = SCAN_LOST_TIME;
                        break;
                    case FINE_MODE:
                        g_scanInfo.state = SCAN_STOP;
                        g_scanInfo.scanTime = SCAN_FIND_TIME;
                        break;
                    default:
                        break;
                }
                return;
            }
            //扫描下一个信道
            WeatherScanNextChannel();
            sqCnt = 0;
            
            break;
        case WAIT_TIME:
            if(g_radioInform.scanMode == FREQ_MODE)
            {
                if(Rfic_GetSQLinkState() == TRUE)
                {
                    g_scanInfo.scanTime = 50;
                }
            }
            //扫描下一个信道
            WeatherScanNextChannel();
            sqCnt = 0;
            
            break;    
        case WAIT_LOST:
            if(Rfic_GetSQLinkState() == FALSE)
            {
                if( g_scanInfo.scanTime == 0 )
                {
                    g_scanInfo.state = WAIT_RECALL;
                }
            }
            else
            {
                g_scanInfo.scanTime = SCAN_LOST_TIME;
            }
            break;
        case WAIT_RECALL:
            //扫描下一个信道
            WeatherScanNextChannel();
            sqCnt = 0;
            
            break;
        case SCAN_STOP:
            g_scanInfo.state = SCAN_IDLE;
            g_scanInfo.scanTime = 0;
            g_rfRxState = RX_READY;
            sqCnt = 0;
            break; 
        default:
            break;
    }
}

extern void KeyProcess_Weather(U8 keyEvent)
{
    switch(keyEvent)
    {
        case KEYID_UP:
            if(g_scanInfo.state == SCAN_IDLE)
            {
                WeatherChannelUp(0);
            }
            else
            {
                g_scanInfo.direction = SCAN_UP;
                BeepOut(BEEP_FMUP);
                g_scanInfo.scanTime = 0;
                WeatherScanNextChannel();
                DisplayResetRxFlag();
            }
            break;
        case KEYID_DOWN:
            if(g_scanInfo.state == SCAN_IDLE)
            {
                WeatherChannelDown(0);
            }
            else
            {
                g_scanInfo.direction = SCAN_DOWN;
                BeepOut(BEEP_FMUP);
                g_scanInfo.scanTime = 0;
                WeatherScanNextChannel();
                DisplayResetRxFlag();
            }
            break;
        case KEYID_WX:
        case KEYID_EXIT:
            ExitWeatherMode();
            break;
        case KEYID_SCAN:
           if(g_scanInfo.state == SCAN_IDLE)
           {
               WeatherScanStart();
           }
           else
           {
               WeatherScanOff();
           }
            break;      
        case KEYID_LOCK:
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


