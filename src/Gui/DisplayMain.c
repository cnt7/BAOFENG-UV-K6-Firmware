#include "includes.h"

const U8 *SignalLevelStr[] = 
{
    iconSingalEmpty,iconSingal1,iconSingal2,iconSingal3,iconSingalFull
};

extern void DisplayProgrom(void)
{
    LCD_ClearFullBuf();
    LCD_DisplayPicture(8,48,ICON_PROG_SIZEX,ICON_PROG_SIZEY,iconProgrom,LCD_DIS_NORMAL);
    LCD_DisplayText(42,24, "Program...", FONTSIZE_16x16,LCD_DIS_NORMAL);
    LCD_UpdateFullScreen();

    //开启按键背光灯
    LCD_BackLightSetOn();
}

extern void DisplayUpdateLockFlag(U8 UpdateF)
{
    if(g_radioInform.keyLock)
    {       
        LCD_DisplayPicture(0,88,ICON_LOCK_SIZEX,ICON_LOCK_SIZEY,iconLock,LCD_DIS_NORMAL);
    }
    else
    {
        LCD_ClearArea(0,88,ICON_LOCK_SIZEX,ICON_LOCK_SIZEY);
    }

    if(UpdateF)
    {
        LCD_UpdateStateBar();
    }
}


extern void DisplayStateBar(void)
{
    U8 reverse;

    LCD_ClearStateBar();

    reverse = g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].reverseFlag;

    //显示声控发射图标
    if(g_radioInform.voxSwitch)
    {
        LCD_DisplayPicture(0,34,ICON_VOX_SIZEX,ICON_VOX_SIZEY,iconVox,LCD_DIS_NORMAL);
    }

    //显示双频守候图标
    if(g_radioInform.dualRxFlag == 1)
    {
        LCD_DisplayPicture(0,67,ICON_DUAL_SIZEX,ICON_DUAL_SIZEY,iconDual,LCD_DIS_NORMAL);
    }

    //更新显示键盘锁定图标
    DisplayUpdateLockFlag(0);

    //显示侧音开关图标
    if(g_radioInform.dtmfTone)
    {
        LCD_DisplayPicture(0,17,ICON_DTMF_SIZEX,ICON_DTMF_SIZEY,iconDTMF,LCD_DIS_NORMAL);
    }

    /*倒频功能标志*/
    if(reverse == 2)
    {
        LCD_DisplayPicture(0,1,ICON_UNNET_SIZEX,ICON_UNNET_SIZEY,iconUnNet,LCD_DIS_NORMAL);
    }
    else if(reverse == 1)
    {
        LCD_DisplayPicture(0,1,ICON_REVERSE_SIZEX,ICON_REVERSE_SIZEY,iconReverse,LCD_DIS_NORMAL);
    }
    else
    {
    }    

    DisplayBattaryFlag(0);

    LCD_UpdateStateBar();
}

extern void TranStrToMiddle(String  *targeBuf,String *sourceBuf,U8 len)
{
    if (strlen(sourceBuf) >= len)
    {
        memcpy(targeBuf,sourceBuf,len);
    }else
    {
        memset(targeBuf,' ',len);
        memcpy(&targeBuf[(len-strlen(sourceBuf))/2],sourceBuf,strlen(sourceBuf));
    }
}

Boolean FillChannelName2Buf(U8 *src,String *dest)
{
    U8 i;
    String buf[13] = {0};

    if(src[0] == 0xFF || src[0] == 0x00)
    {//
       return FALSE;
    }

    for(i=0;i<12;i++)
    {
        if(src[i] == 0xFF || src[i] == 0x00)
        {
            break;
        }
        buf[i] = src[i];
    }
    TranStrToMiddle(dest,buf,12);
	dest[12] = '\0';

    return TRUE;
}

extern void DisplayChannelNum(U16 currNum,U8 disAB)
{
    U8 buf[5] = {0},posY;

    if(0XFFFF == currNum)
    {
        sprintf((String *)buf,"VFO");
    }
    else
    {
        sprintf((String *)buf,"M%3d",currNum+1);   
    }

    if(g_radioInform.dualRxFlag == 0)
    {
        posY = 47;
    }
    else
    {
        if(disAB == 0)
        {
            posY = 21;
        }
        else
        {
            posY = 49;
        }
    }
    LCD_ClearArea( posY, 24, 5, 7);
    LCD_DisplayBoldNum6X7(posY,1,buf);
}

