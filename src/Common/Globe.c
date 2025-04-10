#include "includes.h"

//定义对讲机参数全局变量
STR_RADIOINFORM g_radioInform;
STR_FMINFOS g_FMInform;
STR_SYSTEM  g_sysRunPara;
STR_INPUTBOX g_inputbuf;


STR_CHANNEL_VFO g_ChannelVfoInfo;
STR_CH_VFO_INFO *g_CurrentVfo;

volatile U8 g_msCont;
volatile U8 g_msFlag;
volatile U8 g_10msCont;
volatile U8 g_100msCont;
volatile U8 g_10msFlag;
volatile U8 g_50msFlag;
volatile U8 g_100msFlag;
volatile U8 g_500msFlag;

Boolean g_UpdateDisplay;
U8 g_rfState;
U8 g_rfRxState;
U8 g_rfTxState;

U8 inputTypeBack;


