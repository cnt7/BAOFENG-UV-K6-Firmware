#include "includes.h"

static U8 periodSqlLevel=0xff;
static U8 sqCnt = 0;

extern void GetHardWorkBand(U16 freq)
{
    if(freq < 2000)
    {
        g_ChannelVfoInfo.BandFlag = FREQ_BAND_VHF;
    }
    else if(freq >= 2000 && freq < 3000)
    {
        g_ChannelVfoInfo.BandFlag = FREQ_BAND_200M;
    }
    else if(freq >= 3000 && freq < 4000)
    {
        g_ChannelVfoInfo.BandFlag = FREQ_BAND_350M;
    }
    else
    {
        g_ChannelVfoInfo.BandFlag = FREQ_BAND_UHF;
    }
}
const U16 TxCheckFreq[][6] =
{
    {1360,1740,4000,4800,2200,2550},  //宝锋内部
    {1440,1480,2220,2250,4200,4500},  //美国 FCC 业余无线电   
    {1440,1480,4300,4500,4300,4500},  //加拿大 IC 业余无线电 02
    {1440,1460,4300,4400,4300,4400},  //欧盟 CE 业余无线电    03
    {1360,1740,4000,4800,4000,4800},  //印尼业余04 IAN Amateur
    {1440,1480,4300,4400,4300,4400},  //中国 05
};

Boolean CheckCanTxOverRange(U32 freq)
{
    U32 tempFreq;
    
    if(g_rfMoudel.txEn520M && (freq >= 48000000 && freq <= 52000000))
    {
        return TRUE;
    }

    if(g_sysRunPara.moduleType <= 1)
    {
        tempFreq = freq / 10000;
        
        if(g_rfMoudel.txEn220M && (tempFreq >= TxCheckFreq[g_sysRunPara.moduleType][4] && tempFreq < TxCheckFreq[g_sysRunPara.moduleType][5]))
        {
            return TRUE;
        }
    }

    return FALSE;
}
Boolean CheckFreqInTxFreqRange(U32 freq)
{
    U32 tempFreq;
    
    tempFreq = freq / 10000;

    //获取当前工作频段
    GetHardWorkBand(tempFreq);

    if(CheckCanTxOverRange(freq) == TRUE)
    {
        return TRUE;
    }

    if(g_sysRunPara.moduleType <= 1)
    {
        if((g_rfMoudel.txEn220M == 0) && (tempFreq >= TxCheckFreq[g_sysRunPara.moduleType][4] && tempFreq < TxCheckFreq[g_sysRunPara.moduleType][5]))
        {//强制关闭220M发射 
            return FALSE;
        }
    }

    //通过机型码判断频段信息
    if(((tempFreq >= TxCheckFreq[g_sysRunPara.moduleType][0]) && (tempFreq < TxCheckFreq[g_sysRunPara.moduleType][1])) || ((tempFreq >= TxCheckFreq[g_sysRunPara.moduleType][2]) && (tempFreq < TxCheckFreq[g_sysRunPara.moduleType][3]))
     || ((tempFreq >= TxCheckFreq[g_sysRunPara.moduleType][4]) && (tempFreq < TxCheckFreq[g_sysRunPara.moduleType][5])))
    { 
        return TRUE;
    }

    if((tempFreq >= bandRang.bandFreq.uhfL) && (tempFreq < bandRang.bandFreq.uhfH))
    { 
        return TRUE;
    }

    if((tempFreq >= bandRang.bandFreq.B350ML) && (tempFreq < bandRang.bandFreq.B350MH))
    { 
        return TRUE;
    }
    if((tempFreq >= bandRang.bandFreq.vhf2L) && (tempFreq < bandRang.bandFreq.vhf2H))
    { 
        return TRUE;
    }
    if((tempFreq >= bandRang.bandFreq.vhfL) && (tempFreq < bandRang.bandFreq.vhfH))
    { 
        return TRUE;
    }
  
    return FALSE;
}

