#include "includes.h"

const U8  addrMap[8] = {0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00};

U8  powerOnMsg[17] = {0};
STR_RF_MODELE g_rfMoudel;
STR_BAND bandRang;
/*********************************************************************
* 函 数 名: Flash_ModifyChannelData
* 功能描述: 保存信道信息，包括信息数据和信道名称
* 输入参数：channelNum:信道号  chData:数据指针  chName:信道名称数据指针
* 输出参数:
* 返　　回:
* 说    明：
***********************************************************************/
extern void Flash_ModifyChannelData(U16 channelNum,U8 * chData,U8 * chName)
{
    U8  buf[4*1024]; // 4K
    U32 indexSector; // 索引
    U32 indexChannelNum;
    U32 addr = CHAN_ADDR;
    
    //计算存储信道逻辑地址
    indexSector = channelNum / 128; // 4k / 32byte = 128
    addr = addr + (indexSector * 0x1000);

    //换算信道位置
    indexChannelNum = channelNum % 128;

    // 读出数据/擦除FLASH
    SpiFlash_ReadBytes(addr, buf, 4*1024);
    SpiFlash_EraseSector(addr);

    //修改信道数据，不改变信道名称
    memcpy(buf + (indexChannelNum * CHAN_SIZE), chData, sizeof(STR_CHANNEL));
    //保存信道名称
    memcpy(buf + (indexChannelNum * CHAN_SIZE + NAME_ADDR_SHIFT), chName, NAME_SIZE);
    
    SpiFlash_WriteBytes(addr, buf, 4*1024);
}

/*********************************************************************
* 功能描述: 保存信道信息，包括信息数据和信道名称
* 输入参数：channelNum:信道号  chData:信道信息数据指针 chName:信道名称数据指针
* 输出参数:
* 返　　回:
* 说    明：数据有改变时才会进行数据存储操作
***********************************************************************/
extern void Flash_SaveChannelData(U16 channelNum,U8 * chData,U8 * chName)
{
    U32 addr;
    U8  channelBuf[32] = {0x00};

    addr = g_ChannelVfoInfo.currentChannelNum*CHAN_SIZE+CHAN_ADDR;
    SpiFlash_ReadBytes(addr,channelBuf, CHAN_SIZE);

    //判断数据是否有改变
    if(memcmp(channelBuf,chData,sizeof(STR_CHANNEL)) != 0 || memcmp(chName,&chData[NAME_ADDR_SHIFT],NAME_SIZE) != 0)
    {
        Flash_ModifyChannelData(channelNum,chData,chName);
    }
}

/*********************************************************************
* 功能描述: 删除指定信道数据
* 输入参数：channelNum:信道号
* 输出参数:
* 返　　回:
* 说    明：只删除前8个字节频率数据，将数据清0
***********************************************************************/
extern void Flash_DeleteChannelData(U16 channelNum)
{
    U32 addr;
    U8  resetBuf[8];

    addr = channelNum*CHAN_SIZE+CHAN_ADDR;
    memset(resetBuf,0x00,8);
    SpiFlash_WriteBytes(addr,resetBuf, 8);
}

/*********************************************************************
* 函 数 名: Flash_GetLogicAddrShift
* 功能描述: 获取地址偏移位置
* 输入参数：addr:存储块地址   useByte:需要使用到的byte bit/8
* 输出参数:
* 返　　回:
* 说    明：
***********************************************************************/
U16 Flash_GetLogicAddrShift(U32 addr,U8  useByte)
{
    U8  i,j;
    U8  addrMask;
    U8  addrMap[32];
    U16 addrOffset = 0xFF;  // 地址偏移

    SpiFlash_ReadBytes(addr, addrMap,useByte);

    for(i = 0; i < useByte; i++)
    {
        addrMask = 0x01;
        for(j = 0; j < 8; j++)
        {
            if(addrMap[i] & addrMask)
            {
                addrOffset = i * 8 + j;

                return addrOffset;
            }
            else
            {
                addrMask <<= 1;
            }
        }
    }
    return addrOffset;
}

