#ifndef __APPSTOPWATCH_H
    #define __APPSTOPWATCH_H

typedef struct
{
    U8 minute;       // 分
    U8 second;       // 秒
    U8 millisecond;  // 10毫秒

    U8 flagStart;    // 开关
} STU_STOPWATCH;    

extern void StopWatchDisplayHome(void);
extern void StopWatchDisplayTime(void);
extern void EnterStopWatchMode(void);
extern void ExitStopWatchMode(void);
extern void StopWatchTick(void);
extern void StopWatch_Start(void);
extern void KeyProcess_StopWatch(U8 keyEvent);

#endif
