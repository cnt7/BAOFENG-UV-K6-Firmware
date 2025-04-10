#include "includes.h"

STR_MENUINFO  g_menuInfo;

extern void ResetMenuExitTime(void)
{
    g_menuInfo.menuExitTime = 50 * (g_radioInform.menuExitTime + 1);

    if(g_radioInform.menuExitTime == 10)
    {
        g_menuInfo.menuExitTime += 50;
    }
}

extern U32 loadCtcssVal(U16 ctcss)
{
    U8 i = 0;
    
    if(ctcss < CTCS_TAB[0] || ctcss > 26000)
    {
        g_menuInfo.inputVal = ctcss;
    }
    else
    {
        for(i=0;i<51;i++)
        {
            if(ctcss == CTCS_TAB[i])
            {
                break;
            }
            if(ctcss< CTCS_TAB[i])
            {
                i -= 1;
                break;
            }
        }
        g_menuInfo.inputVal = ctcss;
    }
    return i;
}

extern void Menu_GetSubItemPara(U8 menuIndex)
{
    g_menuInfo.inputMode = MENU_ONE_SELECT;
    switch(menuIndex)
    {
        case S_CHNAME:
            g_menuInfo.inputMode = MENU_ONE_CHAR;
            g_inputbuf.maxLen = 12;
            g_inputbuf.isFirstInput = 0xaa;
        
            if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == VFO_MODE)
            {//频率模式不需要设置信道名称
                g_menuInfo.selectedItem = 0xFFFF;
                break;
            }
        
            if(!(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].channelName[0] == 0xFF || g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].channelName[0] == 0x00))
            {
                g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].channelName[12] = 0;
                g_inputbuf.len = sprintf(g_inputbuf.buf,"%s",g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].channelName);
            }
            g_menuInfo.selectedItem = 0;
            break;
        case S_RXFREQ:
            g_menuInfo.inputMode = MENU_CH_FREQ;
            g_menuInfo.selectedItem = g_CurrentVfo->rx->frequency;
            break;
        case S_TXFREQ:
            g_menuInfo.inputMode = MENU_CH_FREQ;
            g_menuInfo.selectedItem = g_CurrentVfo->tx->frequency;
            break;
        case S_RXCTS:
            g_menuInfo.inputMode = MENU_ONE_CTCSS;
            g_menuInfo.subMaxItem = 51;
            g_menuInfo.selectedItem = loadCtcssVal(g_CurrentVfo->rx->dcsCtsNum);
            
            break;    
        case S_RXDCS:
            g_menuInfo.inputMode = MENU_ONE_DECODE;
            g_menuInfo.subMaxItem = 212;

            if((g_CurrentVfo->rx->dcsCtsNum & 0xA0000000) == 0XA0000000)
            {//将破码固定为211
                g_menuInfo.selectedItem = 211;
                g_sysRunPara.decoderCode = g_CurrentVfo->rx->dcsCtsNum;
            }
            else if(g_CurrentVfo->rx->dcsCtsNum  > 210)
            {
                g_menuInfo.selectedItem =  0;
            }
            else
            {
                g_menuInfo.selectedItem = g_CurrentVfo->rx->dcsCtsNum;
            }
            break;
        case S_TXCTS: 
            g_menuInfo.inputMode = MENU_ONE_CTCSS;
            g_menuInfo.subMaxItem = 51;
            g_menuInfo.selectedItem = loadCtcssVal(g_CurrentVfo->tx->dcsCtsNum);
            break;
        case S_TXDCS: 
            g_menuInfo.inputMode = MENU_ONE_DECODE;
            g_menuInfo.subMaxItem = 212;
            if((g_CurrentVfo->tx->dcsCtsNum & 0xA0000000) == 0XA0000000)
            {//将破码固定为211
                g_menuInfo.selectedItem = 211;
                g_sysRunPara.decoderCode = g_CurrentVfo->rx->dcsCtsNum;
            }
            else if(g_CurrentVfo->tx->dcsCtsNum  > 210)
            {
                g_menuInfo.selectedItem =  0;
            }
            else
            {
                g_menuInfo.selectedItem = g_CurrentVfo->tx->dcsCtsNum;
            }
            break;
        case S_WN: 
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_CurrentVfo->wideNarrow;
            break;
        case S_TXPR: 
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_CurrentVfo->txPower;
            break;
        case S_SPMUTE:
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_CurrentVfo->spMute;
            break;
        case S_TXFORBID:
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.txForbid;
            break;
        case S_SFTD:
            g_menuInfo.subMaxItem = 3;
            g_menuInfo.selectedItem = g_CurrentVfo->freqDir;
            break;
        case S_OFFSE:
            g_menuInfo.inputMode = MENU_ONE_FREQ;
            g_menuInfo.selectedItem = g_CurrentVfo->freqOffset;
            g_menuInfo.inputVal = g_menuInfo.selectedItem;
            break;
        case S_MEMCH: 
        case S_DELCH:
            g_menuInfo.subMaxItem = 999;
            g_menuInfo.inputMode = MENU_ONE_CODE;
            g_menuInfo.selectedItem = g_ChannelVfoInfo.channelNum[g_ChannelVfoInfo.switchAB];
            break;
        case S_VFOSCAN:
            g_menuInfo.inputMode = MENU_ONE_VFOSCAN;
            g_menuInfo.selectedItem = g_radioInform.vfoScanRangeH+g_radioInform.vfoScanRangeL*1000L;
            break;
        case S_SCREV:
            g_menuInfo.subMaxItem = 3;
            g_menuInfo.selectedItem = g_radioInform.scanMode;
            break;
        case S_DTST:
            g_menuInfo.subMaxItem = 4;
            g_menuInfo.selectedItem = g_radioInform.dtmfTone;
            break;
        case S_PTTID: 
            g_menuInfo.subMaxItem = 4;
            g_menuInfo.selectedItem = g_CurrentVfo->pttIdMode;
            break;
        case S_PTTLT:
            g_menuInfo.subMaxItem = 7;
            g_menuInfo.selectedItem = g_radioInform.pttIdTime;
            break;
        case S_SQL:
            g_menuInfo.subMaxItem = 10;
            g_menuInfo.selectedItem = g_radioInform.sqlLevel;
            break;
        case S_SAVE:
            g_menuInfo.subMaxItem = 4;
            g_menuInfo.selectedItem = g_radioInform.saveLevel;
            break;
        case S_VOX:
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.voxSwitch;
            break;
        case S_VOXLV: 
            g_menuInfo.inputMode = MENU_ONE_CODE;
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.voxLevel;
            break;
        case S_VOXDLY:
            g_menuInfo.subMaxItem = 16;
            g_menuInfo.selectedItem = g_radioInform.voxDelay;
            break;
        case S_TOT:
            g_menuInfo.subMaxItem = 13;
            g_menuInfo.selectedItem = g_radioInform.totLevel;
            break;
        case S_LAN:
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.language;
            break;
        case S_VOIC:
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.voiceSw;
            break;
        case S_MENUEXIT:
            g_menuInfo.subMaxItem = 11;
            g_menuInfo.selectedItem = g_radioInform.menuExitTime;
            break;
        case S_BEEP:
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.beepsSwitch;
            break;
        case S_ROGE: 
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.txOffTone;
            break;
        case S_BUSYLOCK: 
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_CurrentVfo->busyLock;
            break;
        case S_PONTYPE:
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.OpFlag1.Bit.b0;
            break;
        case S_PONTONE:
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.OpFlag1.Bit.b2;
            break;
        case S_PONMSG:
            g_menuInfo.inputMode = MENU_ONE_CHAR;
            g_inputbuf.maxLen = 16;
            g_inputbuf.isFirstInput = 0xaa;
        
            //名称未设置
            if(!(powerOnMsg[0] == 0xFF || powerOnMsg[0] == 0x00))
            {
                g_inputbuf.len = sprintf(g_inputbuf.buf,"%s",powerOnMsg);
            }
            g_menuInfo.selectedItem = 0;
            break;
        case S_PWR:
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.pwrPwdFlag;
            break;
        case S_TDR:
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.dualRxFlag;
            break;
        case S_MDF1: 
            g_menuInfo.subMaxItem = 4;
            g_menuInfo.selectedItem = g_radioInform.channleDisA;
            break;
        case S_MDF2: 
            g_menuInfo.subMaxItem = 4;
            g_menuInfo.selectedItem = g_radioInform.channleDisB;
            break;
        case S_RPSTE:
            g_menuInfo.subMaxItem = 11;
            g_menuInfo.selectedItem = g_radioInform.rpste;
            break;
        case S_RPTRL:
            g_menuInfo.subMaxItem = 11;
            g_menuInfo.selectedItem = g_radioInform.rptrl;
            break;
        case S_RTONE: 
            g_menuInfo.subMaxItem = 4;
            g_menuInfo.selectedItem = g_radioInform.rtone;
            break;
        case S_STEP: 
            g_menuInfo.subMaxItem = 8;
            g_menuInfo.selectedItem = g_CurrentVfo->freqStep;
            break;
        case S_TAIL:  
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.tailSwitch;
            break;
        case S_ALMOD: 
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.alarmMode;
            break;
        case S_SK1:
            g_menuInfo.subMaxItem = 7;
            g_menuInfo.selectedItem = g_radioInform.userSideKey[0];
            break;
        case S_SKL1:
            g_menuInfo.subMaxItem = 7;
            g_menuInfo.selectedItem = g_radioInform.userSideKey[1];
            break;
        case S_SK2:
            g_menuInfo.subMaxItem = 7;
            g_menuInfo.selectedItem = g_radioInform.userSideKey[2];
            break;
        case S_ABR:  
            g_menuInfo.subMaxItem = 5;
            g_menuInfo.selectedItem = g_radioInform.autoBack;
            break;
        case S_BRIGHT:
            g_menuInfo.inputMode = MENU_ONE_CODE;
            g_menuInfo.subMaxItem = 5;
            g_menuInfo.selectedItem = g_radioInform.brightness;
            break;
        case S_REFLEX:
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.DisplayStyles&0x01;
            break;
        case S_AUTOLK: 
            g_menuInfo.subMaxItem = 4;
            g_menuInfo.selectedItem = g_radioInform.keyAutoLock;
            break;
        case S_FMINT: 
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = g_radioInform.fmInterrupt&0x01;
            break;
        case S_RESET:  
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = 0;
            break;
        case S_WATCH:
            g_menuInfo.subMaxItem = 1;
            g_menuInfo.selectedItem = 0;
            break;
        case S_INFO:
        default:
            g_menuInfo.inputMode = MENU_ONE_NULL;
            g_menuInfo.subMaxItem = 2;
            g_menuInfo.selectedItem = 0;
            break;
    }
}