extern void DisplayCurFreq(U8 posx,U8 posy,U32 freq)
{
    String freqBuf[12] = {0};
    U8 fontBuf[3] ={0};

    sprintf(freqBuf,"%03d.%05d",freq/100000,freq%100000);

    memcpy(fontBuf,&freqBuf[7],2);
    freqBuf[7] = 0;
    fontBuf[2] = 0;
    
    LCD_DisplayBoldNum12X13(posy,posx,(U8 *)freqBuf);
    LCD_DisplayBoldNum6X7(posy + 6,posx+85,fontBuf);
}

extern void DisplayCurFreq6X7(U8 posx,U8 posy,U32 freq)
{
    String freqBuf[12] = {0};

    sprintf(freqBuf,"%03d.%05d",freq/100000,freq%100000);
    freqBuf[9] = 0;
    
    LCD_DisplayBoldNum6X7(posy,posx,(U8 *)freqBuf);
}

extern void DisplayUpdateChFlag(U8 disAB)
{
    U8 ctsType;
    U8 posY = 0;

    if(g_radioInform.dualRxFlag == 0)
    {
        posY = 31;
    }
    else
    {
        if(disAB == 0)
        {//显示在操作界面
            posY = 29;
        }
        else
        {//B段在主界面时显示图标位置
            posY = 57;
        }
    }

    if(g_ChannelVfoInfo.chVfoInfo[disAB].txPower)
    {
        LCD_DisplayPicture(posY,46,ICON_DTXPWR_SIZEX,ICON_DTXPWR_SIZEY,iconPowerL,0);
    }
    else
    {
        LCD_DisplayPicture(posY,46,ICON_DTXPWR_SIZEX,ICON_DTXPWR_SIZEY,iconPowerH,0);
    }
    
    /*显示宽窄带标志*/
    if(g_ChannelVfoInfo.chVfoInfo[disAB].wideNarrow)
    {
        LCD_DisplayPicture(posY,65,ICON_NARROW_SIZEX,ICON_NARROW_SIZEY,iconNarrow,LCD_DIS_NORMAL);
    }
    else
    {
        LCD_ClearArea(posY,65,ICON_NARROW_SIZEX,ICON_NARROW_SIZEY);
    }
				
    /*显示AM标志*/
    if(g_ChannelVfoInfo.chVfoInfo[disAB].rx->frequency >= 10800000 && g_ChannelVfoInfo.chVfoInfo[disAB].rx->frequency < 13600000 )
    {
        LCD_ClearArea(posY,85,ICON_QT_SIZEX,ICON_QT_SIZEY);
        LCD_DisplayPicture(posY,85,ICON_AM_SIZEX,ICON_AM_SIZEY,iconAM,LCD_DIS_NORMAL);
        
        //AM模式下 亚音频 跳频 倒频均无效
        LCD_ClearArea(posY,55,ICON_ADDSUB_SIZEX,ICON_ADDSUB_SIZEY);
        return;
    }
    else
    {
        if(g_ChannelVfoInfo.chVfoInfo[disAB].spMute == 1)
        {
            LCD_DisplayPicture(posY,85,ICON_QT_SIZEX,ICON_QT_SIZEY,iconQT_DTMF,LCD_DIS_NORMAL);
        }
        else
        {
            LCD_ClearArea(posY,85,ICON_QT_SIZEX,ICON_QT_SIZEY);
        }
    }

    /*显示亚音频类型*/
    if(g_rfState == RF_RX)
    {
        ctsType = g_ChannelVfoInfo.chVfoInfo[disAB].rx->dcsCtsType;
    }
    else
    {
        ctsType = g_ChannelVfoInfo.chVfoInfo[disAB].tx->dcsCtsType;
    }
				
	if(ctsType == SUBAUDIO_NONE)
	{
        LCD_ClearArea(posY,26,ICON_DCS_SIZEX,ICON_DCS_SIZEY);
	}
	else if(ctsType == SUBAUDIO_CTS)
	{
        LCD_DisplayPicture(posY,26,ICON_DCS_SIZEX,ICON_DCS_SIZEY,iconCtcss,LCD_DIS_NORMAL);
	}
	else 
	{
        LCD_DisplayPicture(posY,26,ICON_DCS_SIZEX,ICON_DCS_SIZEY,iconDcs,LCD_DIS_NORMAL);
	}

	/*显示频差频率*/
	if(g_ChannelVfoInfo.chVfoInfo[disAB].freqDir == 1)
	{
		LCD_DisplayPicture(posY,55,ICON_ADDSUB_SIZEX,ICON_ADDSUB_SIZEY,iconAdd,LCD_DIS_NORMAL);
	}
	else if(g_ChannelVfoInfo.chVfoInfo[disAB].freqDir == 2)
	{
		LCD_DisplayPicture(posY,55,ICON_ADDSUB_SIZEX,ICON_ADDSUB_SIZEY,iconSub,LCD_DIS_NORMAL);
	}
	else
	{
		LCD_ClearArea(posY,55,ICON_ADDSUB_SIZEX,ICON_ADDSUB_SIZEY);
	}

	/*显示跳频标志*/
	if(g_ChannelVfoInfo.chVfoInfo[disAB].fhssFlag)
	{
		LCD_DisplayPicture(posY,75,ICON_FHSS_SIZEX,ICON_FHSS_SIZEY,iconFhss,LCD_DIS_NORMAL);
	}
	else
	{
		LCD_ClearArea(posY,75,ICON_FHSS_SIZEX,ICON_FHSS_SIZEY);
	}
				
	if(g_ChannelVfoInfo.chVfoInfo[disAB].scarmble)
	{
		LCD_DisplayPicture(posY,109,ICON_SCR_SIZEX,ICON_SCR_SIZEY,iconScr,LCD_DIS_NORMAL);
	}
	else
	{
		LCD_ClearArea(posY,109,ICON_SCR_SIZEX,ICON_SCR_SIZEY);
	}
}

