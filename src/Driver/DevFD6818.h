#ifndef  __RF_RFIC_H__
    #define  __RF_RFIC_H__

/**************************常量声明区************************************/
#define RF_GPIO7    0X0080
#define RF_GPIO6    0X0040
#define RF_GPIO5    0X0020
#define RF_GPIO4    0X0010
#define RF_GPIO3    0X0008
#define RF_GPIO2    0X0004
#define RF_GPIO1    0X0002
#define RF_GPIO0    0X0001

/**************************结构体************************************/
enum { GPIOx_RFIC_L=0, GPIOx_RFIC_H };
enum { BAND_WIDE, BAND_NARROW, BAND_W20K};
enum { RFIC_IDLE, RFIC_RXON, RFIC_TXON, RFIC_TONE, RFIC_TXTONE };
enum { ModeAudio=0,ModeScramble,ModeDtmf,ModeFSK,ModeSelcall,ModeDet1050, ModeAM, ModeFM };//work mode

typedef enum
{
    SUBAUDIO_NONE,
	SUBAUDIO_CTS,
	SUBAUDIO_DCS_N,
	SUBAUDIO_DCS_I,
} SUBAUDIO_TYPE;


/***************************** 宏定义函数 *******************************/
#ifndef RFFLAG_SQ
    #define RFFLAG_SQ           (Rfic_ReadWord(0X0C) & BIT1)
#endif

#ifndef RFFLAG_CTC

#endif

extern U16 hoping_freq;//跳频码 宝锋用
extern const U16  DCS_TAB[105];
extern const U16  CTCS_TAB[51];

extern U8  OFFSET_MODULATION[16];                  // 调制度
#define  DEPTH_MIC_MODULATION        OFFSET_MODULATION[0]   //话音调制度
#define  DEPTH_CTS_MODULATION        OFFSET_MODULATION[1]   //模拟亚音调制度    最大值 127   默认值66
#define  DEPTH_DCS_MODULATION        OFFSET_MODULATION[2]   //数字亚音调制度    最大值 127   默认值53
#define  VOLUMN_WIDEBAND             OFFSET_MODULATION[3]   //宽带音量          最大值 31   默认25
#define  VOLUMN_NARROWBAND           OFFSET_MODULATION[4]   //窄带音量          最大值 31   默认25
#define  XTAL_ADJUST                 OFFSET_MODULATION[6]  // 26M晶体校准

#define AF_RX_300HZ                  OFFSET_MODULATION[11]
#define AF_RX_3KHZ                   OFFSET_MODULATION[12]
#define AF_TX_300HZ                  OFFSET_MODULATION[13]
#define AF_TX_3KHZ                   OFFSET_MODULATION[14]

extern  U8  TABLE_RF_GAIN[]; // Rfic射频输出增益

extern U8  *TXPWR_U_400[3];
extern U8  *TXPWR_V_136[3];
extern U8  *TXPWR_V_200[3];
extern U8  *TXPWR_U_350[3];

extern U8  *TXPWR_U_400_RF[3];
extern U8  *TXPWR_V_136_RF[3];
extern U8  *TXPWR_V_200_RF[3];

extern U8  TH_SQL_TAB[13];                        // 静噪等级补偿值  1 -- 9级
extern U8  TH_SQL_TAB_MUTE[13];
extern U8  OFFSET_SQL_U_400[16];                  // U段静噪等级补偿      间隔10MHz
extern U8  OFFSET_SQL_V_136[16];                  // V-136段静噪等级补偿  间隔5MHz
extern U8  OFFSET_SQL_V_200[16];                  // V-200段静噪等级补偿  间隔5MHz


extern U8  TXPWR_H_U_400[16];
extern U8  TXPWR_H_V_136[16];
extern U8  TXPWR_H_V_200[16];
extern U8  TXPWR_H_U_350[16];


extern U8  TXPWR_M_U_400[16];
extern U8  TXPWR_M_V_136[16];
extern U8  TXPWR_M_V_200[16];
extern U8  TXPWR_M_U_350[16];


