#include "includes.h"

U8 FastChangeVoice = 0;
STR_VOICE voice;
STR_VOICE_ONFO g_voiceInform;
static short s_valprev;
static char s_index; 

static void DmaUpdateMemery(DMA_Channel_TypeDef* channelx,uint32_t memoryAddr,uint32_t number)
{
    DMA_Cmd(DMA1_Channel5, DISABLE);
    DMA1_Channel5->CNDTR = number;
    DMA1_Channel5->CMAR = memoryAddr;

    DMA_Cmd(DMA1_Channel5, ENABLE);
}   

extern void VoiceOutput_Interrupt(void)
{
    U16 length;
    
    g_voiceInform.voicePlay.usedLen += 512;

    if((g_voiceInform.voicePlay.usedLen >= g_voiceInform.voiceIndex.length) || (g_voiceInform.voicePlay.finishFlag == 1))
    {
        g_voiceInform.voicePlay.finishFlag = 1;
        DMA_Cmd(DMA1_Channel5, DISABLE);     
    }
    else
    {
        if((g_voiceInform.voicePlay.usedLen + 512) > g_voiceInform.voiceIndex.length)
        {
            length = (g_voiceInform.voiceIndex.length - g_voiceInform.voicePlay.usedLen) * 2;
            g_voiceInform.voicePlay.lastPackage = 1;
        }
        else
        {
            length = 1024;
        }
    
        if(g_voiceInform.voicePlay.dmaBufUsed == 1)
        {//使用缓存B
            g_voiceInform.voicePlay.dmaBufUsed = 0;
            g_voiceInform.voicePlay.dmaBufAUseFlag = 1;
            DmaUpdateMemery(DMA1_Channel5,(uint32_t)g_voiceInform.voicePlay.dmaBufB,length);
        }
        else
        {//使用缓存A
            g_voiceInform.voicePlay.dmaBufUsed = 1;
            g_voiceInform.voicePlay.dmaBufBUseFlag = 1;
            DmaUpdateMemery(DMA1_Channel5,(uint32_t)g_voiceInform.voicePlay.dmaBufA,length);
        }
    }
}


extern void AudioHard_Init(void)
{
    DMA_InitTypeDef  DMA_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef        TIM_OCInitStructure = {0};
    
    /* enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* DMA1 Channel5 Config */
    DMA_DeInit(DMA1_Channel5);    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)0x40012C4C;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_voiceInform.voicePlay.dmaBufA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 1024;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    /*DMA 允许数据传输完成中断*/
    DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);
    
    /* DMA1 Channel5 enable */
    DMA_Cmd(DMA1_Channel5, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11,GPIO_AF_2);
    
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* TIMER configuration 设置载波频率为48K 
    配置每4次改变PWM 1次实现输出频率固定为8K*/
    TIM_TimeBaseStructure.TIM_Prescaler = 0;   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
    TIM_TimeBaseStructure.TIM_Period = 2000;          
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 5; //6 固定为8K
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
        
    /* Channel 4 Configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode =  TIM_OCMode_PWM2;    
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_Pulse = 0;  
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);  
    
    /* TIM1 DMA Update enable */
    TIM_DMACmd(TIM1, TIM_DMA_Update, ENABLE);

    //TIM_SelectCCDMA(TIM1, ENABLE);
    TIM_DMAConfig(TIM1, TIM_DMABase_CCR4, TIM_DMABurstLength_1Transfer);
    
    /* TIM1 enable */
    TIM_Cmd(TIM1, ENABLE);
    
    /* TIM1 PWM Outputs Enable */
    TIM_CtrlPWMOutputs(TIM1, DISABLE);
    //TIM_CtrlPWMOutputs(TIM1, ENABLE);
}


extern void Audio_PlayVoice(U8 Data)	
{   
    if(Data == 0)
    {
        return;
    }

    if(g_radioInform.voiceSw == 0) 
    {   
        //语音关闭
        if(g_radioInform.beepsSwitch == 1)
        {
            BeepOut(BEEP_ERROR);
        }   
        return;
    }
    else if(g_radioInform.language != LANG_CN) 
    {
        Data += vo_switch_en;
    }
    
    if(voice.voiceState)
    {//打断播报
        Audio_PlayStop();
    }
    
    voice.voiceState  = 1;
    voice.voiceBuf[0] = Data;
    voice.voiceCnt    = 1;
    voice.busyFlag = 0;
}


