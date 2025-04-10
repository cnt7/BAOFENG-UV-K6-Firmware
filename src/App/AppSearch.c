#include "includes.h"

STR_SEARCHFREQ searchFreqImofs; 
U8 searchSaveFlag;

extern void EnterSearchFreqMode(void)
{
    Menu_ExitMode();

    //关闭双守功能
    DualStandbyWorkOFF();

    SpeakerSwitch(OFF);
    Rfic_SetAfout(OFF);
    LedRxSwitch(LED_OFF);

    g_sysRunPara.sysRunMode = MODE_SEARCH;

    searchFreqImofs.band = FREQ_BAND_UHF;
    searchFreqImofs.step = SF_Setup;
    searchFreqImofs.cntSample = 0;
    searchFreqImofs.timesForSwitchBand = 5;
    g_rfState = RF_NONE;
    searchSaveFlag = 1;
    
    Rfic_SwitchFM_AM(ModeFM);

    RF_PowerSet( FREQ_BAND_UHF, PWR_RXON );//关闭所有电源
    SearchFreqDisplayHome();
}


void SearchFreqModeBandSwitch(void)
{
    if(searchFreqImofs.band == FREQ_BAND_UHF)
    {
        searchFreqImofs.band = FREQ_BAND_VHF;
        RF_PowerSet( FREQ_BAND_VHF, PWR_RXON );
    }
    else if(searchFreqImofs.band == FREQ_BAND_VHF)
    {
        searchFreqImofs.band = FREQ_BAND_200M;
        RF_PowerSet( FREQ_BAND_VHF, PWR_RXON );
    }

    else if(searchFreqImofs.band == FREQ_BAND_200M)
    {
        searchFreqImofs.band = FREQ_BAND_350M;
        RF_PowerSet( FREQ_BAND_UHF, PWR_RXON );
    }
    else if(searchFreqImofs.band == FREQ_BAND_VHF)
    {
        searchFreqImofs.band = FREQ_BAND_350M;
        RF_PowerSet( FREQ_BAND_UHF, PWR_RXON );
    }
    else
    {
        searchFreqImofs.band = FREQ_BAND_UHF;
        RF_PowerSet( FREQ_BAND_UHF, PWR_RXON );
    }     
    searchFreqImofs.step = SF_Setup;
}

extern void ExitSearchFreqMode(U8  disHome)
{
    if( g_sysRunPara.sysRunMode != MODE_SEARCH )
    {
        return;
    }
    
    g_sysRunPara.sysRunMode = MODE_MAIN;
    g_rfRxState = RX_READY;
    g_rfState = RF_RX;

    Rfic_FreqScan_Disable();

    //切换为显示主界面
    if(disHome)
    {
        searchSaveFlag = 0;
        DisplayHomePage();
    }
}

U16 CheckIsStandardDCS(U32 dcsData)
{
    U8  i,j;
    U32 tempDCS;

    dcsData &= 0x007FFFFF;
    for( i = 0; i < 105; i++)
    {
        tempDCS = GOLAY_ENCODE(DCS_TAB[i]);
        
        for( j = 0; j < 23; j++ )
        {
            if( tempDCS & 0x00400000 )
            {
                tempDCS <<= 1;
                tempDCS |= 0x00000001;
            }
            else
            {
                tempDCS <<= 1;
            }
            tempDCS &= 0x007FFFFF;

            if( dcsData == tempDCS)
            {
                searchFreqImofs.CtsResult = i + 1;
                return DCS_TAB[i];
            }
        }
    }

    return 0;
}

extern U16 CheckIsStandardCTCSS(U32 dcsData)
{
    U8  i;
    U32 temp;

    dcsData &= 0x007FFFFF;
    for( i = 0; i < 51; i++)
    {
        if(dcsData > CTCS_TAB[i])
        {
            temp = dcsData - CTCS_TAB[i];
        }
        else
        {
            temp = CTCS_TAB[i] - dcsData;
        }

        if(temp < 10)
        {
            searchFreqImofs.CtsResult = CTCS_TAB[i];
            return CTCS_TAB[i];
        }
    }

    return searchFreqImofs.CtsResult;
}

