#include "includes.h"

const U8 VOX_TH[11]={127,   52,   62,   72,   84,   95,   106,  117,  125,   132,   140};
#define VOX_OFF_DIFF    8

extern void ResetInputBuf(void)
{
    memset((U8 *)&g_inputbuf.time,0x00,sizeof(STR_INPUTBOX));
    inputTypeBack = 0xFF;
}

extern void ExitAllFunction(U8 flag)
{
    ResetInputBuf(); 
    
    ExitSearchFreqMode(1); 
    ExitWeatherMode();
    ExitStopWatchMode();

    if(g_sysRunPara.sysRunMode == MODE_FM)
    {
        FmEnterSleepMode();
        if(flag)
        {
            FMSwitchExit();
        }
    }

    if(g_sysRunPara.sysRunMode == MODE_MENU)
    {
        Menu_ExitMode();
        return;
    }

    /*if(g_sysRunPara.sysRunMode != MODE_MAIN)
    {
        DisplayHomePage();
    }*/
}

extern void CheckAutoKeyLockTask(void)
{
    if(g_radioInform.keyAutoLock)
    {//自动键盘锁开启
        if(g_sysRunPara.sysRunMode == MODE_MENU || g_sysRunPara.sysRunMode == MODE_SEARCH || g_sysRunPara.sysRunMode == MODE_STOPWATCH)
        {
            //菜单模式下，键盘不自动锁定
            return;
        }
    
        if(g_sysRunPara.keyAutoTime && (g_rfRxState != WAIT_RXEND) && g_rfState == RF_RX && (g_rfRxState != RX_MONI))
        {
            g_sysRunPara.keyAutoTime--;
            if(g_sysRunPara.keyAutoTime == 0)
            {
                if(g_radioInform.keyLock == 0)
                {
                    g_radioInform.keyLock = 1;
                    DisplayUpdateLockFlag(1);
                    Audio_PlayVoice(vo_keylock);
				}
            }
        }
    }
}

extern void KeyLockFunSwitch(void)
{
    if(g_radioInform.keyLock)
    {
        g_radioInform.keyLock = 0;
        Audio_PlayVoice(vo_unlock);
    }
    else
    {
        g_radioInform.keyLock = 1;
        Audio_PlayVoice(vo_keylock);
    }
    
    DisplayUpdateLockFlag(1);
}

extern void DualModeFastSwitch(void)
{
    ResetInputBuf();
    DualStandbyWorkOFF();

    g_radioInform.dualRxFlag = (g_radioInform.dualRxFlag + 1) % 3;

    DisplayHomePage();

    BeepOut(BEEP_FMSW1);
}

extern void ChDisModeFastSwitch(void)
{
     ResetInputBuf();
     DualStandbyWorkOFF();
    
     if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == VFO_MODE)
     {
         BeepOut(BEEP_NULL);
     }
     else
     {
         if(g_ChannelVfoInfo.switchAB == 0)
         {
             g_radioInform.channleDisA = (g_radioInform.channleDisA+1)%3;  
         }
         else
         {
             g_radioInform.channleDisB = (g_radioInform.channleDisB+1)%3;    
         }
         BeepOut(BEEP_FMSW1);
         DisplayRadioHome();
    }
}

extern void VoxSwitchOnOff(void)
{
    if(g_radioInform.voxSwitch == 0)
    {
        g_radioInform.voxSwitch = 1;
    }
    else
    {
        g_radioInform.voxSwitch = 0;
    }
    //更新系统图标
    DisplayStateBar();
    BeepOut(BEEP_FMSW1);
}

