#ifndef __INC_PUBLTYPE_H__
    #define __INC_PUBLTYPE_H__  
//-----------------------------------------------------------------------------
typedef char                String;

typedef int8_t              S8;          
typedef int16_t             S16;          
typedef int32_t             S32;       
typedef int64_t             S64; 

typedef uint8_t             U8;       
typedef uint16_t            U16;   
typedef uint32_t            U32;   
typedef uint64_t            U64;        

typedef int_fast8_t         Boolean;         // 至少为1bit


#define __BYTE              uint8_t

typedef void (*pFunction)(void);

#define BIT0            0x0001
#define BIT1            0x0002
#define BIT2            0x0004
#define BIT3            0x0008
#define BIT4            0x0010
#define BIT5            0x0020
#define BIT6            0x0040
#define BIT7            0x0080
#define BIT8            0x0100
#define BIT9            0x0200
#define BIT10           0x0400
#define BIT11           0x0800
#define BIT12           0x1000
#define BIT13           0x2000
#define BIT14           0x4000  
#define BIT15           0x8000
//----------------------------------------------------------------------------------------------------------
//定义需要使用的的常亮
#define FERROR          1
#define OK              0

typedef enum 
{
    OFF=0,ON
}ENUM_ONOFF;

typedef enum
{
  FALSE = 0, TRUE  = !FALSE
}
bool;

enum {LANG_EN=0,LANG_CN};
//----------------------------------------------------------------------------------------------------------
enum{KEY_CLICKED=0,KEY_LONG,KEY_CONTINUE};
//-------------------------------------------------------------------------------------------------------------
typedef union 
{
    U32 dest;
    U8 src[4];
}UninoU32;

//------------------------------------------------------------------------------------------------------------
#define INPUT_TIME_OUT         50
#define INPUT_DTMF_TIME_OUT    100

typedef struct
{
    U8             time;                       //输入超时时间
    U8             len;                        //输入的字串长度
    U8             pos;
    U8             maxLen;                     //允许输入的最大长度
    U8             inputType;                  //输入法类型
    U8             isFirstInput;               //是否是第一次输入
    String         buf[20];                    //输入的字串资料
} STR_INPUTBOX;
//--------------------------以下为系统运行需要的结构体--------------------------------------------------------
enum {RF_NONE=0,RF_RX,RF_TX};
enum {RX_READY=0,GET_CALL,WAIT_RXEND,RX_MONI};            //RXSTATE
enum {TX_READY=0,WAIT_PTT_RELEASE,PTT_RELEASE,TX_STOP,ALARM_TXID,TX_KILLED}; //TXSTATE
enum {VFO_MODE=0,CHAN_MODE};
enum {CHAN_DISABLE=0,CHAN_ACTIVE};

//定义发射使用结构体
typedef struct
{
    U16   totTime;                                //发射超时时间
    U16   voxDetDly;                              //声控延时检测，用于声音  
    U16   voxWorkDly;                             //声控结束后发射延时时间
    U16   relayTailSetTime;                       //过中继尾音消除时间
    U8    txChEnable;                             //信道模式下发射允许
    U8    txEnable[4];                            //发射允许
}__attribute__( ( packed ) )STR_TXFLAG;
//定义接收使用结构体
typedef struct
{
    U8    rxReceived;                             //已经接收到射频信号,但是亚音频未解开，标志位置1     
    U16   relayTailDetTime;                       //过中继尾音检测时间
    U16   codeDetTime;                            //亚音频检测时间
    U16   rxFlashTime;                            //接收闪烁标志
    U16   txDly;                                  //发射转接收延时
    U8    rxReceiveOn;                            //接收到射频信号,亚音频已解开，打开喇叭，标志位置1  
}__attribute__( ( packed ) )STR_RXFLAG;

typedef struct
{
    U8  codeLen;                                 //输入的DTMF编码长度
    U8  code[16];                                //要发送的DTMF编码
}STR_DTMFTYPEIN;

typedef struct
{       
    U8    sysRunMode;
    STR_TXFLAG    rfTxFlag;
    STR_RXFLAG    rfRxFlag;
    U8    moniFlag;
    U8    ledState;                               //手电筒功能
    U16   keyAutoTime;                            //自动锁键盘时间
    U16   lcdAutoLight;                           //自动背光时间
    U8    dtmfToneFlag;                           //按键DTMF拨号标志
    U8    pttFlag;                                //PTT标志位，用于切换PTTA B 使用
    U8    batState;                               //电池电量 0:电池电量正常    1: 电池电量过低
    U8    lcdBackSwtch;                           //液晶背光灯状态
    STR_DTMFTYPEIN txDtmfCode;                    //输入需要发送的DTMF编码,或者选择需要发送的编码
    U8    flagDisANI;                             //身份码显示标志
    U32   decoderCode;                            //破码数据
    U8    moduleType;                             //机型码，根据机型来判断发射频率
}STR_SYSTEM;

