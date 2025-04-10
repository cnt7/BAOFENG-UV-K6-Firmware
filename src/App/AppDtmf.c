#include "includes.h"
enum{ DTMF_ID_0, DTMF_ID_1, DTMF_ID_2, DTMF_ID_3, DTMF_ID_4, DTMF_ID_5, DTMF_ID_6, DTMF_ID_7, 
      DTMF_ID_8, DTMF_ID_9, DTMF_ID_A, DTMF_ID_B, DTMF_ID_C, DTMF_ID_D, DTMF_ID_STAR, DTMF_ID_POUND};

const DTMFCODESTRUCT DTMFCODE[] = 
{
    {0X25F3,  0X35E1},     //  0  941  1336
    {0X1C1C,  0X30C2},     //  1  697  1209
    {0X1C1C,  0X35E1},     //  2  697  1336
    {0X1C1C,  0X3B91},     //  3  697  1477
    {0X1F0E,  0X30C2},     //  4  770  1209
    {0X1F0E,  0X35E1},     //  5  770  1336
    {0X1F0E,  0X3B91},     //  6  770  1477
    {0X225C,  0X30C2},     //  7  852  1209
    {0X225C,  0X35E1},     //  8  852  1336
    {0X225C,  0X3B91},     //  9  852  1477
    {0X1C1C,  0X41DC},     //  A  697  1633
    {0X1F0E,  0X41DC},     //  B  770  1633
    {0X225C,  0X41DC},     //  C  852  1633
    {0X25F3,  0X41DC},     //  D  941  1633  
    {0X25F3,  0X30C2},     //  *  941  1209
    {0X25F3,  0X3B91},     //  # 941  1477
    {0X2248,   0},         //  850  16
    {0X2854,   0},         //  1000 17
    {0X3A7A,   0},         //  1450 18
};

//发射单音
const U8  SingalTone[] = 
{
    100,
    145,
    175,
    210
};

// 分隔符列表
const U8  TblSeprator[] = {
    DTMF_ID_A, DTMF_ID_B, DTMF_ID_C, DTMF_ID_D, DTMF_ID_STAR, DTMF_ID_POUND,
};
// 组呼符列表
const U8  TblGroupCall[] = {
    0XFF, DTMF_ID_A, DTMF_ID_B, DTMF_ID_C, DTMF_ID_D, DTMF_ID_STAR, DTMF_ID_POUND,
};

STF_DTMFSTORE g_dtmfStore;
STR_DTMFINFO dtmfInfo;

extern void EnterDtmfEditMode(void)
{
    if(g_sysRunPara.sysRunMode != MODE_MAIN && (g_rfRxState == WAIT_RXEND))
    {
        BeepOut(BEEP_NULL);
        return;
    }
    BeepOut(BEEP_FASTSW);
    //需要关闭双守候，避免出错
    DualStandbyWorkOFF();
    g_sysRunPara.sysRunMode = MODE_DTMF;

    ResetInputBuf();

    //显示DTMF输入界面
    DisplayDtmfEditHome();
}

void InputDtmfCode(U8 codeData)
{
    g_inputbuf.time = INPUT_DTMF_TIME_OUT;
    if(g_inputbuf.len < 15)
    {
        g_inputbuf.buf[g_inputbuf.len] = codeData;
        g_inputbuf.len++;
    }
    else
    {
        BeepOut(BEEP_FMSW1);
        return;
    }

    BeepOut(BEEP_NULL);
    DisplayInputDtmf();
}

extern void GetDtmfEditCode(void)
{
    U8 i;

    if(g_sysRunPara.sysRunMode != MODE_DTMF)
    {
        return;
    }
    
    DisplayRadioHome();

    if(g_inputbuf.len == 0)
    {//无输入时，直接退出该模式
        g_sysRunPara.sysRunMode = MODE_MAIN;
        return; 
    }
    
    g_sysRunPara.txDtmfCode.codeLen = g_inputbuf.len;

    memset(g_sysRunPara.txDtmfCode.code,0xFF,16);
    for(i=0;i<g_inputbuf.len;i++)
    {
        if(g_inputbuf.buf[i] > 0x39 && g_inputbuf.buf[i] <= 0x44)
        {//ABCD
            g_sysRunPara.txDtmfCode.code[i] = g_inputbuf.buf[i]- 0x37;
        }
        else if(g_inputbuf.buf[i] == '*')
        {//*
            g_sysRunPara.txDtmfCode.code[i] = 0x0E;
        }
        else if(g_inputbuf.buf[i] == '#')
        {//#
            g_sysRunPara.txDtmfCode.code[i] = 0x0F;
        }
        else
        {//0-9
            g_sysRunPara.txDtmfCode.code[i] = g_inputbuf.buf[i] &0x0f;
        }
    }
    //获取到DTMF发码后，切换为正常模式
    g_sysRunPara.sysRunMode = MODE_MAIN;
}

