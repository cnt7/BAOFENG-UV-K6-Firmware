#ifndef __APPDTMF_H
    #define __APPDTMF_H

enum{ CALLTYPE_NONE, CALLTYPE_NID, CALLTYPE_ID, CALLTYPE_GROUP, CALLTYPE_ALL };
    
#define DTMF_ANI_LEN       3      //定义本机ID长度
    
typedef struct 
{
    const U16 tone1Freq;
    const U16 tone2Freq;
}DTMFCODESTRUCT;

typedef struct
{
    U8  machineId[5];//本机设备ID
    U8  dtmfAlarmWord;         //报警码
    U8  dtmfFlag;              //BIT1:  PTT松开发送本机ID
                                  //BIT0:  PTT按下发送本机ID
    U8  onTime;                //DTMF码持续时间  范围：80-2000MS （步进为10MS）,取值:0 1 2 3 .... 195 默认：0
    U8  offTime;               //DTMF码间断时间  范围：80-2000MS （步进为10MS）,取值:0 1 2 3 .... 195 默认：0
    U8  separator;            // 分隔符
    U8  groupCall;            // 组呼符
}__attribute__( ( packed ) )STF_DTMFSTORE;

//发码模式
enum{DTMF_ONLINE=1,DTMF_OFFLINE=2,DTMF_ALARMCODE=4,DTMF_ALARMID,DTMF_TYPEIN,DTMF_ANI=8};

//DTMF工作状态
enum {DTMF_OVER = 0,DTMF_SETUP,DTMF_FREQ,DTMF_STOP};
typedef struct
{
    U8  id[5];
    U8  name[11];
} STR_CONTACT;

typedef struct
{
    U8  code[16];              //DTMF发送的编码
    U8  cntRxDtmf;             //DTMF解码数量
    U8  state;                 //工作状态
    U8  enCodeNum;             //当前发码序号
    U8  enCode;                //当前需要发送的编码
    U8  sendFlag;              //正在发码标志
    U16 timeOut;               //DTMF发码时间或者发码等待时间
    U16 detTime;               //DTMF检测时间
    U8  timeRxOut;             //接收解码超时时间
    String aniCode[8];            //呼叫方号码
    String callCode[8];           //呼叫信息
    U8  callType;              //呼叫类型   个呼\带身份个呼\组呼\群呼
    U8  matchTime[2];          //匹配持续时间
    U8  flagAck;               //身份识别回传标志
    U8  timerDlyTxEnd;         //发码结束延时时间

    U8  timerDtmfGroupRst;     //DTMF呼叫复位时间   30s内不需要再次检测
    U8  flagDtmfMatch;         //DTMF呼叫检测标志

    STR_CONTACT contact[20];

    U8  onlineCode[16];      // 上线码  
    U8  offlineCode[16];     // 下线码
    U8  killCode[16];         // 遥毙码
    U8  reliveCode[16];       // 唤醒码

}STR_DTMFINFO;

extern STF_DTMFSTORE g_dtmfStore;
extern STR_DTMFINFO dtmfInfo;
extern const DTMFCODESTRUCT DTMFCODE[21];
/************************************************************/
extern void EnterDtmfEditMode(void);
extern void ExitDtmfEditMode(void);
extern void GetDtmfEditCode(void);
extern void ResetDtmfEditCode(void);
extern void KeyProcess_DtmfInput(U8 keyEvent);

extern void DtmfSendKeypadCode(U8 code);
extern void DtmfSendTxOver(void);
extern void DtmfSendCodeOn(U8 type);
extern void DtmfTask(void);
extern void Task_HangUp(void);
extern void DtmfReceiveSetup(void);
extern void DtmfReceiveTask(void);
extern void DtmfClrMatchTimer(void);
extern void DtmfRstMatchTimer(U8 set);

extern void ClearDisANIFlag(void);
extern void DtmfSendANIAck(void);

extern void DtmfInfoInit(void);
extern U8 DtmfGetMatchStatue(void);


#endif
