#include "includes.h"

STR_FMSTATE    fmInfo;

extern Boolean CheckFmVfoMode(void)
{
    if(g_FMInform.fmChVfo == VFO_MODE)
    {
        return TRUE;
    }

    return FALSE;
}

extern void FmBandConfig(void)
{
    if(fmInfo.band)
    {
        fmInfo.freq = 650;
    }
    else
    {
        fmInfo.freq = 760;
    }
    fmInfo.mode = FM_READY;
    fmInfo.timeOut = FM_FREQSW_TIME;
    g_FMInform.FmCurFreq = fmInfo.freq;
}

extern void ResumeFmMode(void)
{
    if(g_radioInform.fmEnale || g_rfRxState == WAIT_RXEND)
    {//收音机功能禁用
        BeepOut(BEEP_ERROR);
        return;
    }    
    //进入收音机时关闭双守
    DualStandbyWorkOFF();    
    ResetInputBuf();

    g_sysRunPara.sysRunMode = MODE_FM;
    fmInfo.mode = FM_READY;

    //现在收音机频率范围
    if((fmInfo.freq < 650)||(fmInfo.freq > 1080))
    {
        if(fmInfo.band)
        {
            fmInfo.freq = 650;
        }
        else
        {
            fmInfo.freq = 760;
        }
    }
    FmDisplayHome();

    Flash_SaveFmData();
    BeepOut(BEEP_EXITMENU);
}

Boolean CheckFmChActive(U8 curChanNum)
{
    return fmInfo.fmChList[curChanNum/8]&(1<<(curChanNum%8));
}

U8  SeekActiveFmChDown(U8  curChanNum)
{
    U8  i;
    U8  bufChanNum = 0;
    
    if( curChanNum == 0 )
    {
        bufChanNum = FM_MAX_CH_NUM - 1;
    }
    else
    {
        bufChanNum = curChanNum - 1;
    }

    for(i=0;i<FM_MAX_CH_NUM;i++)
    {
        if(CheckFmChActive(bufChanNum))
        {
            return bufChanNum;
        }
        else
        {
            if( bufChanNum == 0 )
            {
                bufChanNum = FM_MAX_CH_NUM - 1;
            }
            else
            {
                bufChanNum = bufChanNum - 1;
            }
        }
    }

    return 0xFF;
}

U8  SeekActiveFmChUp(U8  curChanNum)
{
    U8  i,bufChanNum;
    bufChanNum = (curChanNum+1) % FM_MAX_CH_NUM;

    for(i=0;i<FM_MAX_CH_NUM;i++)
    {
        if(CheckFmChActive(bufChanNum))
        {
            return bufChanNum;
        }
        else
        {
            bufChanNum = (bufChanNum + 1) % FM_MAX_CH_NUM;
        }
    }
    return 0xFF;    
}

extern void FmCheckChannelActive(void)
{
    U8 i;

    fmInfo.fmChActive = 0;
    for(i=0;i<FM_MAX_CH_NUM;i++)
    {
        if(g_FMInform.FmCHs[i] >= 650 && g_FMInform.FmCHs[i] <= 1080)
        {
            ListFlagSet(fmInfo.fmChList,i,1);
            fmInfo.fmChActive = 1;
        }
    }

    if(g_FMInform.fmChNum >= FM_MAX_CH_NUM)
    {
        g_FMInform.fmChNum = 0;
    }
    
    if(fmInfo.fmChActive == 0)
    {//无信道时强制转换为频率模式
        g_FMInform.fmChVfo = VFO_MODE;
        
    }
    else
    {
        if(CheckFmChActive(g_FMInform.fmChNum) == 0)
        {
            g_FMInform.fmChNum = SeekActiveFmChUp(g_FMInform.fmChNum);
        }
    }
}

extern void FmSwitchChVfo(void)
{
    U8 temp;
    
    if(fmInfo.fmChActive)
    {
        if(g_FMInform.fmChVfo == CHAN_MODE)
        {//频率模式
            g_FMInform.fmChVfo = VFO_MODE;
            fmInfo.freq = g_FMInform.FmCurFreq;
        }
        else
        {
            g_FMInform.fmChVfo = CHAN_MODE;
            fmInfo.freq = g_FMInform.FmCHs[g_FMInform.fmChNum];
        }
    }

    if(g_FMInform.fmChVfo == CHAN_MODE)
    {
        temp = vo_Channelmode;
    }
    else
    {
        temp = vo_Freqmode;
    }

    Audio_PlayVoice(temp);

    //保存信道和频率模式
    Flash_SaveFmData();
    
    fmInfo.mode = FM_READY;
    fmInfo.timeOut = FM_FREQSW_TIME;
    FmDisplayFreq();
}