// 保存对讲机 对讲机功能信息/收音机信道信息
extern void Flash_SaveRadioImfosData(void)
{
    U8  addrOffset;
    U8  dataMap;
    U32 logicAddr;
    U16 checkSum,CheckSumRd;
    U8  buf[96] = {0x00};//对讲机功能信息1+对讲机功能信息2+收音机功能信息 = 94Byte + 2Byte(CheckSum)
                            // (84 - 16Byte) / 96 = 42  衰减次数: 42
    U8  cmpBuf[80];
    
    //读取存储位置信息
    addrOffset = Flash_GetLogicAddrShift(RADIO_IMFOS_ADDR,6);

    memcpy(buf, (U8  *)&g_radioInform.sqlLevel, sizeof(STR_RADIOINFORM));
    //用于存储开机显示信息
    memcpy(buf+64,powerOnMsg, 16);

    //计算CRC校验
    checkSum = CRC_ValidationCalc(buf,94);
    memcpy(buf+94,(U8  *)&checkSum,2);

    if(addrOffset < 41)
    {
        logicAddr = addrOffset * 96 + RADIO_IMFOS_ADDR + 16;
        //读取当前校验和比较
        SpiFlash_ReadBytes(logicAddr+94, (U8  *)&CheckSumRd, 2);
        if(CheckSumRd == checkSum)
        {
            SpiFlash_ReadBytes(logicAddr, cmpBuf, 80);
            if(memcmp(cmpBuf,buf,80) == 0)
            {//数据相同直接返回
                return;
            }
        }
        //数据有改变才保存
        logicAddr += 96;
        SpiFlash_WriteBytes(logicAddr, buf, 96);
        
        //标记地址工作块
        dataMap = addrMap[(addrOffset%8)];
        SpiFlash_WriteBytes((RADIO_IMFOS_ADDR + (addrOffset/8)), &dataMap, 1);
    }
    else
    {// 擦除
        SpiFlash_EraseSector(RADIO_IMFOS_ADDR);
        SpiFlash_WriteBytes((RADIO_IMFOS_ADDR + 16), buf, 96);
    }
}

// 读取对讲机 频率模式数据/对讲机功能信息/收音机信息
extern void Flash_ReadRadioImfosData(void)
{
    U8  addrOffset;
    U32 logicAddr;
    U16 checkSum;
    U8  buf[96]={0x00};//对讲机功能信息1+对讲机功能信息2+收音机功能信息 = 94Byte + 2Byte(CheckSum)
                           // (4K - 16Byte) / 96 = 42  衰减次数: 42
    //读取存储位置信息
    addrOffset = Flash_GetLogicAddrShift(RADIO_IMFOS_ADDR,6);
    
    if(addrOffset < 42)
    {
        logicAddr = addrOffset* 96 + RADIO_IMFOS_ADDR + 16;
        SpiFlash_ReadBytes(logicAddr, buf, 96);
        
        memcpy((U8  *)&checkSum,&buf[94],2);
        if(checkSum == CRC_ValidationCalc(buf,94))
        {// 校验通过
            memcpy((U8  *)&g_radioInform.sqlLevel, buf, sizeof(STR_RADIOINFORM));
            memcpy(powerOnMsg, buf+64,16);
            return;
        }
    }
    // 数据错误，校验未通过/从备份区提取数据
    SpiFlash_EraseSector(RADIO_IMFOS_ADDR);
    ResetRadioFunData();

}

