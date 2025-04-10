#ifndef _VOICEINDEX_H
    #define _VOICEINDEX_H

/***********************************************************/
/*****定义语音片播报命令************************************/
/***********************************************************/
//                             命令码          播报内容
#define  vo_Null               0x00
#define  vo_0                  0x10         // 0
#define  vo_1         	       0x11         // 1
#define  vo_2         	       0x12         // 2
#define  vo_3         	       0x13         // 3
#define  vo_4         	       0x14         // 4
#define  vo_5         	       0x15         // 5
#define  vo_6         	       0x16         // 6
#define  vo_7         	       0x17         // 7
#define  vo_8         	       0x18         // 8
#define  vo_9         	       0x19         // 9
#define  vo_10         	       0x1A         // 10
#define  vo_100         	   0x1B         // 百
                               
#define  vo_Welcome	           0x1C         // 欢迎使用
#define  vo_keylock            0x1D         // 键盘锁定
#define  vo_unlock             0x1E         // 键盘开锁
#define  vo_scanbegin          0x1F         // 开始扫描
#define  vo_scanstop           0x20         // 停止扫描
#define  vo_CTCSS              0x21         // 模拟亚音
#define  vo_DCS                0x22         // 数字亚音
#define  vo_power              0x23         // 发射功率
#define  vo_savemode           0x24         // 省电模式
#define  vo_Memorychl          0x25         // 存储信道
#define  vo_Deletechl          0x26         // 信道删除
#define  vo_Step               0x27         // 步进频率
#define  vo_Squelch            0x28         // 静噪等级
#define  vo_BackLight          0x2A         // 背光选择
#define  vo_Txovertime         0x29         // 发射超时
#define  vo_VOX                0x2B         // 声控发射
#define  vo_Freqdir            0x2C         // 频差方向
#define  vo_Offsetfreq         0x2D         // 频差频率
#define  vo_Txmemory           0x2E         // 发射存储
#define  vo_Rxmemory           0x2F         // 接收存储
#define  vo_EmerCall           0x30         // 紧急呼叫
#define  vo_Lowvoltage         0x31         // 电量不足
#define  vo_Channelmode        0x32         // 信道模式
#define  vo_Freqmode           0x33         // 频率模式
#define  vo_Voicepromrt        0x34         // 语言选择
#define  vo_bandselect         0x35         // 频段选择
#define  vo_Dualstandby        0x36         // 双频守候
#define  vo_Chlbandwidth       0x37         // 信道带宽
#define  vo_Optsignal          0x38         // 可选信令
#define  vo_Mutemode           0x39         // 静音方式
#define  vo_Busylockout        0x3A         // 遇忙禁发
#define  vo_Beepprompt         0x3B         // 提示音
#define  vo_ANIcode            0x3C         // 身份码
#define  vo_initialization     0x3D         // 初始化
#define  vo_Confirm            0x3E         // 确定
#define  vo_Cancel             0x3F         // 取消
#define  vo_ON                 0x40         // 开启
#define  vo_OFF                0x41         // 关闭
#define  vo_Menu               0x42         // 菜单设置
#define  vo_FreqScan           0x43         // 扫频


#define  vo_zero               0x44         // 
#define  vo_ten                0x4E         // 
#define  vo_hundred            0x4F         // 

                               
#define  vo_eleven             0x78         // 
#define  vo_twelve             0x79         // 
#define  vo_thirteen           0x7A         // 
#define  vo_fourteen           0x7B         // 
#define  vo_fifteen            0x7C         // 
#define  vo_sixteen            0x7D         // 
#define  vo_seventeen          0x7E         // 
#define  vo_eighteen           0x7F         // 
#define  vo_nineteen           0x80         // 
#define  vo_twenty             0x82         // 
#define  vo_thirty             0x83         // 
#define  vo_forty              0x84         // 
#define  vo_fifty              0x85         // 
#define  vo_sixty              0x86         // 
#define  vo_seventy            0x87         // 
#define  vo_eighty             0x88         // 
#define  vo_ninety             0x89         // 
#define  vo_and                0x89         // 
                               
#define  vo_switch_en          0x34         // 将中文语音播报切换为英文
#define  vo_index_start        0x10         // 语音播报索引地址

#endif

