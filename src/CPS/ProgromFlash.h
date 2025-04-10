#ifndef __INC_PROGROMTYPE_H__
    #define __INC_PROGROMTYPE_H__
/***************************************************************************************/
#define FLASH_PRO_HEAD           0xA5       //命令包头  

#define FLASH_PRO_CMD_ADDR       0x01
#define FLASH_PRO_PACKED_ADDR    0x02
#define FLASH_PRO_LEN_ADDR       0x04
#define FLASH_PRO_DATA_ADDR      0x06

//定义命令
#define PRO_CMD_HANDSHAKE        0x02
#define PRO_CMD_ADDR             0x03
#define PRO_CMD_EARSE            0x04
#define PRO_CMD_END              0x06      //重启命令
#define PRO_CMD_WRITE            0x57

#define PRO_CMD_ERROR            0xEE

//定义错误代码
#define ERROR_CODE_OK            0X59       //数据正确
#define ERROR_CODE_HEAD          0x01       //包头错误
#define ERROR_CODE_HAND          0x02       //握手码错误
#define ERROR_CODE_CMD           0x03       //命令错误
#define ERROR_CODE_DATA          0x04       //数据错误
#define ERROR_CODE_WRITE         0x05       //Flash写入错误
#define ERROR_CODE_ADDR          0x06       //Flash地址错误

//定义擦除模式
#define ERASE_MODE               0X45
#define ERASE_MODE_CHIP          0X01       //整片擦除
#define ERASE_MODE_4K            0X02       //擦除4k
#define ERASE_MODE_32K           0X03       //擦除32k
#define ERASE_MODE_64K           0X04       //擦除64k

enum{FLASH_PRG_CHECKHEAD=0,FLASH_PRG_CMD,FLASH_PRG_END,FLASH_PRG_ERROR};

typedef struct
{
    U8  cmd;                  //当前接收到的命令
    U32 StartAddr;            //Flash开始编辑的首地址
    U16 packageNum;           //当前已经接收到的数据包
    U16 length;               //当前接收到的数据长度
    U16 txLength;             //发送的数据长度
    U8  txBuf[64];
}STR_FLASH_PROGROM;

extern STR_FLASH_PROGROM    flashProgrom;    
/***************************************************************************************/
#define UART_MAX_NUM           1040        //数据缓存区72个字节，其他用于Flash编辑使用

#define UARTBUF_NUM      144//72        //数据缓存区72个字节

#define UART_TIMEOUT     2000      //2S内未接收到数据超时退出

#define PROGROMLEN       64        //写入长度固定为32字节

enum{PRG_CHECKHEAD=0,PRG_FRQRANGE,PRG_WR,PRG_END,PRG_ERROR};

typedef  void (*pFunction)(void);

typedef struct
{
    U8  states;                 //写频状态
    U8  errCnt;                 //错误次数
    U8  enterMode;              //是否进入读写频模式标志
    U8  recFlag;                //接收到数据标志
    U16 dataNum;                //接收到的数据长度
    U16 timeOut;                //读写频超时时间
    U8  packageTime;            //接收数据包时间
    U8  rxBuf[1040];    //接收数据缓存
}STR_PROGROM;

extern STR_PROGROM progrom;

/***************************************************************************************/
extern const U8  strModelType[];
/***************************************************************************************/
extern Boolean CheckLinkHead(void);
extern void ProgromInit(void);
extern void CheckProgromMode(U8  rxData);
extern void UartSendBuf(U8  *buf,U16 len);
extern void EnterProgromMode(void);
extern void EnterFlashProgromMode(void);

#endif
