#include "includes.h"

const U8  RADIO_INFORM_DEFAULT[64] = 
{
    3, // 静噪等级    0 - 9
	1, // 省电模式    0: 关  1. 1:1  2. 1:2  3. 1:3  4. 1:4
	0, // VOX声控发射 0：关  1 - 9
	3, // 自动背光    0 - 5s
	1, // 双守        0: 关  1: 
	8, // TOT         0 - 180s(步进 15s), 取值 0 - 12,  0为关闭 
	1, // beep音      0: 关  1: 开
	1, // 语音开关    0: 关  1: 开
    0, // 语言        0:英文 1: 中文
	0, // DTMF侧音    0: 关  1:按键侧音 2:发身份码侧音  3: 按键侧音 + 发身份码侧音
	1, // 扫描方式    0: 时间  1: 载波  2: 搜索   默认: 载波      
	0, // PTTID       0: 无    1: BOT(发射开始)   2: EOT(发射结束)   3: 两者  默认: 无
	5, // PTTLT发ID延时  0-30秒（步进为1 秒）  
	1, // MDFA A段信道显示方式  00: 信道名称+信道号  01: 频率+信道号
	1, // MDFB B段信道显示方式  00: 信道名称+信道号  01: 频率+信道号
	0, // BCL  遇忙禁发        0: 关      1: 开
	0, // AUTOLK键盘自动锁定   0: 关      1: 开
	0, // ALMOD 报警模式       0:现场     1:发射报警音    2: 发射报警码
	1, // 报警时本机是否发出报警音 0: 关      1: 开
	0, // 双守发射     0: 关      1: A段   2: B段
	1, // 尾音消除     0: 关      1: 开
	5, // 过中继尾音消除	0-1000MS（步进为100MS）。取值:0 1 2 3 .... 10 默认：0 
	5, // 过中继尾音检测	0-1000MS（步进为100MS）。取值:0 1 2 3 .... 10 默认：0     
	0, // 发射结束提示音	0: 关      1: 开 
	1, // 当前工作模式 	0:B段	     1:A段	
	0, // FM调频收音机允许使用  0:允许	   1:不允许
    0x11,// 当前工作方式     0: 频率模式      1: 信道模式
	0, // LOCK   键盘锁	0: 关      1: 开
    0, // 开机显示电压 0:图片 1:电池电压
	2, // rtone
    0, // 预留               
    0, // 预留  
    
    5,//声控延时时间 0.5S -- 2.0S 步进0.1S 
    0,//菜单自动退出时间                   
    0,
    0,  
    0, 
    0,//预留 
    0x00,0x00, //预留 

    0,//发射超时预警
    3,//背光亮度选择                       
    0,//背景颜色选择                       
    0,//亚音频扫描保存方式                                
    0x90,0x01,//频率模式扫描范围 400                  
    0xD6,0x01,//频率模式扫描范围 470    
    
    0,                      
    0,                 
    1,5,6,0,//支持侧键自定义功能，目前默认4个  
    0,//天气预报信道号
    0x01, //复位允许标志
    
    0xFF, 
    0X00, //信令类型 0:DTMF 1: 2TONE  2:5TONE
    0,    //声控开关, 0:关闭 1:开启 便宜版本暂时不使用
    0xFF, //时区设置，默认为8  不带GPS的不使用
    0,    //开机密码标志, 0:关闭 1:开启 
    0X00, 0X00, 0X00, //开机密码，默认6个0目前仅仅支持写频软件修改
};

