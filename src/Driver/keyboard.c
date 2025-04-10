#include "includes.h"

STR_KEYSCAN g_keyScan;
static U8 PressedKey = KEYID_NONE;
static U8 KeyPressCount;
static U8 SubmitKey = 0;

#define KeyReadDelay()                  DelayUs(5)

const U16 keyscanTab1[] = { 0x0E11, 0x0E10, 0x0E01, 0x0A11, 0x0611, 0x0C11};  // 扫描列表_输出高电平
const U16 keyscanTab2[] = { 0x0000, 0x0001, 0x0010, 0x0400, 0x0800, 0x0200};  // 扫描列表_输出低电平

#define MASK_MATRIX_LINE           0x6003
#define MASK_MATRIX_LINE_1         0x4003
#define MASK_MATRIX_LINE_2         0x2003
#define MASK_MATRIX_LINE_3         0x6001
#define MASK_MATRIX_LINE_4         0x6002
        
// 矩阵键值
const KeyID_Enum KEYBOARD_TABLE[][4] =
{
    { KEYID_NONE,KEYID_SIDEKEY1,   KEYID_NONE,      KEYID_SIDEKEY2},
    { KEYID_7,     KEYID_8,        KEYID_WELL,      KEYID_9,      },
    { KEYID_4,     KEYID_5,        KEYID_0,         KEYID_6,      },
    { KEYID_1,     KEYID_2,        KEYID_STAR,      KEYID_3,      },
    { KEYID_MENU,  KEYID_UP,       KEYID_EXIT,      KEYID_DOWN,   },
    { KEYID_VM,    KEYID_AB,       KEYID_NONE,      KEYID_BAND,   },
};

static U16 KEY_ReadGpioInput(GPIO_TypeDef* GPIOx)
{
    U8 readBit;
    U16 readData = 0x00;

    readData = GPIO_ReadInputData(GPIOC) & 0x6000;
    readBit = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
    readData |= readBit;

    readBit = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6);
    readData |= (readBit << 1);

    return readData;
}

KeyID_Enum GetKeyCode(void)
{
    U8 i;

    for( i = 0; i < 6; i++ )
    {
        GPIO_SetBits( GPIOB, keyscanTab1[i]  );
        GPIO_ResetBits( GPIOB, keyscanTab2[i] );
        
        KeyReadDelay();
        switch( KEY_ReadGpioInput(GPIOB) & MASK_MATRIX_LINE )
        {
            case MASK_MATRIX_LINE_1:  return KEYBOARD_TABLE[i][0];
            case MASK_MATRIX_LINE_2:  return KEYBOARD_TABLE[i][1];
            case MASK_MATRIX_LINE_3:  return KEYBOARD_TABLE[i][2];
            case MASK_MATRIX_LINE_4:  return KEYBOARD_TABLE[i][3];
        }
    }
    
    return KEYID_NONE;
}

extern void KeyScanReset(void)
{
    PressedKey = KEYID_NONE;
    g_keyScan.keyEvent = KEYID_NONE;
    g_keyScan.keyPara = KEY_CLICKED;
}

