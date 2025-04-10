#ifndef __KEYBOARD_H
    #define __KEYBOARD_H

typedef enum
{
    KEYID_SIDEKEY1=0,KEYID_SIDEKEY2,
    KEYID_VM=2,      KEYID_AB,  KEYID_BAND,  KEYID_NO,
    KEYID_MENU = 6,  KEYID_UP,  KEYID_DOWN,  KEYID_EXIT,
	KEYID_0 = 10,    KEYID_1,   KEYID_2,     KEYID_3,     
	KEYID_4 = 14,    KEYID_5,   KEYID_6,     KEYID_7,  
	KEYID_8 = 18,    KEYID_9,   KEYID_STAR,  KEYID_WELL,

	KEYID_SIDEKEYL1 = 22,KEYID_SIDEKEYL2,KEYID_CHDIS,KEYID_DUAL_SW,
	KEYID_SEARCH = 26, KEYID_LOCK, KEYID_WX,KEYID_SCAN,
	KEYID_PWRSW, KEYID_UP_CONTINUE,KEYID_DOWN_CONTINUE,KEYID_MONIEXIT,
	KEYID_LIGHT, KEYID_VOX,KEYID_FM,KEYID_SOS,
	
	KEYID_NONE = 0XFF,
} KeyID_Enum;  

#define KEYSTATE_NONE        0
#define KEYSTATE_CLICKED     1
#define KEYSTATE_RELEASE     2
#define KEYSTATE_LONG        3

typedef struct
{
    U8 keyEvent;
    U8 keyPara;
    U8 longPress;
}STR_KEYSCAN;

extern STR_KEYSCAN g_keyScan;

extern KeyID_Enum GetKeyCode(void);
extern Boolean KEY_GetKeyEvent(void);
extern void KeyScanReset(void);
extern void KEY_ScanTask(void);
extern U8 Key_GetRealEvent(void);

#endif
