#ifndef __APPSCAN_H
    #define __APPSCAN_H

enum 
{
    TIME_MODE=0,FREQ_MODE,FINE_MODE
};
enum 
{
    SCAN_DOWN=0,SCAN_UP
};

typedef enum 
{
    SCAN_IDLE=0,SCANNING,WAIT_TIME,WAIT_LOST,WAIT_RECALL,SCAN_STOP
}ENUM_SCANSTATE;    

#define SCAN_LOST_TIME             50
#define SCAN_FIND_TIME             10


typedef struct
{
    ENUM_SCANSTATE state;                           //扫描状态
    U8 direction;                               //扫描方向
    U16 scanTime;
}STR_SCAN;

extern STR_SCAN  g_scanInfo; 
/******************************************************************/
extern U8 CheckInputScanRange(U16 freqL,U16 freqH);
extern void EnterScanMode(void);
extern void ScanStart(void);
extern void ScanOff(void);
extern void ScanTask(void);
extern void KeyProcess_Scan(U8 keyEvent);

#endif
