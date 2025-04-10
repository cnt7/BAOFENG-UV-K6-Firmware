#ifndef __BATTERY_H
    #define __BATTERY_H

#define BATT_low            0
#define BATT_0              1
#define BATT_1              2
#define BATT_2              3
#define BATT_3              4
#define BATT_4              5
#define BATT_high           6

#define BAT_LEVEL_NUM          7

typedef enum 
{
    BATT_LEVEL1=0,BATT_LEVEL2,BATT_LEVEL3,BATT_FULL,BATT_LOW,BATT_LOWFLASH,BATT_LOWSTOP,BATT_HIGH
}ENUM_BATT;    

typedef struct
{
    U8     CurVolt;             
    ENUM_BATT batteryLevel;     
    U8     batDetDly;            
    U8     flashTime;           
    U8     voltList[7]; 
}STR_BATTERY;

extern const U8  *battIcon[];

extern STR_BATTERY battery;

typedef struct
{  
    U8   flag;
    U8   mode;
    U16  time;
}STR_POWERSAVE;

extern STR_POWERSAVE powerSave; 
/**********************************************************************/
extern void DisplayBattaryFlag(U8 updateFlag);
extern void BatteryInitLevel(void);
extern void BatteryGetLevel(void);
extern Boolean CheckBatteryCanTx(void);
extern void BatteryCheckTask(void);
extern Boolean CheckPowerBusy(void);
extern void PowerSaveTask(void);
extern void ResetTimeKeyLockAndPowerSave(void);

#endif
