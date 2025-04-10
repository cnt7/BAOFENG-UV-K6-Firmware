#include "includes.h"

const U16 stepList[9] = {250,500,625,1000,1250,2000,2500,5000,10000};
//定义频率修正补偿值
const U8 MOD_LIST[10] = {0,25,50,75,100,0,25,50,75,25};

extern void RadioConfig_Init(void)
{
    Flash_ReadSystemRunData();
    Flash_ReadRadioImfosData();
    Flash_ReadDebugImfosData();
    DtmfInfoInit();
    Flash_ReadFmData();
    FmCheckChannelActive();
}

void ListFlagSet(U8  *listbuf,U16 Data,U8  val)
{
    if(val)
    {
        listbuf[Data>>3]|= (1<<(Data&0x07));
    }
    else
    {
        listbuf[Data>>3]&= ~(1<<(Data&0x07));
    }
}

U8 GetCTSDCSType(U16 CTSDCSNum)
{
    if( CTSDCSNum > 250 )
	{
	    return SUBAUDIO_CTS;
	}
	else if( CTSDCSNum >= 1 )
	{
	    if( CTSDCSNum <= 105 )
	    {
            return SUBAUDIO_DCS_N;
		}
		else
		{
            return SUBAUDIO_DCS_I;
		}
	}
	else
	{
        return SUBAUDIO_NONE;
	}
}

Boolean CheckFreqInRange(U32 freq)
{
    U32 tempFreq;
    
    tempFreq = freq / 10000;
    
    if((tempFreq >= 1080) && (tempFreq < 1360))
    {
        if(g_rfMoudel.amRxEn == 0)
        {//不允许航空接收
            return FALSE;
        }
        return TRUE;
    }
    else if((tempFreq >= bandRang.bandFreq.vhf2L) && (tempFreq < bandRang.bandFreq.vhf2H))
    { 
        return TRUE;
    }
    else if((tempFreq >= bandRang.bandFreq.B350ML) && (tempFreq < bandRang.bandFreq.B350MH))
    { 
        return TRUE;
    }
    else if((tempFreq >= bandRang.bandFreq.vhfL) && (tempFreq < bandRang.bandFreq.vhfH))
    { 
        return TRUE;
    }
    else if((tempFreq >= bandRang.bandFreq.uhfL) && (tempFreq < bandRang.bandFreq.uhfH))
    { 
        return TRUE;
    }

    return FALSE;
}

Boolean CheckChannelActive(U16 curChanNum,U8  isScan)
{
    if(isScan)
    {
        return g_ChannelVfoInfo.scanList[curChanNum/8]&(1<<(curChanNum%8));
    }
    else
    {
        return g_ChannelVfoInfo.chanActiveList[curChanNum/8]&(1<<(curChanNum%8));
    }
}

U16 SeekActiveChannel_Up(U16 curChanNum, U8  isScan)
{
    U16 i,bufChanNum;

    bufChanNum = (curChanNum+1) % 999;

    for(i=0;i<999;i++)
    {
        if(CheckChannelActive(bufChanNum,isScan))
        {
            return bufChanNum;
        }
        else
        {
            bufChanNum = (bufChanNum + 1) % 999;
        }
    }
    return 0xFFFF;
}

U16 SeekActiveChannel_Down(U16 curChanNum, U8  isScan)
{
    U16 i;
    U16 bufChanNum = 0;

    if( curChanNum == 0 )
    {
        bufChanNum = 998;
    }
    else
    {
        bufChanNum = curChanNum - 1;
    }

    for(i=0;i<999;i++)
    {
        if(CheckChannelActive(bufChanNum,isScan))
        {
            return bufChanNum;
        }
        else
        {
            if( bufChanNum == 0 )
            {
                bufChanNum = 998;
            }
            else
            {
                bufChanNum = bufChanNum - 1;
            }
        }
    }
    return 0xFFFF;
}

