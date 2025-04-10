#include "includes.h"

const U8 sideKeyEvent[] = {KEYID_NONE,KEYID_LIGHT,KEYID_PWRSW,KEYID_SCAN,KEYID_VOX,KEYID_SOS,KEYID_FM};
const U8 DefaultSideKeyEvent[3] = {KEYID_LIGHT,KEYID_SOS,KEYID_FM};

extern U8 Sidekey_GetRemapEvent(U8 event)
{
    U8 realEvent;

    switch(event)
    {
        case KEYID_SIDEKEY1:
             if(g_radioInform.userSideKey[0] > 6)
             {
                 realEvent = DefaultSideKeyEvent[0];
             }
             else
             {
                 realEvent = sideKeyEvent[g_radioInform.userSideKey[0]];
             }
             break;
        case KEYID_SIDEKEYL1:
             if(g_radioInform.userSideKey[1] > 6)
             {
                 realEvent = DefaultSideKeyEvent[1];
             }
             else
             {
                 realEvent = sideKeyEvent[g_radioInform.userSideKey[1]];
             }
             break;
        case KEYID_SIDEKEY2:
             if(g_radioInform.userSideKey[2] > 6)
             {
                 realEvent = DefaultSideKeyEvent[2];
             }
             else
             {
                 realEvent = sideKeyEvent[g_radioInform.userSideKey[2]];
             }
             break;
        default:
             realEvent = KEYID_NONE;
           break;
    }

    return realEvent;
}

extern void SideKey_Process(U8 realEvent)
{
    
    if(realEvent == KEYID_NONE)
    {
        BeepOut(BEEP_NULL);
        return;
    }

    switch(realEvent)
    {
        case KEYID_LIGHT:
            g_sysRunPara.ledState = (g_sysRunPara.ledState+1)%3;
            LightSwitch(g_sysRunPara.ledState);
            BeepOut(BEEP_FASTSW);
            break;
        case KEYID_PWRSW:
            Radio_TxPowerSWitch();
            break;
        case KEYID_SCAN:
            if(g_sysRunPara.sysRunMode == MODE_SCAN)
            {
                ScanOff();
            }
            else
            {
                ScanStart();
            }
            break;
        case KEYID_VOX:
            VoxSwitchOnOff();
            break;
        case KEYID_SOS:
            if(alarmDat.alarmStates == ON)
            {
                AlarmFuncSwitch(OFF);
            }
            else
            {
                AlarmFuncSwitch(ON);
            }
            break;
        case KEYID_FM:
            if(g_sysRunPara.sysRunMode == MODE_FM)
            {
                ExitFmMode();
            }
            else
            {
                EnterFmMode();
            }
            break;
        default:
            break;
    }
}

extern void Main_KeyDigitalInput(U8 keyEvent)
{
    g_inputbuf.buf[g_inputbuf.len] = keyEvent - KEYID_0 + 0x30;
    g_inputbuf.time = INPUT_TIME_OUT;
    if(g_inputbuf.len < 8)
    {
        g_inputbuf.len++;
    }
    
    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
    {
        ChannelNumTypeIn(&g_inputbuf);
    }
    else
    {
        VfoFreqTypeIn(&g_inputbuf);
    }
}

extern void Main_KeyUpFun(U8 flag)
{
    ResetInputBuf();
    
    if(flag && FastChangeVoice == 0)
    {
        DisplayResetRxFlag();
        FastChangeVoice = 1;
        BeepOut(BEEP_FMUP);
    }
    DualStandbyWorkOFF();

    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
    {
        ChannelUp(0,flag);
    }
    else
    {
        VfoFreqUp(0);
    }

	if(flag == 0)
    {
        if(!Audio_CheckBusy())
        {
            BeepOut(BEEP_FMUP);
        }
        DisplayResetRxFlag();
    }
}