typedef struct
{
    U16    vhfL;
    U16    vhfH;
    U16    uhfL;
    U16    uhfH;
    U16    vhf2L;
    U16    vhf2H;
    U16    B350ML;
    U16    B350MH;
    U32    freqVL;
    U32    freqVH;
    U32    freqUL;
    U32    freqUH;
    U32    freqV2L;
    U32    freqV2H;
    U32    freq350ML;
    U32    freq350MH;
}STR_FREQBAND;

#define BAND_BUF_CNT       4  //默认支持UV双段

typedef struct
{
    U16 freq[2*BAND_BUF_CNT];
    U32 freq32[2*BAND_BUF_CNT];
}STR_BANDBUF;

//定义频段范围结构体
typedef union
{
    STR_FREQBAND bandFreq;
    STR_BANDBUF  bandbuf;
}STR_BAND;

//-------------------------------------------------------------------------------------------------------------
typedef union {
	__BYTE	Byte;
	struct {
			__BYTE	b0:1;
			__BYTE	b1:1;
			__BYTE	b2:1;
			__BYTE	b3:1;
			__BYTE	spMute:2;
			__BYTE	b6:1;
			__BYTE	b7:1;
	} Bit;
} ChParaFlag1;    

typedef union {
    __BYTE  Byte;
    struct {
            __BYTE  b0:2;
            __BYTE  b2:1;
            __BYTE  b3:1;
            __BYTE  b4:1;
            __BYTE  b5:1;
            __BYTE	b6:1;
			__BYTE	b7:1;
    } Bit;
}VfoParaFlag1;
/***************************对讲机信道信息定义********************************************************/
typedef struct
{
    U32 rxFreq;    
    U32 txFreq;      
    U16 rxDCSCTSNum; 
    U16 txDCSCTSNum; 
    U8  dtmfgroup;    
    U8  pttID;       
    U8  txPower;      
    ChParaFlag1 chFlag3; /*channel flag 3*/
                        //BIT6: 宽窄带       0: 宽带（25K）1: 窄带（12.5K）2:20K 带宽 默认: 宽带  
                        //BIT5 BIT4: 接收静音模式 0:QT  1:DTMF  2:QT+DTMF  3:QT*DTMF
                        //BIT3: 繁忙信道锁定 00: OFF 01: ON  默认: 无
                        //BIT2: 扫描添加     00: OFF 01: ON  默认: 无
                        //BIT1: 保留
                        //BIT0: 破码标志 0:无  1:有 (带破码功能的机器)
    //U8  channelName[16];  //信道名称      
    U32 decoderCode;   
}STR_CHANNEL;

/***************************对讲机频率模式定义********************************************************/
typedef struct
{
    U8  freq[8];      
    U16 rxDCSCTSNum; 
    U16 txDCSCTSNum;  
    U8  remain0[2];
  
    U8  dtmfgroup;        
    U8  ANI;                       
    U8  txPower;     
    VfoParaFlag1 vfoFlag;     /*channel flag 3*/
                         //BIT6: 宽窄带       0: 宽带（25K）1: 窄带（12.5K） 2:20K 带宽 默认: 宽带
                         //BIT4 BIT5: 
                         //其他:保留 
                         //BIT0:
    U8  remain1;   
    U8  STEP;         // 步进频率 :序号 0-7    
    U8  Offset[7];  
    U8  spMute;       
    U32 decoderCode; 
}STR_VFOMODE; 

/*----------------------------------------------------------------------------------------------------------
      定义信道运行使用结构体
-----------------------------------------------------------------------------------------------------------*/
typedef struct
{
    U32 frequency;
    U8  dcsCtsType;
    U32 dcsCtsNum;
}STR_FREQINFO;

typedef struct
{
    STR_FREQINFO freqRx;
    STR_FREQINFO freqTx;
    STR_FREQINFO *rx;    //倒频时候使用
    STR_FREQINFO *tx;    //倒频时候使用
    U8  wideNarrow;
    U8  txPower;
    U8  fhssFlag;
    U8  scarmble;
    U8  spMute;
    U8  busyLock;
    U8  dtmfgroup;
    U8  chVfoMode;
    U8  freqDir;
    U8  freqStep;
    U32 freqOffset;
    U8  reverseFlag;
    U8  pttIdMode;
    U8  channelName[16];
}STR_CH_VFO_INFO;