void FmChNumTypeIn(U8  input)
{
    U8  temp;

    g_inputbuf.time = INPUT_TIME_OUT;
    g_inputbuf.buf[g_inputbuf.len++] = input;
    
    if(g_inputbuf.len == 1 || g_inputbuf.len > 2)
    {
        if(input > '3')
        {
            g_inputbuf.len = 2;
            g_inputbuf.buf[0] = '0';
            g_inputbuf.buf[1] = input;
        }
        else
        {
            g_inputbuf.len = 1;
            g_inputbuf.buf[0] = input;
        }
    }
    
    //播报数字
    temp = input - '0';
    if(g_radioInform.voiceSw == 0)
	{
	    BeepOut(BEEP_NULL);
	}
	else
	{
        Audio_PlayChanNum(temp);
	}

    if(g_inputbuf.len == 2)
    {
        g_inputbuf.buf[g_inputbuf.len] = 0;
        temp = atol(g_inputbuf.buf);
        ResetInputBuf();

        if(temp > 0 && temp <= 32)
        {//实际信道号0到31
            temp -= 1;
            if(g_FMInform.FmCHs[temp] >= 650 && g_FMInform.FmCHs[temp] <= 1080)
            {
                g_FMInform.fmChNum = temp;
                fmInfo.freq = g_FMInform.FmCHs[temp];
                fmInfo.mode = FM_READY;
                fmInfo.timeOut = FM_FREQSW_TIME;
            }
        }
        else
        {//无效播报取消
            Audio_PlayVoice(vo_Cancel);
        }

        FmDisplayFreq();
        return;
    }

    FmDisplayInputChNum();
} 
void FmFreqAdd(void)
{
    if(g_FMInform.fmChVfo == CHAN_MODE)
    {//信道模式
        g_FMInform.fmChNum = SeekActiveFmChUp(g_FMInform.fmChNum);

        if(g_FMInform.fmChNum == 0xFF)
        {
            BeepOut(BEEP_EXITMENU);
            return;
        }
        fmInfo.freq = g_FMInform.FmCHs[g_FMInform.fmChNum];
    }
    else
    {
        if(fmInfo.freq < 1080)
        {

            fmInfo.freq++;
        }
        else
        {
            fmInfo.freq = 760;
        }
    }

    fmInfo.mode = FM_READY;
    fmInfo.timeOut = FM_FREQSW_TIME;
    g_FMInform.FmCurFreq = fmInfo.freq;
    FmDisplayFreq();
}

void FmFreqSub(void)
{
    if(g_FMInform.fmChVfo == CHAN_MODE)
    {//信道模式
        g_FMInform.fmChNum = SeekActiveFmChDown(g_FMInform.fmChNum);

        if(g_FMInform.fmChNum == 0xFF)
        {
            BeepOut(BEEP_EXITMENU);
            return;
        }
        fmInfo.freq = g_FMInform.FmCHs[g_FMInform.fmChNum];
    }
    else
    {
        if(fmInfo.freq > 760)
        {
            fmInfo.freq--;
        }
        else
        {
            fmInfo.freq = 1080;
        }
    }

    fmInfo.mode = FM_READY;
    fmInfo.timeOut = FM_FREQSW_TIME;
    g_FMInform.FmCurFreq = fmInfo.freq;
    FmDisplayFreq();
}

void FmAutoSeek(void)
{
    fmInfo.mode = FM_SEEK;
    fmInfo.timeOut = FM_SEEK_TIMEOUT;
    RDA5807_Seek();
    FmDisplaySeek();
}

void FmBandSwitch(void)
{
    if(fmInfo.band)
    {
        fmInfo.band = 0;
        fmInfo.freq = 760;
    }
    else
    {
        fmInfo.band = 1;
        fmInfo.freq = 650;
    }
    fmInfo.mode = FM_READY;
    fmInfo.timeOut = FM_FREQSW_TIME;
    g_FMInform.FmCurFreq = fmInfo.freq;
    FmDisplayFreq();
}