void WaitPttRelease(void)
{
    Rfic_RxTxOnOffSetup(RFIC_RXON);

    Rfic_SetToneFreq(F480HZ);
    Rfic_TxSingleTone_On(0);
    SpeakerSwitch(ON);
    beepDat.beepTime = 260;
    beepDat.beepCnt = 1;

    while(PTT_GetKeyVal() == 0)   
    {
        if(beepDat.beepTime == 0)
        {
            if(beepDat.beepCnt == 1)
            {
                beepDat.beepCnt = 0;
                Rfic_SetToneFreq(0);
            }
            else
            {
                beepDat.beepCnt = 1;
                Rfic_SetToneFreq(F480HZ);
            }
            beepDat.beepTime = 260;
        }
    }
    SpeakerSwitch(OFF);
    DelayMs(30);
    Rfic_TxSingleTone_Off();
    Rfic_RxTxOnOffSetup(RFIC_IDLE);
}

extern void Radio_EnterTxMode(void)
{
    U8 beepFlag = 0,txFlag;
    
    ClearDisANIFlag();
    GetDtmfEditCode();

    ExitAllFunction(0);
    if(g_sysRunPara.sysRunMode == MODE_SCAN)
    {
        if(g_scanInfo.state != WAIT_RECALL)   
        {
            ChannelNumChangeRead(0,1);
            g_scanInfo.state = SCAN_IDLE;
            g_scanInfo.scanTime = 0;
            g_sysRunPara.sysRunMode = MODE_MAIN;
        }    
    }

    if(g_sysRunPara.moniFlag)
    {
        ExitMoniMode();
    }

    DualStandbyWorkOFF();

    txFlag = CheckFreqInTxFreqRange(g_CurrentVfo->freqTx.frequency);

    if((g_CurrentVfo->busyLock && g_sysRunPara.rfRxFlag.rxReceiveOn == ON) || g_radioInform.txForbid)
	{//遇忙禁发
        beepFlag = 1;   
    }
    else if(CheckBatteryCanTx() == TRUE && txFlag == TRUE && g_sysRunPara.rfTxFlag.txEnable[g_ChannelVfoInfo.BandFlag] == 1)
    {    
        g_rfState = RF_TX;
        g_rfTxState = TX_READY;
    }
    else
    {
        beepFlag = 1;
    }

    if(beepFlag)
    {
        WaitPttRelease(); 
        ResetTimeKeyLockAndPowerSave();
        ResetDtmfEditCode();
        return;
    }

    DisplayTxMode();
}

extern void RxReset(void)
{
    LedRxSwitch(LED_OFF);
    Rfic_SetAfout(OFF);
    //SpeakerSwitch(SPK_RX_OFF);

    g_rfRxState = RX_READY;
    g_sysRunPara.rfRxFlag.rxReceived = OFF; 
    g_sysRunPara.rfRxFlag.rxReceiveOn = OFF; 
    //接收结束后重新计算省电时间
    ResetTimeKeyLockAndPowerSave();
}


extern void RfOff(void)
{
    LedTxSwitch(LED_OFF);
    LedRxSwitch(LED_OFF); 
    RF_PowerSet(g_ChannelVfoInfo.BandFlag, PWR_OFF);
    //SpeakerSwitch(SPK_RX_OFF);
    Rfic_SetAfout(OFF);
}

extern void RF_TxEnd(void)
{
    LedTxSwitch(LED_OFF);

    if(g_CurrentVfo->pttIdMode != 0)
    {
        DtmfRstMatchTimer(1);
    }

    g_rfTxState = TX_STOP;
    g_sysRunPara.rfTxFlag.totTime = 0;
    g_sysRunPara.rfTxFlag.voxDetDly = 10;
    g_sysRunPara.rfTxFlag.voxWorkDly = 0;
    g_rfRxState = RX_READY;

    if(g_radioInform.rptrl)
    {  
        g_sysRunPara.rfTxFlag.relayTailSetTime = g_radioInform.rptrl + 2;
    }
}

extern void RF_TxRoger(void)
{
    if(g_radioInform.txOffTone == 1)
    {
        DtmfSendTxOver();
    }
    else if(g_radioInform.txOffTone == 2)
    {
        Rfic_EnterMDC1200Mode();
        Rfic_MDC1200ToneTx();
        Rfic_ExitMDC1200Mode();
    }
    else
    {
    }

    if(g_radioInform.tailSwitch)
    {
        RF_SendTail(ON);
        DelayMs(300);
    }
    RF_TxEnd();
}