typedef struct
{
   STR_CH_VFO_INFO   chVfoInfo[2];      //计算后工作存储，AB段信息
   STR_VFOMODE vfoInfo[2];              
   STR_CHANNEL channelInfo[2];
   U8   scanList[999/8 + 1];     //扫描列表
   U8   chanActiveList[999/8 + 1];//有效信道列表
   U8   switchAB;         //AB段切换
   U8   dualAB;
   U8   BandFlag;         //工作频段
   U8   haveChannel;      //是否存在有效信道
   U8   haveScan;         //是否存在可以扫描的信道
   U16  channelNum[2];
   U16  currentChannelNum;
}STR_CHANNEL_VFO;
/*----------------------------------------------------------------------------------------------------------
      定义使用的位域信息
-----------------------------------------------------------------------------------------------------------*/
typedef union {
    __BYTE  Byte;
    struct {
            __BYTE  b0:1;
            __BYTE  b1:1;
            __BYTE  b2:1;
            __BYTE  b3:1;
            __BYTE  b4:1;
            __BYTE  b5:1;
            __BYTE  b6:1;
            __BYTE  b7:1;
    } Bit;
}UNION_FLAG1;

typedef union {
    __BYTE  Byte;
    struct {
            __BYTE  b0:2;
            __BYTE  b2:2;
            __BYTE  b4:1;
            __BYTE  b5:1;
            __BYTE  b6:1;
            __BYTE  b7:1;
    } Bit;
}UNION_FLAG2;

typedef union {
    __BYTE  Byte;
    struct {
            __BYTE  TxTone:1;
            __BYTE  Remain:3;
            __BYTE  AutoOff:4;
    } Bit;
}UNION_FLAG3;

