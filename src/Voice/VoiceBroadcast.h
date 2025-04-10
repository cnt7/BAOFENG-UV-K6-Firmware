#ifndef __VOICEBROADCAST_H
    #define __VOICEBROADCAST_H

/**************************************************************************************************/
/*****定义语音播放索引常量*************************************************************************/
/**************************************************************************************************/
#include "VoiceIndex_Girl.h"

/**************************************************************************************************/
typedef struct
{
    U32 length;          //音频文件总长度
    U32 dataAddr;        //音频文件起始地址
}STR_VOICE_INDEX;

//定义DMA播放使用的结构体
typedef struct
{
    U32 logicAddr;       //实际播放时读取的地址
    U32 usedLen;         //已经播放的数据长度
    volatile U8  finishFlag;      //播放完成标志
    volatile U8  dmaBufUsed;      //当前使用的缓存区域
    volatile U8  dmaBufAUseFlag;  //当前使用的缓存区域是否开始使用
    volatile U8  dmaBufBUseFlag;  //当前使用的缓存区域是否开始使用
    U16 dmaBufA[1024];
    U16 dmaBufB[1024];
    U8  lastPackage;
}STR_VOICE_PLAY;

typedef struct
{
    STR_VOICE_INDEX voiceIndex;
    STR_VOICE_PLAY voicePlay;
}STR_VOICE_ONFO;

//作为语音播报时使用的结构体
typedef struct
{
    U8  voiceState;     //语音播放标志，判断是否在语音播报模式 
    U8  busyFlag;    //是否正在播放状态
    U8  voiceCnt;       //当前需要播报语音的数量
    U8  voiceBuf[6];    //当前播报语音命令缓存
}STR_VOICE;

extern STR_VOICE voice;
extern STR_VOICE_ONFO g_voiceInform;
extern U8 FastChangeVoice;

extern void VoiceOutput_Interrupt(void);
extern void AudioHard_Init(void);

extern void Audio_PlayVoice(U8 Data);
extern void Audio_PlayVoiceLock(U8 Data);
extern void Audio_PlayNumInQueue(U8 Data);
extern void Audio_PlayChanNum(U8 Data);	

extern U8 Audio_CheckBusy(void);
extern void Audio_PlayStop(void);
extern void Audio_PlayTask(void);
extern void VoiceBroadcastWithBeepLock(U8 voiceDat,ENUM_BEEPMODE beepData);

#endif
