#ifndef __DISPLAYMENU_H
    #define __DISPLAYMENU_H

#define MENU_MAX_CNT              55

typedef struct
{
    uint8_t voiceId;
    const char nameEn[16];  //英文名称
    const char nameCn[16];  //中文名称
}STR_MENU_ITEM;

//定义菜单序列
enum{   
    S_CHNAME=0,
    S_RXFREQ,
    S_TXFREQ,
    S_RXCTS,
    S_RXDCS,
    S_TXCTS, 
    S_TXDCS, 
    S_WN, 
    S_TXPR, 
    S_SPMUTE,
    S_TXFORBID,
    S_SFTD,
    S_OFFSE,
    S_MEMCH, 
    S_DELCH,
    S_VFOSCAN,
    S_SCREV,
    S_DTST, 
    S_PTTID, 
    S_PTTLT,
    S_SQL,
    S_SAVE, 
    S_VOX, 
    S_VOXLV, 
    S_VOXDLY,
    S_TOT,
    S_LAN,
    S_VOIC,
    S_MENUEXIT,
    S_BEEP,
    S_ROGE, 
    S_PONTYPE,
    S_PONTONE,
    S_PONMSG,
    S_PWR,
    S_TDR,
    S_MDF1, 
    S_MDF2, 
    S_RPSTE,
    S_RPTRL,
    S_RTONE, 
    S_STEP,
    S_BUSYLOCK,
    S_TAIL,  
    S_ALMOD, 
    S_SK1,
    S_SKL1,
    S_SK2,
    S_ABR,  
    S_BRIGHT,
    S_REFLEX,
    S_AUTOLK, 
    S_FMINT,   
    S_RESET,    
    S_WATCH,
    S_INFO
};
extern String disBuf[17];
/********************************************************************************************************************/
extern const STR_MENU_ITEM MenuList[];
extern U8 inputTypeBack;
extern void DisplayInputType(void);
extern void MenuShowInputChar(void);
extern void Menu_GetSubItemString(U8 menuIndex);
extern void Menu_DisplayFreqError(void);
extern void Menu_Display(void);

#endif
