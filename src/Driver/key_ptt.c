#include "includes.h"

extern Boolean PTT_GetKeyVal(void)
{
    if(GPIOA->IDR & GPIO_Pin_10)
    {
        return 1;
    }
    else
    {
       return 0;
    }
}


extern void PTT_ScanTask(void)
{
    static U8 pttCnt;
    
    if(g_rfState == RF_TX && g_rfTxState != TX_STOP && alarmDat.alarmStates != ON)
    {
        return;
    }
    else
    {
        if(PTT_GetKeyVal() == 0)
        {
            pttCnt++;
            if(pttCnt < 3)
            {
                return;
            }
        }
        else
        {
            pttCnt = 0;
            return;
        }

        LCD_BackLightSetOn();
        ResetTimeKeyLockAndPowerSave();

        if(alarmDat.alarmStates)
        {
            AlarmFuncSwitch(OFF);
        }

        if(Audio_CheckBusy())
        {
            Audio_PlayStop();
            return;
        }

        Radio_EnterTxMode();
    }
}

