#include "includes.h"

extern void SysTickHandler(void)
{
    g_msCont++;
    g_msFlag = TRUE;
    if(g_msCont >= 10)
    {
        g_msCont = 0;
        g_10msFlag = TRUE;
        g_10msCont++;

        StopWatchTick();

        if(g_10msCont%5 == 0 && g_10msCont != 0)
        {
            g_50msFlag = TRUE;
        }

        if(g_10msCont >= 10)
        {
            g_10msCont = 0;
            g_100msCont++;
            g_100msFlag = TRUE;
            if(g_100msCont >= 5)
            {
                g_100msCont = 0;
                g_500msFlag = TRUE;
            }

            if(g_sysRunPara.rfTxFlag.totTime)
            {
                g_sysRunPara.rfTxFlag.totTime--;
            }
            if(powerSave.time)
            {
                powerSave.time--;
            }

            if(dtmfInfo.matchTime[0] > 0)
            {
                dtmfInfo.matchTime[0]--;
            }
    
            if(dtmfInfo.matchTime[1] > 0)
            {
                dtmfInfo.matchTime[1]--;
            }
            if(dtmfInfo.timeRxOut)
            {
                dtmfInfo.timeRxOut--;
            }
            if((g_sysRunPara.sysRunMode == MODE_MAIN || g_sysRunPara.sysRunMode == MODE_FM || g_sysRunPara.sysRunMode == MODE_WEATHER ) && g_sysRunPara.lcdAutoLight)
            {
                g_sysRunPara.lcdAutoLight--;
            }

            //过中继尾音消除检测时间
            if(g_sysRunPara.rfRxFlag.relayTailDetTime)
            {
                g_sysRunPara.rfRxFlag.relayTailDetTime--;
            }
    
            //发射过中继尾音消除时间
            if(g_sysRunPara.rfTxFlag.relayTailSetTime)
            {
                g_sysRunPara.rfTxFlag.relayTailSetTime--;
            }
    
            //接收显示闪烁
            if(g_sysRunPara.rfRxFlag.rxFlashTime)
            {
                g_sysRunPara.rfRxFlag.rxFlashTime--;
            }
    
            //扫描切换时间,接收时候时间才递减
            if(g_scanInfo.scanTime && g_rfState == RF_RX)
            {
                g_scanInfo.scanTime--;
            }

            if(g_inputbuf.time)
            {
                g_inputbuf.time--;
            }

            if(g_menuInfo.menuExitTime)
            {
                g_menuInfo.menuExitTime--;
            }
            if(g_menuInfo.fastInTime)
            {
                g_menuInfo.fastInTime--;
            }

            if(g_sysRunPara.rfTxFlag.voxDetDly)
            {
                g_sysRunPara.rfTxFlag.voxDetDly--;
            }

            if(g_sysRunPara.rfTxFlag.voxWorkDly)
            {
                g_sysRunPara.rfTxFlag.voxWorkDly--;
            }
        }
    }
    if(beepDat.beepTime)
    {
       beepDat.beepTime--;
    }
    CheckAlarmDelay();

    if(dualStandby.dualRxTime)
    {
        dualStandby.dualRxTime--;
    }  

    if(dtmfInfo.timeOut)
    {
        dtmfInfo.timeOut--;
    }
}

extern void DelaySysMs(U16 time)
{
    while(time)
    {
        if(g_msFlag)
        {
            g_msFlag = FALSE;
            time--;
        }
    }
}

