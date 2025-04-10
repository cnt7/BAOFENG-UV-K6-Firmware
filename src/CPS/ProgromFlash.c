#include "includes.h"

STR_PROGROM progrom;
STR_FLASH_PROGROM    flashProgrom;

const U8 linkHead[]     = {"PROGRAMBF-K6PROU"};
const U8 updateFlag[]   = {"UPDATE"};
const U8 strModelType[] = {"UVK6"};

#define CONNECT_START          linkHead[3]
#define PRG_CMD_FREQ           'F'              //发送对讲机支持的频率范围命令
#define PRG_CMD_ACK            0x06
#define PRG_CMD_MODEL          'M'
#define PRG_CMD_FLASH          'D'               //更新开机界面
#define PRG_CMD_WRITE          'W'               //写频命令
#define PRG_CMD_READ           'R'               //读频命令
#define PRG_CMD_END            'E'               //结束命令

/*********************************************************************/
//定义命令标志位
#define progromCmd             progrom.rxBuf[0]
#define progromAdr0            progrom.rxBuf[1]
#define progromAdr1            progrom.rxBuf[2]
#define progromLen             progrom.rxBuf[3]
#define progromData0           progrom.rxBuf[4]

/*********************************************************************
* 函 数 名: ProgromInit
* 功能描述: 初始化写频模式参数
* 全局变量: 
* 输入参数：
* 输出参数:
* 返　　回:
* 说    明：
***********************************************************************/
extern void ProgromInit(void)
{
    memset(&progrom,0x00,sizeof(STR_PROGROM));
}
/*********************************************************************
* 函 数 名: UartSendBuf
* 功能描述: 串口发送数据包
* 全局变量: 
* 输入参数：*buf:数据指针   len:数据长度
* 输出参数:
* 返　　回:
* 说    明：
***********************************************************************/
extern void UartSendBuf(U8 *buf,U16 len)
{
    U16 i;
    
    for(i=0;i<len;i++)
    {
        uartSendChar(buf[i]);
    }
    
}
/*********************************************************************
* 函 数 名: CheckProgromMode
* 功能描述: 读写频模式检测是否进入该模式，以及检测是否接收到数据
* 全局变量: 
* 输入参数：rxData:串口接收到的数据
* 输出参数:
* 返　　回:
* 说    明：
***********************************************************************/
extern void CheckProgromMode(U8 rxData)
{
    static U8 rxPreData;
    
    progrom.timeOut = UART_TIMEOUT;
    if(g_sysRunPara.sysRunMode == MODE_FLASH_PROGRAM)
    {
        progrom.rxBuf[progrom.dataNum] = rxData;
        progrom.dataNum = (progrom.dataNum + 1) % UART_MAX_NUM;
        progrom.packageTime = 5;
        progrom.errCnt  = 0;
    }
    else
    {
        if(!progrom.enterMode)
        {
        //采用比较严格的方式判断是否进入写频模式
            if((rxData == linkHead[1] && rxPreData == linkHead[0]) || (rxData == linkHead[2] && rxPreData == linkHead[1]))
            {
                progrom.enterMode = 1;
                g_sysRunPara.sysRunMode = MODE_PROGRAM;
            }
            rxPreData = rxData;
            progrom.dataNum = 0;  
        }
        else
        {
            progrom.rxBuf[progrom.dataNum] = rxData;
            progrom.dataNum = (progrom.dataNum + 1) % UARTBUF_NUM;
            progrom.recFlag = 1;
            progrom.errCnt  = 0;
        }
    }
}
/*********************************************************************
* 函 数 名: CheckProgromTimeout
* 功能描述: 写频模式延时函数，用于精确计算时间
* 全局变量: 
* 输入参数：
* 输出参数:
* 返　　回:
* 说    明：
***********************************************************************/
extern void CheckProgromTimeout(void)
{
    if(progrom.timeOut)
    {
        progrom.timeOut--;

        if(progrom.timeOut == 0)
        {//超时复位设备
            progrom.states = PRG_END;
        }
    }
}
/*********************************************************************
* 函 数 名: ProgromSendAck
* 功能描述: 发送回复信息
* 全局变量: 
* 输入参数：
* 输出参数:
* 返　　回:
* 说    明：
***********************************************************************/
void ProgromSendAck(void)
{
    uartSendChar(PRG_CMD_ACK);
    progrom.dataNum = 0;
}

