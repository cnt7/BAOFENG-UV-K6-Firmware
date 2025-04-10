#ifndef __APPSEARCH_H
    #define __APPSEARCH_H

enum{ SF_Setup, SF_Wait, SF_Check, SCtsDcs_Setup, SCtsDcs_Wait, SCtsDcs_Issue };
typedef struct
{
    U8  step;
	U32 freq;
    U32 bufFreq[2];
    U8  cntSample;
	U8  band;
    U16 overTime;
    U8  dcsIsStandard;
    U32 CtsResult;
    U8  dcsCtsType;
    U8  timesForSwitchBand;
    U8  flagRx;
}STR_SEARCHFREQ;     
extern STR_SEARCHFREQ searchFreqImofs;
extern U8 searchSaveFlag;

extern void SearchFrequencyInit(void);
extern void SearchFreqTask(void);
extern void EnterSearchFreqMode(void);
extern void ExitSearchFreqMode(U8 disHome);
extern U16 CheckIsStandardCTCSS(U32 dcsData);
extern U16 CheckIsStandardDCS(U32 dcsData);

extern void KeyProcess_Search(U8 keyEvent);

#endif