//保存对讲机频率模式数据,使用一块flash 4K,前16byte用于指示工作块
//A段频率模式+B段频率模式 = 64 + 2Byte(CheckSum)
//(4K - 16Byte) / 66 = 61  衰减次数: 61
// 前16字节用于衰减算法  实际使用41bit
U8  VfoBuf[66]={0x00};
extern void Flash_SaveVfoData(U8  workAB)
{
    U8  addrOffset;
    U8  dataMap;
    U32 logicAddr;
    U16 checkSum;

    if(workAB == 1)
    {
        if(memcmp(VfoBuf+32,g_ChannelVfoInfo.vfoInfo[1].freq,32) == 0)
        {//数据未改变不保存
            return;
        }
        memcpy(VfoBuf+32, g_ChannelVfoInfo.vfoInfo[1].freq, 32);
    }
    else if(workAB == 0)
    {
        if(memcmp(VfoBuf,g_ChannelVfoInfo.vfoInfo[0].freq,32) == 0)
        {//数据未改变不保存
            return;
        }
        memcpy(VfoBuf, g_ChannelVfoInfo.vfoInfo[0].freq,32);
    }
    else if(workAB == 0XFF)
    {// 写频用
        
    }
    else
    {//复位专用
        memcpy(VfoBuf, g_ChannelVfoInfo.vfoInfo[0].freq,32);
        memcpy(VfoBuf+32, g_ChannelVfoInfo.vfoInfo[1].freq, 32);
    }
    checkSum = CRC_ValidationCalc(VfoBuf,64);

    memcpy(VfoBuf+64,(U8  *)&checkSum,2);

    addrOffset = Flash_GetLogicAddrShift(VFO_INFO_ADDR,8);

    if(addrOffset < 60)
    {//最后块地址时就需要擦除
        logicAddr = (addrOffset + 1) * 66+ VFO_INFO_ADDR + 16;
        SpiFlash_WriteBytes(logicAddr, VfoBuf, 66);
        //标记地址工作块
        dataMap = addrMap[(addrOffset%8)];
        SpiFlash_WriteBytes((VFO_INFO_ADDR + (addrOffset/8)), &dataMap, 1);

    }
    else
    {//整块擦除后写入
        SpiFlash_EraseSector(VFO_INFO_ADDR);
        SpiFlash_WriteBytes((VFO_INFO_ADDR + 16), VfoBuf, 66);
    }
}

//读取对讲机频率模式数据
extern void Flash_ReadVfoData(U8  workAB)
{
    U8  addrOffset;
    U32 logicAddr;
    U16 checkSum;

    addrOffset = Flash_GetLogicAddrShift(VFO_INFO_ADDR,8);
    if(addrOffset < 61)
    {
        logicAddr = (addrOffset * 66) + VFO_INFO_ADDR + 16;
        SpiFlash_ReadBytes(logicAddr, VfoBuf, 66);

        memcpy((U8  *)&checkSum,&VfoBuf[64],2);
        if(checkSum == CRC_ValidationCalc(VfoBuf,64))
        {//数据校验正确
            if(workAB == 1)
            {
                memcpy(&g_ChannelVfoInfo.vfoInfo[1], VfoBuf+32, 32);
            }
            else
            {
                memcpy(&g_ChannelVfoInfo.vfoInfo[0], VfoBuf, 32);
            }
            return;
        }
    }
    //数据错误，从初始化数据恢复
    SpiFlash_EraseSector(VFO_INFO_ADDR);
    ResetVfoModeData();

}

//保存对讲机信道号,天气预报信道号,收音机当前频率
//预留14Byte + 2Byte(CheckSum)
//(4K - 32Byte) / 16 = 254  衰减次数: 254
// 前16字节用于衰减算法  实际使用61bit
extern void Flash_SaveSystemRunData(void)
{
    U16 addrOffset;
    U8  dataMap;
    U32 logicAddr;
    U16 checkSum;
    U8  buf[16]={0x00};

    addrOffset = Flash_GetLogicAddrShift(SYSTEMRAN_ADDR,32);

    //提取数据
    memcpy(buf, (U8  *)&g_ChannelVfoInfo.channelNum, 2);

    //计算CRC校验
    checkSum = CRC_ValidationCalc(buf,14);
    memcpy(buf+14,(U8  *)&checkSum,2);

    if(addrOffset < 253)
    {
        logicAddr = (addrOffset + 1) * 16 + SYSTEMRAN_ADDR + 32;
        SpiFlash_WriteBytes(logicAddr, buf, 16);
        //标记地址工作块
        dataMap = addrMap[(addrOffset%8)];
        SpiFlash_WriteBytes((SYSTEMRAN_ADDR + (addrOffset/8)), &dataMap, 1);
    }
    else
    {//整块擦除后写入
        SpiFlash_EraseSector(SYSTEMRAN_ADDR);
        SpiFlash_WriteBytes((SYSTEMRAN_ADDR + 32), buf, 16);
    }
}