void FmFreqTypeIn(U8 input)
{
    U8 temp;
    U32 tempFreq;

    g_inputbuf.time = INPUT_TIME_OUT;
    g_inputbuf.buf[g_inputbuf.len++] = input;
    
    if(g_inputbuf.len == 1 || g_inputbuf.len > g_inputbuf.maxLen)
    {
        g_inputbuf.len = 1;
        g_inputbuf.buf[0] = input;
    }

    if(g_inputbuf.buf[0] == '0' || (g_inputbuf.buf[0] > '1' && g_inputbuf.buf[0] < '6'))
    {
        g_inputbuf.len = 0;
        BeepOut(BEEP_NULL);

        return;
    }
    if(g_inputbuf.buf[0] == '1')
    {
        g_inputbuf.maxLen = 4;
    }
    else
    {
        g_inputbuf.maxLen = 3;
    }

    //播报数字
    temp = g_inputbuf.buf[g_inputbuf.len-1] - '0';
    if(g_radioInform.voiceSw == 0)
	{
	    BeepOut(BEEP_NULL);
	}
	else
	{
        Audio_PlayChanNum(temp);
	}

    if(g_inputbuf.len == g_inputbuf.maxLen)
    {
        g_inputbuf.buf[g_inputbuf.len] = 0;
        tempFreq = atol(g_inputbuf.buf);
        ResetInputBuf();

        if(tempFreq >= 650 && tempFreq <= 1080)
        {
            fmInfo.freq = tempFreq;
            fmInfo.mode = FM_READY;
            fmInfo.timeOut = FM_FREQSW_TIME;
            g_FMInform.FmCurFreq = fmInfo.freq;
        }
        FmDisplayFreq();
        return;
    }

    FmDisplayInputFreq();
}


extern void FmCheckTimeOut(void)
{
    if(fmInfo.mode == FM_SLEEP)
    {
        if(g_rfState == RF_RX && g_rfRxState <= GET_CALL && g_sysRunPara.sysRunMode == MODE_MAIN)
        {//收发都在空闲状态时进入递减函数
            if(fmInfo.timeOut)
            {
                fmInfo.timeOut--;
                
                if(fmInfo.timeOut == 0)
                {
                    g_sysRunPara.sysRunMode = MODE_FM;
                    fmInfo.mode = FM_READY;
                    DualStandbyWorkOFF();
                    FmDisplayHome(); 
                }
            }
        }
        else
        {
            fmInfo.timeOut = FM_RETURN_TIME;
        }
    }
    else
    {
        if(g_sysRunPara.sysRunMode != MODE_FM)
        {//不在菜单模式，直接退出
            return;
        }
        if(fmInfo.timeOut)
        {
            fmInfo.timeOut--;
        }
    }
}

extern void EnterFmMode(void)
{
    if(g_radioInform.fmEnale || g_rfRxState == WAIT_RXEND)
    {//收音机功能禁用
        BeepOut(BEEP_NULL);
        return;
    }
    
    //进入收音机时关闭双守
    DualStandbyWorkOFF();

    //如果在菜单模式，则退出菜单
    if(g_sysRunPara.sysRunMode == MODE_MENU)
    {
        Menu_ExitMode();
    }
    ResetInputBuf();

    g_sysRunPara.sysRunMode = MODE_FM;
    fmInfo.mode = FM_READY;

    if(g_FMInform.fmChVfo == CHAN_MODE)
    {
        fmInfo.freq = g_FMInform.FmCHs[g_FMInform.fmChNum];
    }
    else
    {
        fmInfo.freq = g_FMInform.FmCurFreq;
    }
    //现在收音机频率范围
    if((fmInfo.freq < 650)||(fmInfo.freq > 1080))
    {
        if(fmInfo.band)
        {
            fmInfo.freq = 650;
        }
        else
        {
            fmInfo.freq = 760;
        }
    }  
    RDA5807_PowerOn();

    FmDisplayHome();
    VoiceBroadcastWithBeepLock(vo_ON,BEEP_FASTSW);
}

extern void FMSwitchExit(void)
{
    fmInfo.mode = FM_STOP;
    fmInfo.timeOut = 0;
}

extern void ExitFmMode(void)
{
    g_sysRunPara.sysRunMode = MODE_MAIN;
    ResetInputBuf();
    DualStandbyWorkOFF();
    
    SpeakerSwitch(OFF);

    //切换为显示主界面
    DisplayHomePage();    

    if(fmInfo.mode != FM_SLEEP)
    {    
        RDA5807_PowerOff();
    }
    fmInfo.mode = FM_READY;
    Flash_SaveFmData(); 
    
    VoiceBroadcastWithBeepLock(vo_OFF,BEEP_EXITMENU); 
}

