#ifndef _RADIODATASTORAGE_H
   #define _RADIODATASTORAGE_H

typedef enum { MODIFY, ADD, DELET } ENUM_CH_MODIFY_TYPE;

extern U8 VfoBuf[66];
extern U8 powerOnMsg[17];
extern STR_RF_MODELE g_rfMoudel;
extern STR_BAND bandRang;

extern void Flash_ReadVfoData(U8 workAB);
extern void Flash_SaveVfoData(U8 workAB);

extern void Flash_ModifyChannelData(U16 channelNum,U8* chData,U8* chName);
extern void Flash_SaveChannelData(U16 channelNum,U8* chData,U8* chName);
extern void Flash_DeleteChannelData(U16 channelNum);

extern void Flash_SaveRadioImfosData(void);
extern void Flash_ReadRadioImfosData(void);
extern void Flash_ReadDebugImfosData(void);

extern void Flash_SaveSystemRunData(void);
extern void Flash_ReadSystemRunData(void);

extern void Flash_SaveDtmfInfo(void);

extern void Flash_SaveRfMoudelType(void);
extern void Flash_ReadRfMoudelType(void);

extern void Flash_ReadFmData(void);
extern void Flash_SaveFmData(void);

#endif

