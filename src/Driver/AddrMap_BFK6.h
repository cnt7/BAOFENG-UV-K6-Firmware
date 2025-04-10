#ifndef _DATAADDRMAPTYPE_H_
    #define _DATAADDRMAPTYPE_H_

//定义使用到的Flash的地址
#define CHAN_SIZE                 32       //信道信息大小
#define CHAN_ADDR                 0x0000   //信道首地址
#define NAME_ADDR_SHIFT           20       //信道名称地址偏移
#define NAME_SIZE                 12       //信道名称信息大小为16字节

#define VFO_INFO_ADDR             0x8000   //频率模式存储地址
#define VFO_SIZE                  32       //频率模式信道信息大小
#define VFO1_ADDR                 0x8000   //频率模式地址
#define VFO2_ADDR                 0x8020   //频率模式地址

#define RADIO_IMFOS_ADDR          0X9000   //对讲机 频率模式/功能信息/收音机 地址      
#define RADIO_SIZE                64       //可选信息大小

#define PWR_MSG_ADDR              0x9040   //开机显示信息

//双音多频[DTMF]
#define DTMFINFOR_ADDR            0xA000   

//遥毙码地址16个字节
#define DTMF_KILLED_ADDR          0xA010   

//定义DTMF存储地址  16  
#define DTMF_SIZE                 10
#define DTMF_CODE_ADDR            0xA020


//扫描列表使用地址
#define SCAN_LIST_ADDR            0xB000

/************************************************************************/
#define FM_IMFOS_ADDR             0XC000   //对讲机功能信息/收音机 地址  
#define FM_SIDZE                  32
#define FM_ADDR                   0xC000   //收音机频点存储地址

/************************************************************************/
#define RF_MODEL_ADDR             0xD000  //机型码存储地址

#define RF_TXEN_ADDR              0xD001  //发射允许 220M 350M  520M 发射允许  航空频段接收允许
/************************************************************************/
/*本机运行信息存储*/
/************************************************************************/
//定义信道号、收音机等使用地址
#define SYSTEMRAN_ADDR            0xE000
#define F1CHAN_ADDR               0xE000  
#define F2CHAN_ADDR               0xE002

/************************************************************************/
/*                         调 试 信 息 地 址  0xF000开始                */
/************************************************************************/
#define DEBUG_ADDR                0XF000

//电池电压
#define DEV_BATT_ADDR             0XF200 
//调制度
#define RF_MODULATION_ADDR        0XF210
//中英文
#define DEV_LANGUAGE_ADDR         0XF220
//蓝牙使能
#define DEV_BTEN_ADDR             0XF221 

// APC输出
#define RF_ADJUST_BASE_ADDR       0XF000
#define RF_PWR_H_U_400_ADDR       0XF000
#define RF_PWR_H_V_136_ADDR       0XF010
#define RF_PWR_H_V_200_ADDR       0XF020
#define RF_PWR_H_U_350_ADDR       0XF030
#define RF_PWR_M_U_400_ADDR       0XF040
#define RF_PWR_M_V_136_ADDR       0XF050
#define RF_PWR_M_V_200_ADDR       0XF060
#define RF_PWR_M_U_350_ADDR       0XF070
#define RF_PWR_L_U_400_ADDR       0XF080
#define RF_PWR_L_V_136_ADDR       0XF090
#define RF_PWR_L_V_200_ADDR       0XF0A0
#define RF_PWR_L_U_350_ADDR       0XF0B0

// SQL门限
#define RF_SQL_TAB_ADDR           0XF0C0
#define RF_SQL_TAB_MUTE_ADDR      0XF0D0
#define RF_SQL_U_400_ADDR         0XF0E0  //静噪修正值
#define RF_SQL_V_136_ADDR         0XF0F0  //静噪修正值
#define RF_SQL_V_200_ADDR         0XF100  //静噪修正值
#define RF_SQL_U_350_ADDR         0XF110  //静噪修正值

#define BAND_ADDR                 0XF230  // V/U/200M 频段范围
#define BAND2_ADDR                0XF240  //300M 频段范围 

#define MODEL_ADDR                0xF250  //机型码存储地址
/************************************************************************/

#endif

