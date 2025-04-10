#include "includes.h"

volatile STR_BEEP beepDat;

const U8  BEEP_tone[9][3] = 
{
    {F1500HZ,    0,   0  },    //空NULL
    {F750HZ,   F450HZ,0  },    //FMUP
    {F450HZ,   F750HZ,0  },    //FMDOWN
    {F700HZ,   F590HZ,0  } ,   //FMSW1
    {F1400HZ,  0,  F1400HZ},   //FMSW2开机提示音
    {F590HZ,   F700HZ,0  },    //EXITMENU
    {F700HZ,   F930HZ,0  },    //FASTSW
    {F1500HZ,  0,  F1500HZ  }, //ERROR
    {F1200HZ,  F660HZ, F530HZ} //LOWBAT 
};
const U8  BEEP_time[9][3]= 
{
    {    80, 35,0 },    //空
    {    70, 70,0 },    //FMUP
    {    70, 70,0 },    //FMDOWN
    {    65, 50,0 },    //FMSW1
    {    65, 70,65},    //FMSW2
    {    50, 65,0 },    //EXITMENU
    {    50, 65,0 },    //FASTSW
    {    50, 25,50},    //ERROR  
    {    60, 70,60}     //LOWBAT 
};

extern void BeepOut(ENUM_BEEPMODE beepmode)
{
    U16 temp;
    
    if((alarmDat.alarmStates) || (g_radioInform.beepsSwitch == 0))
    {//正在报警状态或者是按键音关闭
        return;
    }
        
    beepDat.beepMode = beepmode;
    if(beepmode == BEEP_NULL)
    {
        beepDat.beepCnt = 1;
    }
    else if(beepmode == BEEP_FMSW2 || beepmode == BEEP_ERROR)
    {
        beepDat.beepCnt = 3;
    }
    else
    {
        beepDat.beepCnt = 2;
    }

    Rfic_TxSingleTone_On(0);
    while(1)
    {
        if(beepDat.beepTime==0)
        {
            if(beepDat.beepCnt == 0)
            {
                SpeakerSwitch(OFF);
                DelayMs(30);
                Rfic_TxSingleTone_Off();
                if(g_rfRxState == RX_MONI)
                {
                    Rfic_RxTxOnOffSetup(RFIC_RXON);
                    Rfic_SetAfout(ON);
                    SpeakerSwitch(OFF);
                }
                else
                {
                    if((g_scanInfo.state == SCAN_IDLE)&&(g_rfState == RF_RX))
                    {
                        Rfic_RxTxOnOffSetup(RFIC_RXON);
                    }
                    else
                    {
                        Rfic_RxTxOnOffSetup(RFIC_IDLE);
                    }
                }
                    
                if(g_rfRxState == WAIT_RXEND)
                {
                    g_rfRxState = RX_READY;
                }
                g_sysRunPara.rfTxFlag.voxDetDly = 4; 
                
                return;
            }
            else
            {
                beepDat.beepCnt--;

                beepDat.beepTime = BEEP_time[beepDat.beepMode][beepDat.beepCnt];
                temp = BEEP_tone[beepDat.beepMode][beepDat.beepCnt];

                if(temp)
                {
                    Rfic_SetToneFreq(temp);
                    SpeakerSwitch(ON);
                    //RF_SetSingleTone(temp); 
                }
                else
                { 
                    Rfic_SetToneFreq(0);  
                } 
            }
        }    
    }
}