extern void ChannelCheckActiveAll(void)
{
    U16  i;
    U16 addr=CHAN_ADDR;
    U8  readData[4];
    U32 bufFreq;
    U8  j;

    memset(g_ChannelVfoInfo.chanActiveList,0x00,sizeof(g_ChannelVfoInfo.chanActiveList));
    memset(g_ChannelVfoInfo.scanList,0x00,sizeof(g_ChannelVfoInfo.scanList));

    g_ChannelVfoInfo.haveChannel = 0;
    g_ChannelVfoInfo.haveScan = 0;
    for(i=0;i<999;i++)
    {
        SpiFlash_ReadBytes(addr, readData, 4);

        if((readData[0]  != 0xff) && (readData[3] != 0))
        {
            bufFreq = 0;
            for(j = 4; j > 0; j--)
            {
                readData[j-1] = changeHexToInt(readData[j-1]);
                bufFreq = (bufFreq * 100) + readData[j-1];
            }

            if( CheckFreqInRange(bufFreq) == TRUE)
            {
                ListFlagSet(g_ChannelVfoInfo.chanActiveList,i,1);
                g_ChannelVfoInfo.haveChannel = 1;
                SpiFlash_ReadBytes(addr+15, readData, 1);

                if(readData[0]&BIT2)
                {
                    ListFlagSet(g_ChannelVfoInfo.scanList,i,1);
                    g_ChannelVfoInfo.haveScan = 1;
                }
            }
        }
        addr += CHAN_SIZE;
    }
    
    if( g_ChannelVfoInfo.haveChannel == 1 )
    {
        if(g_ChannelVfoInfo.channelNum[0] > 999)
        {
            g_ChannelVfoInfo.channelNum[0] = 0;
        }
        if(g_ChannelVfoInfo.channelNum[1] > 999)
        {
            g_ChannelVfoInfo.channelNum[1] = 0;
        }

        //检查信道是否有效
        if( CheckChannelActive(g_ChannelVfoInfo.channelNum[0], 0) == CHAN_DISABLE )
        {
            g_ChannelVfoInfo.channelNum[0] = SeekActiveChannel_Up(g_ChannelVfoInfo.channelNum[0], 0);
        }

        if( CheckChannelActive(g_ChannelVfoInfo.channelNum[1], 0) == CHAN_DISABLE )
        {
            g_ChannelVfoInfo.channelNum[1] = SeekActiveChannel_Up(g_ChannelVfoInfo.channelNum[1], 0);
        }
    }
}

void ChannelNumChangeRead(U8  isScan,U8  isTypeIn)
{
    g_ChannelVfoInfo.channelNum[g_ChannelVfoInfo.switchAB] = g_ChannelVfoInfo.currentChannelNum;

    if(isTypeIn)
    {//复位输入的buf
        ResetInputBuf();
    }

    ChannleVfoDataInit(g_ChannelVfoInfo.switchAB,1);
    if(isScan == 0)
    {
        if(!isTypeIn)
        {
            Audio_PlayChanNum(g_ChannelVfoInfo.currentChannelNum+1);
        }
        
        Flash_SaveSystemRunData();
    }
    RxReset();
}

extern void ChannelUp(U8  isScan, U8  brocast)
{
    g_ChannelVfoInfo.currentChannelNum = SeekActiveChannel_Up(g_ChannelVfoInfo.currentChannelNum, isScan);
    
    if(g_ChannelVfoInfo.currentChannelNum == 0xFFFF)
    {//没有信道时默认退出信道模式
        g_ChannelVfoInfo.haveChannel =  0;
        return;
    }
    ChannelNumChangeRead(isScan,brocast);
    DisplayHomePage();
    DtmfClrMatchTimer();
}

extern void ChannelDown(U8  isScan, U8  brocast)
{
    g_ChannelVfoInfo.currentChannelNum = SeekActiveChannel_Down(g_ChannelVfoInfo.currentChannelNum, isScan);

    if(g_ChannelVfoInfo.currentChannelNum == 0xFFFF)
    {//没有信道时默认退出信道模式
        g_ChannelVfoInfo.haveChannel =  0;
        return;
    }
    ChannelNumChangeRead(isScan,brocast);  
    DisplayHomePage();
    DtmfClrMatchTimer();
}

extern void ChannelNumTypeIn(STR_INPUTBOX *input)
{
    U8  temp;
    U16 tempNum;

    DisplayInputChNum();

    //播报数字
    temp = input->buf[input->len-1] - 0x30;
    if(g_radioInform.voiceSw == 0)
	{
	    BeepOut(BEEP_NULL);
	}
	else
	{
        Audio_PlayNumInQueue(temp);
	}

    if(input->len == 3)
    {
        DualStandbyWorkOFF();
        
        input->buf[input->len] = 0;
        tempNum = atoi(input->buf);

        if(tempNum == 0 || tempNum > 999 || CheckChannelActive((tempNum-1),0) == CHAN_DISABLE) 
        {
            BeepOut(BEEP_NULL);
            ResetInputBuf();
            tempNum = g_ChannelVfoInfo.currentChannelNum;
        }
        else
        {
            g_ChannelVfoInfo.currentChannelNum = tempNum-1;   
            ChannelNumChangeRead(0,1);
        }
        DisplayHomePage();
    }
}

