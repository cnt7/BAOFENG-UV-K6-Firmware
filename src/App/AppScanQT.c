#include "includes.h"

STR_REMOTESCANQT g_ScanQTInfo;

extern void EnterRemoteScanQTMode(void)
{
    //进入收音机时关闭双守
    DualStandbyWorkOFF();

    //如果在菜单模式，则退出菜单
    if(g_sysRunPara.sysRunMode == MODE_MENU)
    {
        Menu_ExitMode();
    }
    ResetInputBuf();

    g_sysRunPara.sysRunMode = MODE_SCAN_QT;
    g_ScanQTInfo.state = SCANQT_STEPUP;
 
    QTScanDisplayHome();
}

extern void ExitRemoteScanQTMode(void)
{
    g_sysRunPara.sysRunMode = MODE_MAIN;
    ResetInputBuf();
    DualStandbyWorkOFF();

    //切换为显示主界面
    DisplayRadioHome();   
}

extern void TaskRemoteScanQT(void)
{
    static U8 sqCnt = 0;

    if(g_sysRunPara.sysRunMode != MODE_SCAN_QT)
    {
        return;
    }

    switch(g_ScanQTInfo.state)
    {
        case SCANQT_STEPUP:
            if(Rfic_GetSQLinkState() == FALSE)
            {
                sqCnt = 0;
                break;
            }
            else
            {
                if(sqCnt < 5)
                {
                    sqCnt++;
                    break;
                }
            }

            Rfic_ScanQT_Enable();
            QTScanDisplayStatus(1);
            sqCnt = 0;
            g_ScanQTInfo.state = SCANQT_WAIT;
            break;

        case SCANQT_WAIT:
            if(Rfic_GetSQLinkState() == FALSE)
            {
                if(sqCnt < 5)
                {
                    sqCnt++;
                    break;
                }
                else
                {
                    QTScanDisplayStatus(0);
                    g_ScanQTInfo.state = SCANQT_STEPUP;
                    break;
                }
            }
            else
            {
                sqCnt = 0;
            }

            g_ScanQTInfo.dcsCtsDat = Rfic_GetCtsDcsData();
            if(g_ScanQTInfo.dcsCtsDat != 0)
            {
                g_ScanQTInfo.dcsCtsType = Rfic_GetSCtsDcsType();
                if(g_ScanQTInfo.dcsCtsType == SUBAUDIO_CTS)
                {
                    g_ScanQTInfo.dcsCtsDat = g_ScanQTInfo.dcsCtsDat * 10 * 10000 / 206489;
                    if(g_ScanQTInfo.dcsCtsDat > 600)
                    {
                        g_ScanQTInfo.dcsCtsDat = CheckIsStandardCTCSS(g_ScanQTInfo.dcsCtsDat);
                        QTScanDisplay(SUBAUDIO_CTS, g_ScanQTInfo.dcsCtsDat, 0);
                    }
                    else
                    {
                        QTScanDisplay(SUBAUDIO_NONE, 0, 0);
                        g_ScanQTInfo.dcsCtsType = SUBAUDIO_NONE;
                    }
                }
                else
                {
                    U32 tempDCS;

                    tempDCS = CheckIsStandardDCS(g_ScanQTInfo.dcsCtsDat);
                    if(tempDCS != 0)
                    {
                        g_ScanQTInfo.dcsCtsDat = tempDCS;
                        g_ScanQTInfo.dcsIndex = searchFreqImofs.CtsResult;
                        g_ScanQTInfo.isStandarDCS = 1;
                        QTScanDisplay(SUBAUDIO_DCS_N, g_ScanQTInfo.dcsCtsDat, 1);
                    }
                    else
                    {
                        g_ScanQTInfo.dcsIndex = 0;
                        g_ScanQTInfo.isStandarDCS = 0;
                        QTScanDisplay(SUBAUDIO_DCS_N, g_ScanQTInfo.dcsCtsDat, 0);
                    }  
                }

                g_ScanQTInfo.state = SCANQT_STOP;
            }
            
            break;

        case SCANQT_STOP:
            
            break;
    }
}

extern void SaveQTScanResult(void)
{ 
    if(g_ScanQTInfo.dcsCtsType == SUBAUDIO_CTS)
    {
        if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == VFO_MODE)
        {
            g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].rxDCSCTSNum = g_ScanQTInfo.dcsCtsDat;
            g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].txDCSCTSNum = g_ScanQTInfo.dcsCtsDat;
        }
        else
        {
            g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].rxDCSCTSNum = g_ScanQTInfo.dcsCtsDat;
            g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].txDCSCTSNum = g_ScanQTInfo.dcsCtsDat;
            g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].chFlag3.Byte &= 0xFE;     
        }
    }
    else if(g_ScanQTInfo.dcsCtsType == SUBAUDIO_DCS_N)
    {
        if(g_ScanQTInfo.isStandarDCS == 1)
        {
            if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == VFO_MODE)
            {
                if(g_ScanQTInfo.dcsIndex < 211)
                {//选择了跳频以外的数字亚音频
                    g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].vfoFlag.Byte &= 0xFE; // 清除学习跳频标志
                    g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].rxDCSCTSNum = g_ScanQTInfo.dcsIndex;
                }
            }
            else
            {
                if(g_ScanQTInfo.dcsIndex < 211)
                {//选择了跳频以外的数字亚音频
                    g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].chFlag3.Byte &= 0xFE; // 清除学习跳频标志
                    g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].rxDCSCTSNum = g_ScanQTInfo.dcsIndex;
                }
            }
        }
        else
        {
            if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
            {
                g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].decoderCode = g_ScanQTInfo.dcsCtsDat;
                g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].decoderCode &= 0X007FFFFF;
                g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].decoderCode |= 0xA0000000; // 表示学习跳频
                g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].chFlag3.Byte |= 0X01;//破码标志                             
            }
            else
            {
                g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].decoderCode = g_ScanQTInfo.dcsCtsDat;
                g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].decoderCode &= 0X007FFFFF;
                g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].decoderCode |= 0xA0000000; // 表示学习跳频
                g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].vfoFlag.Byte |= 0X01;//破码标志            
            }
        }
    }
    ChannleVfoDataInit(g_ChannelVfoInfo.switchAB,0);

    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
    {
        Flash_SaveChannelData(g_ChannelVfoInfo.currentChannelNum,(U8 *)&g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].rxFreq,g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].channelName);
    }
    else
    {
        Flash_SaveVfoData(g_ChannelVfoInfo.switchAB);
    }
}

extern void KeyProcess_ScanQt(U8 keyEvent)
{
    switch(keyEvent)
    {
        case KEYID_MENU:
            if(g_ScanQTInfo.state == SCANQT_STOP)
            {
                SaveQTScanResult();
                ExitRemoteScanQTMode();
                VoiceBroadcastWithBeepLock(vo_Confirm,BEEP_FASTSW);
            }
            else
            {
                BeepOut(BEEP_ERROR);
            }
            break;
        case KEYID_UP:
        case KEYID_DOWN:
            QTScanDisplayStatus(0);
            g_ScanQTInfo.state = SCANQT_STEPUP;
            break;

        case KEYID_EXIT:
            ExitRemoteScanQTMode();
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


