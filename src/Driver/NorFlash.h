#ifndef _INC_NORFLASH_H
    #define _INC_NORFLASH_H
    
//定义flash使用相关参数
#define SPIFLASH_PAGESIZE              256      //定义flash每页的数据大小
#define SPIFLASH_SECTORSIZE           4096      //一块4KByte

/*************************************************************************/
//定义Flsh使用相关命令
#define SPIFLASH_WRITE_ENABLE          0x06
#define SPIFLASH_WRITE_DISABLE         0x04
#define SPIFLASH_READSTATUS_REG1       0x05
#define SPIFLASH_READSTATUS_REG2       0x35
#define SPIFLASH_READSTATUS_REG3       0x15
#define SPIFLASH_WRITESTATUS_REG1      0x01
#define SPIFLASH_WRITESTATUS_REG2      0x31
#define SPIFLASH_WRITESTATUS_REG3      0x11
#define SPIFLASH_READ_DATA             0x03
#define SPIFLASH_FASTREAD_DATA         0x0B
#define SPIFLASH_FASTREAD_DUAL         0x3B
#define SPIFLASH_PAGE_PROGRAM          0x02
#define SPIFLASH_ERASE_64K_BLOCK       0XD8
#define SPIFLASH_ERASE_32K_BLOCK       0X52
#define SPIFLASH_ERASE_SECTOR          0X20
#define SPIFLASH_ERASE_CHIP            0xC7  //or 0X60
#define SPIFLASH_POWERDOWN             0XB9
#define SPIFLASH_RELEASE_POWERDOWN     0XAB
#define SPIFLASH_DEVICEID              0XAB
#define SPIFLASH_READ_CHIPID           0X4B
#define SPIFLASH_4BYTEALIGN_ENABLE     0XB7
#define SPIFLASH_4BYTEALIGN_DISABLE    0XE9

/*************************************************************************/
extern void SpiFlash_Init(void);
extern void SpiFlash_ReadBytes(uint32_t addr,uint8_t *buf_p,uint16_t length);
extern void SpiFlash_WriteBytes(uint32_t addr,const uint8_t *buf_p,uint32_t length);
extern void SpiFlash_WriteByte_InPage(uint32_t addr,const uint8_t *buf_p,uint16_t length);
extern void SpiFlash_EraseSector(uint32_t addr);
extern void SpiFlash_Erase32kBlock(uint32_t addr);
extern void SpiFlash_Erase64kBlock(uint32_t addr);
extern void SpiFlash_EraseChip(void);

#endif