//读取对讲机信道号,天气预报信道号,收音机当前频率
//预留14Byte + 2Byte(CheckSum)
//(4K - 32Byte) / 16 = 254  衰减次数: 254
// 前16字节用于衰减算法  实际使用61bit
extern void Flash_ReadSystemRunData(void)
{
    U16 addrOffset;
    U32 logicAddr;
    U16 checkSum;
    U8  buf[16]={0x00};

    addrOffset = Flash_GetLogicAddrShift(SYSTEMRAN_ADDR,32);

    if(addrOffset < 254)
    {
        logicAddr = (addrOffset * 16) + SYSTEMRAN_ADDR + 32;
        SpiFlash_ReadBytes(logicAddr, buf, 16);

        memcpy((U8  *)&checkSum,&buf[14],2);
        if(checkSum == CRC_ValidationCalc(buf,14))
        {//数据校验正确
            memcpy((U8  *)&g_ChannelVfoInfo.channelNum, buf, 2);
            
            return;
        }
    }
    //数据错误，从初始化数据恢复
    g_ChannelVfoInfo.channelNum[0] = 0;
    g_ChannelVfoInfo.channelNum[1] = 0;
}

//保存DTMF本机ID
extern void Flash_SaveDtmfInfo(void)
{
     U8  buf[288]={0xFF};
     
     SpiFlash_ReadBytes(DTMFINFOR_ADDR, buf, 288); //32+16*16

     memcpy(buf,(U8  *)&g_dtmfStore, sizeof(g_dtmfStore));
     SpiFlash_EraseSector(DTMFINFOR_ADDR);

     SpiFlash_WriteBytes(DTMFINFOR_ADDR, buf, 288);
}

//定义固定频点信息
const U8  fixBand1[][16] = 
{
  {0x01,0x01,0x36,0x01,0x74,0x01,0x04,0x00,0x06,0x00,0x01,0x02,0x00,0x02,0x60,0x00},
}; 

const U8  fixBand2[][8] = 
{
  {0x00,0x03,0x50,0x03,0x90,0x00,0x00,0x00},
}; 

// 读取对讲机 调试参数
extern void Flash_ReadDebugImfosData(void)
{
    U8  bandData[16];
    U8  bandData1[16];
    U8  i;

    SpiFlash_ReadBytes(RF_PWR_H_U_400_ADDR, &i, 1);
    if(i != 0xFF)
    {
        SpiFlash_ReadBytes(RF_PWR_H_U_400_ADDR, TXPWR_H_U_400, 16);
        SpiFlash_ReadBytes(RF_PWR_H_V_136_ADDR, TXPWR_H_V_136, 16);
        SpiFlash_ReadBytes(RF_PWR_H_V_200_ADDR, TXPWR_H_V_200, 16);
    
        SpiFlash_ReadBytes(RF_PWR_L_U_400_ADDR, TXPWR_L_U_400, 16);
        SpiFlash_ReadBytes(RF_PWR_L_V_136_ADDR, TXPWR_L_V_136, 16);
        SpiFlash_ReadBytes(RF_PWR_L_V_200_ADDR, TXPWR_L_V_200, 16);
        
        SpiFlash_ReadBytes(RF_SQL_TAB_ADDR, TH_SQL_TAB, 10 );
        SpiFlash_ReadBytes(RF_SQL_TAB_MUTE_ADDR, TH_SQL_TAB_MUTE, 10);
    
        SpiFlash_ReadBytes(RF_SQL_U_400_ADDR, OFFSET_SQL_U_400, 16);
        SpiFlash_ReadBytes(RF_SQL_V_136_ADDR, OFFSET_SQL_V_136, 16);
        SpiFlash_ReadBytes(RF_SQL_V_200_ADDR, OFFSET_SQL_V_200, 16);
        SpiFlash_ReadBytes(RF_SQL_U_350_ADDR, OFFSET_SQL_U_350, 16);

        SpiFlash_ReadBytes(RF_MODULATION_ADDR, OFFSET_MODULATION, 16);

        //SpiFlash_ReadBytes(DEV_BATT_ADDR, battery.voltList, BAT_LEVEL_NUM);
    }
    
    //读取机型码
    Flash_ReadRfMoudelType();
    g_sysRunPara.moduleType = g_rfMoudel.moduleType;
    
    if(g_sysRunPara.moduleType >= 0x30 && g_sysRunPara.moduleType <= 0x35)
    {
        g_sysRunPara.moduleType &= 0x0F;
    }
    else
    {
        g_sysRunPara.moduleType = 0;
    }
    memcpy(bandData,fixBand1[0],16);
    memcpy(bandData1,fixBand2[0],8);
    
    g_sysRunPara.rfTxFlag.txEnable[0] = bandData[0];
    g_sysRunPara.rfTxFlag.txEnable[1] = bandData[5];
    g_sysRunPara.rfTxFlag.txEnable[2] = bandData[10];

    // 计算频段范围
    for(i = 0; i < 16; i++)
    {
       bandData[i] = changeHexToInt(bandData[i]);
    }
    // U段频率范围
    bandRang.bandFreq.vhfL   = bandData[1]*1000+bandData[2]*10;
    bandRang.bandFreq.vhfH   = bandData[3]*1000+bandData[4]*10;
    bandRang.bandFreq.freqVL = bandRang.bandFreq.vhfL*10000;
    bandRang.bandFreq.freqVH = bandRang.bandFreq.vhfH*10000;

    // V段频率范围
    bandRang.bandFreq.uhfL   = bandData[6]*1000+bandData[7]*10;
    bandRang.bandFreq.uhfH   = bandData[8]*1000+bandData[9]*10;
    bandRang.bandFreq.freqUL = bandRang.bandFreq.uhfL*10000;
    bandRang.bandFreq.freqUH = bandRang.bandFreq.uhfH*10000;
    
    // 200M频率范围
    bandRang.bandFreq.vhf2L   = bandData[11]*1000+bandData[12]*10;
    bandRang.bandFreq.vhf2H   = bandData[13]*1000+bandData[14]*10;
    bandRang.bandFreq.freqV2L = bandRang.bandFreq.vhf2L*10000;
    bandRang.bandFreq.freqV2H = bandRang.bandFreq.vhf2H*10000;

    //350M频率范围
    for(i = 0; i < 8; i++)
    {
       bandData1[i] = changeHexToInt(bandData1[i]);
    }
    bandRang.bandFreq.B350ML = bandData1[1]*1000+bandData1[2]*10;
    bandRang.bandFreq.B350MH = bandData1[3]*1000+bandData1[4]*10;
    bandRang.bandFreq.freq350ML = bandRang.bandFreq.B350ML*10000;
    bandRang.bandFreq.freq350MH = bandRang.bandFreq.B350MH*10000;
}