extern void VoxCheckTask(void)
{
    U8 voxData,voxLevel;
    //String disbuf[16]={0};

    if(g_radioInform.voxSwitch == 0)
    {//声控功能未开启
        return;
    }  

    if(g_rfRxState == WAIT_RXEND || (g_rfState == RF_RX && g_sysRunPara.rfRxFlag.rxReceived == ON))
    {//接收状态下不声控
        g_sysRunPara.rfTxFlag.voxDetDly = 15;//1.5S
        return;
    }

    if(Audio_CheckBusy() || alarmDat.alarmStates || g_sysRunPara.sysRunMode >= MODE_MENU )
    {
        g_sysRunPara.rfTxFlag.voxDetDly = 15;//1.5S
        return;
    }

    if(g_sysRunPara.rfTxFlag.voxDetDly)
    {//用于延时声控检测，在声音播放完成后消除抖动使用
        return;
    }
    voxData = UserADC_GetValOfVox();
    voxLevel = VOX_TH[g_radioInform.voxLevel+1];

    if(g_rfState == RF_TX)
    {//开启声控发射后，停止的值需要小1.2mv左右
        voxLevel -= VOX_OFF_DIFF;
    }
    if(voxData > voxLevel)
    {	    
        g_sysRunPara.rfTxFlag.voxWorkDly = 5 + g_radioInform.voxDelay;   //支持0.5S到2S

        if(g_rfState == RF_RX)
        {
            Radio_EnterTxMode();
            if(g_rfState != RF_TX)
            {//延时避免禁发出错
                g_sysRunPara.rfTxFlag.voxDetDly = 12; 
                g_sysRunPara.rfTxFlag.voxWorkDly = 0;
            }
        }
    }
}

extern void Radio_TxPowerSWitch(void)
{
    ResetInputBuf();
    DualStandbyWorkOFF();

    g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].txPower = (g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].txPower + 1)&0x01;

    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == VFO_MODE)
    {
        U8 tempPower;

        tempPower = g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].txPower&0x0f;
        tempPower = (tempPower+1)&0x01;
        g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].txPower = (g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].txPower & 0xF0) | tempPower;
    }
    DisplayRadioHome();
}

extern void Radio_TxKeyTone(U8 event, U8 para)
{
    if(alarmDat.alarmStates)
    {
        return;
    }

    if(g_sysRunPara.dtmfToneFlag == 1 && (para == KEYSTATE_RELEASE))
    {//有播报过按键侧音同时按键释放时，需要关闭按键侧音
        Rfic_SetDtmfFreq(0,0);
        Rfic_ExitDTMFMode();
        Rfic_TxSingleTone_Off();
        SpeakerSwitch(OFF);
        g_sysRunPara.dtmfToneFlag = 0;
        g_keyScan.longPress = KEY_CLICKED;
        g_keyScan.keyEvent = KEYID_NONE;
    }
    else
    {
        if(para == KEYSTATE_RELEASE)
        {
           return;
        }
        if(g_sysRunPara.dtmfToneFlag == 1)
        {
            return;
        }
        
        switch(event)
        {
            case KEYID_0:
               DtmfSendKeypadCode(0);
               break;
            case KEYID_1:
               DtmfSendKeypadCode(1);
               break;   
            case KEYID_2:
               DtmfSendKeypadCode(2);
               break;    
            case KEYID_3:
               DtmfSendKeypadCode(3);
               break;   
            case KEYID_4:
               DtmfSendKeypadCode(4);
               break;
            case KEYID_5:
               DtmfSendKeypadCode(5);
               break;   
            case KEYID_6:
               DtmfSendKeypadCode(6);
               break;    
            case KEYID_7:
               DtmfSendKeypadCode(7);
               break;    
            case KEYID_8:
               DtmfSendKeypadCode(8);
               break;    
            case KEYID_9:
               DtmfSendKeypadCode(9);
               break;
            case KEYID_BAND:   
            case KEYID_MENU:
               DtmfSendKeypadCode(10);
               break;
            case KEYID_VM:   
            case KEYID_UP:
               DtmfSendKeypadCode(11);
               break;
            case KEYID_DOWN:
               DtmfSendKeypadCode(12);
               break;   
            case KEYID_EXIT:
               DtmfSendKeypadCode(13);
               break;    
            case KEYID_STAR:   
               DtmfSendKeypadCode(14);
               break; 
            case KEYID_WELL:
               DtmfSendKeypadCode(15);
               break; 
            case KEYID_SIDEKEY2:
               if(g_radioInform.rtone > 3)
                {//限制发送1750信令是超出范围
                    g_radioInform.rtone = 3;
                }
                DtmfSendKeypadCode(g_radioInform.rtone+17);
               break;
            default:
               break;
        }  
    }
}