extern void DisplayChannelMsg(U8 disMode,U8 disAB, U8 txOrRx)
{
    String nameBuf[16] = {0};
    U8 ypox,clrY;
    U16 chNum;
    
    if(disAB == 0)
    {	
        if(g_radioInform.dualRxFlag == 0)
        {
            ypox = 39;
            disMode = CH_DISFREQ;
            clrY = 39;
        }
        else     
        {
            if(disMode == CH_DISNAME_FREQ)
			{
			    ypox = 8;
			}
            else if(disMode == CH_DISNAME)
            {
                ypox = 11;
            }
            else
            {
                ypox = 14;
            }
            clrY = 12;
        }
        chNum = g_ChannelVfoInfo.channelNum[0];   
    }
    else
    {
        if(g_radioInform.dualRxFlag == 0)
        {
            ypox = 39;
            disMode = CH_DISFREQ;
            clrY = 39;
        }
        else
        {
            if(disMode == CH_DISNAME_FREQ)
			{
			    ypox = 36;
			}
            else if(disMode == CH_DISNAME)
            {
                ypox = 39;
            }
            else
            {
                ypox = 42;
            }
            clrY = 40;
        }
        chNum = g_ChannelVfoInfo.channelNum[1];  
    }

    if(g_sysRunPara.sysRunMode == MODE_SCAN)
    {
        chNum = g_ChannelVfoInfo.currentChannelNum;
    }

    LCD_ClearArea(clrY-4, 30, 98,21);

    if(disMode == CH_DISNAME_FREQ)
    {
        if(FillChannelName2Buf(g_ChannelVfoInfo.chVfoInfo[disAB].channelName,nameBuf) == TRUE)
        {
            LCD_DisplayText(ypox,30,(U8 *)nameBuf,FONTSIZE_12x12,LCD_DIS_NORMAL);
        }
        else
        {
            LCD_DisplayText(ypox,30,(U8 *)((g_radioInform.language == LANG_CN)?"   未命名   ":"   No Name  "),FONTSIZE_12x12,LCD_DIS_NORMAL);
        }
				
        if(txOrRx == DIS_TX)
        {
            DisplayCurFreq6X7(40,ypox+13,g_ChannelVfoInfo.chVfoInfo[disAB].tx->frequency);
        }
        else
        {
            DisplayCurFreq6X7(40,ypox+13,g_ChannelVfoInfo.chVfoInfo[disAB].rx->frequency);
        }
    }
    else if(disMode == CH_DISNAME && FillChannelName2Buf(g_ChannelVfoInfo.chVfoInfo[disAB].channelName,nameBuf) == TRUE)
    {//显示信道名称
        LCD_DisplayText(ypox,30,(U8 *)nameBuf,FONTSIZE_16x16,LCD_DIS_NORMAL);
    }
    else if(disMode == CH_DISCHNUM)
    {
        //清除显示区域的数据
        memset(nameBuf, ' ', 12);
		nameBuf[12] = '\0';
        LCD_DisplayText(ypox-2,30,(U8 *)nameBuf,FONTSIZE_16x16,LCD_DIS_NORMAL);

        sprintf((String *)(nameBuf)," CH-%03d",chNum+1);			
	    LCD_DisplayBoldNum12X13(ypox,30,(U8 *)nameBuf);
    }
    else
    {//显示信道频率
		if(txOrRx == DIS_TX)
        {
            DisplayCurFreq(30,ypox,g_ChannelVfoInfo.chVfoInfo[disAB].tx->frequency);
        }
        else
        {
            DisplayCurFreq(30,ypox,g_ChannelVfoInfo.chVfoInfo[disAB].rx->frequency);
        }
    }
    //显示信道标志
    DisplayUpdateChFlag(disAB);
}