extern void Menu_GetFmSubItemPara(U8 menuIndex)
{
    g_menuInfo.inputMode = MENU_ONE_SELECT;
    switch(menuIndex)
    {
        case 0: 
            g_menuInfo.subMaxItem = 30;
            g_menuInfo.selectedItem = g_FMInform.fmChNum;
            break;
        case 1:     
        default:    
            g_menuInfo.subMaxItem = 2;
            if(g_FMInform.fmChVfo == CHAN_MODE)
            {//信道模式
                g_menuInfo.selectedItem = 0xFFFF;
            }
            else
            {
                g_menuInfo.selectedItem = fmInfo.band;
            }
            break;
    }
}

extern void Menu_EnterMode(void)
{
    ResetInputBuf();

    LCD_ClearWorkArea();
    //关闭双守功能
    DualStandbyWorkOFF();
    g_menuInfo.menuMaxItem = MENU_MAX_CNT;
    g_menuInfo.isSubMenu = 0;
    g_menuInfo.fastSelect = 0;
    g_menuInfo.fastInTime = 0;
    g_menuInfo.inputLen = 0;
    g_menuInfo.menuType = 0;
    ResetMenuExitTime();
    g_sysRunPara.sysRunMode = MODE_MENU;
    Menu_GetSubItemPara(g_menuInfo.menuIndex);
    
    Menu_Display();
    VoiceBroadcastWithBeepLock(vo_Menu,BEEP_FASTSW);
}

extern void Menu_ExitMode(void)
{
    if(g_sysRunPara.sysRunMode != MODE_MENU)
    {
        return;
    }

    //保存设置的数据
    Flash_SaveRadioImfosData();
    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
    {
        //保存信道数据/信道名称
        Flash_SaveChannelData(g_ChannelVfoInfo.channelNum[g_ChannelVfoInfo.switchAB],(U8 *)&g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].rxFreq,g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].channelName);
    }
    else
    {
        //保存频率模式数据
        Flash_SaveVfoData(g_ChannelVfoInfo.switchAB);
    }
    g_menuInfo.preIndex = g_menuInfo.menuIndex;

    g_sysRunPara.sysRunMode = MODE_MAIN;
    g_rfRxState = RX_READY;
    g_menuInfo.menuExitTime = 0;

    ResetInputBuf();
    //切换为显示主界面
    DisplayHomePage();
}