extern void VfoFrequency2Buf(U32 freq,U8  *dest,U8  len)
{   
    String buf[9];
    U8  i;
    
    sprintf(buf,"%06d",freq);

    for(i=0;i<len;i++)
    {//将ASC转换为hex
        dest[i] = buf[i] - 0x30;
    }
}

extern U8  GetCurWorkBand(U16 freq)
{
    U8  i;

    for(i=0;i<BAND_BUF_CNT;i++)
    {
        if(freq >= bandRang.bandbuf.freq[2*i] && freq < bandRang.bandbuf.freq[2*i+1])
        {
            return i;
        }
    }

    return 0;
}

extern void CalculateVfoTxFreq(void)
{
    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqDir == 1)
    {
        g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqTx.frequency = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency + g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqOffset;
    }
    else if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqDir == 2)
    {
        g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqTx.frequency = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency - g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqOffset;
    }
    else
    {
        g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqTx.frequency = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency;
    }
}

extern void VfoFreqSave(void)
{
    U8  *buf;
    U32 tempFreq;

    buf = g_ChannelVfoInfo.vfoInfo[g_ChannelVfoInfo.switchAB].freq;
    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].reverseFlag) 
    {
        tempFreq = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqTx.frequency;
    }
    else
    {
        tempFreq = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency;
    }

    VfoFrequency2Buf(tempFreq,buf,8);
    
    Flash_SaveVfoData(g_ChannelVfoInfo.switchAB);
}


extern void VfoFreqUp(U8  isScan)
{
    U32 tempFreq;
    U8  workBand;
    U32 lastFreq;

    //切换频率时需要先关闭倒频，才能切换会主状态
    Radio_ReverseOff();

    lastFreq = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency / 10000;

    g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency += stepList[g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqStep];
    
    tempFreq = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency / 10000;

    if(isScan)
    {
        if(tempFreq >= (g_radioInform.vfoScanRangeH*10) || tempFreq < g_radioInform.vfoScanRangeL * 10)
        {
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency = g_radioInform.vfoScanRangeL * 100000L;
        }
    }
    else
    {
        if(g_rfMoudel.amRxEn == 1 && lastFreq < 1360)
        {
            if(tempFreq >= 1360)
            {//AM模式
                g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency = 10800000;
            }
        }
        else
        {
            //获取当前工作频段
            workBand = GetCurWorkBand(lastFreq);
    
            if(tempFreq >= bandRang.bandbuf.freq[workBand*2+1]) 
            {
                g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency = bandRang.bandbuf.freq32[workBand*2];
            }
        }
    }

    //计算发射频率
    CalculateVfoTxFreq();

    VfoFreqSave();
    RxReset();
    
    //显示当前频率信息
    DisplayRadioHome();

    DtmfClrMatchTimer();
}

extern void VfoFreqDown(U8  isScan)
{
    U32 tempFreq;
    U32 lastFreq;
    U8  workBand;

    //切换频率时需要先关闭倒频，才能切换会主状态
    Radio_ReverseOff();

    lastFreq = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency / 10000;
    g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency -= stepList[g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqStep];
    
    tempFreq = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency / 10000;

    if(isScan)
    {
        if(tempFreq < (g_radioInform.vfoScanRangeL*10) || tempFreq > (g_radioInform.vfoScanRangeH*10))
        {
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency = g_radioInform.vfoScanRangeH * 100000L - stepList[g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqStep];
        }
    }
    else
    {
        if((g_rfMoudel.amRxEn == 1) && (lastFreq < 1080 && lastFreq >= 1360))
        {
            if(tempFreq < 10800000)
            {//AM模式
                g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency = 13600000 - stepList[g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqStep];
            }
        }
        else
        {
            //获取当前工作频段
            workBand = GetCurWorkBand(lastFreq);
    
            if(tempFreq < bandRang.bandbuf.freq[workBand*2]) 
            {
                g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency = bandRang.bandbuf.freq32[workBand*2+1] - bandRang.bandbuf.freq32[workBand*2] + g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency;
            }
        }
    }

    //计算发射频率
    CalculateVfoTxFreq();
    
    VfoFreqSave();
    RxReset(); 
    
    //显示当前频率信息
    DisplayRadioHome();

    DtmfClrMatchTimer();
}