extern void DisplayChannelName(U8 disAB)
{
    String nameBuf[16] = {0};
    
    if(g_ChannelVfoInfo.chVfoInfo[disAB].chVfoMode == VFO_MODE)
    {
        if(disAB)
        {
            sprintf(nameBuf,"   VFO B   ");
        }
        else
        {
            sprintf(nameBuf,"   VFO A   ");
        }
    }
    else
    {
        if(FillChannelName2Buf(g_ChannelVfoInfo.chVfoInfo[disAB].channelName,nameBuf) == FALSE)
        {
            memset(nameBuf,' ',12);
        }
    }

    LCD_DisplayText(14,16,(U8 *)nameBuf,FONTSIZE_16x16,LCD_DIS_NORMAL);
}


extern void DisplayRadioHome(void)
{
    U8 chDisMode;
    
    if(g_radioInform.dualRxFlag == 0)
    {
        if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].chVfoMode == VFO_MODE)
        { 
            if(g_ChannelVfoInfo.switchAB == 0)
            {
                DisplayChannelMsg(CH_DISFREQ,0,DIS_RX);
                DisplayChannelNum(0xFFFF,0);
            }
            else
            {
                DisplayChannelMsg(CH_DISFREQ,1,DIS_RX);
                DisplayChannelNum(0xFFFF,1);
            }  
        }
        else
        {
            if(g_ChannelVfoInfo.switchAB == 0)
            {
                chDisMode = g_radioInform.channleDisA;
            }
            else
            {
                chDisMode = g_radioInform.channleDisB;
            }
            DisplayChannelMsg(chDisMode,g_ChannelVfoInfo.switchAB,DIS_RX);
            DisplayChannelNum(g_ChannelVfoInfo.channelNum[g_ChannelVfoInfo.switchAB],g_ChannelVfoInfo.switchAB);
        }
        
        DisplayChannelName(g_ChannelVfoInfo.switchAB);
        LCD_DisplayPicture(39,1,ICON_ARROWB_SIZEX,ICON_ARROWB_SIZEY,iconArrowB,LCD_DIS_NORMAL);
    }
    else
    {
        if(g_ChannelVfoInfo.chVfoInfo[0].chVfoMode == CHAN_MODE)
        {
            DisplayChannelMsg(g_radioInform.channleDisA,0,DIS_RX);
            DisplayChannelNum(g_ChannelVfoInfo.channelNum[0],0);
        }
        else
        {
            DisplayChannelMsg(CH_DISFREQ,0,DIS_RX);
            DisplayChannelNum(0xFFFF,0); 
        }
    
        if(g_ChannelVfoInfo.chVfoInfo[1].chVfoMode == CHAN_MODE)
        {
            DisplayChannelMsg(g_radioInform.channleDisB,1,DIS_RX);
            DisplayChannelNum(g_ChannelVfoInfo.channelNum[1],1);
        }
        else
        {
            DisplayChannelMsg(CH_DISFREQ,1,DIS_RX);
            DisplayChannelNum(0xFFFF,1);
        } 

        if(g_ChannelVfoInfo.switchAB == 0)
        {
            LCD_DisplayPicture(13,1,ICON_ARROWA_SIZEX,ICON_ARROWA_SIZEY,iconArrowA,LCD_DIS_NORMAL);
            LCD_ClearArea(41,1,ICON_ARROWB_SIZEX,ICON_ARROWB_SIZEY);
        }
        else
        {
            LCD_DisplayPicture(41,1,ICON_ARROWB_SIZEX,ICON_ARROWB_SIZEY,iconArrowB,LCD_DIS_NORMAL);
            LCD_ClearArea(13,1,ICON_ARROWA_SIZEX,ICON_ARROWA_SIZEY);
        }
    }
    
    LCD_UpdateWorkAre();
}