void TotTimeWarning(void)
{
    static U8 flashFlag = 0;
    
    if(g_radioInform.totLevel == 0)
    {//发射超时功能关闭
        return;
    }
    if(g_sysRunPara.rfTxFlag.totTime < (g_radioInform.toa * 10))
    {
        if(g_sysRunPara.rfTxFlag.totTime%3 == 0)
        {//发射LED闪烁,0.3S闪烁一次
            if(flashFlag == 0)
            {
                LedTxSwitch(LED_FLASH);
                flashFlag = 1;
            }
        }
        else
        {
            flashFlag = 0;
        }
    }
    
    if(g_sysRunPara.rfTxFlag.totTime == 0)
    {//发射结束
        if(g_sysRunPara.dtmfToneFlag == 1)
        {//防止一直按按键到发射超时
            Rfic_SetDtmfFreq(0,0);
            Rfic_EnterDTMFMode(0);
            Rfic_TxSingleTone_Off();
            Rfic_SetAfout(0);

            SpeakerSwitch(OFF);
            g_sysRunPara.dtmfToneFlag = 0;
            KeyScanReset();
        }
        RF_TxRoger();
        g_rfState = RF_RX;
        Rfic_SetPA(0);
        RF_PowerSet(g_ChannelVfoInfo.BandFlag,PWR_OFF);
        Rfic_RxTxOnOffSetup(RFIC_IDLE);
        Audio_PlayVoiceLock(vo_Txovertime);
        WaitPttRelease();
        DisplayRadioHome();
    }
}


extern void RF_TxTask(void)
{
    switch(g_rfTxState)
    {
        case TX_READY:
            RfOff();
            Rfic_ConfigTxMode();
            
            g_sysRunPara.rfTxFlag.totTime = g_radioInform.totLevel*150;
            g_rfTxState = WAIT_PTT_RELEASE;
            if(SetAlarmCode() == FALSE)
            {
                if(g_sysRunPara.txDtmfCode.codeLen)
                {
                    DtmfSendCodeOn(DTMF_TYPEIN);
                }
                else if(g_CurrentVfo->spMute == 1)
                {
                    DtmfSendCodeOn(DTMF_ANI);
                }
            }
            break;
        case WAIT_PTT_RELEASE:
           TotTimeWarning();

           if(alarmDat.alarmStates || g_sysRunPara.rfTxFlag.voxWorkDly)
           {
               return;
           }
           if(PTT_GetKeyVal() == 0)
           {
               return;
           }
           //DtmfRstMatchTimer(0);

           if(g_CurrentVfo->pttIdMode&BIT1)
           {
               DtmfSendCodeOn(g_dtmfStore.dtmfFlag&BIT1);
           }

           RF_TxRoger();
           break;
        case ALARM_TXID:
           DtmfSendCodeOn(DTMF_ALARMID);
           g_rfTxState = WAIT_PTT_RELEASE;
           break;

        case TX_STOP:
           if(g_sysRunPara.rfTxFlag.relayTailSetTime)
           {
              return;
           }
           Rfic_SetPA(0);
           Rfic_RxTxOnOffSetup(RFIC_IDLE);
           ResetTimeKeyLockAndPowerSave();
           DisplayHomePage();
           RF_PowerSet(g_ChannelVfoInfo.BandFlag,PWR_OFF);
           g_rfTxState = TX_READY;
           g_rfState = RF_RX;
           break;

        default:
           break;
    }
}
const U8 RssiLevel[5][4] =
{   
    {135,128,124,120}, //V段 ,116
    {101, 94, 90, 86}, //U段, 82
    {108,101, 97, 93}, //220-260, 89
    { 85, 78, 74, 71}, //350-390, 67
    {132,128,124,120}  //AM ,116
};

extern void CalculateSqlLevel(void)
{
    U16 rssi;    
    
    if(!(g_rfState == RF_RX && g_rfRxState == WAIT_RXEND))
    {
        return;
    }

    if(g_sysRunPara.rfRxFlag.rxReceiveOn == ON)   
    {
        //计算SQL信号强度等级
        rssi = Rfic_GetRssiVal();
        if(rssi<RssiLevel[g_ChannelVfoInfo.BandFlag][3])
        {
            rssi = 0;
        }
        else if(rssi<RssiLevel[g_ChannelVfoInfo.BandFlag][2])
        {
            rssi = 1;
        }
        else if(rssi<RssiLevel[g_ChannelVfoInfo.BandFlag][1])
        {
            rssi = 2;
        }
        else if(rssi<RssiLevel[g_ChannelVfoInfo.BandFlag][0])
        {
            rssi = 3;
        }
        else
        {
            rssi = 4;
        }  

        if(periodSqlLevel != rssi)
        {
            DisplaySingalFlag(rssi,1);
            periodSqlLevel = rssi;
        }
    }
    else
    {
        if( periodSqlLevel != 0xff )
        {
            periodSqlLevel = 0xff;
        }
    } 
}


