#ifndef __BEEP_H
    #define __BEEP_H

typedef enum 
{
    BEEP_NULL=0,BEEP_FMUP,BEEP_FMDOWN,BEEP_FMSW1,BEEP_FMSW2,BEEP_EXITMENU,BEEP_FASTSW,BEEP_ERROR,BEEP_LOWBAT
}ENUM_BEEPMODE;

/********************************************************************************************/
#define F260HZ       26
#define F300HZ       30
#define F330HZ       33
#define F400HZ       40
#define F440HZ       44
#define F450HZ       45 
#define F480HZ       48 
#define F530HZ       53 
#define F590HZ       59 
#define F600HZ       60 
#define F630HZ       63
#define F660HZ       66
#define F700HZ       70 
#define F710HZ       71 
#define F750HZ       75 
#define F780HZ       78
    
#define F850HZ       85 
#define F900HZ       90 
#define F930HZ       93 
    
#define F1000HZ      100 
#define F1050HZ      105
#define F1200HZ      120 
#define F1300HZ      130
#define F1320HZ      132
#define F1400HZ      140
#define F1500HZ      150 
#define F1570HZ      157 
/********************************************************************************************/
typedef struct
{
    ENUM_BEEPMODE beepMode;      //Tone音模式
    U8 beepCnt;                  //需要发出的Tone音频种类
    U16 beepTime;                 //Tone音时间
}STR_BEEP;

extern volatile STR_BEEP beepDat;
extern void BeepOut(ENUM_BEEPMODE beepmode);

#endif
