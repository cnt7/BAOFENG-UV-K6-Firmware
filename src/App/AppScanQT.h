#ifndef __APPSCANQT_H
    #define __APPSCANQT_H

typedef enum 
{
    SCANQT_OFF=0,SCANQT_STEPUP,SCANQT_WAIT,SCANQT_STOP
}ENUM_SCANQT;

enum 
{
    SCAN_SUBAUDIO_NONE, SCAN_SUBAUDIO_CTCS, SCAN_SUBAUDIO_DCS
};


typedef struct
{
    ENUM_SCANQT state;                           //亚音频扫描状态
    U16 timeOut;                                 //扫描超时时间
    U8  dcsCtsType;                              //亚音频类型
    U32 dcsCtsDat;
    U8  isStandarDCS;
    U16 dcsIndex;
}STR_REMOTESCANQT;

extern void EnterRemoteScanQTMode(void);
extern void ExitRemoteScanQTMode(void);
extern void TaskRemoteScanQT(void);    
extern void KeyProcess_ScanQt(U8 keyEvent);

#endif