extern void ResetDtmfEditCode(void)
{
    g_sysRunPara.txDtmfCode.codeLen = 0;
}

extern void ExitDtmfEditMode(void)
{    
    g_sysRunPara.sysRunMode = MODE_MAIN;

    BeepOut(BEEP_EXITMENU);
    
    ResetInputBuf();
    //需要恢复双守候功能
    DualStandbyWorkOFF();
    DisplayHomePage();
}

extern void KeyProcess_DtmfInput(U8 keyEvent)
{
    U8 input;
    
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
            input = keyEvent - KEYID_0 + 0x30;
            InputDtmfCode(input);
            break;
        case KEYID_MENU:
            InputDtmfCode('A');
            break;
        case KEYID_UP:
            InputDtmfCode('B');
            break;
        case KEYID_DOWN:
            InputDtmfCode('C');
            break;
        case KEYID_EXIT:
            InputDtmfCode('D');
            break;
        case KEYID_STAR:
            InputDtmfCode('*');
            break;     
        case KEYID_WELL:
            InputDtmfCode('#');
            break;         

        case KEYID_SIDEKEY1: //退格键
            if(g_inputbuf.len)
            {
                g_inputbuf.len--;
                g_inputbuf.time = INPUT_DTMF_TIME_OUT;
                g_inputbuf.buf[g_inputbuf.len] = 0x00;
                //显示输入模式
                DisplayInputDtmf();
                BeepOut(BEEP_FMSW1);
            }
            else
            {
                BeepOut(BEEP_NULL);
            }
            break;
        case KEYID_SIDEKEY2:
            ExitDtmfEditMode();
            break;
            
        default:
            BeepOut(BEEP_NULL);
            break;
    }
}

extern void DtmfInfoInit(void)
{
    SpiFlash_ReadBytes(DTMFINFOR_ADDR, (U8  *)&g_dtmfStore.machineId[0], sizeof(STF_DTMFSTORE));
    if(g_dtmfStore.separator > 5)
    {
        g_dtmfStore.separator = 4;
    }

    if(g_dtmfStore.groupCall > 6)
    {
        g_dtmfStore.groupCall = 5;
    }

    if(g_dtmfStore.onTime > 5)
    {
        g_dtmfStore.onTime = 1;
    }

    if(g_dtmfStore.offTime > 5)
    {
        g_dtmfStore.offTime = 1;
    }

    SpiFlash_ReadBytes(DTMFINFOR_ADDR + 0x20, (U8  *)&dtmfInfo.contact[0].id[0], sizeof(STR_CONTACT) * 20);

    SpiFlash_ReadBytes(DTMFINFOR_ADDR + 0x0180, (U8  *)&dtmfInfo.onlineCode, 16);
    SpiFlash_ReadBytes(DTMFINFOR_ADDR + 0X0180 + 16, (U8  *)&dtmfInfo.offlineCode, 16);
}

extern void DtmfSendKeypadCode(U8 code)
{
    if(code >= 17)
    {// 单音
        Rfic_TxSingleTone_On(1);
    }
    else
    {
        Rfic_EnterDTMFMode(1);  
    }
    
    if(g_radioInform.dtmfTone&BIT0)
    {
        SpeakerSwitch(ON);
        Rfic_RxTxOnOffSetup(RFIC_TXTONE);
        Rfic_SetAfout(3);
    }

    if(code >= 17)
    {
        Rfic_SetToneFreq(SingalTone[code-17]);
    }
    else
    {
        Rfic_SetDtmfFreq(DTMFCODE[code].tone1Freq,DTMFCODE[code].tone2Freq);
    }
    g_sysRunPara.dtmfToneFlag = 1;
}