const U8  DTMF_INFORM_DEFAULT[] =
{
    0X01, 0X02, 0X03, 0XFF, 0XFF, // 本机ID码
    0x00,
    0x00,
    0x03,
    0x03,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

const U8  DTMF_CODE_DEFAULT[] = 
{
    0X08, 0X00, 0X08, 0X00, 0X01, 0XFF, 0XFF, 0XFF
};

const U8  CHANNEL_1_DEFAULT_DATA[20] = 
{
    0X00, 0X25, 0X76, 0X46, 0X00, 0X25, 0X76, 0X46, 
    0X00, 0X00, 0X00, 0X00, 
    0X00, 
    0X00, 
    0X00, 
    0X02,
    0X00, 0X00, 0X00, 0X00, 
};

const U8  CHANNEL_128_DEFAULT_DATA[20] = 
{
    0X00, 0X75, 0X56, 0X14, 0X00, 0X75, 0X56, 0X14, // 频率
    0X00, 0X00, 0X00, 0X00,                         // 亚音
    0X00,                                           // 信令码
    0X00,                                           // PTTID:  0无 1BOT 2EOT 3BOTH
    0X00,                                           // 功率控制  0高   1低
    0X02,                                           // BIT6  0:宽带   1:窄带  //BIT1 0:禁止发射  1:允许发射
    0X00, 0X00, 0X00, 0X00, 
};

const U8  VFO_A_DEFAULT_DATA[32] = 
{
   4, 3, 5, 6, 2, 5, 0, 0,                          // 频率
   0X00, 0X00, 0X00, 0X00,                          // 亚音
   0x00,                                            // 繁忙锁定：1开启0关闭
   0x00,                                            // 保留
   0X00,                                            // BIT4 + BIT5 -   BIT0-BIT3:信令编码
   0X00,                                            // 保留
   0X00,                                            // 发射功率 0:高功率    1:低功率
   0X00,                                            // BIT6: 宽窄带  0:宽带  1:窄带
   0X01,                                            // 频段  1:UHF   0:VHF
   0X00,                                            // 步进频率: 0 - 7
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00,              // 频差频率:
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00               // 保留
};

const U8  VFO_B_DEFAULT_DATA[32] =
{
   1, 4, 5, 5, 2, 5, 0, 0,                          // 频率
   0X00, 0X00, 0X00, 0X00,                          // 亚音
   0x00,                                            // 繁忙锁定：1开启0关闭
   0x00,                                            // 保留
   0X00,                                            // BIT4 + BIT5 -   BIT0-BIT3:信令编码
   0X00,                                            // 保留
   0X00,                                            // 发射功率 0:高功率    1:低功率
   0X00,                                            // BIT6: 宽窄带  0:宽带  1:窄带
   0X00,                                            // 频段  1:UHF   0:VHF
   0X00,                                            // 步进频率: 0 - 7
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00,              // 频差频率:
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00               // 保留
};

const U8  ChFreqTbl[21][8] = 
{
    {0X00, 0X25, 0X49, 0X14, 0X00, 0X25, 0X49, 0X14}, // 频率
    {0X00, 0X25, 0X45, 0X14, 0X00, 0X25, 0X45, 0X14},
    {0X00, 0X25, 0X51, 0X14, 0X00, 0X25, 0X51, 0X14},
    {0X00, 0X25, 0X55, 0X14, 0X00, 0X25, 0X55, 0X14},
    {0X00, 0X85, 0X59, 0X14, 0X00, 0X85, 0X59, 0X14},
    {0X00, 0X25, 0X02, 0X43, 0X00, 0X25, 0X02, 0X43},
    {0X00, 0X25, 0X12, 0X43, 0X00, 0X25, 0X12, 0X43},
    {0X00, 0X25, 0X22, 0X43, 0X00, 0X25, 0X22, 0X43},
    {0X00, 0X25, 0X32, 0X43, 0X00, 0X25, 0X32, 0X43},
    {0X00, 0X25, 0X42, 0X43, 0X00, 0X25, 0X42, 0X43},
    {0X00, 0X25, 0X52, 0X43, 0X00, 0X25, 0X52, 0X43},
    {0X00, 0X25, 0X62, 0X43, 0X00, 0X25, 0X62, 0X43},
    {0X00, 0X25, 0X72, 0X43, 0X00, 0X25, 0X72, 0X43},
    {0X00, 0X25, 0X82, 0X43, 0X00, 0X25, 0X82, 0X43},
    {0X00, 0X25, 0X92, 0X43, 0X00, 0X25, 0X92, 0X43}, 
    {0X00, 0X75, 0X99, 0X43, 0X00, 0X75, 0X99, 0X43}, 
    {0X00, 0X25, 0X15, 0X43, 0X00, 0X25, 0X15, 0X43}, 
    {0X00, 0X25, 0X25, 0X43, 0X00, 0X25, 0X25, 0X43}, 
    {0X00, 0X25, 0X35, 0X43, 0X00, 0X25, 0X35, 0X43},
    {0X00, 0X25, 0X45, 0X43, 0X00, 0X25, 0X45, 0X43},
    {0X00, 0X25, 0X55, 0X43, 0X00, 0X25, 0X55, 0X43}, 
};
extern void Reset21Chans(void)
{
    U8  writeBuf[32],i;
    U32 addr = CHAN_ADDR;
    
    //擦除信道数据
    SpiFlash_Erase32kBlock(CHAN_ADDR);

    memset(writeBuf,0x00,32);

    //高功率
    writeBuf[14] = 0X00;
    writeBuf[15] = 0X06;
    for(i=0;i<21;i++)
    {
        writeBuf[12] = i % 20;
        memcpy(writeBuf,ChFreqTbl[i],8);
        if(i >= 16)
        {
            writeBuf[14] = 0X01;
        }
        SpiFlash_WriteBytes(addr, writeBuf, 32);
        addr += 32;
    }
}

/*********************************************************************
* 函 数 名: ResetChannelData
* 功能描述: 复位对讲机信道信息
* 全局变量: 
* 输入参数：
* 输出参数:
* 返　　回: 
* 说    明：
***********************************************************************/
extern void ResetChannelData(void)
{
    Reset21Chans();

    g_ChannelVfoInfo.channelNum[0] = 0;
    g_ChannelVfoInfo.channelNum[1] = 20;
    Flash_SaveSystemRunData();
}

/*********************************************************************
* 函 数 名: ResetVfoModeData
* 功能描述: 复位对讲机频率模式信息
* 全局变量: 
* 输入参数：
* 输出参数:
* 返　　回: 
* 说    明：
***********************************************************************/
extern void ResetVfoModeData(void)
{
    memcpy(g_ChannelVfoInfo.vfoInfo[0].freq,VFO_A_DEFAULT_DATA, 32);
    memcpy(g_ChannelVfoInfo.vfoInfo[1].freq,VFO_B_DEFAULT_DATA, 32);    

    Flash_SaveVfoData(4);
}

/*********************************************************************
* 函 数 名: ResetRadioFunData
* 功能描述: 复位对讲机全局功能信息
* 全局变量: 
* 输入参数：
* 输出参数: 
* 返　　回: 
* 说    明：
***********************************************************************/
extern void ResetRadioFunData(void)
{
    U8  i;
    U32 addr;
    U8  writeBuf[16] = {0x00};
    
    memcpy((U8  *)&g_radioInform.sqlLevel,RADIO_INFORM_DEFAULT, sizeof(STR_RADIOINFORM));
    memset(powerOnMsg,0xFF,16);
    
    Flash_SaveRadioImfosData();

    SpiFlash_EraseSector(DTMFINFOR_ADDR);
    SpiFlash_WriteBytes( DTMFINFOR_ADDR, DTMF_INFORM_DEFAULT, sizeof(DTMF_INFORM_DEFAULT));
    
    addr = DTMF_CODE_ADDR;
    memcpy(writeBuf,DTMF_CODE_DEFAULT,sizeof(DTMF_CODE_DEFAULT));
    for(i=1;i<21;i++)
    {
        if( i < 10)
        {
            writeBuf[4] = i;
        }
        else
        {
            writeBuf[4] = i % 10;
            writeBuf[3] = i / 10;
        }
        SpiFlash_WriteBytes( addr,writeBuf,sizeof(DTMF_CODE_DEFAULT));
        addr += 16;
    }
    
}
/*********************************************************************
* 函 数 名: ResetRadioData
* 功能描述: 对讲机信息初始化
* 全局变量: 
* 输入参数：
* 输出参数:
* 返　　回: 
* 说    明：
***********************************************************************/
extern void ResetRadioData(void)
{
    ResetChannelData();
    ResetVfoModeData();
	ResetRadioFunData();
}