extern void Menu_Up(void)
{
    ResetMenuExitTime();
    ResetInputBuf();
    if(g_menuInfo.isSubMenu)
    {
        if(g_menuInfo.selectedItem < g_menuInfo.subMaxItem - 1)
        {
            g_menuInfo.selectedItem++;
        }
        else
        {
            g_menuInfo.selectedItem = 0;
        }
        g_menuInfo.inputVal = 0;
    }
    else
    {
        if(g_menuInfo.menuIndex < g_menuInfo.menuMaxItem - 1)
        {
            g_menuInfo.menuIndex++;
        }
        else
        {
            g_menuInfo.menuIndex = 0;
        }
        if(g_menuInfo.menuType == 1)
        {
            Menu_GetFmSubItemPara(g_menuInfo.menuIndex);
        }
        else
        {
            Menu_GetSubItemPara(g_menuInfo.menuIndex);
        }
    }
    Menu_Display();
}

extern void Menu_Down(void)
{
    ResetMenuExitTime();
    ResetInputBuf();
    if(g_menuInfo.isSubMenu)
    {
        if(g_menuInfo.selectedItem)
        {
            g_menuInfo.selectedItem--;
        }
        else
        {
            g_menuInfo.selectedItem = g_menuInfo.subMaxItem - 1;
        }
        g_menuInfo.inputVal = 0;
    }
    else
    {
        if(g_menuInfo.menuIndex)
        {
            g_menuInfo.menuIndex--;
        }
        else
        {
            g_menuInfo.menuIndex = g_menuInfo.menuMaxItem - 1;
        }
        if(g_menuInfo.menuType == 1)
        {
            Menu_GetFmSubItemPara(g_menuInfo.menuIndex);
        }
        else
        {
            Menu_GetSubItemPara(g_menuInfo.menuIndex);
        }
    }
    Menu_Display();
}

const U16 CtcssCalcTbl[10] = {0,1000,2000,300,400,500,600,700,800,900};  
const U16 CtcssCalcMul[3] = {100,10,1}; 
void CtcssTypeIn(U8 num)
{
    U8 input;
    U8 i;

    input = num - '0';
    if(g_menuInfo.inputLen == 0 || g_menuInfo.inputLen == 4)
    {
        if(input)
        {
            if(input < 3 || input > 5)
            {
                g_menuInfo.inputVal = CtcssCalcTbl[input];
                if(input < 3)
                {
                    g_menuInfo.inputLen = 1;
                }
                else
                {
                    g_menuInfo.inputLen = 2;
                }
            }
            else
            {
                return;
            }
        }
        else
        {
            g_menuInfo.selectedItem = 0;
            g_menuInfo.inputVal = 0;
        }
    }
    else
    {
        g_menuInfo.inputVal += CtcssCalcMul[g_menuInfo.inputLen - 1]*input;
        g_menuInfo.inputLen++;

        if(g_menuInfo.inputVal <= CTCS_TAB[0])
        {
            g_menuInfo.selectedItem = 0;
        }
        else
        {
            for(i = 0; i< 51;i++)
            {
                if(g_menuInfo.inputVal == CTCS_TAB[i])
                {
                    break;
                }
                if(g_menuInfo.inputVal < CTCS_TAB[i])
                {
                    i -= 1;
                    break;
                }
            }
            g_menuInfo.selectedItem = i;
        }
    }
    BeepOut(BEEP_FASTSW);  
}

void FreqTypeIn(U8 input)
{ 
    U8 temp;
    U32 tempFreq;

    g_inputbuf.buf[g_inputbuf.len++] = input;

    if(g_inputbuf.len == 1 || g_inputbuf.len >= 7)  
    {
        g_inputbuf.len = 1;
        g_inputbuf.buf[0] = input;
        g_menuInfo.inputVal = 0;
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

    if(g_inputbuf.len == 6)
    {
        g_inputbuf.buf[g_inputbuf.len] = 0;
        tempFreq = atol(g_inputbuf.buf);

        if(temp == 9)
        {
            g_menuInfo.inputVal = tempFreq*10 - MOD_LIST[temp];
        }
        else
        {
            g_menuInfo.inputVal = tempFreq*10 + MOD_LIST[temp];
        }
    }
}

void ChanlFreqTypeIn(U8 input)
{ 
    U8 temp;
    U32 tempFreq;

    g_inputbuf.buf[g_inputbuf.len++] = input;
    
    if(g_inputbuf.len == 1 || g_inputbuf.len >= 7)
    {
        g_inputbuf.len = 1;
        g_inputbuf.buf[0] = input;
        g_menuInfo.inputVal = 0;
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

	if(g_inputbuf.len == 6)
    {
        g_inputbuf.buf[g_inputbuf.len] = 0;
        tempFreq = atol(g_inputbuf.buf);

        if(temp == 9)
        {
            g_menuInfo.inputVal = tempFreq*100 - MOD_LIST[temp];
        }
        else
        {
            g_menuInfo.inputVal = tempFreq*100 + MOD_LIST[temp];
        }
    }
}

void ScanRangeTypeIn(U8 input)
{ 
    U8 temp;

    g_inputbuf.buf[g_inputbuf.len++] = input;
    
    if(g_inputbuf.len == 1 || g_inputbuf.len >= 7)
    {
        g_inputbuf.len = 1;
        g_inputbuf.buf[0] = input;
        g_menuInfo.inputVal = 0;
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

	if(g_inputbuf.len == 6)
    {
        g_inputbuf.buf[6] = 0;
        g_menuInfo.inputVal = atol(g_inputbuf.buf);
     }
}

void LongIntoI2C(U8 * Ibuf,U32 f_Data)
{   
    U8 i,decData;
    
    for(i = 0; i < 4; i++)
    {
        decData = f_Data%100;
        f_Data = f_Data/100;
        Ibuf[i] = changeIntToHex(decData);
    }
}

extern void SaveRadioFreq(U8 tx)
{
    U32 freq;
    
    if(g_inputbuf.len != 0)
    {
        freq = g_menuInfo.inputVal;
        
        if(CheckFreqInRange(freq) == TRUE)
        {//判断频率是否在范围内
            if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
            {
                if(tx)
                {
                    LongIntoI2C((U8 *)&g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].txFreq,freq);
                }
                else
                {
                    LongIntoI2C((U8 *)&g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].rxFreq,freq);
                }
            }
            else
            {
                VfoFrequency2Buf(freq,(U8 *)&g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].freq,8);
            }
            ChannleVfoDataInit(g_ChannelVfoInfo.switchAB,0);
        }
        else
        {
            Menu_DisplayFreqError();
        }
    }    
}