extern void VfoFreqTypeIn(STR_INPUTBOX *input)
{
    U32 tempFreq;
    U8  temp;
    U8  i;
    
    Radio_ReverseOff();  

    if(input->len == 1)
    {
        if(input->buf[0] == '0')
        {
            ResetInputBuf();
            BeepOut(BEEP_NULL);
            return;
        }
    }

    //显示输入模式
    DisplayInputVfoFreq();

    //播报数字
    temp = input->buf[input->len-1] - 0x30;

    if(g_radioInform.voiceSw == 0)
	{
	    BeepOut(BEEP_NULL);
	}
	else
	{
        Audio_PlayNumInQueue(temp);  
	}

    if(input->len == 6)
    {
        //双守开启时需要关闭，才能切换回主信道
        DualStandbyWorkOFF();
        
        input->buf[input->len] = 0;
        //tempFreq = atol(input->buf);
        tempFreq = 0;
        for(i = 0; i < 6; i++)
        {
            tempFreq *= 10;
            tempFreq += (input->buf[i] - 0x30);
        }
        
        input->len = 0;

        tempFreq = tempFreq * 100; 
        
        if(temp == 9)
        {
            tempFreq -= MOD_LIST[temp];
        }
        else
        {
            tempFreq += MOD_LIST[temp];
        }

        if(CheckFreqInRange(tempFreq))
        {
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx.frequency = tempFreq;
            //计算发射频率
            CalculateVfoTxFreq();

            VfoFreqSave();
        }
        else
        {
            VoiceBroadcastWithBeepLock(vo_Cancel,BEEP_EXITMENU);
        }
        RxReset();

        //更新显示信息
        DisplayRadioHome();

        DtmfClrMatchTimer();
    }
}

extern void Radio_SwitchChannelOrVfo(void)
{
    DualStandbyWorkOFF();

    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == VFO_MODE)
    {
        g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode = CHAN_MODE;
    }
    else
    {
        g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode = VFO_MODE;
    }
    if(g_ChannelVfoInfo.switchAB)
    {
        g_radioInform.chOrVfoMode.Bit.chVofB = g_ChannelVfoInfo.chVfoInfo[1].chVfoMode;
    }
    else
    {
        g_radioInform.chOrVfoMode.Bit.chVofA = g_ChannelVfoInfo.chVfoInfo[0].chVfoMode;
    }
    ChannleVfoDataInit(g_ChannelVfoInfo.switchAB,1);
    DisplayRadioHome();
    RxReset();
    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == VFO_MODE)
    {
        Audio_PlayVoice(vo_Freqmode);
    }
    else
    {
        Audio_PlayVoice(vo_Channelmode);
    }
}

extern void Radio_SwitchAOrB(void)
{
    DualStandbyWorkOFF();

    if(g_ChannelVfoInfo.switchAB)
    {
        g_ChannelVfoInfo.switchAB = 0;
    }
    else
    {
        g_ChannelVfoInfo.switchAB = 1;
    }
    g_CurrentVfo = &g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB];
    DisplayRadioHome();
    RxReset();
}

extern void Radio_ReverseSwitch(void)
{
    U8 reverse;
    
    ResetInputBuf();
    DualStandbyWorkOFF();

    if( g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqDir == 0 )
    {
        BeepOut(BEEP_ERROR);
        return;
    }

    reverse = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].reverseFlag;
    if(CheckFreqInRange(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqTx.frequency) == TRUE)
    {
        reverse = (reverse + 1)%3;
        BeepOut(BEEP_FASTSW);
    
        g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].reverseFlag = reverse;   

        if(reverse == 1)
        {
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].rx = &g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqTx;
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].tx = &g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx;
        }
        else if(reverse == 2)
        {
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].rx = &g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx;
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].tx = &g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx;
        }
        else
        {
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].rx = &g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx;
            g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].tx = &g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqTx;
        }
    }
    else
    {
        Radio_ReverseOff(); 
        BeepOut(BEEP_NULL);
    }
    RxReset();
    DisplayHomePage();
}

extern void Radio_ReverseOff(void)
{
    g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].reverseFlag = 0;     
    g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].rx = &g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqRx;
    g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].tx = &g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].freqTx;
}