extern void Audio_PlayNumInQueue(U8 Data)
{
    U8 i;
    
  	if(g_radioInform.voiceSw == 0) 
	{   
	    //语音关闭
	    return;
	}
	
	Data += vo_0;

	if(g_radioInform.language != LANG_CN) 
	{
        Data += vo_switch_en;
	}
	
	if(voice.voiceCnt)
	{
	    for(i=voice.voiceCnt;i>0;i--)
	    {
	        voice.voiceBuf[i] = voice.voiceBuf[i-1];
	    }
	    //memcpy(&voice.voiceBuf[1],voice.voiceBuf,voice.voiceCnt);
	}
	voice.voiceState = 1;
    voice.voiceBuf[0] = Data;
    if(voice.voiceCnt < 5)
	{
        voice.voiceCnt++;
	}

}

extern void Audio_PlayChanNum(U8 Data)	
{
    U8 i,j;
    U8 k;
    
    if(g_radioInform.voiceSw == 0)
    {
        return;
    }
    voice.voiceState = 1;
    j= Data;

    i = Data / 100;
    j = Data % 100 / 10;
    k = Data % 10;

    if( g_radioInform.language == LANG_CN )
    {
        if( i > 0 )
        {
            if( k != 0 && j != 0 )
            {
                voice.voiceBuf[4] = i + vo_0;
                voice.voiceBuf[3] = vo_100;

                voice.voiceCnt = 5;
            }
            else
            {
                if( k == 0 && j == 0 )
                {
                    voice.voiceBuf[1] = i + vo_0;
                    voice.voiceBuf[0] = vo_100;

                    voice.voiceCnt = 2;
                }
                else
                {
                    voice.voiceBuf[3] = i + vo_0;
                    voice.voiceBuf[2] = vo_100;

                    voice.voiceCnt = 4;
                }
            }
            
        }

        if( j > 0 )
        {
            if( k != 0 )
            {
                voice.voiceBuf[2] = j + vo_0;
                voice.voiceBuf[1] = vo_10;
                voice.voiceBuf[0] = k + vo_0;

                if(i == 0)
                {
                    voice.voiceCnt = 3;
                }
            }
            else
            {
                voice.voiceBuf[1] = j + vo_0;
                voice.voiceBuf[0] = vo_10;

                if( i == 0 )
                {
                    voice.voiceCnt = 2;
                }
            }
        }
        else
        {
            if( i == 0 )
            {
                voice.voiceBuf[0] = k + vo_0;
                voice.voiceCnt = 1;
            }
            else
            {
                if( k != 0 )
                {
                    voice.voiceBuf[1] = vo_0;
                    voice.voiceBuf[0] = k + vo_0;
                }
            }
        }

    }
    else
    {
        if( Data < 20 || (i == 0 && j > 0 && k == 0))
        {//个数为1
            if( Data <= 10 )
            {
                voice.voiceBuf[0] = Data + vo_zero;
            }
            else if(Data > 10 && Data < 20)
            {
                voice.voiceBuf[0] = k + vo_eleven - 1;
            }
            else
            {
                voice.voiceBuf[0] = j + vo_twenty - 3;
            }

            voice.voiceCnt = 1;
        }
        else if( Data > 100 && (j < 2 || (k == 0 && j > 0)) )
        {//个数为4
            if( j < 2 )
            {
        #if vo_and
               voice.voiceBuf[3] = i + vo_zero;
               voice.voiceBuf[2] = vo_hundred; // Hundred
               voice.voiceBuf[1] = vo_and;
        #else
               voice.voiceBuf[2] = i + vo_zero;
               voice.voiceBuf[1] = vo_hundred; // Hundred
        #endif
               if(j == 1)
               {
                   if( k == 0 )
                   {
                       voice.voiceBuf[0] = 10 + vo_zero;;
                   }
                   else
                   {
                       voice.voiceBuf[0] = k + vo_eleven - 1;
                   }
               }
               else
               {
                   voice.voiceBuf[0] = k + vo_zero;
               }
            }
            else
            {
        #if vo_and
               voice.voiceBuf[3] = i + vo_zero;
               voice.voiceBuf[2] = vo_hundred;
               voice.voiceBuf[1] = vo_and;
               voice.voiceBuf[0] = j + vo_twenty - 3;
        #else
               voice.voiceBuf[2] = i + vo_zero;
               voice.voiceBuf[1] = vo_hundred;
               voice.voiceBuf[0] = j + vo_twenty - 3;
        #endif
            }
    #if vo_and
            voice.voiceCnt = 4;
    #else
            voice.voiceCnt = 3;
    #endif
        }
        else if( Data > 100 && (j >= 2 && k != 0) )
        {//个数为5
    #if vo_and
            voice.voiceBuf[4] = i + vo_zero;
            voice.voiceBuf[3] = vo_hundred;
            voice.voiceBuf[2] = vo_and;
            voice.voiceBuf[1] = j + vo_twenty - 3;
            voice.voiceBuf[0] = k + vo_zero;

            voice.voiceCnt = 5;
            
    #else
            voice.voiceBuf[3] = i + vo_zero;
            voice.voiceBuf[2] = vo_hundred;
            voice.voiceBuf[1] = j + vo_twenty - 3;
            voice.voiceBuf[0] = k + vo_zero;

            voice.voiceCnt = 4;
    #endif
        }
        else
        {//个数为2
            if( (Data >= 100 && (Data % 100 == 0)))
            {
                voice.voiceBuf[1] = i + vo_zero;
                voice.voiceBuf[0] = vo_hundred;
            }
            else
            {
                voice.voiceBuf[1] = j + vo_twenty - 3;
                voice.voiceBuf[0] = k + vo_zero;
            }

            voice.voiceCnt = 2;
        }
    }
}

