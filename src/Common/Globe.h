#ifndef __GLOBE_H
    #define __GLOBE_H

/**************************************************************************/
#define FREQ_BAND_VHF           0                
#define FREQ_BAND_UHF           1
#define FREQ_BAND_200M          2
#define FREQ_BAND_350M          3
#define FREQ_BAND_AM            4        //AM频段    
/**************************************************************************/

extern STR_RADIOINFORM g_radioInform;
extern STR_FMINFOS g_FMInform;
extern STR_SYSTEM  g_sysRunPara;
extern STR_CHANNEL_VFO g_ChannelVfoInfo;
extern STR_CH_VFO_INFO *g_CurrentVfo;
extern STR_INPUTBOX g_inputbuf;

extern volatile U8 g_msCont;
extern volatile U8 g_msFlag;
extern volatile U8 g_10msCont;
extern volatile U8 g_100msCont;
extern volatile U8 g_10msFlag;
extern volatile U8 g_50msFlag;
extern volatile U8 g_100msFlag;
extern volatile U8 g_500msFlag;

extern Boolean g_UpdateDisplay;
extern U8 g_rfState;
extern U8 g_rfRxState;
extern U8 g_rfTxState;

extern U8 inputTypeBack;
#endif
