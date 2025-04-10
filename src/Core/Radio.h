#ifndef __RADIO_H
    #define __RADIO_H
 
extern const U8 MOD_LIST[10];

extern void ListFlagSet(U8  *listbuf,U16 Data,U8  val);
extern void GetHardWorkBand(U16 freq);
extern void RadioConfig_Init(void);
extern Boolean CheckFreqInRange(U32 freq);
extern Boolean CheckChannelActive(U16 curChanNum,U8  isScan);
extern void ChannelCheckActiveAll(void);
extern void VfoFrequency2Buf(U32 freq,U8  *dest,U8  len);
extern U16 SeekActiveChannel_Up(U16 curChanNum, U8  isScan);
extern U16 SeekActiveChannel_Down(U16 curChanNum, U8  isScan);

extern void ChannelNumChangeRead(U8  isScan,U8  isTypeIn);
extern void ChannelUp(U8 isScan, U8 brocast);
extern void ChannelDown(U8 isScan, U8 brocast);
extern void ChannelNumTypeIn(STR_INPUTBOX *input);
extern void VfoFreqUp(U8 isScan);
extern void VfoFreqDown(U8 isScan);
extern void VfoFreqSave(void);
extern void VfoFreqTypeIn(STR_INPUTBOX *input);

extern void Radio_SwitchChannelOrVfo(void);
extern void Radio_SwitchAOrB(void);
extern void Radio_ReverseOff(void);
extern void Radio_ReverseSwitch(void);

extern void ChannleVfoDataInit(U8 flagAB,U8 readFlag);
extern void RadioVfoInfo_Init(void);
#endif