extern void DisplayHomePage(void)
{
    LCD_ClearFullBuf();

    //显示状态栏
    DisplayStateBar();

    //显示主界面
    DisplayRadioHome();
}

extern void DisplayInputChNum(void)
{
    U8 buf[11],i,flag,posY;

    buf[0] = 'M';
    buf[1] = '-';
    buf[2] = '-';
    buf[3] = '-';

    flag = 0;
    for(i=0;i<g_inputbuf.len;i++)
    {
        if(g_inputbuf.buf[i] == '0' && flag == 0)
        {
            buf[i + 1] = ' ';
        }
        else
        {
            flag = 1;
            buf[i + 1] = g_inputbuf.buf[i];
        }
    }
    buf[4] = 0;

    if(g_radioInform.dualRxFlag == 0)
    {
        posY = 47;
    }
    else
    {
        if(g_ChannelVfoInfo.switchAB)
        {
            posY = 49;
        }
        else
        {
            posY = 21;
        }
    }
    LCD_DisplayBoldNum6X7(posY,1,buf);
    
    LCD_UpdateWorkAre();
}

extern void DisplayInputVfoFreq(void)
{
    U8 i,j,posY;
    U8 buf[10];
    U8 fontBuf[3];
	 
    memset(&buf[0],'-',9);
    buf[3] = '.';
    j = 0;
    for(i=0;i<g_inputbuf.len;i++)
    {
        if(i == 3)
        {
           j++;
        }
        buf[j++] = g_inputbuf.buf[i];
    }
    buf[9] = 0;
    

    memcpy(fontBuf,&buf[7],2);
    buf[7] = 0;	
    fontBuf[2] = 0;

    if(g_radioInform.dualRxFlag == 0)
    {
        posY = 39;
    }
    else
    {
        if(g_ChannelVfoInfo.switchAB)
        {
    	    posY = 43;
        }
        else
        {
    		posY = 15;
        }
    }

    LCD_DisplayBoldNum12X13(posY,30,(U8 *)buf);
    LCD_DisplayBoldNum6X7(posY + 6,115,fontBuf);

    LCD_UpdateWorkAre();
}

extern void DisplayTxSingalFlag(U8 level)
{
    U8 iconY,flagY;
    if(level == 0xff)
    {
        return;
    }
    if(g_radioInform.dualRxFlag == 0)
    {//单显模式
        iconY = 55;
        flagY = 39;
    }
    else
    {
        if(g_ChannelVfoInfo.switchAB)
        {
    		iconY = 57;
            flagY = 41;
        }
        else
        {
            iconY = 29;
            flagY = 13;
        }
    }
    LCD_DisplayPicture(iconY,1,ICON_SINGAL_SIZEX,ICON_SINGAL_SIZEY,SignalLevelStr[level],LCD_DIS_NORMAL);
    LCD_DisplayPicture(flagY,13,ICON_SINGALTX_SIZEX,ICON_SINGALTX_SIZEY,iconSingalTx,LCD_DIS_NORMAL);
}

extern void DisplayRxMode(void)
{
    DisplaySingalFlag(4,0);
    DisplayRadioHome();
}