extern void RF_RxEnd(void)
{
    LedRxSwitch(LED_OFF);

    Rfic_SetAfout(OFF);
    SpeakerSwitch(OFF);

    g_rfRxState = RX_READY;
    g_sysRunPara.rfRxFlag.rxReceived = OFF; 
    g_sysRunPara.rfRxFlag.rxReceiveOn = OFF; 
    g_sysRunPara.rfTxFlag.voxDetDly = 10;

    if(DtmfGetMatchStatue() == 0)
    {
        DualStandbyWorkOFF();
    }

    if(g_sysRunPara.sysRunMode == MODE_SCAN_QT)
    {
        return;
    }
    ResetTimeKeyLockAndPowerSave();

    DisplayHomePage();
}

extern Boolean Rfic_CheckCtsState(void)
{
    static U8 ctsCnt = 0;

    if(g_sysRunPara.sysRunMode == MODE_WEATHER)
    {
        return TRUE;
    }
    
    if(g_CurrentVfo->rx->dcsCtsType == SUBAUDIO_NONE && g_CurrentVfo->fhssFlag == 0)
    {
        return TRUE;
    }

    if(Rfic_SubaudioDetect() == 0)
    {
        ctsCnt = 0;
        return FALSE;
	}

	ctsCnt++;
    if(ctsCnt > 2)
    {  
        ctsCnt = 0;
        return TRUE;
    }

    return FALSE;
}

