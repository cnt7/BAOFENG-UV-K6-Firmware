#include "includes.h"

const U8 TH_BATT_TAB[7] = { 130, 139, 147, 156, 165, 183 };
const U8 *battIcon[] = {iconBatt1,iconBatt2,iconBatt3,iconBattFull,iconBattEmpty};
STR_BATTERY battery;
STR_POWERSAVE powerSave; 

extern void BatteryInitLevel(void)
{
    memset(&battery,0x00,sizeof(STR_BATTERY));

    SpiFlash_ReadBytes(DEV_BATT_ADDR, battery.voltList, BAT_LEVEL_NUM);
    if( battery.voltList[0] == 0x00 || battery.voltList[0] == 0xff )
    {
        memcpy( battery.voltList, TH_BATT_TAB, BAT_LEVEL_NUM );
    }

    BatteryGetLevel();
}

extern Boolean CheckBatteryCanTx(void)
{
    if(battery.batteryLevel >= BATT_LOWSTOP)
    {
        return FALSE;
    }
	
    return TRUE;
}

extern void DisplayBattaryFlag(U8 updateFlag)
{
    U8 battLevel;

    if(battery.batteryLevel >= BATT_HIGH)
    {
        battLevel = BATT_FULL;
    }
    else if(battery.batteryLevel > BATT_LOW)
    {
        battLevel = BATT_LOW;
    }
    else
    {
        battLevel = battery.batteryLevel;
    }

    LCD_DisplayPicture(0,108,ICON_BATT_SIZEX,ICON_BATT_SIZEY,battIcon[battLevel],LCD_DIS_NORMAL);

    if(updateFlag)
    {
        LCD_UpdateStateBar();
    }
}

void BatteryLowFlash(void)
{
    static U8 flash;
    
    if(flash)
    {
        flash = 0;
        SC5260_ClearArea(1,108,ICON_BATT_SIZEX,ICON_BATT_SIZEY, 0);
    }
    else
    {
        flash = 1;
        LCD_DisplayPicture(1,108,ICON_BATT_SIZEX,ICON_BATT_SIZEY,iconBattEmpty,LCD_DIS_NORMAL);
    }
    LCD_UpdateStateBar();
}

extern void BatteryGetLevel(void)
{
    UserADC_GetValOfBatt();
    battery.CurVolt = UserADC_GetValOfBatt();

    if(battery.CurVolt > battery.voltList[BATT_high])
    {
        battery.batteryLevel = BATT_HIGH;
    }
    else if(battery.CurVolt > battery.voltList[BATT_4])
    {
        battery.batteryLevel = BATT_FULL;
    }
    else if(battery.CurVolt > battery.voltList[BATT_3])
    {
        battery.batteryLevel = BATT_LEVEL3;
    }   
    else if(battery.CurVolt > battery.voltList[BATT_2])
    {
        battery.batteryLevel = BATT_LEVEL2;
    }
    else if(battery.CurVolt > battery.voltList[BATT_1])
    {
        battery.batteryLevel = BATT_LEVEL1;
    }
    else if(battery.CurVolt > battery.voltList[BATT_0])
    {
        battery.batteryLevel = BATT_LOW;
    }
    else
    { 
        if(battery.CurVolt < battery.voltList[BATT_low])
        {
            battery.batteryLevel = BATT_LOWSTOP;
        }
        else
        {
            battery.batteryLevel = BATT_LOWFLASH;
        }
    }
}

extern void BatteryCheckTask(void)
{
    U8 lastBatteryLevel = battery.batteryLevel;

    if(g_rfState == RF_TX || alarmDat.alarmStates || g_rfRxState == WAIT_RXEND || Audio_CheckBusy())
    {
        return;
    }


    BatteryGetLevel();

    if(battery.batteryLevel == BATT_LOWFLASH || battery.batteryLevel == BATT_LOWSTOP)
    {
        battery.flashTime = (battery.flashTime + 1) % 20;
        BatteryLowFlash();

        if(battery.flashTime == 0)
        {
            if(g_radioInform.voiceSw == 0)
            {
                ResetTimeKeyLockAndPowerSave();
                BeepOut(BEEP_LOWBAT);
            }
            else
            {
                Audio_PlayVoice(vo_Lowvoltage);
            }
        }

        return;
    }
 
    if(lastBatteryLevel != battery.batteryLevel)
    {
        DisplayBattaryFlag(1);
    }
}


extern void ResetTimeKeyLockAndPowerSave(void)
{
    if(powerSave.flag == ON)
    {
        Rfic_WakeUp();
    }
    g_sysRunPara.keyAutoTime = g_radioInform.keyAutoLock * 50;  
    powerSave.time = 100;
    powerSave.flag = OFF;
}

extern Boolean CheckPowerBusy(void)
{
    return powerSave.flag;
}

extern void PowerSaveTask(void)
{
    if(alarmDat.alarmStates || g_sysRunPara.sysRunMode == MODE_SCAN)
    {//在报警模式或者是在扫描模式不省电
        return;
    }

    if(g_radioInform.saveLevel == 0)
    {//不支持省电模式
        return;
    }

    if(powerSave.time == 0)
    {
        if(powerSave.flag == ON)
        {
            powerSave.flag = OFF;

            powerSave.time = 3;
            
			RF_PowerSet(g_ChannelVfoInfo.BandFlag,PWR_RXON); 
		    Rfic_WakeUp();
            Rfic_RxTxOnOffSetup(RFIC_RXON);
        }
        else
        {
            powerSave.flag = ON;
            powerSave.time = 8;  
            
            RF_PowerSet(g_ChannelVfoInfo.BandFlag,PWR_OFF); 
            Rfic_RxTxOnOffSetup(RFIC_IDLE);
			Rfic_Sleep();
        }
    }
}


