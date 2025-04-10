#include "includes.h"

STR_SCAN  g_scanInfo; 

extern U8 CheckInputScanRange(U16 freqL,U16 freqH)
{
    if(freqL >= freqH)
    {
        return FERROR;
    }
    
    if((freqL >= 1080) && (freqL < 1360))
    {
        if(freqH <= 1360)
        {
            return OK;
        }  
    }
    
    if((freqL >= bandRang.bandFreq.vhf2L) && (freqL < bandRang.bandFreq.vhf2H))
    { 
        if(freqH <= bandRang.bandFreq.vhf2H)
        {
            return OK;
        }       
    }
    
    if((freqL >= bandRang.bandFreq.B350ML) && (freqL < bandRang.bandFreq.B350MH))
    { 
        if(freqH <= bandRang.bandFreq.B350MH)
        {
            return OK;
        }  
    }
    
    if((freqL >= bandRang.bandFreq.vhfL) && (freqL < bandRang.bandFreq.vhfH))
    { 
        if(freqH <= bandRang.bandFreq.vhfH)
        {
            return OK;
        }  
    }

    if((freqL >= bandRang.bandFreq.uhfL) && (freqL < bandRang.bandFreq.uhfH))
    { 
        if(freqH <= bandRang.bandFreq.uhfH)
        {
            return OK;
        }  
    }
    return FERROR;
}

void ScanNextChannel(void)
{
    if(g_scanInfo.scanTime)
    {
        return;
    }

    if(g_scanInfo.direction == SCAN_UP)
    {
        if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
        {
            ChannelUp(1,0);
            DisplayChannelNum(g_ChannelVfoInfo.currentChannelNum,g_ChannelVfoInfo.switchAB);
        }
        else
        {
            VfoFreqUp(1);
        }
    }
    else
    {
        if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
        {
            ChannelDown(1,0);
            DisplayChannelNum(g_ChannelVfoInfo.currentChannelNum,g_ChannelVfoInfo.switchAB);
        }
        else
        {
            VfoFreqDown(1);
        }
    }
    //ResetSqlLevel();
    ResetTimeKeyLockAndPowerSave();
    g_scanInfo.state = SCANNING;
    g_scanInfo.scanTime = 2;
}

extern void ScanStart(void)
{
    DualStandbyWorkOFF();
    
    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
    {//信道模式下，没有信道开启扫描直接返回
         if(g_ChannelVfoInfo.haveScan == 0)
         {
             BeepOut(BEEP_ERROR);
             return;
         }
    }
    else
    {//带扫描范围设置功能的，未设置不让进入扫描
        if(g_radioInform.vfoScanRangeL == g_radioInform.vfoScanRangeH || CheckInputScanRange(g_radioInform.vfoScanRangeL*10,g_radioInform.vfoScanRangeH*10) == FERROR)
        {//频率范围错误不让进入扫描功能
            BeepOut(BEEP_ERROR);
            return;
        }
    }

    ResetInputBuf();
    g_sysRunPara.sysRunMode = MODE_SCAN;
    g_scanInfo.state = SCANNING;
    g_scanInfo.scanTime = 4;
    g_scanInfo.direction = SCAN_UP;

    DisplayRadioHome();

    VoiceBroadcastWithBeepLock(vo_scanbegin,BEEP_FMSW1);

    //强制进入扫描频点
    ScanNextChannel();
}


extern void ScanOff(void)
{
    g_scanInfo.state = SCAN_IDLE;
    g_scanInfo.scanTime = 0;
    g_sysRunPara.sysRunMode = MODE_MAIN;
    g_rfRxState = RX_READY;

    DisplayRadioHome();
    VoiceBroadcastWithBeepLock(vo_scanstop,BEEP_FASTSW);
}

extern void ScanTask(void)
{
    static U8 sqCnt = 0;

    if(g_sysRunPara.sysRunMode != MODE_SCAN)
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
            ScanNextChannel();
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
            ScanNextChannel();
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
            ScanNextChannel();
            sqCnt = 0;
            
            break;
        case SCAN_STOP:
            g_scanInfo.state = SCAN_IDLE;
            g_scanInfo.scanTime = 0;
            g_sysRunPara.sysRunMode = MODE_MAIN;
            g_rfRxState = RX_READY;
            ChannelNumChangeRead(0,1);
            sqCnt = 0;
            
            DisplayRadioHome();
            break; 
        default:
            break;
    }
}

extern void KeyProcess_Scan(U8 keyEvent)
{
    switch(keyEvent)
    {
        case KEYID_UP:
            g_scanInfo.direction = SCAN_UP;
            BeepOut(BEEP_FMUP);
            g_scanInfo.scanTime = 0;
            ScanNextChannel();
            DisplayResetRxFlag();
            break;
        case KEYID_DOWN:
            g_scanInfo.direction = SCAN_DOWN;
            BeepOut(BEEP_FMDOWN);
            g_scanInfo.scanTime = 0;
            ScanNextChannel();
            DisplayResetRxFlag();
            break;

        case KEYID_EXIT:     
        case KEYID_SCAN:
            ScanOff();
            break;      

        case KEYID_SIDEKEY1:
        case KEYID_SIDEKEY2:
        case KEYID_SIDEKEYL1:
            keyEvent = Sidekey_GetRemapEvent(keyEvent);
            if(keyEvent != KEYID_SCAN && keyEvent != KEYID_LIGHT)
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