extern U8  TXPWR_L_U_400[16];
extern U8  TXPWR_L_V_136[16];
extern U8  TXPWR_L_V_200[16];
extern U8  TXPWR_L_U_350[16];

extern U8  OFFSET_SQL_U_400[16];
extern U8  OFFSET_SQL_V_136[16];
extern U8  OFFSET_SQL_V_200[16];
extern U8  OFFSET_SQL_U_350[16];


extern const U8 xtal26MAdjust[];
/**************************函数声明区************************************/
extern void Rfic_WriteWord( U8  devAddr,U16  devData );
extern U16 Rfic_ReadWord(U8  devAddr);

extern Boolean Rfic_GetSQLinkState(void);
extern void Rfic_Sleep(void);
extern void Rfic_WakeUp(void);

extern void  Rfic_Init(void);
extern void  Rfic_ConfigRxMode(void);
extern void  Rfic_ConfigTxMode(void);
extern void  Rfic_RxTxOnOffSetup(U8  ON_FLAG);
extern void  Rfic_SetAfout(U8  state);
extern void  Rfic_SetPA(U16 dat);

extern U16 Rfic_GetRssiVal(void);
extern U8  Rfic_GetNoiseVal(void);
extern U16 Rfic_GetTail(void);

extern void Rfic_VoxSet( U8  voxLv );
extern void Rfic_MicIn_Enable(void);
extern void Rfic_MicIn_Disable(void);

extern U32  GOLAY_ENCODE(U16 dcsCode);
extern void CTS_DCS_RECE_Initial(void);
extern void CTCSS_WithRfic(U32 CTS_Data, U8  sw_fhss);
extern void DTCSS_WithRfic(U32 DCS_Code, U8  flagLearning);
extern void CloseFhss(void);
extern U16 Rfic_DecoderDetect( U8  ctsDcsType );
extern void RF_SendTail( U8  OnOrOff );
extern U16 Rfic_SubaudioDetect( void );

extern void Rfic_GpioSetBit( U16 gpiox, U8  val );
extern void Rfic_GpioFlash( U8  gpiox );

extern void Rfic_SetToneFreq( U16 dat );
extern void  Rfic_TxSingleTone_On(U8  txOn);
extern void Rfic_TxSingleTone_Off(void);

extern void Rfic_SwitchFM_AM(U8 mode);

/*****************************************************************/
/*                            F S K                              */
/*****************************************************************/
extern void Rfic_EnterFSKMode(U8  flagTx);
extern void Rfic_ExitFSKMode(void);
extern U8  Rfic_FskTransmit(U16 *pData);
extern U8  Rfic_GetFskRxFlag(void);
extern U8  Rfic_ReadFskData(U16 *pData);
/*****************************************************************/
/*                            DTMF                               */
/*****************************************************************/
extern void Rfic_EnterDTMFMode(U8  flagTx);
extern void Rfic_ExitDTMFMode(void);
extern void Rfic_SetDtmfFreq( U16 tone1Freq, U16 tone2Freq);
extern U8  Rfic_ReadDTMF(void);
extern U8  Rfic_GetDTMF_Link(void);
/*****************************************************************/
/*                           Freq Scan                           */
/*****************************************************************/
extern void Rfic_FreqScan_Enable(void);
extern void Rfic_FreqScan_Disable(void);
extern U32 Rfic_CheckFreqScan(void);
extern U8  Rfic_GetSCtsDcsType(void);
extern void Rfic_CtcDcsScan_Setup(U32 freq);
extern U32 Rfic_GetCtsDcsData(void);
extern void Rfic_ScanQT_Enable(void);
extern void Rfic_ScanQT_Disable(void);

/*****************************************************************/
/*                           MDC1200                            */
/*****************************************************************/
extern void Rfic_EnterMDC1200Mode(void);
extern void Rfic_ExitMDC1200Mode(void);
extern U8  Rfic_MDC1200Transmit(U16 *pData);
extern U8  Rfic_GetMDC1200RxFlag(void);
extern U8  Rfic_ReadMDC1200Data(U16 *pData);
extern void Rfic_MDC1200ToneTx(void);

#endif