extern void ChangeSigned2unsigned(U16 *dest, U16 *src,U16 length)
{
    U16 i;
    S16 dat;

    for(i=0;i<length;i++)
    {
        dat =  src[i];
        dest[i] = (dat + 2048) >> 1; 
    }
}

int Audio_Decoder(U8 *indata, S16 *outdata, int len)   
{   
    U8 *inp;
    S16 *outp;   
    U16 sign;   
    U16 delta;  
    U16 step; 
    int valpred;  
    U16 vpdiff; 
    int index;    
    U16 inputbuffer = 0;
    int bufferstep; 
    int count = 0;   
    U32 stepAddr = 0x8001040;
    U32 indexAddr = 0x8001000;

    outp = outdata;   
    inp = indata;   

    valpred = s_valprev;   
    index = (int)s_index;  

    step = *((U16*)(stepAddr + (index<<1) ));
    bufferstep = 0;   

    len *= 2;   

    while ( len-- > 0 ) 
    {   
        if ( bufferstep != 0 ) 
        {   
            delta = inputbuffer & 0xf;   
        } 
        else 
        {   
            inputbuffer = (U16)*inp++;   
            delta = (inputbuffer >> 4);   
        }   
        bufferstep = !bufferstep;   

        index += *((U32*)(indexAddr + (delta<<2) ));   
        if ( index < 0 ) index = 0;   
        if ( index > 58 ) index = 58;   
        sign = delta & 8;   
        delta = delta & 7;   
        vpdiff = step >> 3;   
        if ( (delta & 4) != 0 ) vpdiff += step;   
        if ( (delta & 2) != 0 ) vpdiff += step>>1;   
        if ( (delta & 1) != 0 ) vpdiff += step>>2;   

        if ( sign != 0 )   
        {   
            valpred -= vpdiff;   
            if ( valpred < -2048 )   
                valpred = -2048;   
        }   
        else   
        {   
            valpred += vpdiff;   
            if ( valpred > 2047 )   
                valpred = 2047;   
        }   

        step = *((U16*)(stepAddr + (index<<1) ));
        *outp++ = (short)valpred;
        count++;   
    }   

    s_valprev = (short)valpred;   
    s_index = (char)index;   

    return count;   
} 

extern void Audio_PlayStart(U8 index)
{
    U32 indexAddr;
    U16 i,startLen;
    U8  dacBuf[512];
    
    indexAddr = (index - 0x10)* 8 + FLASH_VOICE_BASIC_ADDR;
    SpiFlash_ReadBytes(indexAddr,(U8  *)&g_voiceInform.voiceIndex.length,8);
    g_voiceInform.voiceIndex.dataAddr += FLASH_VOICE_BASIC_ADDR;

    if(g_voiceInform.voiceIndex.length > 40960 || g_voiceInform.voiceIndex.length == 0x00)
    {//无音频数据，直接返回
        return;
    }
    
    //语音播报时将收发全部关闭
    Rfic_RxTxOnOffSetup(RFIC_IDLE);
    GPIOA->BSRR = GPIO_Pin_11; 

    //读取音频数据
    g_voiceInform.voicePlay.lastPackage = 0;
    SpiFlash_ReadBytes(g_voiceInform.voiceIndex.dataAddr,dacBuf,512);
    g_voiceInform.voicePlay.logicAddr = g_voiceInform.voiceIndex.dataAddr + 512;

    s_valprev = 0;
    s_index = 0;
    if(g_voiceInform.voiceIndex.length >= 512)
    {
        Audio_Decoder(dacBuf, (S16 *)g_voiceInform.voicePlay.dmaBufA, 512); 
    }
    else
    {
        startLen = Audio_Decoder(dacBuf, (S16 *)g_voiceInform.voicePlay.dmaBufA, g_voiceInform.voiceIndex.length); 

        for(i=startLen;i<1024;i++)
        {
            g_voiceInform.voicePlay.dmaBufA[i] = 2048;
        }
    }
    ChangeSigned2unsigned(g_voiceInform.voicePlay.dmaBufA,g_voiceInform.voicePlay.dmaBufA,1024);
    g_voiceInform.voicePlay.finishFlag = 0;
    g_voiceInform.voicePlay.dmaBufUsed = 1;
    g_voiceInform.voicePlay.dmaBufAUseFlag = 0;
    g_voiceInform.voicePlay.dmaBufBUseFlag = 1;
    g_voiceInform.voicePlay.usedLen = 0;

     /*DMA 允许数据传输完成中断*/
    DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);
    DMA_ClearFlag(DMA1_FLAG_GL5);    

    //启动语音播报
    DmaUpdateMemery(DMA1_Channel5,(uint32_t)g_voiceInform.voicePlay.dmaBufA,1024);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);      
    voice.busyFlag = 1;
    
    //延时消除开启的破音
    DelayMs(30);
    //开启喇叭
    SpeakerSwitch(ON);

    g_sysRunPara.rfTxFlag.voxDetDly = 10;
}

