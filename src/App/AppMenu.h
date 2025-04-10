#ifndef __APPMENU_H
    #define __APPMENU_H

enum{MENU_ONE_SELECT=0,MENU_ONE_DIGIT,MENU_ONE_CTCSS,MENU_ONE_FREQ,MENU_ONE_CODE,MENU_ONE_SCANCTC,MENU_ONE_SCANDCS,MENU_ONE_CHAR,MENU_ONE_NULL,MENU_ONE_NAME,MENU_ONE_DECODE,MENU_ONE_QRCODE,MENU_CH_FREQ,MENU_ONE_EXIT,MENU_ONE_VFOSCAN};

typedef struct
{ 
   U8 menuType;
   U8 menuMaxItem;
   U8 isSubMenu;
   U8 menuIndex;
   U16 subMaxItem;
   U32 selectedItem;
   U16 subIndex;
   U8  inputMode;
   U8  inputLen;
   U32 inputVal;
   U16 menuExitTime;
   U16 fastInTime;
   U16 fastSelect;
   U8  preIndex;
}STR_MENUINFO;    

extern STR_MENUINFO  g_menuInfo;
extern void ResetMenuExitTime(void);
extern void CheckExitMenu(void);
extern void KeyProcess_Menu(U8 keyEvent);
extern void EnterFMMenu(void);
extern void FastEnterChMemMenu(void);
extern void Menu_EnterMode(void);
extern void Menu_ExitMode(void);

#endif