/*********************************************************************
* 函 数 名: ProgromSendFreqRange
* 功能描述: 发送频段信息到PC端
* 全局变量: 
* 输入参数：
* 输出参数:
* 返　　回:
* 说    明：
***********************************************************************/
void ProgromSendFreqRange(void)
{
    U8 bandData[21];
    U8 i,j;

    SpiFlash_ReadBytes(BAND_ADDR, bandData, 21);

    j = 0;
    for(i=1;i<21;i++)
    {
        if(i == 5 || i == 10  || i == 15 || i == 16)
        {
            i++;
        }
        progrom.rxBuf[j++] = bandData[i];
    }

    progrom.dataNum = 16;

    //发送数据
    UartSendBuf(progrom.rxBuf,progrom.dataNum);
}
/*********************************************************************
* 函 数 名: CheckLinkHead
* 功能描述: 校验握手码是否正确
* 全局变量: 
* 输入参数：
* 输出参数:
* 返　　回: 是否校验成功
* 说    明：
***********************************************************************/
Boolean CheckLinkHead(void)
{
    U8 i;

    for(i=0;i<progrom.dataNum;i++)
    {
        if(CONNECT_START == progrom.rxBuf[i])
        {
            break;
        }
    }

    if((progrom.dataNum - i) >= 8)
    {
        i++;
        if(memcmp(&linkHead[4],&progrom.rxBuf[i],7) == 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}
/*********************************************************************
* 函 数 名: CheckLinkHead
* 功能描述: 校验握手码是否正确
* 全局变量: progrom
* 输入参数：flag: 0:读频  1:写频
* 输出参数:
* 返　　回:
* 说    明：
***********************************************************************/
void ProgromWriteReadData(U8 flag)
{
    U16 address;
    static U8 buf[64];

    address = progromAdr0;
    address = (address<<8)+ progromAdr1;

    if(flag)
    {
        memcpy(buf,&progromData0,PROGROMLEN); 
        switch(address)
        {
            case 0x8000:
                memcpy(VfoBuf, buf, 64);
                Flash_SaveVfoData(0xFF);
                break;                
            case 0x9000:
                memcpy((U8 *)&g_radioInform.sqlLevel, buf, sizeof(STR_RADIOINFORM));
                break;
            case 0x9040:
                memcpy(powerOnMsg, buf, 16);
                Flash_SaveRadioImfosData();
                break;
            case FM_IMFOS_ADDR:
                memcpy((U8 *)&g_FMInform.FmCurFreq, buf,64);
                Flash_SaveFmData();
                break;
            case RF_MODEL_ADDR:
                memcpy((U8 *)&g_rfMoudel.moduleType, buf,5);
                Flash_SaveRfMoudelType();
                break;
            default:
                if((address % 0x1000) == 0)
                {
                    SpiFlash_EraseSector(address);
                }
                SpiFlash_WriteBytes(address, buf, PROGROMLEN);
                
                if(address == BAND2_ADDR)
                {
                    g_rfMoudel.moduleType = buf[21];
                    Flash_SaveRfMoudelType();
                }
                break;
        }
    }
    else
    {
        switch(address)
        {
            case 0x8000:
                Flash_ReadVfoData(0);
                memcpy(&progromData0, VfoBuf, 64);
                break;                

            case 0x9000:
                memcpy(&progromData0, &g_radioInform.sqlLevel, sizeof(STR_RADIOINFORM));
                break;
            case 0x9040:
                memset(&progromData0,0xFF,64);
                memcpy(&progromData0, powerOnMsg, 16);
                break;
            case FM_IMFOS_ADDR:
                memcpy(&progromData0,(U8 *)&g_FMInform.FmCurFreq, 64);
                break;
            case RF_MODEL_ADDR:
                memcpy(&progromData0,(U8 *)&g_rfMoudel.moduleType, 5);
                break;
            default:
                SpiFlash_ReadBytes(address, &progromData0, PROGROMLEN);
                if(address == BAND2_ADDR)
                {
                    buf[21] = g_rfMoudel.moduleType;
                }
                break;
        }
    }
}
/*********************************************************************
* 函 数 名: EnterProgromMode
* 功能描述: 进入写频模式
* 全局变量: 
* 输入参数：
* 输出参数:
* 返　　回:
* 说    明：
***********************************************************************/
extern void EnterProgromMode(void)
{

    RfOff();
    //显示写频模式界面
    DisplayProgrom();

    progrom.states = PRG_CHECKHEAD;
    while(1)
    {
        if(g_msFlag)
        {//延时函数
            g_msFlag = 0;
            CheckProgromTimeout();
        }
        
        if(progrom.recFlag)
        {
            progrom.recFlag = 0;
            
            switch(progrom.states)
            {
                case PRG_FRQRANGE:
                    if(progromCmd == PRG_CMD_FREQ)
                    {
                        ProgromSendFreqRange();
                        progrom.dataNum = 0;
                        progrom.states = PRG_WR;   
                    }
                    else if(progromCmd == PRG_CMD_FLASH)
                    {
                        g_sysRunPara.sysRunMode = MODE_FLASH_PROGRAM;
                        progrom.enterMode = 0;
                        progrom.dataNum = 0;
                        progrom.timeOut = 0;

                        return;
                    }
                    else
                    {
                        progrom.dataNum = 0;
                    }
                    break;
                case PRG_WR:
                    if(progromCmd == PRG_CMD_READ)
                    {
                        if(progrom.dataNum >= 4)
                        {
                            ProgromWriteReadData(0);
                            progrom.dataNum = 4 + progromLen;
                            //发送数据
                            UartSendBuf(progrom.rxBuf,progrom.dataNum);
                            progrom.dataNum = 0;

                            LedTxSwitch(LED_FLASH);
                        }
                    }
                    else if(progromCmd == PRG_CMD_WRITE)
                    {
                        if(progrom.dataNum >= (PROGROMLEN + 4))
                        {
                            ProgromWriteReadData(1);
                            ProgromSendAck();
                            LedRxSwitch(LED_FLASH);
                        }
                    }
                    else
                    {
                        if(progromCmd == PRG_CMD_END)
                        {
                            progrom.states = PRG_END;
							ProgromSendAck();
                        }
                        progrom.dataNum = 0;
                    }
                    break;
                case PRG_END:
                case PRG_ERROR:
                    break;
                case PRG_CHECKHEAD:
                default:
                    if(CheckLinkHead() == TRUE)
                    {
						ProgromSendAck();
                        progrom.states = PRG_FRQRANGE;
                    }
                    break;
            }
        }
        else
        {
            if(progrom.states == PRG_ERROR)
            {
                progrom.states = PRG_CHECKHEAD;
                progrom.errCnt++;

                if(progrom.errCnt >= 3)
                {//错误次数超过三次
                    progrom.states = PRG_END;
                }
            }

            if(progrom.states == PRG_END)
            {
                LedTxSwitch(LED_OFF);
                LedRxSwitch(LED_OFF);
                progrom.dataNum = 0;
			    //复位系统
                NVIC_SystemReset();
            }
        }
    }
}
void StatusAck(U8 cmd, U8 status)
{
    U16 crcTemp;
    
    flashProgrom.txLength = 0;
    flashProgrom.txBuf[flashProgrom.txLength++] = FLASH_PRO_HEAD;
    flashProgrom.txBuf[flashProgrom.txLength++] = cmd;
    //数据包
    flashProgrom.txBuf[flashProgrom.txLength++] = 0x00;
    flashProgrom.txBuf[flashProgrom.txLength++] = 0x00;
    //数据长度
    flashProgrom.txBuf[flashProgrom.txLength++] = 0x00;
    flashProgrom.txBuf[flashProgrom.txLength++] = 0x01;
    //错误代码
    flashProgrom.txBuf[flashProgrom.txLength++] = status;

    crcTemp = CRC_ValidationCalc(&flashProgrom.txBuf[FLASH_PRO_CMD_ADDR], flashProgrom.txLength-1);

    flashProgrom.txBuf[flashProgrom.txLength++] = crcTemp>>8;
    flashProgrom.txBuf[flashProgrom.txLength++] = crcTemp;

    UartSendBuf(flashProgrom.txBuf, flashProgrom.txLength);
    flashProgrom.txLength = 0;

    progrom.timeOut = 0;
}

U8 CheckRxDataOk(void)
{
    U16 crcTemp,crcTemp2;
    
    if(progrom.rxBuf[0] != FLASH_PRO_HEAD)
    {
        StatusAck(PRO_CMD_ERROR, ERROR_CODE_HEAD);
        return FERROR;
    }

    flashProgrom.length = (progrom.rxBuf[FLASH_PRO_LEN_ADDR]<<8) + progrom.rxBuf[FLASH_PRO_LEN_ADDR+1];
    
    crcTemp = (progrom.rxBuf[FLASH_PRO_DATA_ADDR + flashProgrom.length]<<8) + progrom.rxBuf[FLASH_PRO_DATA_ADDR + flashProgrom.length + 1];

    crcTemp2 = CRC_ValidationCalc(&progrom.rxBuf[FLASH_PRO_CMD_ADDR], flashProgrom.length + 5);
    if(crcTemp != crcTemp2)
    {
        StatusAck(PRO_CMD_ERROR, ERROR_CODE_DATA);
        return FERROR;
    }

    flashProgrom.cmd = progrom.rxBuf[FLASH_PRO_CMD_ADDR];
    
    return OK;
}
void HandleProgromCmd(void)
{
    U8 temp;
    U16 packedNum;
    U32 addr;

    memcpy(flashProgrom.txBuf,progrom.rxBuf,4);
    flashProgrom.txLength = 4;
    switch(flashProgrom.cmd)
    {
        case PRO_CMD_ADDR:
            memcpy((U8 *)&flashProgrom.StartAddr,&progrom.rxBuf[FLASH_PRO_DATA_ADDR],4);
            flashProgrom.packageNum = 0;
            break;
        case PRO_CMD_EARSE:
            temp = progrom.rxBuf[FLASH_PRO_PACKED_ADDR + 1];

            //获取数据长度
            if(flashProgrom.length)
            {
                memcpy((U8 *)&addr,&progrom.rxBuf[FLASH_PRO_DATA_ADDR],4); 
                packedNum = (progrom.rxBuf[FLASH_PRO_DATA_ADDR+4]<<8) + progrom.rxBuf[FLASH_PRO_DATA_ADDR+5];
            }

            if(temp == ERASE_MODE_4K)
            {
                for(temp = 0;temp<packedNum;temp++)
                {
                    SpiFlash_EraseSector(addr);
                    addr += SPIFLASH_PAGESIZE;
                }
            }
            else if(temp == ERASE_MODE_32K)
            {
                for(temp = 0;temp<packedNum;temp++)
                {
                    SpiFlash_Erase32kBlock(addr);
                    addr += 0x8000;
                }
            }
            else if(temp == ERASE_MODE_64K)
            {
                for(temp = 0;temp<packedNum;temp++)
                {
                    SpiFlash_Erase64kBlock(addr);
                    addr += 0x10000;
                }
            }
            else
            {
                SpiFlash_EraseChip();
            }
            break;
        case PRO_CMD_WRITE:
            packedNum = (progrom.rxBuf[FLASH_PRO_PACKED_ADDR]<<8) + progrom.rxBuf[FLASH_PRO_PACKED_ADDR+1]; 
            
            if(flashProgrom.packageNum != packedNum)
            {
                StatusAck(PRO_CMD_ERROR, ERROR_CODE_ADDR);
                return;
            }

            SpiFlash_WriteBytes(flashProgrom.StartAddr,&progrom.rxBuf[FLASH_PRO_DATA_ADDR],flashProgrom.length);

            flashProgrom.StartAddr += flashProgrom.length;
            flashProgrom.packageNum++;

            break;   
        case PRO_CMD_END:
            NVIC_SystemReset();
            break;
        default:
            break;
    }

    //返回当前命令
    if(flashProgrom.txLength == 4)
    {
        flashProgrom.txBuf[flashProgrom.txLength++] = 0x00;
        flashProgrom.txBuf[flashProgrom.txLength++] = 0x01;
        flashProgrom.txBuf[flashProgrom.txLength++] = 0x59;
    }

    //计算CRC校验码
    packedNum = CRC_ValidationCalc(&flashProgrom.txBuf[FLASH_PRO_CMD_ADDR], flashProgrom.txLength-1);

    flashProgrom.txBuf[flashProgrom.txLength++] = packedNum>>8;
    flashProgrom.txBuf[flashProgrom.txLength++] = packedNum;

    UartSendBuf(flashProgrom.txBuf, flashProgrom.txLength);
    flashProgrom.txLength = 0;

    //发送完成后清除超时时间，一包数据结束，说明协议完整结束
    progrom.timeOut = 0;
}

extern void EnterFlashProgromMode(void)
{
    progrom.states = FLASH_PRG_CHECKHEAD;
    g_sysRunPara.sysRunMode = MODE_FLASH_PROGRAM;
    progrom.packageTime = 0;
    progrom.timeOut = 0;
    //默认设置起始地址为0
    flashProgrom.StartAddr = 0;

    while(1)
    {
        if(g_msFlag)
        {//延时函数
            g_msFlag = 0;
            if(progrom.timeOut)
            {
                progrom.timeOut--;
        
                if(progrom.timeOut == 0)
                {//超时复位设备
                    progrom.states = FLASH_PRG_END;
                }
            }

            if(progrom.packageTime)
            {
                progrom.packageTime--;
                if(0 == progrom.packageTime)
                {
                    progrom.recFlag = 1;
                }
            }
            CheckPowerOff();
        }

        if(progrom.recFlag)
        {//接收到1包数据
            progrom.recFlag = 0;

            if(CheckRxDataOk() == OK)
            {
                switch(progrom.states)
                {
                    case FLASH_PRG_CHECKHEAD:
                        if(flashProgrom.cmd == PRO_CMD_HANDSHAKE)
                        {
                            progrom.states = FLASH_PRG_CMD;
                            StatusAck(PRO_CMD_HANDSHAKE, ERROR_CODE_OK);
                        }
                        break;
                    case FLASH_PRG_CMD:
                        HandleProgromCmd();
                        break; 
                    default:
                        break;
                }
            }
            progrom.dataNum = 0;
        }
        else
        {
        
            if(progrom.states == FLASH_PRG_ERROR)
            {
                progrom.states = FLASH_PRG_CHECKHEAD;
                progrom.errCnt++;

                if(progrom.errCnt >= 3)
                {//错误次数超过三次
                    progrom.states = FLASH_PRG_END;
                }
            }

            if(progrom.states == FLASH_PRG_END)
            {
                LedTxSwitch(LED_OFF);
                LedRxSwitch(LED_OFF);
                progrom.dataNum = 0;

                //复位系统
                NVIC_SystemReset();
            }
        }
    }   
}


