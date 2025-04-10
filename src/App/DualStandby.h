#ifndef __DUALSTANDBY_H
    #define __DUALSTANDBY_H

typedef struct
{
    U8   dualRxFlag;                             //双守接收标志,守候信道接收到信号时置位
    U8   dualOnFlag;                             //双守开启标志
    U8   dualRxTime;                             //双守切换时间
    U8   UpdateFalg;                             //更新显示标志，用于在守候信道接收到信号时更新标志使用
    U8   dualResumeFlag;                         // 双守信道标志
    U8   dualResumeTime;                         // N秒后自动回到主信道
}STR_DUAL;

extern STR_DUAL dualStandby; 

extern void DualChannelSwitch2Main(void);
extern void DualStandbyWorkON(void);
extern void DualStandbyWorkOFF(void);
extern void ResetDualRxTime(void);
extern void DualStandbyTask(void);

#endif