extern void SaveRadioCtcss(U8 tx)
{
    U16 ctcss;

    if(g_menuInfo.inputVal)
    {
        ctcss = g_menuInfo.inputVal;
    }
    else
    {
        if(g_menuInfo.selectedItem)
        {
            ctcss = CTCS_TAB[g_menuInfo.selectedItem];
        }
        else
        {
            ctcss = 0;
        }
    }

    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
    {
        if(tx)
        {
            g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].txDCSCTSNum = ctcss;
        }
        else
        {
            g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].rxDCSCTSNum = ctcss;
        }
        g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].chFlag3.Byte &= 0xFE;
    }
    else
    {
        if(tx)
        {
            g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].rxDCSCTSNum = ctcss; 
        }
        else
        {
            g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].txDCSCTSNum = ctcss;
        }
        g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].vfoFlag.Byte &= 0xFE;
    }
    ChannleVfoDataInit(g_ChannelVfoInfo.switchAB,0);
}

extern void SaveRadioDcs(U8 tx)
{
    U16 dcs;

    if(g_menuInfo.selectedItem < 211)
    {
        dcs = g_menuInfo.selectedItem;
    }
    else
    {
        dcs = 0;
    }

    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
    {
        if(tx)
        {
            g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].txDCSCTSNum = dcs;
        }
        else
        {
            g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].rxDCSCTSNum = dcs;
        }
        g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].chFlag3.Byte &= 0xFE;
    }
    else
    {
        if(tx)
        {
            g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].rxDCSCTSNum = dcs; 
        }
        else
        {
            g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].txDCSCTSNum = dcs;
        }
        g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].vfoFlag.Byte &= 0xFE;
    }
    ChannleVfoDataInit(g_ChannelVfoInfo.switchAB,0);
}

void OffectFrequency2Buf(U32 freq,U8 *dest,U8 len)
{   
    String buf[9];
    U8 i;
    
    sprintf(buf,"%07d",freq);

    for(i=0;i<len;i++)
    {//将ASC转换为hex
        dest[i] = buf[i] - 0x30;
    }
}

extern void SaveChMemory(void)
{
    U32 addr;
    U16 chNum;   
    U8 chname[NAME_SIZE];
    STR_CHANNEL tempCh;

    chNum = g_menuInfo.selectedItem;

    if(searchSaveFlag)
    {
        memset(&tempCh,0x00,sizeof(STR_CHANNEL));
        tempCh.txDCSCTSNum = 0;
        tempCh.rxDCSCTSNum = 0;
        tempCh.chFlag3.Byte = 0x06;
        if(searchFreqImofs.dcsCtsType == SUBAUDIO_CTS)
        {
            tempCh.txDCSCTSNum = searchFreqImofs.CtsResult;
            tempCh.rxDCSCTSNum = searchFreqImofs.CtsResult; 
            tempCh.chFlag3.Byte &= 0xFE;
        }
        else if(searchFreqImofs.dcsCtsType == SUBAUDIO_DCS_N)
        {
            if(searchFreqImofs.dcsIsStandard == 1)
            {
                tempCh.txDCSCTSNum = searchFreqImofs.CtsResult;
                tempCh.rxDCSCTSNum = searchFreqImofs.CtsResult; 
                tempCh.chFlag3.Byte &= 0xFE;
            }
            else
            {
                tempCh.decoderCode = searchFreqImofs.CtsResult;
                tempCh.decoderCode &= 0X007FFFFF;
                tempCh.decoderCode |= 0xA0000000; // 表示学习跳频
                tempCh.chFlag3.Byte |= 0X01;//破码标志
            }
        }
        tempCh.dtmfgroup = 0;
        tempCh.pttID = 0;
        tempCh.txPower = 0;
        
        LongIntoI2C((U8 *)&tempCh.rxFreq,searchFreqImofs.freq);
        LongIntoI2C((U8 *)&tempCh.txFreq,searchFreqImofs.freq);

        addr = CHAN_ADDR + chNum*CHAN_SIZE;
        SpiFlash_ReadBytes(addr,chname,NAME_SIZE); 
        addr += NAME_ADDR_SHIFT;
    }
    else
    {
        if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
        {
            memcpy(chname,g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].channelName,NAME_SIZE);
            tempCh = g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB];
        }
        else
        {
    
            addr = CHAN_ADDR + chNum*CHAN_SIZE + NAME_ADDR_SHIFT;
            SpiFlash_ReadBytes(addr,chname,NAME_SIZE);
            
            memset(&tempCh,0x00,sizeof(STR_CHANNEL));
    
            if(CheckChannelActive(chNum, 0))
            {
                SpiFlash_ReadBytes(chNum*CHAN_SIZE+CHAN_ADDR , (U8 *)&tempCh.rxFreq, sizeof(STR_CHANNEL));
                
                tempCh.txDCSCTSNum = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqTx.dcsCtsNum;
                LongIntoI2C((U8 *)&tempCh.txFreq,g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqTx.frequency);
                VoiceBroadcastWithBeepLock(vo_Txmemory,BEEP_FMUP);
            }
            else
            {
                tempCh.txDCSCTSNum = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqTx.dcsCtsNum;
                tempCh.rxDCSCTSNum = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.dcsCtsNum;
                
                LongIntoI2C((U8 *)&tempCh.rxFreq,g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency);
                LongIntoI2C((U8 *)&tempCh.txFreq,g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqTx.frequency);
                VoiceBroadcastWithBeepLock(vo_Rxmemory,BEEP_FMDOWN);
            }
    
            tempCh.chFlag3.Bit.b6 = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].wideNarrow;//宽窄带
            tempCh.chFlag3.Bit.b0 = g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].vfoFlag.Bit.b0;
    		tempCh.txPower = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].txPower;//发射功率
            tempCh.chFlag3.Bit.b3 = g_radioInform.txBusyLock;//繁忙锁定
            tempCh.dtmfgroup = g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].dtmfgroup;//信令码
    		tempCh.pttID = g_radioInform.pttIdMode;//PTT_ID
    		tempCh.chFlag3.Bit.b2 = 1;//扫描添加默认ON
    		tempCh.chFlag3.Byte |= 0x02;
        }
    }
    Flash_ModifyChannelData(chNum,(U8 *)&tempCh.rxFreq,chname);

    g_ChannelVfoInfo.currentChannelNum = chNum; 
    g_ChannelVfoInfo.channelNum[g_ChannelVfoInfo.switchAB] = g_ChannelVfoInfo.currentChannelNum;
    Flash_SaveSystemRunData();

    //使能当前信道
    g_ChannelVfoInfo.haveChannel = 1;
    ListFlagSet(g_ChannelVfoInfo.chanActiveList,chNum,1);

    if(tempCh.chFlag3.Bit.b2)
    {
        g_ChannelVfoInfo.haveScan = 1;
        ListFlagSet(g_ChannelVfoInfo.scanList,chNum,1);
    }

    if(g_ChannelVfoInfo.chVfoInfo[0].chVfoMode == CHAN_MODE && g_ChannelVfoInfo.channelNum[0] == chNum)
    {
        ChannleVfoDataInit(0,1);
    }

    if(g_ChannelVfoInfo.chVfoInfo[1].chVfoMode == CHAN_MODE && g_ChannelVfoInfo.channelNum[1] == chNum)
    {
        ChannleVfoDataInit(1,1);
    }

    if(searchSaveFlag)
    {
        searchSaveFlag = 0;
        Menu_ExitMode();
    }
}


