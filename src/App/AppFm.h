#ifndef __APPFM_H
    #define __APPFM_H

//定义FM时间参数
#define FM_FREQSW_TIME    10      //收音机频率切换时间10ms为单位
#define FM_SEEK_TIMEOUT   1000    //收音机自动搜索超时时间 10S
#define FM_SEEK_TIME      FM_SEEK_TIMEOUT - 20 //收音机自动搜索超时时间
#define FM_RETURN_TIME    200     //收音机退出后返回时间

#define FM_MAX_CH_NUM     30      //定义最大存储信道数量    

//定义收音机运行状态
enum {FM_STOP=0,FM_SLEEP,FM_READY, FM_SEEK, FM_PLAY};              
typedef struct
{
    U8  mode;                 //收音机运行状态
    U8  band;                 //工作频段 0:76-108  1:65-76 
    U16 freq;                 //收音机当前频率
    U16 timeOut;              //超时时间，用于搜索，退出后返回
    U8  fmChList[4];          //收音机有效信道列表
    U8  fmChActive;           //收音机信道模式是否有效
}STR_FMSTATE;

extern STR_FMSTATE    fmInfo;
/********************************************************************/
extern Boolean CheckFmChActive(U8 curChanNum);
extern void ResumeFmMode(void);
extern void FmBandConfig(void);
extern Boolean CheckFmVfoMode(void);
extern void FmCheckChannelActive(void);
extern void FmCheckTimeOut(void);
extern void FmEnterSleepMode(void);
extern void FMSwitchExit(void);
extern void EnterFmMode(void);
extern void ExitFmMode(void);
extern void FmTaskFunc(void);
extern void ResetFmSleepTime(void);
extern void KeyProcess_Fm(U8 keyEvent);

#endif
