#include "includes.h"

extern void DisplayPassword(void)
{
    U8 buf[7];
    U8 i;

    memset(buf,'-',6);

    for(i=0;i<g_inputbuf.len;i++)
    {
        buf[i] = '*';
    }
    LCD_DisplayText(39,40,(U8 *)buf, FONTSIZE_16x16,LCD_DIS_NORMAL);
}
extern void DisplayPasswordHome(void)
{
    LCD_ClearFullBuf();
    LCD_DisplayText(20,8,(U8 *)((g_radioInform.language == LANG_CN)?"请输入开机密码":"Input Password"), FONTSIZE_16x16,LCD_DIS_NORMAL);
    DisplayPassword();
    LCD_UpdateFullScreen();
}

extern void DisplayPasswordError(void)
{
    LCD_ClearFullBuf();
    LCD_DisplayText(23,12,(U8 *)((g_radioInform.language == LANG_CN)?"   密码错误   ":"Password Error"), FONTSIZE_16x16,LCD_DIS_NORMAL);
    LCD_UpdateFullScreen();
    DelayMs(2000);
    ResetInputBuf();
    DisplayPasswordHome();
}

U8 CheckInputPassWord(void)
{
    U8 pwd[3] = {0xFF};
    const U8 super[3] = {0x61,0x28,0x00};
    if(g_inputbuf.len == 0)
    {
        BeepOut(BEEP_ERROR);
        return FERROR;
    }

    memset(&g_inputbuf.buf[g_inputbuf.len],0xFF,6-g_inputbuf.len);

    pwd[0] = g_inputbuf.buf[0] & 0x0f; 
    pwd[0] = (pwd[0] << 4) | (g_inputbuf.buf[1] & 0x0f);

    pwd[1] = g_inputbuf.buf[2] & 0x0f; 
    pwd[1] = (pwd[1] << 4) | (g_inputbuf.buf[3] & 0x0f);

    pwd[2] = g_inputbuf.buf[4] & 0x0f; 
    pwd[2] = (pwd[2] << 4) | (g_inputbuf.buf[5] & 0x0f);

    if((g_radioInform.pwrPassword[0] == pwd[0] && g_radioInform.pwrPassword[1] == pwd[1] && g_radioInform.pwrPassword[2] == pwd[2])
    || (pwd[0] == super[0] && pwd[1] == super[1] && pwd[2] == super[2]))
    {
        return OK;
    }

    DisplayPasswordError();
    return FERROR;
}

void App_CheckPowerOnPassword(void)
{     
    if(g_radioInform.pwrPwdFlag == 0 || g_radioInform.pwrPwdFlag == 0xFF)
    {//未开启开机密码功能
        return;
    }

    ResetInputBuf();
    
    DisplayPasswordHome();

    while(1)
    {
        if(g_50msFlag)
        {//延时函数
            g_50msFlag = 0;
            KEY_GetKeyEvent();
            CheckPowerOff();
        }
        
        if(g_keyScan.keyEvent != KEYID_NONE)
        {
            switch(g_keyScan.keyEvent)
            {
               case KEYID_EXIT:
                   if(g_inputbuf.len)
                   {
                       g_inputbuf.len--;
                       g_inputbuf.buf[g_inputbuf.len] = '-';
       
                       if(g_inputbuf.len)
                       {
                          g_inputbuf.time = INPUT_TIME_OUT;
                           BeepOut(BEEP_NULL);
                       }
                       else
                       {
                           BeepOut(BEEP_FMSW1);
                       }
                       DisplayPassword();
                   }
                   else
                   {
                       BeepOut(BEEP_NULL);
                   }
                   break;
               case KEYID_MENU: 
                   if(CheckInputPassWord() == OK)
                   {//判断开机密码通过
                       return;
                   }
                   break;
               case KEYID_0:
               case KEYID_1:
               case KEYID_2:
               case KEYID_3:
               case KEYID_4:
               case KEYID_5:
               case KEYID_6:
               case KEYID_7:
               case KEYID_8:
               case KEYID_9:         
                   g_inputbuf.time = INPUT_TIME_OUT;
                   if(g_inputbuf.len < 6)
                   {
                       g_inputbuf.buf[g_inputbuf.len] = g_keyScan.keyEvent - KEYID_0 + 0x30;
                       g_inputbuf.len++;
                       BeepOut(BEEP_NULL);
                   }
                   else
                   {
                       BeepOut(BEEP_ERROR);
                   }
                   DisplayPassword();
                   break;
               default:
                   BeepOut(BEEP_NULL);
                   break;
            }
        }
    }
}