extern void DtmfSendTxOver(void)
{
    U8 code;
    
    Rfic_EnterDTMFMode(1);
    SpeakerSwitch(ON);

    Rfic_RxTxOnOffSetup(RFIC_TXTONE);
    Rfic_SetAfout(0xF1);    

    for(code = 17;code>15;code--)
    {
        Rfic_SetDtmfFreq(DTMFCODE[code].tone1Freq,DTMFCODE[code].tone2Freq); 
        DelayMs(80);
    }
    SpeakerSwitch(OFF);
    
    Rfic_SetDtmfFreq(0,0);
    Rfic_ExitDTMFMode();
    Rfic_SetAfout(0);        
}

extern void DtmfSendTask(void)
{
    U8  tblOnTime[] = {5, 10, 20, 30, 40, 50};
    U8  tblOffTime[] = {5, 10, 20, 30, 40, 50};

    while(dtmfInfo.state)
    {
        if(dtmfInfo.timeOut == 0)
        {
            switch(dtmfInfo.state)
            {
                case DTMF_SETUP:
                    dtmfInfo.sendFlag = 0;
                    dtmfInfo.enCodeNum = 0;
                    Rfic_EnterDTMFMode(1);
   
                    dtmfInfo.enCode = dtmfInfo.code[dtmfInfo.enCodeNum];
                    
                    if(g_radioInform.dtmfTone&BIT1)    
                    {

                        Rfic_RxTxOnOffSetup(RFIC_TXTONE);
                        Rfic_SetAfout(0xF1);
                        SpeakerSwitch(ON);
                    }
                    dtmfInfo.state = DTMF_FREQ;

                case DTMF_FREQ:
                    if(dtmfInfo.sendFlag == 0)
                    {
                        Rfic_SetDtmfFreq(DTMFCODE[dtmfInfo.enCode].tone1Freq,DTMFCODE[dtmfInfo.enCode].tone2Freq);
                        dtmfInfo.timeOut = tblOnTime[g_dtmfStore.onTime] * 10;
                        dtmfInfo.sendFlag = 1;
                    }
                    else
                    {
                        Rfic_SetDtmfFreq(0,0);
                        dtmfInfo.timeOut = tblOffTime[g_dtmfStore.offTime] * 10;  
                        dtmfInfo.sendFlag = 0;
                        dtmfInfo.enCodeNum++;
                        dtmfInfo.enCode = dtmfInfo.code[dtmfInfo.enCodeNum];

                        if(dtmfInfo.enCode == 0xFF || dtmfInfo.enCodeNum >= 16)
                        {
                            dtmfInfo.state = DTMF_STOP;
                            dtmfInfo.enCodeNum = 0;
                            dtmfInfo.timeOut = 10;

                            SpeakerSwitch(OFF);
                            Rfic_TxSingleTone_Off();
                            Rfic_SetAfout(0);
                            Rfic_ExitDTMFMode(); 
                        }
                    }
                    break;
                case DTMF_STOP:
                default:
                    dtmfInfo.state = DTMF_OVER;
                    dtmfInfo.sendFlag = 0;
                    break;
            }
        }
    }
}