extern void SaveChDelete(void)
{
    U16 chNum;
    
    chNum = g_menuInfo.selectedItem;
    
    if(CheckChannelActive(chNum, 0))
    {
        Flash_DeleteChannelData(chNum);

        //清除当前信道
        ListFlagSet(g_ChannelVfoInfo.chanActiveList,chNum,0);
        ListFlagSet(g_ChannelVfoInfo.scanList,chNum,0);

        g_ChannelVfoInfo.haveChannel = 0;

        for(chNum = 0;chNum<125;chNum++)
        {
            if(g_ChannelVfoInfo.chanActiveList[chNum] != 0)
            {
                g_ChannelVfoInfo.haveChannel = 1;
                break;
            }
        }

        if(g_ChannelVfoInfo.haveChannel == 0)
        {
            //显示请等待
            if(g_radioInform.language == LANG_CN)
            {
                sprintf(disBuf,"%-*.*s\n\r",16,16,"请等待...");
            }
            else
            {
                sprintf(disBuf,"%-*.*s\n\r",16,16,"Wait...");
            }
            LCD_DisplayText(47, 0, (U8 *)disBuf, FONTSIZE_16x16,LCD_DIS_NORMAL);
            LCD_UpdateWorkAre();
            
            //初始化为默认信道信息
            ResetChannelData();
            NVIC_SystemReset();//复位系统
        }
        else
        {
            //信道删除后需要重新加载有效信道，否则当AB设置为相同信道号时会出错
            if( CheckChannelActive(g_ChannelVfoInfo.channelNum[0], 0) == CHAN_DISABLE )
            {
                g_ChannelVfoInfo.channelNum[0] = SeekActiveChannel_Up(g_ChannelVfoInfo.channelNum[0], 0);
                ChannleVfoDataInit(0,1);
            }
    
            if( CheckChannelActive(g_ChannelVfoInfo.channelNum[1], 0) == CHAN_DISABLE )
            {
                g_ChannelVfoInfo.channelNum[1] = SeekActiveChannel_Up(g_ChannelVfoInfo.channelNum[1], 0);
                ChannleVfoDataInit(1,1);
            }
        }
    }
    VoiceBroadcastWithBeepLock(vo_Confirm,BEEP_FASTSW);
}

extern void SaveVfoScanRanger(void)
{
    U16 freqL,freqH;
    
    if(g_inputbuf.len != 0)
    {
        freqL = g_menuInfo.inputVal / 1000;
        freqH = g_menuInfo.inputVal % 1000;

        if(CheckInputScanRange(freqL*10,freqH*10) == OK)
        {
             g_radioInform.vfoScanRangeL = freqL;
             g_radioInform.vfoScanRangeH = freqH;
        }
        else
        {
            Menu_DisplayFreqError();
        }
    }
}