//保存发射允许和频段选择功能
//预留14Byte + 2Byte(CheckSum)
//(4K - 32Byte) / 16 = 254  衰减次数: 254
// 前16字节用于衰减算法  实际使用61bit
extern void Flash_SaveRfMoudelType(void)
{
    U16 addrOffset;
    U8  dataMap;
    U32 logicAddr;
    U16 checkSum;
    U8  buf[16]={0x00};

    addrOffset = Flash_GetLogicAddrShift(RF_MODEL_ADDR,32);

    //提取数据
    memcpy(buf, (U8  *)&g_rfMoudel.moduleType, 5);   

    //计算CRC校验
    checkSum = CRC_ValidationCalc(buf,14);
    memcpy(buf+14,(U8  *)&checkSum,2);

    if(addrOffset < 253)
    {
        logicAddr = (addrOffset + 1) * 16 + RF_MODEL_ADDR + 32;
        SpiFlash_WriteBytes(logicAddr, buf, 16);
        //标记地址工作块
        dataMap = addrMap[(addrOffset%8)];
        SpiFlash_WriteBytes((RF_MODEL_ADDR + (addrOffset/8)), &dataMap, 1);
    }
    else
    {//整块擦除后写入
        SpiFlash_EraseSector(RF_MODEL_ADDR);
        SpiFlash_WriteBytes((RF_MODEL_ADDR + 32), buf, 16);
    }
}