extern void DtmfSendCodeOn(U8  type)
{   
    if(type == DTMF_ALARMCODE)
    {
        dtmfInfo.code[0] = 1;
        dtmfInfo.code[1] = 1;
        dtmfInfo.code[2] = 9;
        dtmfInfo.code[3] = 0xFF;

        dtmfInfo.state = DTMF_SETUP;
        
        dtmfInfo.timeOut = (g_radioInform.pttIdTime + 1) * 100;
    }
    else if(type == DTMF_ALARMID)
    {
        SpiFlash_ReadBytes(DTMF_CODE_ADDR+g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].dtmfgroup*0x10,dtmfInfo.code, 5);
 
        //判断编码是否存在
        if(dtmfInfo.code[0] == 0xff)
        {
            return;
        }
        else
        {
            dtmfInfo.code[8] = DTMF_ID_STAR; 
            memcpy(dtmfInfo.code+9,g_dtmfStore.machineId,DTMF_ANI_LEN);
        }
        
        dtmfInfo.state = DTMF_SETUP;
        dtmfInfo.timeOut = (g_radioInform.pttIdTime + 1) * 100;  
    }
    else if(type == DTMF_TYPEIN)
    {
        memcpy(dtmfInfo.code,g_sysRunPara.txDtmfCode.code,g_sysRunPara.txDtmfCode.codeLen);
        if(g_sysRunPara.txDtmfCode.codeLen < 16)
        {
            dtmfInfo.code[g_sysRunPara.txDtmfCode.codeLen] = 0xFF;
        }
        
        if(g_sysRunPara.txDtmfCode.codeLen == 3)
        {
            dtmfInfo.code[3] = TblSeprator[g_dtmfStore.separator];
            memcpy((U8  *)&dtmfInfo.code[4],(U8  *)&g_dtmfStore.machineId[0],DTMF_ANI_LEN);
            dtmfInfo.code[7] = 0XFF;
        }
   
        g_sysRunPara.txDtmfCode.codeLen = 0;

        dtmfInfo.state = DTMF_SETUP;
        dtmfInfo.timeOut = (g_radioInform.pttIdTime + 1) * 100;
    }
    else if(type == DTMF_ONLINE || type == DTMF_OFFLINE)
    {
        if(g_sysRunPara.rfTxFlag.voxWorkDly == 0 ||(PTT_GetKeyVal() == 0))
        {
            if(type == DTMF_ONLINE)
            {
                memcpy((U8  *)&dtmfInfo.code[0],(U8  *)&dtmfInfo.onlineCode,16);
            }
            else
            {
                memcpy((U8  *)&dtmfInfo.code[0],(U8  *)&dtmfInfo.offlineCode,16);
            }
            
            //判断编码是否存在
            if(dtmfInfo.code[0] == 0xff)
            {
                return;
            }

            dtmfInfo.timeOut = (g_radioInform.pttIdTime + 1) * 100;
            dtmfInfo.state = DTMF_SETUP;
        }
    }
    else
    {
        if(g_sysRunPara.rfTxFlag.voxWorkDly == 0 ||(PTT_GetKeyVal() == 0))
        {
            memcpy((U8  *)&dtmfInfo.code[0],(U8  *)&dtmfInfo.contact[g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].dtmfgroup].id[0],DTMF_ANI_LEN);
            dtmfInfo.code[3] = TblSeprator[g_dtmfStore.separator];
            memcpy((U8  *)&dtmfInfo.code[4],(U8  *)&g_dtmfStore.machineId[0],DTMF_ANI_LEN);
            dtmfInfo.code[7] = 0XFF;
            
            //判断编码是否存在
            if(dtmfInfo.code[0] == 0xff)
            {
                return;
            }

            dtmfInfo.timeOut = (g_radioInform.pttIdTime + 1) * 100;
            dtmfInfo.state = DTMF_SETUP;
        }
    } 

    DtmfSendTask();
}

void DtmfReceiveSetup(void)
{
    if(g_sysRunPara.sysRunMode == MODE_WEATHER)
    {//天气预报模式不解DTMF
        return;
    }

    Rfic_EnterDTMFMode(0);
    memset( dtmfInfo.code, 0xff, 16 );
    dtmfInfo.cntRxDtmf = 0;
    dtmfInfo.callType = CALLTYPE_NONE;
}

void ClearDisANIFlag(void)
{
    if(dtmfInfo.flagDtmfMatch)
    {
        if(g_sysRunPara.sysRunMode != MODE_MAIN)
        {
            dtmfInfo.flagDtmfMatch = 0;
            return;
        }
        dtmfInfo.flagDtmfMatch = 0;
        DisplayRadioHome();
    }
}

void DtmfClrMatchTimer(void)
{
    dtmfInfo.matchTime[0] = 0;
    dtmfInfo.matchTime[1] = 0;
}

void Task_HangUp(void)
{
    if(dtmfInfo.flagDtmfMatch == 0)
    {
        return;
    }

    if(dtmfInfo.matchTime[dtmfInfo.flagDtmfMatch - 1] == 0)
    {
        ClearDisANIFlag();
        dtmfInfo.flagDtmfMatch = 0;
    }
}