extern void EnterResetMode(void)
{    
    //LCD_ClearWorkArea();
    memset(disBuf,0x00,17);
    if(g_radioInform.language == LANG_CN)
    {
        sprintf(disBuf,"%-*.*s\n\r",16,16,"确认初始化?");
    }
    else
    {
        sprintf(disBuf,"%-*.*s\n\r",16,16,"Sure to Reset?");
    }
    LCD_DisplayText(47, 0, (U8 *)disBuf, FONTSIZE_16x16,LCD_DIS_NORMAL);
    LCD_UpdateWorkAre();

    while(1)
    {
        if(g_10msFlag)
        {
            App_10msTask();
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

        if(g_keyScan.keyEvent == KEYID_EXIT)
        {
            Menu_ExitMode();
            BeepOut(BEEP_FMDOWN);
            return;
        }

        if(g_keyScan.keyEvent == KEYID_MENU)
        {
            BeepOut(BEEP_FMUP);
            break;
        }

        if(g_sysRunPara.sysRunMode != MODE_MENU)
        {//按PTT直接退出菜单
            return;
        }
        Audio_PlayTask();
    }

    //显示请等待
    if(g_radioInform.language == LANG_CN)
    {
        sprintf(disBuf,"%-*.*s\n\r",16,16,"  请等待...  ");
    }
    else
    {
        sprintf(disBuf,"%-*.*s\n\r",16,16,"Please Wait...");
    }
    LCD_DisplayText(47, 0, (U8 *)disBuf, FONTSIZE_16x16,LCD_DIS_NORMAL);
    LCD_UpdateWorkAre();

    if(g_menuInfo.selectedItem == 1)
    {
        ResetChannelData();
        ResetVfoModeData();
        ResetRadioFunData();
    }
    else
    {
        ResetVfoModeData();
        ResetRadioFunData();
    }
    
    //延时500ms后重启
    DelayMs(500);
    //复位系统
    NVIC_SystemReset();
}


extern void Menu_SaveSelectItem(U8 menuIndex)
{
    switch(menuIndex)
    {
        case S_CHNAME:
            if(g_inputbuf.len > 12)
            {
                g_inputbuf.len = 12;
            }
            memset(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].channelName,0xff,12);
            memcpy(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].channelName,g_inputbuf.buf, 12);
            LCD_UpdateWorkAre();
            break;
        case S_RXFREQ:
            SaveRadioFreq(0);
            break;
        case S_TXFREQ:
            SaveRadioFreq(1);
            break;
        case S_RXCTS:
            SaveRadioCtcss(0);
            break;    
        case S_RXDCS:
            SaveRadioDcs(0);
            break;
        case S_TXCTS: 
            SaveRadioCtcss(1);
            break;
        case S_TXDCS: 
            SaveRadioDcs(1);
            break;
        case S_WN: 
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].wideNarrow = g_menuInfo.selectedItem;
            if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
            {
                 g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].chFlag3.Bit.b6 = g_menuInfo.selectedItem;
            }
            else
            {
                g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].vfoFlag.Bit.b6  = g_menuInfo.selectedItem;
            }
            break;
        case S_TXPR:
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].txPower = g_menuInfo.selectedItem;
            if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
            {
                 g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].txPower = (g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].txPower &0x0f) | g_menuInfo.selectedItem;
            }
            else
            {
                g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].txPower = (g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].txPower & 0x0f) | g_menuInfo.selectedItem;
            }
            break;
        case S_SPMUTE:
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].spMute = g_menuInfo.selectedItem;
            if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
            {
                 g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].chFlag3.Bit.spMute = g_menuInfo.selectedItem;
            }
            else
            {
                g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].spMute = g_menuInfo.selectedItem;
            }
            break;
        case S_TXFORBID:
            g_radioInform.txForbid = g_menuInfo.selectedItem;
            break;
        case S_SFTD:
            if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
            {
                VoiceBroadcastWithBeepLock(vo_Cancel,BEEP_EXITMENU);
            }
            else
            {
                g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].dtmfgroup &= 0x1F;
                if(g_menuInfo.selectedItem == 1)
                {
                    g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].dtmfgroup |= 0x20;
                }
                else if(g_menuInfo.selectedItem == 2)
                {
                    g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].dtmfgroup |= 0x40;
                }
                else
                {
                }
                ChannleVfoDataInit(g_ChannelVfoInfo.switchAB,0);
            }
            break;
        case S_OFFSE:
            if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == CHAN_MODE)
            {
                VoiceBroadcastWithBeepLock(vo_Cancel,BEEP_EXITMENU);
            }
            else
            {
                if(g_inputbuf.len != 0)
                {
                    g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqOffset = g_menuInfo.inputVal;
                }
                g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqOffset = (g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqOffset+ 20)/50*50;

                OffectFrequency2Buf(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqOffset,g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].Offset,7);
                ChannleVfoDataInit(g_ChannelVfoInfo.switchAB,0);
            }
            break;
        case S_MEMCH: 
            SaveChMemory();
            break;
        case S_DELCH:
            SaveChDelete();
            break;
        case S_VFOSCAN:
            SaveVfoScanRanger();
            break;
        case S_SCREV:
            g_radioInform.scanMode = g_menuInfo.selectedItem;
            break;
        case S_DTST:
            g_radioInform.dtmfTone = g_menuInfo.selectedItem;
            break;
        case S_PTTID: 
            if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == VFO_MODE)
            {
                g_radioInform.pttIdMode = g_menuInfo.selectedItem;
            }
            else
            {
                g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].pttID = g_menuInfo.selectedItem;
            }
            
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].pttIdMode = g_menuInfo.selectedItem;
            break;
        case S_PTTLT:
            g_radioInform.pttIdTime = g_menuInfo.selectedItem;
            break;
        case S_SQL:
            g_radioInform.sqlLevel = g_menuInfo.selectedItem;
            break;
        case S_SAVE: 
            g_radioInform.saveLevel = g_menuInfo.selectedItem;
            break;
        case S_VOX: 
            g_radioInform.voxSwitch = g_menuInfo.selectedItem;
            break;
        case S_VOXLV: 
            g_radioInform.voxLevel = g_menuInfo.selectedItem;
            break;
        case S_VOXDLY:
            g_radioInform.voxDelay = g_menuInfo.selectedItem;
            break;
        case S_TOT:
            g_radioInform.totLevel = g_menuInfo.selectedItem;
            break;
        case S_LAN:
            g_radioInform.language = g_menuInfo.selectedItem;
            break;
        case S_VOIC:
            g_radioInform.voiceSw = g_menuInfo.selectedItem;
            break;
        case S_MENUEXIT:
            g_radioInform.menuExitTime= g_menuInfo.selectedItem;
            ResetMenuExitTime();
            break;
        case S_BEEP:
            g_radioInform.beepsSwitch = g_menuInfo.selectedItem;
            break;
        case S_ROGE: 
            g_radioInform.txOffTone = g_menuInfo.selectedItem;
            break;
        case S_BUSYLOCK:   
            if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == VFO_MODE)
            {
                g_radioInform.txBusyLock = g_menuInfo.selectedItem;
            }
            else
            {
                g_ChannelVfoInfo.channelInfo[g_ChannelVfoInfo.switchAB].chFlag3.Bit.b3 = g_menuInfo.selectedItem;
            }
            
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].busyLock = g_menuInfo.selectedItem;
            break;
        case S_PONTYPE:
            g_radioInform.OpFlag1.Bit.b0 = g_menuInfo.selectedItem;
            break;
        case S_PONTONE:
            g_radioInform.OpFlag1.Bit.b2 = g_menuInfo.selectedItem;
            break;
        case S_PONMSG:
            if(g_inputbuf.len > 16)
            {
                g_inputbuf.len = 16;
            }
            memset(powerOnMsg,0x00,16);
            memcpy(powerOnMsg,g_inputbuf.buf,g_inputbuf.len);
            LCD_UpdateWorkAre();
            break;
        case S_PWR:
            g_radioInform.pwrPwdFlag = g_menuInfo.selectedItem;
            break;
        case S_TDR:
            g_radioInform.dualRxFlag = g_menuInfo.selectedItem;
            break;
        case S_MDF1: 
            g_radioInform.channleDisA = g_menuInfo.selectedItem;
            break;
        case S_MDF2: 
            g_radioInform.channleDisB = g_menuInfo.selectedItem;
            break;
        case S_RPSTE:
            g_radioInform.rpste = g_menuInfo.selectedItem;
            break;
        case S_RPTRL:
            g_radioInform.rptrl = g_menuInfo.selectedItem;
            break;
        case S_RTONE: 
            g_radioInform.rtone = g_menuInfo.selectedItem;
            break;
        case S_STEP: 
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqStep = g_menuInfo.inputVal;
            g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].STEP = g_menuInfo.inputVal;
            break;
        case S_TAIL:  
            g_radioInform.tailSwitch = g_menuInfo.selectedItem;
            break;
        case S_ALMOD: 
            g_radioInform.alarmMode = g_menuInfo.selectedItem;
            break;
        case S_SK1:
            g_radioInform.userSideKey[0] = g_menuInfo.selectedItem;
            break;
        case S_SKL1:
            g_radioInform.userSideKey[1] = g_menuInfo.selectedItem;
            break;
        case S_SK2:
            g_radioInform.userSideKey[2] = g_menuInfo.selectedItem;
            break;
        case S_ABR:  
            g_radioInform.autoBack = g_menuInfo.selectedItem;
            break;
        case S_BRIGHT:
            g_radioInform.brightness = g_menuInfo.selectedItem;
            SC5620_SetContpastRatio(g_radioInform.brightness);
            break;
        case S_REFLEX:
            g_radioInform.DisplayStyles = g_menuInfo.selectedItem;
            LCD_UpdateFullScreen();
            break;
        case S_AUTOLK: 
            g_radioInform.keyAutoLock = g_menuInfo.selectedItem;
            break;
        case S_FMINT:  
            g_radioInform.fmInterrupt = g_menuInfo.selectedItem;
            break;
        case S_RESET:    
            EnterResetMode();
            break;
        case S_WATCH:
            Menu_ExitMode();
            EnterStopWatchMode();
            break;
        case S_INFO:
        default:
            break;
    }
}