extern void DisplayTxMode(void)
{
    if(g_CurrentVfo->txPower)
    {
        DisplayTxSingalFlag(2);
    }
    else
    {
        DisplayTxSingalFlag(4);
    }

    if(g_CurrentVfo->chVfoMode == CHAN_MODE)
    {
        if(g_ChannelVfoInfo.switchAB)
        {
            DisplayChannelMsg(g_radioInform.channleDisB,1,DIS_TX); 
        }
        else
        {
            DisplayChannelMsg(g_radioInform.channleDisA,0,DIS_TX); 
        }
    }
    else
    {
        if(g_ChannelVfoInfo.switchAB)
        {
            DisplayChannelMsg(CH_DISFREQ,1,DIS_TX); 
        }
        else
        {
            DisplayChannelMsg(CH_DISFREQ,0,DIS_TX); 
        }
    } 

    LCD_UpdateWorkAre();
}

U8 GetRxArea(void)
{
    U8 swab;

    if(dualStandby.dualRxFlag == ON)
    {
        if(g_ChannelVfoInfo.switchAB)
        {
            swab =  0;
        }
        else
        {
            swab =  1;
        }
    }
    else
    {
        swab = g_ChannelVfoInfo.switchAB;
    }

    return swab;
}

extern void DisplaySingalFlag(U8 level,U8 UpdateF)
{
    U8 iconY,flagY;
    if(g_sysRunPara.sysRunMode != MODE_MAIN && g_sysRunPara.sysRunMode != MODE_MONI)
    {
        return;
    }
    
    if(g_radioInform.dualRxFlag == 0)
    {//单显模式
        iconY = 55;
        flagY = 39;
    }
    else
    {
        if(GetRxArea())
        {
    		iconY = 57;
            flagY = 41;
        }
        else
        {
            iconY = 29;
            flagY = 13;
        }
    }
    if(level == 0xff)
    {
        if(g_radioInform.dualRxFlag == 0)
        {//单显模式
            LCD_ClearArea(iconY,1,ICON_SINGAL_SIZEX,ICON_SINGAL_SIZEY);
            LCD_ClearArea(flagY,13,ICON_SINGALTX_SIZEX,ICON_SINGALTX_SIZEY);
        }
        else
        {
            if(dtmfInfo.flagDtmfMatch)
            {   
                if(dtmfInfo.flagDtmfMatch == 1)
                {
                    LCD_ClearArea(29,1,ICON_SINGAL_SIZEX,ICON_SINGAL_SIZEY);
                    LCD_ClearArea(13,13,ICON_SINGALTX_SIZEX,ICON_SINGALTX_SIZEY);
                }
                else
                {
                    LCD_ClearArea(57,1,ICON_SINGAL_SIZEX,ICON_SINGAL_SIZEY);
                    LCD_ClearArea(41,13,ICON_SINGALTX_SIZEX,ICON_SINGALTX_SIZEY);
                }
                return;
            }
            else
            {
                LCD_ClearArea(29,1,ICON_SINGAL_SIZEX,ICON_SINGAL_SIZEY);
                LCD_ClearArea(13,13,ICON_SINGALTX_SIZEX,ICON_SINGALTX_SIZEY);
                LCD_ClearArea(57,1,ICON_SINGAL_SIZEX,ICON_SINGAL_SIZEY);
                LCD_ClearArea(41,13,ICON_SINGALTX_SIZEX,ICON_SINGALTX_SIZEY);
            }
        }
    }
    else
    {
        LCD_DisplayPicture(iconY,1,ICON_SINGAL_SIZEX,ICON_SINGAL_SIZEY,SignalLevelStr[level],LCD_DIS_NORMAL);
        LCD_DisplayPicture(flagY,13,ICON_SINGALTX_SIZEX,ICON_SINGALTX_SIZEY,iconSingalRx,LCD_DIS_NORMAL);
    }

    if(UpdateF)
    {
        LCD_UpdateWorkAre();
    }
}