//读取对讲机发射允许和频段选择功能
//预留14Byte + 2Byte(CheckSum)
//(4K - 32Byte) / 16 = 254  衰减次数: 254
// 前16字节用于衰减算法  实际使用61bit
extern void Flash_ReadRfMoudelType(void)
{
    U16 addrOffset;
    U32 logicAddr;
    U16 checkSum;
    U8  buf[16]={0x00};

    addrOffset = Flash_GetLogicAddrShift(RF_MODEL_ADDR,32);
    if(addrOffset < 254)
    {
        logicAddr = (addrOffset * 16) + RF_MODEL_ADDR + 32;
        SpiFlash_ReadBytes(logicAddr, buf, 16);

        memcpy((U8  *)&checkSum,&buf[14],2);
        if(checkSum == CRC_ValidationCalc(buf,14))
        {//数据校验正确
            memcpy((U8  *)&g_rfMoudel.moduleType, buf, 5);
            return;
        }
    }
    //默认工厂模式，同时发射都不允许
    memset((U8  *)&g_rfMoudel.moduleType,0x00,5);

    //提取数据
    memset(buf,0x00,16);

    //计算CRC校验
    checkSum = CRC_ValidationCalc(buf,14);
    memcpy(buf+14,(U8  *)&checkSum,2);
    SpiFlash_EraseSector(RF_MODEL_ADDR);
    SpiFlash_WriteBytes((RF_MODEL_ADDR + 32), buf, 16);
}

//保存对讲机收音机模式信道数据
//收音机数据64Byte + 2Byte(CheckSum)
//(4K - 16Byte) / 66 = 61  衰减次数: 61
// 前16字节用于衰减算法  实际使用120bit
extern void Flash_SaveFmData(void)
{
    U16 addrOffset;
    U8  dataMap;
    U32 logicAddr;
    U16 checkSum,CheckSumRd;
    U8  buf[66] = {0x00};
    U8  cmpBuf[66];
    
    //读取存储位置信息
    addrOffset = Flash_GetLogicAddrShift(FM_IMFOS_ADDR,8);

    //提取数据
    memcpy(buf,   (U8  *)&g_FMInform.FmCurFreq, sizeof(STR_FMINFOS));
    //计算CRC校验
    checkSum = CRC_ValidationCalc(buf,64);
    memcpy(&buf[64], (U8  *)&checkSum,2);

    if(addrOffset < 60)
    {
        logicAddr = addrOffset * 66 + FM_IMFOS_ADDR + 16;
        //读取当前校验和比较
        SpiFlash_ReadBytes(logicAddr+64, (U8  *)&CheckSumRd, 2);
        if(CheckSumRd == checkSum)
        {
            SpiFlash_ReadBytes(logicAddr, cmpBuf, 64);
            if(memcmp(cmpBuf,buf,64) == 0)
            {//数据相同直接返回
                return;
            }
        }
        //数据有改变才保存
        SpiFlash_WriteBytes(logicAddr + 66, buf, 66);
        
        //标记地址工作块
        dataMap = addrMap[(addrOffset%8)];
        SpiFlash_WriteBytes((FM_IMFOS_ADDR + (addrOffset/8)), &dataMap, 1);
    }
    else
    {// 擦除
        SpiFlash_EraseSector(FM_IMFOS_ADDR);
        SpiFlash_WriteBytes((FM_IMFOS_ADDR + 16), buf, 66);
    }
}

//读取收音机当前频率，信道数据
//收音机数据64Byte + 2Byte(CheckSum)
//(4K - 16Byte) / 66 = 61  衰减次数: 61
// 前16字节用于衰减算法  实际使用120bit
extern void Flash_ReadFmData(void)
{
    U16 addrOffset;
    U32 logicAddr;
    U16 checkSum;
    U8  buf[66]={0x00};

    addrOffset = Flash_GetLogicAddrShift(FM_IMFOS_ADDR,8);

    if(addrOffset < 61)
    {
        logicAddr = (addrOffset * 66) + FM_IMFOS_ADDR + 16;
        SpiFlash_ReadBytes(logicAddr, buf, 66);

        memcpy((U8  *)&checkSum,&buf[64],2);
        if(checkSum == CRC_ValidationCalc(buf,64))
        {//数据校验正确
            memcpy((U8  *)&g_FMInform.FmCurFreq, buf, 64); 
            return;
        }
    }
    
    //数据错误，从初始化数据恢复
    g_FMInform.FmCurFreq = 889;
    g_FMInform.fmChNum = 0;
    g_FMInform.fmChVfo = VFO_MODE;

    memcpy(buf,   (U8  *)&g_FMInform.FmCurFreq, sizeof(STR_FMINFOS));
    SpiFlash_EraseSector(FM_IMFOS_ADDR);
    SpiFlash_WriteBytes((FM_IMFOS_ADDR + 16), buf, 66);
}