U32 VfoFreqCalculate(U8 *buf)
{
   U8 i;
   U32 freq;

   freq = 0;
   for(i = 0; i < 8; i++)
   {
       freq = (freq * 10) +  buf[i];
   }

   if(!CheckFreqInRange(freq))
   {
       freq = bandRang.bandFreq.freqVL;
   }

   return freq;
}

U32 VfoOffsetCalculate(U8 *buf)
{
    U8 i;
    U32 freq;
    
    freq = 0;
    for(i = 0; i < 7; i++)
    {
        freq = (freq * 10) +  buf[i];
    }
    return freq *10;
}

//初始化信道数据
extern void ChannleVfoDataInit(U8 flagAB,U8 readFlag)
{
    U16 chNum;
    U8 *buf;
    U8 calcBuf[8];
    U8 i;
    
    g_ChannelVfoInfo.chVfoInfo[flagAB].reverseFlag = 0;

    if(g_ChannelVfoInfo.haveChannel == 0)
    {
        g_ChannelVfoInfo.chVfoInfo[flagAB].chVfoMode = VFO_MODE;
    }
    else
    {
        if(flagAB)
        {
            g_ChannelVfoInfo.chVfoInfo[1].chVfoMode = g_radioInform.chOrVfoMode.Bit.chVofB;
            chNum = g_ChannelVfoInfo.channelNum[1];
        }
        else
        {
            g_ChannelVfoInfo.chVfoInfo[0].chVfoMode = g_radioInform.chOrVfoMode.Bit.chVofA;
            chNum = g_ChannelVfoInfo.channelNum[0];
        }
    }

    if(g_ChannelVfoInfo.chVfoInfo[flagAB].chVfoMode == CHAN_MODE)
    {
        if(readFlag)
        {
            SpiFlash_ReadBytes(chNum*CHAN_SIZE , (U8 *)&g_ChannelVfoInfo.channelInfo[flagAB], sizeof(STR_CHANNEL));
            SpiFlash_ReadBytes(chNum*CHAN_SIZE + NAME_ADDR_SHIFT,g_ChannelVfoInfo.chVfoInfo[flagAB].channelName,NAME_SIZE);
        }

        buf = (U8 *)&g_ChannelVfoInfo.channelInfo[flagAB].rxFreq;
    
        for(i = 0; i < 8; i++)
        {
            calcBuf[i] = changeHexToInt(buf[i]);
        }
        g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.frequency = 0;
        g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx.frequency = 0;
        for(i = 4; i > 0; i--)
        {
            g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.frequency = (g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.frequency * 100) + calcBuf[i-1];
            g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx.frequency = (g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx.frequency * 100) + calcBuf[i+3];
        }
        g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.dcsCtsType = GetCTSDCSType(g_ChannelVfoInfo.channelInfo[flagAB].rxDCSCTSNum);
        g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.dcsCtsNum = g_ChannelVfoInfo.channelInfo[flagAB].rxDCSCTSNum;

        g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx.dcsCtsType = GetCTSDCSType(g_ChannelVfoInfo.channelInfo[flagAB].txDCSCTSNum);
        g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx.dcsCtsNum = g_ChannelVfoInfo.channelInfo[flagAB].txDCSCTSNum;

        g_ChannelVfoInfo.chVfoInfo[flagAB].wideNarrow = g_ChannelVfoInfo.channelInfo[flagAB].chFlag3.Bit.b6;
        g_ChannelVfoInfo.chVfoInfo[flagAB].txPower = g_ChannelVfoInfo.channelInfo[flagAB].txPower&0x0F;
        g_ChannelVfoInfo.chVfoInfo[flagAB].fhssFlag = g_ChannelVfoInfo.channelInfo[flagAB].chFlag3.Byte&0x01;
        g_ChannelVfoInfo.chVfoInfo[flagAB].scarmble = (g_ChannelVfoInfo.channelInfo[flagAB].txPower&0xF0) >> 4;
        g_ChannelVfoInfo.chVfoInfo[flagAB].spMute = g_ChannelVfoInfo.channelInfo[flagAB].chFlag3.Bit.spMute;
        g_ChannelVfoInfo.chVfoInfo[flagAB].busyLock = g_ChannelVfoInfo.channelInfo[flagAB].chFlag3.Bit.b3;
        g_ChannelVfoInfo.chVfoInfo[flagAB].dtmfgroup = g_ChannelVfoInfo.channelInfo[flagAB].dtmfgroup;
        g_ChannelVfoInfo.chVfoInfo[flagAB].pttIdMode = g_ChannelVfoInfo.channelInfo[flagAB].pttID;

        if(g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.frequency > g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx.frequency)
        {
            g_ChannelVfoInfo.chVfoInfo[flagAB].freqDir = 1;
        }
        else if(g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.frequency < g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx.frequency)
        {
            g_ChannelVfoInfo.chVfoInfo[flagAB].freqDir = 2;
        }
        else
        {
            g_ChannelVfoInfo.chVfoInfo[flagAB].freqDir = 0;
        }
    }
    else
    {
        if(readFlag)
        {
            Flash_ReadVfoData(flagAB);
        }

        g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.frequency = VfoFreqCalculate(g_ChannelVfoInfo.vfoInfo[flagAB].freq);
        g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.dcsCtsType = GetCTSDCSType(g_ChannelVfoInfo.vfoInfo[flagAB].rxDCSCTSNum);
        g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.dcsCtsNum = g_ChannelVfoInfo.vfoInfo[flagAB].rxDCSCTSNum;
        g_ChannelVfoInfo.chVfoInfo[flagAB].freqDir = g_ChannelVfoInfo.vfoInfo[flagAB].dtmfgroup>>5;
        g_ChannelVfoInfo.chVfoInfo[flagAB].freqOffset = VfoOffsetCalculate(g_ChannelVfoInfo.vfoInfo[flagAB].Offset);

        if(g_ChannelVfoInfo.chVfoInfo[flagAB].freqDir == 1)
        {
            g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx.frequency = g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.frequency + g_ChannelVfoInfo.chVfoInfo[flagAB].freqOffset;
        }
        else if(g_ChannelVfoInfo.chVfoInfo[flagAB].freqDir == 2)
        {
            g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx.frequency = g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.frequency - g_ChannelVfoInfo.chVfoInfo[flagAB].freqOffset;
        }
        else
        {
            g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx.frequency = g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx.frequency ;
        }
        g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx.dcsCtsType = GetCTSDCSType(g_ChannelVfoInfo.vfoInfo[flagAB].txDCSCTSNum);
        g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx.dcsCtsNum = g_ChannelVfoInfo.vfoInfo[flagAB].txDCSCTSNum;

        g_ChannelVfoInfo.chVfoInfo[flagAB].wideNarrow = g_ChannelVfoInfo.vfoInfo[flagAB].vfoFlag.Bit.b6;
        g_ChannelVfoInfo.chVfoInfo[flagAB].txPower = g_ChannelVfoInfo.vfoInfo[flagAB].txPower&0x0F;
        g_ChannelVfoInfo.chVfoInfo[flagAB].fhssFlag = g_ChannelVfoInfo.vfoInfo[flagAB].vfoFlag.Byte&0x01;
        g_ChannelVfoInfo.chVfoInfo[flagAB].scarmble = (g_ChannelVfoInfo.vfoInfo[flagAB].txPower&0xF0) >> 4;
        g_ChannelVfoInfo.chVfoInfo[flagAB].spMute = g_ChannelVfoInfo.vfoInfo[flagAB].spMute&0x03;
        g_ChannelVfoInfo.chVfoInfo[flagAB].busyLock = g_radioInform.txBusyLock;
        g_ChannelVfoInfo.chVfoInfo[flagAB].dtmfgroup = g_ChannelVfoInfo.vfoInfo[flagAB].dtmfgroup&0x1f;
        g_ChannelVfoInfo.chVfoInfo[flagAB].freqStep = g_ChannelVfoInfo.vfoInfo[flagAB].STEP;
        g_ChannelVfoInfo.chVfoInfo[flagAB].pttIdMode = g_radioInform.pttIdMode;
    }
    g_ChannelVfoInfo.chVfoInfo[flagAB].rx = &g_ChannelVfoInfo.chVfoInfo[flagAB].freqRx;
    g_ChannelVfoInfo.chVfoInfo[flagAB].tx = &g_ChannelVfoInfo.chVfoInfo[flagAB].freqTx;
}

extern void RadioVfoInfo_Init(void)
{
    ChannleVfoDataInit(0,1);
    ChannleVfoDataInit(1,1);
    g_CurrentVfo = &g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB];
    DisplayHomePage();
}