extern void Main_KeyDownFun(U8 flag)
{
    ResetInputBuf();

    if(flag && FastChangeVoice == 0)
    {
        DisplayResetRxFlag();
        FastChangeVoice = 1;
        BeepOut(BEEP_FMDOWN);
    }

    DualStandbyWorkOFF();

    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
    {
        ChannelDown(0,flag);
    }
    else
    {
        VfoFreqDown(0);
    }

    if(flag == 0)
    {
        
        if(!Audio_CheckBusy())
        {
            BeepOut(BEEP_FMDOWN);
        }
        DisplayResetRxFlag();
    }
}

extern void Main_Backspace(void)
{
    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == VFO_MODE)
    {//频率模式下有输入字符时作为退格使用
        if(g_inputbuf.len)
        {
            g_inputbuf.len--;
            g_inputbuf.buf[g_inputbuf.len] = ' ';

            if(g_inputbuf.len)
            {
               g_inputbuf.time = INPUT_TIME_OUT;
                //显示输入模式
                DisplayInputVfoFreq();
                BeepOut(BEEP_NULL);
            }
            else
            {
                DualStandbyWorkOFF();   //临时关闭双守保证显示信息正确
                DisplayRadioHome();
                BeepOut(BEEP_FMSW1);
            }
            return;
        }
        else
        {
            BeepOut(BEEP_NULL);
        }
    }
    else
    {
        //信道模式下退格
        if(g_inputbuf.len)
        {
            g_inputbuf.len--;
            g_inputbuf.buf[g_inputbuf.len] = '-';

            if(g_inputbuf.len)
            {
               g_inputbuf.time = INPUT_TIME_OUT;
                //显示输入模式
                DisplayInputChNum();
                BeepOut(BEEP_NULL);
            }
            else
            {
                DualStandbyWorkOFF();   //临时关闭双守保证显示信息正确
                DisplayRadioHome();
                BeepOut(BEEP_FMSW1);
            }
            return;
        }
        else
        {
            BeepOut(BEEP_NULL);
        }
    }
}

extern void KeyProcess_Main(U8 keyEvent)
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
            Main_KeyDigitalInput(keyEvent);
            break;
        case KEYID_MENU:
            Menu_EnterMode();
            break;
        case KEYID_UP:
            Main_KeyUpFun(0);
            break;
        case KEYID_UP_CONTINUE:
            Main_KeyUpFun(1);
            break;
        case KEYID_DOWN:
            Main_KeyDownFun(0);
            break;
        case KEYID_DOWN_CONTINUE:
            Main_KeyDownFun(1);
            break; 
        case KEYID_EXIT:
            Main_Backspace();
            break;
        case KEYID_VM:
            ResetInputBuf();
            Radio_SwitchChannelOrVfo();
            break;    
        case KEYID_AB:
            Radio_SwitchAOrB();
            BeepOut(BEEP_FMSW1);
            break;
        case KEYID_BAND:
            break;
        case KEYID_STAR:
            Radio_ReverseSwitch();
            break;     
        case KEYID_WELL:
            EnterDtmfEditMode();
            break;      
        case KEYID_SCAN:
            ScanStart();
            break;    
        case KEYID_SEARCH:
            EnterSearchFreqMode();
            break;   
        case KEYID_LOCK:
            KeyLockFunSwitch();
            break;  
        case KEYID_WX:
            EnterWeatherMode();
            break;  
        case KEYID_DUAL_SW:
            DualModeFastSwitch();
            break;    
        case KEYID_CHDIS:
            ChDisModeFastSwitch();
            break;    
        case KEYID_SIDEKEYL2: 
            EnterMoniMode();
            break;

        case KEYID_SIDEKEY1:
        case KEYID_SIDEKEY2:
        case KEYID_SIDEKEYL1:
            keyEvent = Sidekey_GetRemapEvent(keyEvent);
            SideKey_Process(keyEvent);
            break;
            
        default:
            BeepOut(BEEP_NULL);
            break;
    }
}