extern void Menu_SaveFmSelectItem(U8 menuIndex)
{
    switch(menuIndex)
    {
        case 0:
           g_FMInform.FmCHs[g_menuInfo.selectedItem] = (U16)fmInfo.freq;
            g_FMInform.fmChNum = g_menuInfo.selectedItem;
            FmCheckChannelActive();
            break;
        case 1:
        default:
            fmInfo.band = g_menuInfo.selectedItem;
            FmBandConfig();
            break;
    }
}

extern void Menu_PriorLevel(void)
{
    ResetMenuExitTime();
    ResetInputBuf();
    if(g_menuInfo.isSubMenu)
    {
        g_menuInfo.isSubMenu = 0;
        g_menuInfo.fastSelect = 0;
        if(g_menuInfo.menuType == 1)
        {
            Menu_GetFmSubItemPara(g_menuInfo.menuIndex);
        }
        else
        {
            Menu_GetSubItemPara(g_menuInfo.menuIndex);
        }
        if(g_menuInfo.inputMode == MENU_ONE_CHAR)
        {
             LCD_ClearWorkArea();
        }
        Menu_Display();
        BeepOut(BEEP_EXITMENU);
    }
    else
    {
        if(g_menuInfo.menuType == 1)
        {
            g_menuInfo.menuIndex = g_menuInfo.preIndex;
            ResumeFmMode();
        }
        else
        {
            Menu_ExitMode();
            BeepOut(BEEP_EXITMENU);
        }
    }
}


extern void Menu_EnterNextLevel(void)
{
    ResetMenuExitTime();

    if(g_menuInfo.isSubMenu)
    {//选择菜单选项模式
        if(g_menuInfo.inputMode == MENU_ONE_FREQ && (g_inputbuf.len != 0 && g_inputbuf.len != 6))
        {
            BeepOut(BEEP_NULL);
            return;
        }
        
        if((g_menuInfo.inputMode == MENU_CH_FREQ || g_menuInfo.inputMode == MENU_ONE_VFOSCAN) && (g_inputbuf.len != 0 && g_inputbuf.len != 6))
        {//输入频率时特殊处理
            BeepOut(BEEP_NULL);
            return;
        }
        VoiceBroadcastWithBeepLock(vo_Confirm,BEEP_FASTSW);

        //执行菜单保存函数
        if(g_menuInfo.menuType == 1)
        {
            Menu_SaveFmSelectItem(g_menuInfo.menuIndex);
        }
        else
        {
            Menu_SaveSelectItem(g_menuInfo.menuIndex);
        }

        if(g_sysRunPara.sysRunMode != MODE_MENU)
        {//执行菜单后，不在菜单模式，直接退出菜单
            return;
        }
        g_rfRxState = RX_READY;
        g_menuInfo.isSubMenu = 0;
        g_menuInfo.fastSelect = 0;

        ResetInputBuf();
        if(g_menuInfo.menuType == 1)
        {
            Menu_GetFmSubItemPara(g_menuInfo.menuIndex);
        }
        else
        {
            Menu_GetSubItemPara(g_menuInfo.menuIndex);
        }
        Menu_Display();
    }
    else
    {//选择菜单模式
        //ResetInputBuf();
        if(g_menuInfo.inputMode == MENU_ONE_NULL)
        {//当前菜单只用于显示内容，不带操作时，直接返回
            VoiceBroadcastWithBeepLock(vo_Cancel,BEEP_EXITMENU);
            return;
        }
        
        if((g_menuInfo.menuType == 1) && g_menuInfo.selectedItem == 0xFFFF)
        {
            VoiceBroadcastWithBeepLock(vo_Cancel,BEEP_ERROR);
            return;
        }
        else if(g_menuInfo.inputMode == MENU_ONE_CHAR)
        {
            if(g_menuInfo.selectedItem == 0xFFFF)
            {
                VoiceBroadcastWithBeepLock(vo_Cancel,BEEP_ERROR);
                return;
            }
            //清除整个屏幕显示
            LCD_UpdateWorkAre();

        }
        else
        {
        }
        g_menuInfo.isSubMenu = 1;
        g_menuInfo.fastSelect = 0;
        g_menuInfo.inputVal = 0;
        g_menuInfo.inputLen = 0;

        Menu_Display();
        VoiceBroadcastWithBeepLock(MenuList[g_menuInfo.menuIndex].voiceId,BEEP_FASTSW);    
    }     
    
}

extern void CheckExitMenu(void)
{
    if(g_sysRunPara.sysRunMode != MODE_MENU)
    {
        return;
    }
    
    if(g_menuInfo.menuExitTime == 0)
    {
        if(g_menuInfo.menuType == 1)
        {
            g_menuInfo.menuIndex = g_menuInfo.preIndex;
            ResumeFmMode();
        }
        else
        {
            Menu_ExitMode();
        }
    }
}