extern void FmEnterSleepMode(void)
{
    if(g_sysRunPara.sysRunMode != MODE_FM)
    {//如果不在收音机模式，直接返回
        return;
    }
    
    g_sysRunPara.sysRunMode = MODE_MAIN;
    if(fmInfo.mode != FM_SLEEP)
    {
        RDA5807_PowerOff();
    }
	
    SpeakerSwitch(OFF);
    fmInfo.mode = FM_SLEEP;
    fmInfo.timeOut = FM_RETURN_TIME;
}



extern void FmTaskFunc(void)
{
    U8 state;

    FmCheckTimeOut();
    
    if(g_sysRunPara.sysRunMode != MODE_FM)
    {//不在收音机模式，直接返回
        return;
    }
    
    switch(fmInfo.mode)
    {
        case FM_SLEEP:
            break;
        case FM_READY:
            RDA5807_FrequencyToTune();
		    fmInfo.mode = FM_PLAY;
            break;   
        case FM_SEEK:
            if(fmInfo.timeOut < FM_SEEK_TIME)
            {
                state = RDA5807_STC();

                if(state == 2 || fmInfo.timeOut == 0)
                {//搜索到信号或者超时退出
                    fmInfo.mode = FM_READY;
                    if(g_FMInform.fmChVfo == VFO_MODE)
                    {
                        g_FMInform.FmCurFreq = fmInfo.freq;
                    }
                    FmDisplayFreq();
                }
                else if(state == 1)
                {
                    RDA5807_Seek(); 
                }
                else
                {
                }
            }
            break; 
		
        case FM_PLAY:
            fmInfo.timeOut = 0;
            SpeakerSwitch(ON);
            break;
        case FM_STOP:
        default:
            //退出收音机模式
            ExitFmMode();
            break;
    }
}

extern void  Fm_KeyDigitalInput(U8 keyEvent)
{
    U8 iput;

    iput = keyEvent - KEYID_0 + 0x30;
    if(g_FMInform.fmChVfo == CHAN_MODE)
    {
       FmChNumTypeIn(iput);
    }
    else
    {
        FmFreqTypeIn(iput);
    }
}

extern void KeyProcess_Fm(U8 keyEvent)
{
    switch(keyEvent)
    {
        case KEYID_0:
        case KEYID_1:
        case KEYID_2:
        case KEYID_3:
        case KEYID_4:
        case KEYID_5:
        case KEYID_6:
        case KEYID_7:
        case KEYID_8:
        case KEYID_9:
            Fm_KeyDigitalInput(keyEvent);
            break;
        case KEYID_MENU:
            SpeakerSwitch(OFF);
            RDA5807_PowerOff();
            EnterFMMenu();
            break;
        case KEYID_UP:
            FmFreqAdd();
            BeepOut(BEEP_FMUP);
            break;
        case KEYID_UP_CONTINUE:
            if(FastChangeVoice == 0)
            {
                FastChangeVoice = 1;
                BeepOut(BEEP_FASTSW);
            }
            FmFreqAdd();
            break;
        case KEYID_DOWN:
            FmFreqSub();
            BeepOut(BEEP_FMDOWN);
            break;
        case KEYID_DOWN_CONTINUE:
            if(FastChangeVoice == 0)
            {
                FastChangeVoice = 1;
                BeepOut(BEEP_FASTSW);
            }
            FmFreqSub();
            break; 
        case KEYID_EXIT:
            if(g_inputbuf.len)
            {
                g_inputbuf.len--;
                g_inputbuf.buf[g_inputbuf.len] = ' ';

                if(g_inputbuf.len)
                {
                    g_inputbuf.time = INPUT_TIME_OUT;
                    //显示输入模式
                    FmDisplayInputFreq();
                    BeepOut(BEEP_NULL);
                }
                else
                {
                    FmDisplayFreq();
                    BeepOut(BEEP_FMSW1);
                }
            }
            else
            {
                ExitFmMode();
            }
            break;
        case KEYID_VM:
            FmSwitchChVfo();
            break;    
        case KEYID_STAR:
            if(g_FMInform.fmChVfo == CHAN_MODE)
            {
                BeepOut(BEEP_NULL);
            }
            else
            {
                FmAutoSeek();
                BeepOut(BEEP_FMUP);
            }
            break;      
        case KEYID_LOCK:
            KeyLockFunSwitch();
            break;     

        case KEYID_SIDEKEY1:
        case KEYID_SIDEKEY2:
        case KEYID_SIDEKEYL1:
            keyEvent = Sidekey_GetRemapEvent(keyEvent);
            if(keyEvent == KEYID_PWRSW || keyEvent == KEYID_SCAN)
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