void DtmfRstMatchTimer(U8  set)
{
    if(dtmfInfo.flagDtmfMatch == 0)
    {
        return;
    }

    if(dtmfInfo.matchTime[g_ChannelVfoInfo.dualAB] == 0 && set == 0)
    {
        return;
    }

    if(g_radioInform.hangUpTime > 7)
    {
        g_radioInform.hangUpTime = 7;
    }

    dtmfInfo.matchTime[g_ChannelVfoInfo.dualAB] = (g_radioInform.hangUpTime + 3) * 10; //  3 - 10s
}

U8  DtmfGetMatchStatue(void)
{
    return dtmfInfo.flagDtmfMatch;
}

void DtmfAnalyseFunc(void)
{
    U8  i;
    U8  indexSymbolStar[5] = {0}; //*号标志
    U8  countSymbolStar = 0;
    
    if( dtmfInfo.cntRxDtmf < 3 )
    {
        dtmfInfo.cntRxDtmf = 0;
        return;
    }

    //    组呼: 组号+**    全呼: *+#
    for(i = 0; i < dtmfInfo.cntRxDtmf; i++)
    {
        if(dtmfInfo.code[i] == TblSeprator[g_dtmfStore.separator])
        {
            indexSymbolStar[countSymbolStar++] = i;
            if(countSymbolStar >= 5)
            {
                countSymbolStar = 4;
            }
        }
    }

    if(g_ChannelVfoInfo.chVfoInfo[g_ChannelVfoInfo.switchAB].spMute != 1)
    {
        return;
    }

    // 解析呼叫类型
    if(countSymbolStar > 0)
    {
        if(indexSymbolStar[0] == 3 && dtmfInfo.cntRxDtmf == 7)
        {// 连接符号 表示呼叫
            for(i = 0; i < 3; i++)
            {
                dtmfInfo.callCode[i] = dtmfInfo.code[i];
                dtmfInfo.aniCode[i] = dtmfInfo.code[i + 4];
            }
            dtmfInfo.callCode[3] = '\0';
            dtmfInfo.aniCode[3] = '\0';

            for(i = 0; i < 3; i++)
            {
                if(dtmfInfo.callCode[i] != g_dtmfStore.machineId[i])
                {
                    if(g_dtmfStore.groupCall != 0)
                    {
                        if(dtmfInfo.callCode[i] != TblGroupCall[g_dtmfStore.groupCall])
                        {
                            dtmfInfo.callType = CALLTYPE_NONE;
                            return;
                        }
                    }
                    else
                    {
                        dtmfInfo.callType = CALLTYPE_NONE;
                        return;
                    }
                }
            }
            dtmfInfo.callType = CALLTYPE_ID;
            dtmfInfo.flagDtmfMatch = g_ChannelVfoInfo.dualAB + 1;
            DtmfRstMatchTimer(1);
            DisplayAniMsg((U8  *)dtmfInfo.aniCode, (U8  *)dtmfInfo.callCode);
        }
    }
    
}

/**************************************************************************************
* 函 数 名: DtmfReceiveTask
* 功能描述: DTMF 解码任务
* 全局变量: 
* 输入参数：
* 输出参数:
* 返　　回:
* 说    明：2019.09.05 zjr  v1.0  
**************************************************************************************/
extern void DtmfReceiveTask(void)
{  	
    if( g_sysRunPara.rfRxFlag.rxReceived == 0)
    {
        dtmfInfo.cntRxDtmf = 0;
        return;
    }
    
    if(dtmfInfo.timeRxOut == 0 && dtmfInfo.cntRxDtmf > 0)
    {
        DtmfAnalyseFunc();
        dtmfInfo.cntRxDtmf = 0;
        memset( dtmfInfo.code, 0xff, 16 );
    }

    if(Rfic_GetDTMF_Link())
    {
        dtmfInfo.code[dtmfInfo.cntRxDtmf++] = Rfic_ReadDTMF();
      
        // 大于等于6位或者接收到#号，表示该DTMF段接收结束
        dtmfInfo.timeRxOut = 4; // 400ms
        if( dtmfInfo.cntRxDtmf >= 16)
        {
            if( dtmfInfo.cntRxDtmf >= 3 )
            {
                DtmfAnalyseFunc();
            }
            
            dtmfInfo.cntRxDtmf = 0;
        }   
    }
}