extern void RF_RxTask(void)
{
    static U8 ctcsDetFlag,tailDetFlag;

    if(Audio_CheckBusy() || alarmDat.alarmStates)
    {
        return;
    }
    
    switch(g_rfRxState)
    {
        case RX_READY:
           if(g_sysRunPara.sysRunMode == MODE_WEATHER)
           {
               WeatherInit(g_radioInform.weatherNum);
           }
           else
           {
               Rfic_ConfigRxMode();
           }
           if(g_sysRunPara.moniFlag)
           {
               LedRxSwitch(LED_ON);
               Rfic_TxSingleTone_Off();
               Rfic_SetAfout(ON);
               SpeakerSwitch(ON);
               g_rfRxState = RX_MONI;
               break;
           }
           else
           {
               if(dualStandby.dualOnFlag == OFF && (g_sysRunPara.sysRunMode == MODE_MAIN))
               {
                   DualStandbyWorkON();
               }
               ResetDualRxTime();
               g_sysRunPara.rfRxFlag.rxReceived = OFF;
               g_sysRunPara.rfRxFlag.rxReceiveOn = OFF;
               g_sysRunPara.rfRxFlag.relayTailDetTime = 0;
               sqCnt = 0;
               g_rfRxState = GET_CALL;
           }
        case GET_CALL:
           if(Rfic_GetSQLinkState() == TRUE)
           {
               sqCnt++;  
               if(sqCnt < 3)
               {
                   return;
               }
               ResetTimeKeyLockAndPowerSave();
               ResetDualRxTime();
               g_sysRunPara.rfRxFlag.rxReceived = ON;
               LedRxSwitch(LED_ON); 

               if(g_sysRunPara.sysRunMode != MODE_WEATHER)
               {
                   if(Rfic_CheckCtsState() == FALSE)
                   {
                       return;
                   }
                   if(g_CurrentVfo->spMute == 1)
                   {
                        if(dtmfInfo.matchTime[g_ChannelVfoInfo.dualAB] == 0)
                        {
                            if(dtmfInfo.callType == CALLTYPE_NONE)
                            {
                                return;
                            }

                            DtmfRstMatchTimer(1);
                        }
                   }
               }
               
               sqCnt = 0;
               if(g_radioInform.fmInterrupt == 0 && g_sysRunPara.sysRunMode == MODE_FM)
               {//收音机开启不允许打断
                   return;
               }
               FmEnterSleepMode();

               if(g_sysRunPara.sysRunMode == MODE_MAIN)
               {
                   DisplayRxMode(); 
               }
               LCD_BackLightSetOn();
               Rfic_SetAfout(ON);
               if(g_sysRunPara.sysRunMode != MODE_SCAN_QT)
               {
                   SpeakerSwitch(ON);
               }
               g_sysRunPara.rfRxFlag.relayTailDetTime = 20;

               g_sysRunPara.rfRxFlag.rxReceiveOn = ON; 
               g_sysRunPara.rfRxFlag.rxReceived = ON;  
               ctcsDetFlag = 0;
               tailDetFlag = 0;
               g_rfRxState = WAIT_RXEND;
           }
           else
           {
               sqCnt = 0;
               if(g_sysRunPara.rfRxFlag.rxReceived == ON)
               {
                   g_sysRunPara.rfRxFlag.rxReceived = OFF;  
                   LedRxSwitch(LED_OFF);
               }

               if(g_sysRunPara.sysRunMode == MODE_SCAN || g_sysRunPara.sysRunMode == MODE_FM || g_sysRunPara.sysRunMode == MODE_SCAN_QT)
               {
                   return;
               }
               PowerSaveTask();
               DualStandbyTask();
           }
           break;

        case WAIT_RXEND:
           if(Rfic_GetSQLinkState() == FALSE)
           {      
               if(sqCnt > 0)
               {
                   sqCnt--;
                   return;
               } 
               if(g_sysRunPara.rfTxFlag.relayTailSetTime == 0)
               {
                   RF_RxEnd();
               }
               ResetTimeKeyLockAndPowerSave();
           }
           else
           {
               sqCnt = 1;
               DtmfRstMatchTimer(0);

               if(g_CurrentVfo->rx->dcsCtsType)
               {
                   if(Rfic_GetTail())
                   {//判断是否是尾音消除
                       ctcsDetFlag = 1;
                       if(tailDetFlag == 0)
                       {
                           g_sysRunPara.rfRxFlag.relayTailDetTime = 10;
                           tailDetFlag = 1;
                           if(g_sysRunPara.rfTxFlag.relayTailSetTime == 0)
                           {
                               SpeakerSwitch(OFF);
                           }
                           return;
                       }
                   }
                   else
                   {
                       if(Rfic_CheckCtsState() == TRUE && dtmfInfo.matchTime[g_ChannelVfoInfo.dualAB] == 0)    
                       {   
                           if(g_sysRunPara.rfRxFlag.relayTailDetTime == 0 && ctcsDetFlag == 0)
                           {//延时检测亚音，避免亚音频干扰
                               
                               ctcsDetFlag = 1;
                               //亚音检测不对时，需要结束接收流程
                               g_sysRunPara.rfRxFlag.relayTailDetTime = 8;
                               if(g_sysRunPara.rfTxFlag.relayTailSetTime == 0)
                               {
                                   SpeakerSwitch(OFF);
                               }
                           }
                       }
                       else
                       {
                           ctcsDetFlag = 0;
                           //判断亚音频延时使用
                           g_sysRunPara.rfRxFlag.relayTailDetTime = 20;
                       }
                   }
               }
               else
               {
                   if(Rfic_GetTail())
                   {
                       ctcsDetFlag = 1;
                       if(tailDetFlag == 0)
                       {
                           tailDetFlag = 1;
                           g_sysRunPara.rfRxFlag.relayTailDetTime = 8;
                           if(g_sysRunPara.rfTxFlag.relayTailSetTime == 0)
                           {
                               SpeakerSwitch(OFF);
                           }
                           return;
                       }
                   }
                   
                   if(ctcsDetFlag != 1 && g_sysRunPara.sysRunMode != MODE_SCAN_QT)
                   {
                       SpeakerSwitch(ON);
                   }

                   if((g_sysRunPara.rfRxFlag.relayTailDetTime == 0) && (ctcsDetFlag == 1))
                   {               
                       RF_RxEnd();
                   }
               }
           }
           break;

        case RX_MONI:
           break;
           
        default:
           break;
    }
}

extern void RF_Task(void)
{
    switch(g_rfState)
    {
       case RF_TX:
          RF_TxTask();
          break;
       case RF_RX:
          RF_RxTask();
          break;
       case RF_NONE:
       default:
          break;
    }
}

