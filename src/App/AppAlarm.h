#ifndef __APPALARM_H
    #define __APPALARM_H

enum{ALARM_LOCAL=0,ALARM_TONE,ALARM_CODE};

extern STR_ALARM       alarmDat;
/***************************************************************************/
extern void CheckAlarmDelay(void);
extern Boolean SetAlarmCode(void);
extern void AlarmTask(void);
extern void AlarmFuncSwitch(ENUM_ONOFF flag);

#endif