extern void SearchFreqTask(void)
{
    U8 noise;
    U32 tempFreq;

    if(g_sysRunPara.sysRunMode != MODE_SEARCH)
    {//不在扫频模式直接返回
        return;
    }

    switch(searchFreqImofs.step)
    {
        case SF_Setup:
            if(searchFreqImofs.flagRx == 1)
            {
                SpeakerSwitch(OFF);
                Rfic_SetAfout(OFF);
                LedRxSwitch(LED_OFF);
            }
            searchFreqImofs.flagRx = 0;
            
            Rfic_FreqScan_Enable();

            searchFreqImofs.cntSample = 0;
            searchFreqImofs.dcsCtsType = SUBAUDIO_NONE;
            searchFreqImofs.step = SF_Wait;
            
            break;
            
        case SF_Wait:

            tempFreq = Rfic_CheckFreqScan();
            if(tempFreq == 0)
            {
                break;
            }

            Rfic_FreqScan_Disable();

            searchFreqImofs.bufFreq[searchFreqImofs.cntSample++] = tempFreq;
            if(searchFreqImofs.cntSample >= 2)
            {
                searchFreqImofs.cntSample = 0;
                if(searchFreqImofs.bufFreq[0] > searchFreqImofs.bufFreq[1])
                {
                    tempFreq = searchFreqImofs.bufFreq[0] - searchFreqImofs.bufFreq[1];
                }
                else
                {
                    tempFreq = searchFreqImofs.bufFreq[1] - searchFreqImofs.bufFreq[0];
                }

                if(tempFreq < 25)
                {
                    searchFreqImofs.freq = (searchFreqImofs.bufFreq[0] + searchFreqImofs.bufFreq[1]) / 2;
                    switch(searchFreqImofs.band)
                    {
                        case FREQ_BAND_UHF:
                            if(searchFreqImofs.freq > 52000000)
                            {
                                searchFreqImofs.freq = searchFreqImofs.freq / 2;
                            }

                            if(searchFreqImofs.freq < 40000000)
                            {
                                searchFreqImofs.step = SF_Setup;
                                return;
                            }
                                
                            break;

                        case FREQ_BAND_VHF:
                            if(searchFreqImofs.freq >= 36000000)
                            {
                                searchFreqImofs.freq = searchFreqImofs.freq / 3;
                            }
                            else if(searchFreqImofs.freq > 25000000)
                            {
                                searchFreqImofs.freq = searchFreqImofs.freq / 2;
                            }
                            else if(searchFreqImofs.freq < 10000000)
                            {
                                searchFreqImofs.step = SF_Setup;
                                return;
                            }

                            if(searchFreqImofs.freq > 20000000)
                            {
                                searchFreqImofs.step = SF_Setup;
                                return;
                            }
                            
                            break;

                        case FREQ_BAND_200M:
                            if(searchFreqImofs.freq >= 40000000)
                            {
                                searchFreqImofs.freq = searchFreqImofs.freq / 2;
                            }
                            else if(searchFreqImofs.freq < 20000000)
                            {
                                searchFreqImofs.step = SF_Setup;
                                return;
                            }

                            if(searchFreqImofs.freq > 30000000)
                            {
                                searchFreqImofs.step = SF_Setup;
                                return;
                            }
                            break;

                        case FREQ_BAND_350M:
                            if(searchFreqImofs.freq >= 70000000)
                            {
                                searchFreqImofs.freq = searchFreqImofs.freq / 2;
                            }
                            else if(searchFreqImofs.freq < 30000000)
                            {
                                searchFreqImofs.step = SF_Setup;
                                return;
                            }

                            if(searchFreqImofs.freq > 40000000)
                            {
                                searchFreqImofs.step = SF_Setup;
                                return;
                            }
                            break;
                    }

                    searchFreqImofs.step = SF_Check;
                    return;
                }
            }
            
            Rfic_FreqScan_Enable();
            break;

        case SF_Check:
            if(XTAL_ADJUST > 16)
            {
                XTAL_ADJUST = 8; // 零点
            }
            tempFreq = searchFreqImofs.freq * xtal26MAdjust[XTAL_ADJUST] / 10000000L;
            if(XTAL_ADJUST > 8)// 8为0点的索引
            {
                searchFreqImofs.freq -= tempFreq;
            }
            else
            {
                searchFreqImofs.freq += tempFreq;
            }
            //按250Hz取整
            searchFreqImofs.freq = (searchFreqImofs.freq + 13) / 25 * 25;
            //限制频率超过范围问题
            switch(searchFreqImofs.band)
            {
                case FREQ_BAND_VHF:
                    if(searchFreqImofs.freq > 17400000)
                    {
                        searchFreqImofs.freq = 17400000;
                    }
                    else if(searchFreqImofs.freq < 13600000)
                    {
                        searchFreqImofs.freq = 13600000;
                    }
                    else
                    {
                    }
                    break;

                case FREQ_BAND_200M:
                    if(searchFreqImofs.freq > 26000000)
                    {
                        searchFreqImofs.freq = 26000000;
                    }
                    else if(searchFreqImofs.freq < 20000000)
                    {
                        searchFreqImofs.freq = 20000000;
                    }
                    else
                    {
                    }
                    break;

                case FREQ_BAND_350M:
                    if(searchFreqImofs.freq > 39000000)
                    {
                        searchFreqImofs.freq = 39000000;
                    }
                    else if(searchFreqImofs.freq < 30000000)
                    {
                        searchFreqImofs.freq = 35000000;
                    }
                    break;    
                default:
                case FREQ_BAND_UHF:
                    if(searchFreqImofs.freq > 52000000)
                    {
                        searchFreqImofs.freq = 52000000;
                    }
                    else if(searchFreqImofs.freq < 40000000)
                    {
                        searchFreqImofs.freq = 40000000;
                    }
                    else
                    {
                    }
                    break;    
            }
            Rfic_CtcDcsScan_Setup(searchFreqImofs.freq);
            searchFreqImofs.step = SCtsDcs_Setup;  
            SearchFreqModeDisplayStepMsg( STEP_SEEK_CTSDCS );

            break;

        case SCtsDcs_Setup:
            
            searchFreqImofs.overTime = 15; // 1.5s
            searchFreqImofs.step = SCtsDcs_Wait;
            
            break;

        case SCtsDcs_Wait:

            searchFreqImofs.CtsResult = Rfic_GetCtsDcsData();
            if(searchFreqImofs.CtsResult != 0)
            {
                searchFreqImofs.dcsCtsType = Rfic_GetSCtsDcsType();
                if(searchFreqImofs.dcsCtsType == SUBAUDIO_CTS)
                {
                    searchFreqImofs.CtsResult = searchFreqImofs.CtsResult * 10 * 10000 / 206489;
                    DelayMs(200);
                    
                    if(Rfic_SubaudioDetect() == 0)
                    {
                         SearchFreqModeDisplayDCSData(SUBAUDIO_NONE, 0, 0);
                         searchFreqImofs.dcsCtsType = SUBAUDIO_NONE;
                    }
                    else
                    {
                        if(searchFreqImofs.CtsResult > 600)
                        {
                            searchFreqImofs.CtsResult = CheckIsStandardCTCSS(searchFreqImofs.CtsResult);
                            SearchFreqModeDisplayDCSData(SUBAUDIO_CTS, searchFreqImofs.CtsResult, 0);
                        }
                        else
                        {
                            SearchFreqModeDisplayDCSData(SUBAUDIO_NONE, 0, 0);
                            searchFreqImofs.dcsCtsType = SUBAUDIO_NONE;
                        }
                    }
                    
                }
                else
                {
                    DTCSS_WithRfic(searchFreqImofs.CtsResult, 1);
                    DelayMs(200);
                    if(Rfic_SubaudioDetect() == 0)
                    {
                        SearchFreqModeDisplayDCSData(SUBAUDIO_NONE, 0, 0);
                        searchFreqImofs.dcsCtsType = SUBAUDIO_NONE;
                    }
                    else
                    {
                        {
                            U32 tempDCS;

                            tempDCS = CheckIsStandardDCS(searchFreqImofs.CtsResult);
                            if(tempDCS != 0)
                            {
                                searchFreqImofs.dcsIsStandard = 1; 
                                SearchFreqModeDisplayDCSData(SUBAUDIO_DCS_N, tempDCS, 1);
                            }
                            else
                            {
                                searchFreqImofs.dcsIsStandard = 0; 
                                SearchFreqModeDisplayDCSData(SUBAUDIO_DCS_N, searchFreqImofs.CtsResult, 0);
                            }  
                        }
                    }
                }

                searchFreqImofs.step = SCtsDcs_Issue;
            
                return;
            }

            if(searchFreqImofs.overTime > 0)
            {
                searchFreqImofs.overTime--;
            }
            else
            {// 超时
                SearchFreqModeDisplayDCSData(SUBAUDIO_NONE, 0, 0);

                searchFreqImofs.CtsResult = 0;
                searchFreqImofs.dcsCtsType = SUBAUDIO_NONE;
                searchFreqImofs.step = SCtsDcs_Issue;
            }

            break;

        case SCtsDcs_Issue:
            if(Rfic_GetSQLinkState() == TRUE)
            {
                //增加判断亚音频
                if(searchFreqImofs.dcsCtsType > SUBAUDIO_NONE)
                {
                    if(Rfic_SubaudioDetect() == 0)
                    {
                        searchFreqImofs.flagRx = 0;
                        SpeakerSwitch(OFF);
                        Rfic_SetAfout(OFF);
                        LedRxSwitch(LED_OFF);
                        break;
                    }
                }

                if(searchFreqImofs.flagRx == 0)
                {
                    searchFreqImofs.flagRx = 1;
                    Rfic_SetAfout(ON);
                    SpeakerSwitch(ON);
                    LedRxSwitch(LED_ON);
                }
            }
            else
            {
                if(searchFreqImofs.flagRx == 1)
                {
                    searchFreqImofs.flagRx = 0;
                    SpeakerSwitch(OFF);
                    Rfic_SetAfout(OFF);
                    LedRxSwitch(LED_OFF);
                }
            }
            break;

        default:
            //退出扫频模式
            ExitSearchFreqMode(1);
            break;
    }
}

extern void KeyProcess_Search(U8 keyEvent)
{
    switch(keyEvent)
    {
        case KEYID_MENU:
            if(searchFreqImofs.step == SCtsDcs_Issue)
            {
                ExitSearchFreqMode(0);
                FastEnterChMemMenu();
            }
            else
            {
                BeepOut(BEEP_NULL);
            }
            break;
        case KEYID_UP:
        case KEYID_DOWN:
            if(searchFreqImofs.step < SCtsDcs_Setup)
            {
                return;
            }
            
            SearchFreqModeDisplayStepMsg( STEP_SEEK_FREQ );
            searchFreqImofs.step = SF_Setup;
            break;

        case KEYID_SEARCH:    
        case KEYID_EXIT:
            ExitSearchFreqMode(1);
            BeepOut(BEEP_EXITMENU);
            break;
            
        case KEYID_AB:
        case KEYID_WELL:
            SearchFreqModeBandSwitch();
            SearchFreqModeDisplayStepMsg( STEP_SEEK_FREQ );
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