extern Boolean KEY_GetKeyEvent(void)
{
    static KeyID_Enum preKey = KEYID_NONE;
    static KeyID_Enum key = KEYID_NONE;

    preKey = key;
    key = GetKeyCode();

    if(preKey != key)
    {
        return FALSE;
    }

    if(key == KEYID_NONE)
    {
        if(PressedKey == KEYID_NONE)
        {
            KeyPressCount = 0;
            return FALSE;
        }
        
        if(SubmitKey == 0 || g_rfState == RF_TX)
        {
            KeyPressCount = 0;
            g_keyScan.keyEvent = PressedKey;
            g_keyScan.keyPara = KEYSTATE_RELEASE;
            g_keyScan.longPress = KEY_CLICKED;
            PressedKey = KEYID_NONE;
            return TRUE;
        }    
        PressedKey = KEYID_NONE;
        SubmitKey = 0;
        return FALSE;
    }
    
    if(PressedKey == KEYID_NONE)
    {//判断是否支持长按键
        PressedKey = key;
        KeyPressCount = 1;

        if(g_rfState == RF_TX)
        {
            g_keyScan.keyPara = KEYSTATE_CLICKED;
            g_keyScan.keyEvent = PressedKey;
            g_keyScan.longPress = KEY_CLICKED;
            return TRUE; 
        }
    }
    

    if(PressedKey == key)
    {
        KeyPressCount++;

        if(KeyPressCount == 100) 
        {//支持长按键
            if(PressedKey == KEYID_UP || PressedKey == KEYID_DOWN  || PressedKey == KEYID_VM || PressedKey == KEYID_BAND
               || PressedKey == KEYID_STAR || PressedKey == KEYID_WELL  || PressedKey == KEYID_0 || PressedKey == KEYID_8
               || PressedKey == KEYID_EXIT || PressedKey == KEYID_SIDEKEY1 || PressedKey == KEYID_SIDEKEY2)
            {
                g_keyScan.keyEvent = PressedKey;
                g_keyScan.keyPara = KEYSTATE_LONG;
                g_keyScan.longPress = KEY_LONG;
                SubmitKey = 1;
                return TRUE; 
            }
        }

        if(KeyPressCount >= 150)
        {
            KeyPressCount = 140;

            if(PressedKey == KEYID_UP || PressedKey == KEYID_DOWN)
            {
                g_keyScan.keyEvent = PressedKey;
                g_keyScan.keyPara = KEYSTATE_NONE;
                g_keyScan.longPress = KEY_CONTINUE;

                SubmitKey = 1;
                return TRUE; 
            }         
        }
    }
    else
    {//新按键按下
        if(SubmitKey == 0 || g_rfState == RF_TX)
        {
            g_keyScan.keyEvent = PressedKey;
            g_keyScan.longPress = KEY_CLICKED;
            g_keyScan.keyPara = KEYSTATE_RELEASE;
            KeyPressCount = 0;
            PressedKey = KEYID_NONE;
            return TRUE; 
        }        
        KeyPressCount = 0;
        PressedKey = KEYID_NONE;
    }
    return FALSE; 
}


extern void KEY_ScanTask(void)
{
    if(KEY_GetKeyEvent() == FALSE)
    {
        return;
    }

    g_sysRunPara.rfTxFlag.voxDetDly = 4;

    if(g_radioInform.keyLock && (g_keyScan.keyEvent != KEYID_SIDEKEY1 && g_keyScan.keyEvent != KEYID_SIDEKEY2 && !(g_keyScan.keyEvent == KEYID_STAR && g_keyScan.longPress == KEY_LONG)))
    {
       g_keyScan.keyEvent = KEYID_NONE;
       g_keyScan.longPress = 0;
       BeepOut(BEEP_NULL);
    }
    
    LCD_BackLightSetOn();
    ResetTimeKeyLockAndPowerSave();
}

extern U8 Key_GetRealEvent(void)
{
    U8 keyEvent;
    
    if(g_keyScan.longPress == KEY_LONG)
    {
        switch(g_keyScan.keyEvent)
        {
            case KEYID_SIDEKEY1:
                keyEvent = KEYID_SIDEKEYL1;
                break;
            case KEYID_SIDEKEY2:
                keyEvent = KEYID_SIDEKEYL2;
                break;
            case KEYID_VM:
                keyEvent = KEYID_CHDIS;
                break;
            case KEYID_AB:
                keyEvent = KEYID_DUAL_SW;
                break;    
            case KEYID_BAND:
            case KEYID_EXIT:
                keyEvent = KEYID_SEARCH;
                break; 
            case KEYID_STAR:
                keyEvent = KEYID_LOCK;
                break;  
            case KEYID_WELL:
                keyEvent = KEYID_SCAN;
                break;  
            case KEYID_0:
                keyEvent = KEYID_WX;
                break;     
            case KEYID_8:
                keyEvent = KEYID_PWRSW;
                break;     
            default:
                keyEvent = KEYID_NONE;
                break;
        }
    }
    else if(g_keyScan.longPress == KEY_CONTINUE)
    {
        if(g_keyScan.keyEvent == KEYID_DOWN)
        {
            keyEvent =  KEYID_DOWN_CONTINUE;
        }
        else
        {
            keyEvent = KEYID_UP_CONTINUE;
        }
    }
    else
    {
        keyEvent = g_keyScan.keyEvent;
    }

    g_keyScan.longPress = KEY_CLICKED;
    g_keyScan.keyEvent = KEYID_NONE;

    return keyEvent;
}