typedef union {
    __BYTE  Byte;
    struct {
            __BYTE  chVofA:4;
            __BYTE  chVofB:4;
    } Bit;
}UNION_VM;
/*----------------------------------------------------------------------------------------------------------
      对讲机可选功能信息(Radio Information)  
-----------------------------------------------------------------------------------------------------------*/
typedef struct
{ 
    U8   sqlLevel;        //SQL静噪电平          0 ~ 9 
    U8   saveLevel;       //省电模式             0: 关      1: 1:1     2: 1:2     3: 1:3    4: 1:4
    U8   voxLevel;        //VOX声控发射          0: 1~ 9
    U8   autoBack;        //ABR自动背光          0-5s                
    U8   dualRxFlag;      //TDR启动双守          0: 关      1: 开  (三段显示时: 0:关 1: A+B 2: A+C 3: B+C)  
    U8   totLevel;        //TOT发射超时定时器        0-180秒（步进为15秒）。取值:0 1 2 3 .... 12 默认：0为关闭      
    U8   beepsSwitch;     //BEEP提示音           0: 关      1: 开 
    U8   voiceSw;         //VOICE操作提示语音    0:无       1: 开启
    
    U8   language;        //显示和提示语音       0: 英文    1: 中文       
    U8   dtmfTone;        //DTMF侧音      0: 关  1: 按键侧音     2: 发身份码侧音     3: 按键侧音  + 发身份码侧音   
    U8   scanMode;        //SCREV   扫描模式   00: 时间  01: 载波  10: 搜索   默认: 载波        
    U8   pttIdMode;       //PTTID              00: 无      01: BOT(发射开始)   10: EOT(发射结束)   11: 两者 默认: 无
    U8   pttIdTime;       //PTTLT发ID延时         0-3（步进为100 ms）  
    U8   channleDisA;     //MDFA A段信道显示方式  00: 信道名称+信道号  01: 频率+信道号 (S19机型 0:信道频率 1:信道号 2:信道名称)
    U8   channleDisB;     //MDFB B段信道显示方式  00: 信道名称+信道号  01: 频率+信道号 (S19机型 0:信道频率 1:信道号 2:信道名称)     
    U8   txBusyLock;      //BCL  遇忙禁发        0: 关      1: 开  

    U8   keyAutoLock;     //AUTOLK键盘自动锁定   0: 关      1: 开   S19:OFF/5/10/15
    U8   alarmMode;       //ALMOD 报警模式       0:现场  1:发射报警音    2: 发射报警码
    U8   alarmLocal;      //报警时本机是否发出报警音 0: 关      1: 开
    U8   dualTxMode;      //TXAB  双守发射       0: 关      1: A段   2: B段   3: C段
    U8   tailSwitch;      //STE   尾音消除       0: 关      1: 开
    U8   rpste;           //RPSTE 过中继尾音消除 0-1000MS（步进为100MS）。取值:0 1 2 3 .... 10 默认：0 
    U8   rptrl;           //RPTRL 过中继尾音检测 0-1000MS（步进为100MS）。取值:0 1 2 3 .... 10 默认：0     
    U8   txOffTone;       //ROGER 发射结束提示音 0: 关      1: 开 

    U8   switchAB;        //当前工作模式     0:B段       1:A段
    U8   fmEnale;         //FM调频收音机允许使用  1:允许 0:不允许
    UNION_VM chOrVfoMode;    //独立设置时高4位B段模式 低4位A段模式
    U8   keyLock;         //LOCK   键盘锁    0: 关      1: 开
    UNION_FLAG2  OpFlag1;    //bit0~1:  开机显示:0:无: 1:预设图标(公司logo)  2:预设字符2行 3:电池电压 
                             //bit2~3:  开机提示音: 0:无 1:音调  2:语音
    U8   rtone;           //1750Hz中继Tone             
    U8   weatheSwitch;    //天气预报功能开关 预留
    U8   weatherAlert;    //天气警报功能 预留

    U8   voxDelay;        //声控延时时间 0.5S -- 2.0S 步进0.1S
    U8   menuExitTime;    //菜单自动退出时间
    U8   remain0[6];      //保留

    U8   toa;             //发射超时预警 0-10S
    U8   brightness;      //背光对比度选择
    U8   DisplayStyles;   //屏幕反向显示，黑白反显示
    U8   scanCtcsMode;    //亚音频扫描保存方式
    U16  vfoScanRangeL;   //频率模式扫描范围
    U16  vfoScanRangeH;   //频率模式扫描范围

    U8   fmInterrupt;     //收音机接收打断  0:允许打断  1:禁止打断
    U8   txForbid;        //发射禁止 0:频段范围内允许发射 1:禁止发射
    U8   userSideKey[4];  //支持侧键自定义功能，目前默认4个
    U8   weatherNum;      //天气预报信道号
    U8   menuSet;         //菜单选项配置 bit0:复位允许 bit1: GPS允许(默认关闭)

    U8   hangUpTime;      //挂起时间
    U8   signalType;      //信令类型 0:DTMF 1: 2TONE  2:5TONE
    U8   voxSwitch;       //声控开关, 0:关闭 1:开启
    U8   remain1;         
    U8   pwrPwdFlag;      //开机密码标志
    U8   pwrPassword[3];  //开机密码，最多支持6位,不足6位自动补0

}STR_RADIOINFORM;  

/*----------------------------------------------------------------------------------------------------------
      定义报警模式使用结构体       
-----------------------------------------------------------------------------------------------------------*/
typedef struct
{
    U8  alarmStates;              //报警状态 
    U8  toneFlag;                 //报警音标志，递增或者递减模式
    U16 alarmFreq;                //报警频率
    U16 freqSwTime;               //报警频率变化时间
    U16 ledFlashTime;             //报警LED闪烁时间
    U16 alarmTime;                //报警时间
}STR_ALARM;

/***************************收音机存储结构体********************************************************/
typedef struct
{
    U16  FmCurFreq;    //频率模式收音机频率
    U16  FmCHs[30];    //收音机信道
    U8   fmChNum;      //收音机信道号
    U8   fmChVfo;      //收音机信道和频率模式切换
} __attribute__( ( packed ) )STR_FMINFOS;

/*----------------------------------------------------------------------------------------------------------
      定义发射频段选中和扩展发射使用结构体       
-----------------------------------------------------------------------------------------------------------*/
typedef struct
{
    U8  moduleType;          //
    U8  txEn220M;            //220M发射允许  220-260M
    U8  txEn350M;            //350M发射允许  350-390M
    U8  txEn520M;            //520M发射允许  480-520M
    U8  amRxEn;              //航空频段接收允许 暂时不使用
}STR_RF_MODELE;

#define changeIntToHex(dec)		( ( ((dec)/10) <<4 ) + ((dec)%10) )
#define changeHexToInt(hex)		( ( ((hex) >> 4) * 10 ) + ((hex) & 0x0f) )
#endif