extern void Audio_PlayContinue(void)
{
    U8 dacBuf[512];

    if(g_voiceInform.voicePlay.finishFlag == 1)
    {
        voice.busyFlag = 0;
        return;
    }
    if(g_voiceInform.voicePlay.dmaBufAUseFlag)
    {
        SpiFlash_ReadBytes(g_voiceInform.voicePlay.logicAddr,dacBuf,512);
        g_voiceInform.voicePlay.logicAddr = g_voiceInform.voicePlay.logicAddr + 512;

        Audio_Decoder(dacBuf, (S16 *)g_voiceInform.voicePlay.dmaBufA, 512);   
        ChangeSigned2unsigned(g_voiceInform.voicePlay.dmaBufA,g_voiceInform.voicePlay.dmaBufA,1024);
        g_voiceInform.voicePlay.dmaBufAUseFlag = 0;
    }

    if(g_voiceInform.voicePlay.dmaBufBUseFlag)
    {
        SpiFlash_ReadBytes(g_voiceInform.voicePlay.logicAddr,dacBuf,512);
        g_voiceInform.voicePlay.logicAddr = g_voiceInform.voicePlay.logicAddr + 512;

        Audio_Decoder(dacBuf, (S16 *)g_voiceInform.voicePlay.dmaBufB, 512);
        ChangeSigned2unsigned(g_voiceInform.voicePlay.dmaBufB,g_voiceInform.voicePlay.dmaBufB,1024);
        g_voiceInform.voicePlay.dmaBufBUseFlag = 0;
    }
}


extern void Audio_PlayStop(void)
{
    SpeakerSwitch(OFF);    

    DMA_Cmd(DMA1_Channel5, DISABLE);     
    TIM_CtrlPWMOutputs(TIM1, DISABLE); 
    //清除中断标志
    DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,DISABLE);
    DMA_ClearFlag(DMA1_FLAG_GL5); 
    
    voice.voiceState = 0;
    voice.voiceCnt = 0;
    voice.busyFlag = 0;  
}


extern U8 Audio_CheckBusy(void)
{
    return voice.voiceState;
}

extern void Audio_PlayTask(void)
{
    if(voice.voiceState == 1)
    {
        if(voice.busyFlag == 0)
        {
            if(voice.voiceCnt == 0)
            {
                //语音播报结束
                voice.voiceState = 0;
                g_sysRunPara.rfTxFlag.voxDetDly = 8;
    
                //关闭喇叭
                SpeakerSwitch(OFF);
                DelayMs(30);

                TIM_CtrlPWMOutputs(TIM1, DISABLE);    

                if(g_rfRxState == WAIT_RXEND)
                {
                    g_rfRxState = RX_READY;
                }
                else
                {
                    Rfic_RxTxOnOffSetup(RFIC_RXON);
                }
                return;
            }
            else
            {
                Audio_PlayStart(voice.voiceBuf[--voice.voiceCnt]);
            }
        }
        else
        {
            Audio_PlayContinue();
        }
    }
}

extern void Audio_PlayVoiceLock(U8 Data)
{
    Audio_PlayVoice(Data);

    while(voice.voiceState) 
    {  
        Audio_PlayTask();
    }
}

extern void VoiceBroadcastWithBeepLock(U8 voiceDat,ENUM_BEEPMODE beepData)
{
    if(g_radioInform.voiceSw == 0 || voiceDat == 0)
    {
        BeepOut(beepData);
    }
    else
    {
        Audio_PlayVoiceLock(voiceDat);
    }
}