extern void Menu_KeyDigitalInput(U8 input)
{
    U16 select,selectVal;
    U16 maxItem;
    U8 flag = 0;

    ResetMenuExitTime();

    if(g_menuInfo.isSubMenu)
    {
        maxItem = g_menuInfo.subMaxItem;
        selectVal = g_menuInfo.selectedItem;
    }
    else
    {
        maxItem = g_menuInfo.menuMaxItem;
        selectVal = g_menuInfo.menuIndex;
    }

    switch(g_menuInfo.inputMode)
    {
        case MENU_ONE_CHAR:              //输入字母或者拼音
            if(g_menuInfo.isSubMenu)
            {
                if (NumToChar(input) != OK)
                {
                    BeepOut(BEEP_NULL);
                }
                break;
            }
        case MENU_ONE_CTCSS:             //模拟亚音频选择和输入
            if(g_menuInfo.isSubMenu)
            {
                CtcssTypeIn(input);
                break;
            }
        case MENU_ONE_FREQ:              //频率输入
            if(g_menuInfo.isSubMenu)
            {
                FreqTypeIn(input);  
                break;
            }
        case MENU_CH_FREQ:
            if(g_menuInfo.isSubMenu)
            {
                ChanlFreqTypeIn(input);  
                break;
            }
        case MENU_ONE_VFOSCAN:
            if(g_menuInfo.isSubMenu)
            {
                ScanRangeTypeIn(input);  
                break;
            }
        case MENU_ONE_CODE:
            if(g_menuInfo.isSubMenu)
            {
                flag = 1;
                maxItem += 1;
            }
        case MENU_ONE_DECODE:
            if((g_menuInfo.isSubMenu) && (flag == 0) && (g_sysRunPara.decoderCode == 0))
            {//flag用于判断是否在code模式时，没有破码时，最大数量需要减1
                maxItem -= 1;
            }
        case MENU_ONE_SELECT:
        default:
            if(g_menuInfo.fastInTime == 0)
            {
                g_menuInfo.fastSelect = 0;
            }
            //用于菜单改变时快速选择菜单输入
            if(flag)
            {
                if((selectVal+1) != g_menuInfo.fastSelect)
                {
                    g_menuInfo.fastSelect = 0;
                }
            }
            else
            {
                if(selectVal != g_menuInfo.fastSelect)
                {
                    g_menuInfo.fastSelect = 0;
                }
            }
        
            select = input-0x30;
            Audio_PlayChanNum(select);
            g_menuInfo.fastSelect = g_menuInfo.fastSelect*10 + select;

            if(g_menuInfo.fastSelect >= maxItem)
            {
                if(select < maxItem)
                {
                    g_menuInfo.fastSelect = select;
                }
                else
                {
                    g_menuInfo.fastSelect = 0;
                }
            }
            
            if(flag)
            {
                if(g_menuInfo.fastSelect == 0)
                {
                    BeepOut(BEEP_NULL);
                    return;
                }
                selectVal = g_menuInfo.fastSelect - 1;
            }
            else
            {
                selectVal = g_menuInfo.fastSelect;
            }

            if(g_menuInfo.isSubMenu == 0)
            {//选择菜单模式
                g_menuInfo.menuIndex = selectVal;

                if(g_menuInfo.menuType == 1)
                {
                    Menu_GetFmSubItemPara(g_menuInfo.menuIndex); 
                }
                else
                {
                    Menu_GetSubItemPara(g_menuInfo.menuIndex); 
                }
            }
            else
            {
                g_menuInfo.selectedItem = selectVal;
            }
            BeepOut(BEEP_FASTSW);
            break;
    }
    g_menuInfo.fastInTime = 15;
    Menu_Display();
}
extern void DcsSwitchPolarity(void)
{ 
    ResetMenuExitTime();

    g_menuInfo.fastSelect = 0;

    if(g_menuInfo.selectedItem == 0 || g_menuInfo.selectedItem == 211)
    {//如果是关闭状态或者破码状态则不切换
        return;
    }

    if(g_menuInfo.selectedItem <= 105)
    {//正码切换为反码
        g_menuInfo.selectedItem += 105;
        BeepOut(BEEP_FMDOWN);
    }
    else
    {//反码切换为正码
        g_menuInfo.selectedItem -= 105;
        BeepOut(BEEP_FMUP);
    }
    Menu_Display();
}

extern void EnterFMMenu(void)
{
    LCD_ClearWorkArea();
    
    ResetInputBuf();
    DualStandbyWorkOFF();
    g_menuInfo.preIndex = g_menuInfo.menuIndex;
    g_menuInfo.menuIndex = 0;
    g_menuInfo.menuMaxItem = 2;
    g_menuInfo.isSubMenu = 0;
    g_menuInfo.fastSelect = 0;
    g_menuInfo.fastInTime = 0;
    g_menuInfo.inputLen = 0;
    g_menuInfo.menuType = 1;
    ResetMenuExitTime();
    g_sysRunPara.sysRunMode = MODE_MENU;
    Menu_GetFmSubItemPara(g_menuInfo.menuIndex);
    Menu_Display();
}

extern void FastEnterChMemMenu(void)
{
    LCD_ClearWorkArea();

    ResetInputBuf();
    DualStandbyWorkOFF();
    g_menuInfo.preIndex = g_menuInfo.menuIndex;
    g_menuInfo.menuIndex = S_MEMCH;
    g_menuInfo.menuMaxItem = MENU_MAX_CNT;
    g_menuInfo.isSubMenu = 0;
    g_menuInfo.fastSelect = 0;
    g_menuInfo.fastInTime = 0;
    g_menuInfo.inputLen = 0;
    g_menuInfo.menuType = 0;
    g_sysRunPara.sysRunMode = MODE_MENU;
    Menu_GetSubItemPara(g_menuInfo.menuIndex);
    
    Menu_EnterNextLevel();
}

extern U8 checkInputCharMode(void)
{
    if(g_menuInfo.isSubMenu == 1 && g_menuInfo.inputMode == MENU_ONE_CHAR)
    {
        return 1;
    }
    return  0;
}

extern void KeyProcess_Menu(U8 keyEvent)
{
    U8 numEvent;
    
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
            numEvent = keyEvent - KEYID_0 + 0x30;
            Menu_KeyDigitalInput(numEvent);
            break;
        case KEYID_MENU:
            if((g_menuInfo.inputMode == MENU_ONE_CHAR) && (pyBuf.pos != 0))
            {
                EnterSelectMode();
            }
            else
            {
                Menu_EnterNextLevel();
            }
            break;
        case KEYID_UP:
            if(checkInputCharMode() == 1)
            {
                T9Select(1);
                MenuShowInputChar();
            }
            else
            {
                Menu_Up();
            }
            BeepOut(BEEP_FMUP);
            break;
        case KEYID_UP_CONTINUE:
            if(FastChangeVoice == 0)
            {
                FastChangeVoice = 1;
                BeepOut(BEEP_FMUP);
            }
            if(checkInputCharMode() == 1)
            {
                T9Select(1);
                MenuShowInputChar();
            }
            else
            {
                Menu_Up();
            }
            break;
        case KEYID_DOWN:
            if(checkInputCharMode() == 1)
            {
                T9Select(0);
                MenuShowInputChar();
            }
            else
            {
                Menu_Down();
            }
            BeepOut(BEEP_FMDOWN);
            break;
        case KEYID_DOWN_CONTINUE:
            if(FastChangeVoice == 0)
            {
                FastChangeVoice = 1;
                BeepOut(BEEP_FMDOWN);
            }
            if(checkInputCharMode() == 1)
            {
                T9Select(0);
                MenuShowInputChar();
            }
            else
            {
                Menu_Down();
            }
            break; 
        case KEYID_EXIT:
            if(g_menuInfo.inputMode == MENU_ONE_CHAR && (BackSpace() == OK))
            {
                MenuShowInputChar();
                BeepOut(BEEP_FMDOWN);
            }
            else
            {
                Menu_PriorLevel();
            }    
            break;
        case KEYID_STAR:
            EnterRemoteScanQTMode();
            break;     
        case KEYID_WELL:
            if (g_menuInfo.inputMode == MENU_ONE_DECODE)
            {//输入数字亚音模式切换正反码使用
                DcsSwitchPolarity();
            }
            else
            {
                if(checkInputCharMode() == 1)
                {
                    ChangeInputType();
                }
                BeepOut(BEEP_NULL);
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