// 填充呼叫方名称
U8 FillAniName(String *pDisName, String *pId)
{
    U8 i,j;
    U8 count = 0;

    for(i = 0; i < 20; i++)
    {
        if(memcmp(dtmfInfo.contact[i].id, pId, 3) == 0)
        {
            for(j = 0; j < 10; j++)
            {
                if(dtmfInfo.contact[i].name[j] != 0xFF)
                {
                    count++;
                }
            }
            dtmfInfo.contact[i].name[count++] = '\0';
            memcpy(pDisName, dtmfInfo.contact[i].name, count);
            return 1;
        }
    }

    return 0;
}
extern void DisplayAniMsg(U8 *pCallerId, U8 *pCalledId)
{
    String dtmfSymbol[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','*','#'};
    U8 xpos,ypox;
    U8 i;
    U8 swAB;
    String nameBuf[16];

    if(g_sysRunPara.sysRunMode != MODE_MAIN)
    {
        return;
    }

    if(g_radioInform.dualRxFlag == 0)
    {
        LCD_ClearArea(11, 0, 128,28);
            
        xpos = 9;
        ypox = 10;
    }
    else
    {
        swAB = GetRxArea();
    
        if(swAB == 1)
        {
            LCD_ClearArea(11, 0, 128,32);
    
            xpos = 9;
            ypox = 10;
        }
        else
        {
            LCD_ClearArea(39, 0, 128,32);
    
            xpos = 9;
            ypox = 38;
        }
    }

    nameBuf[0] = 'C';
    nameBuf[1] = 'A';
    nameBuf[2] = 'L';
    nameBuf[3] = 'L';
    nameBuf[4] = ':';
    if( FillAniName(nameBuf + 5, (String *)pCallerId) == 0)
    {
        for(i = 0; i < 3; i++)
        {
            nameBuf[5 + i] = dtmfSymbol[pCallerId[i]];
        }
        nameBuf[8] = '\0';
    }
    
    LCD_DisplayText(ypox,xpos,(U8 *)nameBuf,FONTSIZE_16x16,LCD_DIS_NORMAL);

    nameBuf[0] = '>';
    nameBuf[1] = '>';
    nameBuf[2] = '>';
    for(i = 0; i < 3; i++)
    {
        nameBuf[3 + i] = dtmfSymbol[pCalledId[i]];
    }
    nameBuf[6] = '\0';
    LCD_DisplayText(ypox+13,xpos,(U8 *)nameBuf,FONTSIZE_16x16,LCD_DIS_NORMAL);
    
    LCD_UpdateWorkAre();
}

/*********************************************************************
* 函 数 名: ClearAniDisplay
* 功能描述: 清除身份码显示信息
* 全局变量: 
* 输入参数：
* 输出参数:
* 返　　回:
* 说    明：
***********************************************************************/
extern void ClearAniDisplay(void)
{
    if( dtmfInfo.flagDtmfMatch )
    {
        if(dtmfInfo.flagDtmfMatch == 2 || (g_radioInform.dualRxFlag == 0))
        {
            //清除A段显示区域
            LCD_ClearArea(11, 0, 128,32);
        }
        else
        {
            //清除B段显示区域
            LCD_ClearArea(39, 0, 128,32);
        }
        dtmfInfo.flagDtmfMatch = 0;
    }
}

extern void DisplayResetRxFlag(void)
{

}

const U8 *modelTypeStr[] = 
{
    "BF",
    "FCC",
    "IC",
    "CE",
    "IND",
    "CN"   
};

extern void DisplaySoftVersion(void)
{
    U8 disbuf[11];
    U8 len;

    LCD_ClearFullBuf();    
    len = sprintf((String *)&disbuf,"%sNRF",strModelType);  
    LCD_DisplayText(4,60-(len<<2),disbuf,FONTSIZE_16x16,LCD_DIS_NORMAL);  
  
    //显示版本号
    sprintf((String *)&disbuf,"VER:%s","V0.22" );
    LCD_DisplayText(24,28,disbuf,FONTSIZE_16x16,LCD_DIS_NORMAL);  

    //显示国家码
    sprintf((String *)&disbuf,"%s",modelTypeStr[g_sysRunPara.moduleType]);
    LCD_DisplayText(44,51,disbuf,FONTSIZE_16x16,LCD_DIS_NORMAL);  

    LCD_UpdateFullScreen();
    LcdBackLightSwitch(LED_ON);

    //延时2S用于显示版本号
    DelaySysMs(2000);
}


