#ifndef __DISPLAYMAIN_H
    #define __DISPLAYMAIN_H

typedef enum 
{
    DISPLAY_MAIN = 0,
    DISPLAY_MENU,
    DISPLAY_FM,
    DISPLAY_SEARCH,
    DISPLAY_SCANQT,
    DISPLAY_WEATHER,
    DISPLAY_STOPWATCH,
    DISPLAY_PROGRAM,
    DISPLAY_INVALID = 0xFF
}Display_Type;

enum{CH_DISNAME=0,CH_DISFREQ, CH_DISCHNUM,CH_DISNAME_FREQ,VFO_DISFREQ}; 
enum{DIS_RX=0,DIS_TX};

extern Display_Type gDisplayMode;
extern void TranStrToMiddle(String  *targeBuf,String *sourceBuf,U8 len);
extern void DisplayProgrom(void);
extern void DisplayUpdateLockFlag(U8 UpdateF);
extern void DisplayStateBar(void);
extern void DisplayHomePage(void);
extern void DisplayRadioHome(void);
extern void DisplayInputChNum(void);
extern void DisplayInputVfoFreq(void);
extern void DisplayTxMode(void);
extern void DisplayRxMode(void);
extern void DisplayChannelNum(U16 currNum,U8 disAB);
extern void DisplaySingalFlag(U8 level,U8 UpdateF);

extern void ClearAniDisplay(void);
extern void DisplayAniMsg(U8 *pCallerId, U8 *pCalledId);
extern void DisplaySoftVersion(void);
extern void DisplayResetRxFlag(void);
#endif
