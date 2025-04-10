#include "includes.h"

T9PY_Struct t9Pinyin;
PyBuf_Struct pyBuf;
String lcdDispBuf[2][18];  
const U8 InputChar[] = {"adgjmptw"};

U16 SJCharIndex,SJPriorChar,SJTimeout;

//0 :小写 1:大写
U8  IME_TYPE;//输入方式

String  const * const SJCharTbl[] =
{
    "()/\\# \x30",
    ".,?!':1",
    "ABC2",
    "DEF3",
    "GHI4",
    "JKL5",
    "MNO6",
    "PQRS7",
    "TUV8",
    "WXYZ9",
    "@$%&",//*
    " =+-_"//#
};

String  const * const NUMBER_SJCharTbl[] =
{
    "0",//0
    "1",//1
    "2",//2
    "3",//3
    "4",//4
    "5",//5
    "6",//6
    "7",//7
    "8",//8
    "9",//9
    "*",//*
    "#" //#
};

const U8 t9PY_indexlen[10] = {1,1,55,33,38,57,44,79,19,74};

const U32 t9PY_index_headno[] =
{
    ADDR_PY_INDEX_BASE,
    ADDR_PY_INDEX_BASE+16,
    ADDR_PY_INDEX_BASE+32,
    ADDR_PY_INDEX_BASE+912,
    ADDR_PY_INDEX_BASE+1440,
    ADDR_PY_INDEX_BASE+2048,
    ADDR_PY_INDEX_BASE+2960,
    ADDR_PY_INDEX_BASE+3664,
    ADDR_PY_INDEX_BASE+4928,
    ADDR_PY_INDEX_BASE+5232,
}; 

U8  ChangeCapsLow(U8  ch)
{
    if(IN_EN_L == g_inputbuf.inputType)
    {
        if (ch >= 'A' && ch <= 'Z')
        {
            ch += 32; 
        }
    }
    return(ch);
}

void ChkFirstInput(void)
{
    if (g_inputbuf.isFirstInput == 0xaa)
    {
        ClearPyIndex();
        ClearInputBuffer();
        g_inputbuf.isFirstInput = 0;
    }
}


U8  CharInput(String ch)
{
    if (g_inputbuf.maxLen != 0)
    {
        if (g_inputbuf.len >= g_inputbuf.maxLen)
        {
            return (FERROR);
        }
    }
    if (g_inputbuf.len < (sizeof(g_inputbuf.buf)-1))
    {
        return (InsertChar(ch));
    }
    else
    {
        return (FERROR);
    }
}

U8  NumToChar(U8  ch)
{
    String const * ptr;

    ResetMenuExitTime();
    
    ChkFirstInput();
    switch(g_inputbuf.inputType)
    {
        case IN_EN_L:
        case IN_EN_U:
            ptr = SJCharTbl[ch-'0'];
            break;
        case IN_PINYIN:
            SJTimeout = 0;
            SJPriorChar = 0;
            SJCharIndex = 0;
            ptr = NUMBER_SJCharTbl[ch-'0'];
            
            return (InputPinYin(ptr[0]));
        case IN_NUMBER:
            default:
            ptr = NUMBER_SJCharTbl[ch-'0'];
            break;
    }
    SJTimeout = 3;
    
    if (SJPriorChar == ch)
    {
        //输入字符与上一个字符一样,改变最后一个字符
        SJCharIndex++;
        while((ch =(U8 ) ptr[SJCharIndex]) == 0)
        {
            SJCharIndex = 0;
        }
        ch = ChangeCapsLow(ch);
        if(g_inputbuf.pos == 0)
        {
            return (CharInput(ch));
        }
        g_inputbuf.buf[g_inputbuf.pos-1] = ch;
        return (OK);
    }
    if((g_inputbuf.inputType == IN_NUMBER)&&((ch >='0')&&ch <='9'))
    {
        //输入一个新数字，不需要切换
        SJTimeout = 0;
        SJPriorChar = 0;
        SJCharIndex = 0;
    }
    else
    {
        //输入一个新字符
        SJPriorChar = ch;
        SJCharIndex = 0;
    }
    ch = ChangeCapsLow(ptr[0]);
    return (CharInput(ch));
}

extern void ChangeInputType(void)
{
    U8  typeMax;

    ResetMenuExitTime();
    if (g_menuInfo.inputMode == MENU_ONE_CHAR) 
    {
        ClearPyIndex();
        if(g_inputbuf.inputType == IN_PINYIN)
        {//切换输入法时清除显示
            SetInputDisMode(DROW_BACK);
            MenuShowInputChar();
        }
        g_inputbuf.inputType++;
        typeMax = (g_radioInform.language == LANG_EN)?IME_ENMAX:IME_MAXMODE;
        if (g_inputbuf.inputType >= typeMax)
        {
            g_inputbuf.inputType = IN_EN_L;
        }
        DisplayInputType();
    }
}

void ChangePos(U8  flag)
{
    if(flag)
    {
       if (g_inputbuf.pos >= g_inputbuf.len)
        {
            g_inputbuf.pos = g_inputbuf.len;
            return;
        }

        if(g_inputbuf.buf[g_inputbuf.pos] > 0x80)
        {
            g_inputbuf.pos += 2;
        }
        else
        {
            g_inputbuf.pos++;
        }
    }
    else
    {
        if(g_inputbuf.buf[g_inputbuf.pos-1] > 0x80)
        {
            if(g_inputbuf.pos <= 2)
            {
                return;
            }
            g_inputbuf.pos -= 2;
        }
        else
        {
            if(g_inputbuf.pos <= 1)
            {
                return;
            }
            g_inputbuf.pos--;
        }
    }
}

U8  BackSpaceChar(void)
{
    U8  i,k;

    ResetMenuExitTime();
    if (0 == g_inputbuf.pos || g_inputbuf.len == 0)
    {
        return (FERROR);
    }
    
    g_inputbuf.isFirstInput = 0;
    
    if(g_inputbuf.buf[g_inputbuf.pos-1] > 0x80)
    {
        g_inputbuf.pos -= 2;
        k = 2; 
    }
    else
    {
        g_inputbuf.pos--;
        k = 1; 
    }
    
    for (i=g_inputbuf.pos; i<g_inputbuf.len; i++)
    {
        g_inputbuf.buf[i] = g_inputbuf.buf[i+k];
    }
    g_inputbuf.len -= k;
    
    g_inputbuf.buf[g_inputbuf.len] = 0;
    return (OK);
}

U8  InsertChar(String  ch)
{
    U8  i;

    for (i=g_inputbuf.len; i>g_inputbuf.pos; i--)
    {
        g_inputbuf.buf[i] = g_inputbuf.buf[i-1];
    }
    g_inputbuf.buf[g_inputbuf.pos++] =(U8 )ch;

    g_inputbuf.len++;
    return (OK);
}

extern U8  NumInput(U8  ch)
{
    ChkFirstInput();
    if (isdigit(ch) == 0)
    {
        //如果输入的不是数字,则错误
        return (FERROR);
    }
    return (CharInput(ch));
}

void ClearInputBuffer(void)
{
    g_inputbuf.len = 0;
    g_inputbuf.pos = 0;
    memset((String *)g_inputbuf.buf,0,sizeof(g_inputbuf.buf));
    SJPriorChar = 0;
    SJCharIndex = 0;
}

extern void CheckSjTimeout(void)
{
    if (SJTimeout > 0)
    {
        if (--SJTimeout == 0)
        {
            SJPriorChar = 0;
            SJCharIndex = 0;
        }
    }
}

extern void SetInputDisMode(U8 mode)
{
    t9Pinyin.disMode = mode;
}

void DisplaySelectPy(U8 flag)
{
    U16 i,k;
    U16 len;
    U8 selPos,shiftPos = 0;
    T9PY_Index tempIndex;
    U32 addr;
    U8 readBuf[16];
    
    memset(lcdDispBuf[0],' ',16);
    
    addr = t9Pinyin.t9PyAddr + t9Pinyin.mbtimes * T9PY_INDEX_LENGTH;
    SpiFlash_ReadBytes(addr,(U8 *)&t9Pinyin.headIndex.t9PY,T9PY_INDEX_LENGTH);
    if(FERROR == flag)
    {
        for(i=0;i<pyBuf.pos;i++)
        {
            lcdDispBuf[0][i] = InputChar[pyBuf.buf[i]-0x32];
        }
        lcdDispBuf[0][i] = 0;
    }
    else
    {
        if(t9Pinyin.mbtimes < (t9Pinyin.mblen-1))
        {
            SpiFlash_ReadBytes(addr+T9PY_INDEX_LENGTH,(U8 *)&tempIndex.t9PY,T9PY_INDEX_LENGTH);
            sprintf((String *)lcdDispBuf[0],"%s %s",t9Pinyin.headIndex.pyStr,
                    tempIndex.pyStr);
        }
        else
        {
            sprintf((String *)lcdDispBuf[0],"%s",t9Pinyin.headIndex.pyStr);
        }
    }  
    
    memset(lcdDispBuf[1],' ',16);
    lcdDispBuf[1][16] = 0;
    len = t9Pinyin.headIndex.strLen;
    
    if(SELECT_BD == t9Pinyin.step)
    {
        k = (t9Pinyin.hzIndex / 7) * 7; 
        len -= k;
        if(len > 7)
        {
            len = 7;
        }
        addr = t9Pinyin.headIndex.hzAddr + k + ADDR_PY_INDEX_BASE;

        SpiFlash_ReadBytes(addr,readBuf,len);
        for(i=0;i<len;i++)
        {
            lcdDispBuf[1][2*i+1] = readBuf[i];
        }

        len =  t9Pinyin.hzIndex % 7;
        lcdDispBuf[1][len*2] = 0x08;
        
        return;
    }

    k = (t9Pinyin.hzIndex / 10) * 10;                       //每行显示5个汉字
    len = (len - k) / 2;
    if(len > 5)
    {
        len = 5;
    }

    addr = t9Pinyin.headIndex.hzAddr + k + ADDR_PY_INDEX_BASE;
    SpiFlash_ReadBytes(addr,readBuf,10);

    //计算选中字符位置
    if(SELECT_HZ == t9Pinyin.step)
    {
        selPos =  (t9Pinyin.hzIndex % 10) / 2;
        
        if(selPos == 0)
        {
            shiftPos = 1;
            //插入选中字符串位置
            lcdDispBuf[1][0] = 0x08;
        }
        else
        {
            shiftPos = 0;
            //插入选中字符串位置
            lcdDispBuf[1][selPos*3 - 1] = 0x08;
        }
    }

    for(i=0;i<len;i++)
    {
        lcdDispBuf[1][3*i+shiftPos]   = readBuf[2*i];
        lcdDispBuf[1][3*i+1+shiftPos] = readBuf[2*i+1];
    } 
}

extern U8 DrowInputWindow(void)
{
    switch(t9Pinyin.disMode)
    {
        case DROW_INPUT:
            SC5260_ClearArea(30, 2, 124, 1, 1);
            break;
        case DROW_BACK:
            LCD_ClearArea(13,2,124,35);
            memset(lcdDispBuf,0x00,18*2);
            break;
        case DROW_NORMAL:
        default:
            break;
    }
    SetInputDisMode(DROW_NORMAL);

    if(pyBuf.pos != 0)
    {
        return 1;
    }
    return 0;
}

U32 Asc7B_To_Bcd4B(U8  *str)
{
    UninoU32 dest;
    U8  src[7];

    memset(src,0,7);
    dest.dest = strlen((String *)str);
    memcpy(&src[7-dest.dest],str,dest.dest);
    
    dest.src[3] = src[0]&0x0F;
    dest.src[2] = ((src[1]&0x0F)<<4) | (src[2]&0x0F);
    dest.src[1] = ((src[3]&0x0F)<<4) | (src[4]&0x0F); 
    dest.src[0] = ((src[5]&0x0F)<<4) | (src[6]&0x0F); 
    
    return (dest.dest);
}

U8  SearchPyIndex(void)
{
    U8  i,len,shift;
    U8  flag;
    U8  mblenTemp;
    T9PY_Index tempIndex;
    U32 addr;

    flag = 0x55;
    t9Pinyin.t9py = Asc7B_To_Bcd4B(pyBuf.buf);
    mblenTemp = t9Pinyin.mblen;
    t9Pinyin.mblen = 0;

    if((t9Pinyin.firstNo == ' ') || (0 == pyBuf.pos))
    {
        return FERROR;
    }

    if(0x01 == t9Pinyin.firstNo)
    {//输入的是标点直接返回
        t9Pinyin.t9PyAddr = t9PY_index_headno[t9Pinyin.firstNo];
        return OK;
    }

    addr = t9PY_index_headno[t9Pinyin.firstNo];
    for(i=0;i<t9PY_indexlen[t9Pinyin.firstNo];i++)
    {
        SpiFlash_ReadBytes(addr,(U8 *)&tempIndex.t9PY,4);
        if (t9Pinyin.t9py == tempIndex.t9PY)
        {
            t9Pinyin.mblen++;
            t9Pinyin.t9PyAddr = addr;
            flag = 0xaa;

            addr += T9PY_INDEX_LENGTH;
            for (i++;i<t9PY_indexlen[t9Pinyin.firstNo];i++)
            {
                SpiFlash_ReadBytes(addr,(U8 *)&tempIndex.t9PY,4);
                if (t9Pinyin.t9py == tempIndex.t9PY)
                {
                    t9Pinyin.mblen++;
                }
                else
                {
                    break;
                }
            	addr += T9PY_INDEX_LENGTH;
            }
            break;
        }
        addr += T9PY_INDEX_LENGTH;
    }
    if (flag == 0x55)		   
    {
        addr = t9PY_index_headno[t9Pinyin.firstNo];
        for (i=0;i<t9PY_indexlen[t9Pinyin.firstNo];i++)
        {
            SpiFlash_ReadBytes(addr,(U8 *)&tempIndex.t9PY,13);            
            len = strlen((String *)tempIndex.pyStr);

            //判断实际长度需要大于输入长度
            if(pyBuf.pos < len)
            {
                shift = 4*(len-pyBuf.pos);
                tempIndex.t9PY = tempIndex.t9PY >> shift;

                if (tempIndex.t9PY == t9Pinyin.t9py)
                {
                    t9Pinyin.mblen++;
                    if(flag == 0x55)
                    {
                        flag = 0xaa;
                        t9Pinyin.t9PyAddr = addr;
                    }
                }
                else
                {
                    if(flag == 0xaa)
                    {//从匹配的跳转的不匹配的直接结束循环
                        break;
                    }
                }
            }
            else
            {
                if(flag == 0xaa)
                {//从匹配的跳转的不匹配的直接结束循环
                    break;
                }
            }


            addr += T9PY_INDEX_LENGTH; 
        }
        if (flag == 0x55)
        {//循环后依然没有找到对应的
            t9Pinyin.t9py >>= 4;
            t9Pinyin.mblen = mblenTemp;
            return FERROR;
        }
    }

    return OK;
}

void ClearPyIndex(void)
{
    memset((U8  *)&t9Pinyin,0,sizeof(T9PY_Struct));
    t9Pinyin.firstNo = ' ';
    memset((U8  *)&pyBuf,0,sizeof(PyBuf_Struct));
    memset(lcdDispBuf,0x00,18*2);
}

U8  InputPinYin(String ch)
{
    if(0 == pyBuf.pos)
    {
        if(ch == '0')
        {//0转换为空格输入
            return (CharInput(' '));
        }
        else
        {
            t9Pinyin.firstNo = ch - 0x30;
            SetInputDisMode(DROW_INPUT);
        }
    }

    if(pyBuf.pos > 6)
    {
        return FERROR;
    }

    pyBuf.buf[pyBuf.pos++] = ch;
    pyBuf.buf[pyBuf.pos] = 0;

    if(0x01 ==  t9Pinyin.firstNo)
    {
        t9Pinyin.step = SELECT_BD;
    }
    else
    {
        t9Pinyin.step = SELECT_PY;

        if('1' == ch)
        {
            pyBuf.pos--;
            pyBuf.buf[pyBuf.pos] = 0;
            
            EnterSelectMode();    
        }
    }
	
    if(SearchPyIndex() == OK)
    {
        DisplaySelectPy(OK);
    }
    else
    {
        if(pyBuf.pos == 1)
        {
            DisplaySelectPy(FERROR);
        }
        else
        {
            pyBuf.pos--;
            pyBuf.buf[pyBuf.pos] = 0;
            return FERROR;
        }
    }
    return OK;
}

void SelectPinYin(U8  direction)
{
    if(t9Pinyin.mblen < 2)
    {
        return;
    }
    
    if(direction)
    {
        t9Pinyin.mbtimes = (t9Pinyin.mbtimes + 1) % t9Pinyin.mblen;
    }
    else
    {
        if(t9Pinyin.mbtimes > 0)
        {
            t9Pinyin.mbtimes--;
        }
        else
        {
            t9Pinyin.mbtimes = t9Pinyin.mblen - 1;
        }
    }
}
void SelectHZ(U8  direction)
{
    U8  len;

    len = t9Pinyin.headIndex.strLen; 

    if(len <= 2)
    {
        return;
    }

    if(direction)
    {
        t9Pinyin.hzIndex = (t9Pinyin.hzIndex + 2)%len;
    }
    else
    {
        if(t9Pinyin.hzIndex >= 2)
        {
            t9Pinyin.hzIndex -= 2;
        }
        else
        {
            t9Pinyin.hzIndex = len-2; 
        }
    }
}

void SelectBD(U8  direction)
{
    U8  len;

    len = t9Pinyin.headIndex.strLen;
    if(len <= 2)
    {
        return;
    }

    if(direction)
    {
        t9Pinyin.hzIndex = (t9Pinyin.hzIndex + 1)%len;
    }
    else
    {
        if(t9Pinyin.hzIndex >= 1)
        {
            t9Pinyin.hzIndex--;
        }
        else
        {
            t9Pinyin.hzIndex = len-1; 
        }
    }
}

U8  T9Select(U8  direction)
{
    ResetMenuExitTime();
    
    if(0 == pyBuf.pos)
    {
        ChangePos(direction);
        return FERROR;
    }

    if(SELECT_PY == t9Pinyin.step)
    {
        SelectPinYin(direction);
    }
    else if(SELECT_BD == t9Pinyin.step)
    {
        SelectBD(direction);
    }
    else
    {
        SelectHZ(direction);
    }
    DisplaySelectPy(OK);
    return OK;
}

void EnterSelectMode(void)
{
    U8  readBuf[3] = {0};
    U32 addr;

    ResetMenuExitTime();

    if(SELECT_PY == t9Pinyin.step)
    {
        t9Pinyin.step = SELECT_HZ;
    }
    else
    {
        addr = t9Pinyin.headIndex.hzAddr + t9Pinyin.hzIndex + ADDR_PY_INDEX_BASE;
        SpiFlash_ReadBytes(addr,readBuf,2);

        if(SELECT_HZ == t9Pinyin.step)
        {
            if(g_inputbuf.len >= (g_inputbuf.maxLen-1))
            {
                BeepOut(BEEP_ERROR);
                return;    
            }
            
            InsertChar(readBuf[0]);
            InsertChar(readBuf[1]);
        }
        else
        {
            if(g_inputbuf.len >= g_inputbuf.maxLen)
            { 
                BeepOut(BEEP_ERROR);
                return;
            }
            InsertChar(readBuf[0]);
        }
        
        ClearPyIndex();
        SetInputDisMode(DROW_BACK); 
    }
    DisplaySelectPy(OK);
    MenuShowInputChar();
}

U8  BackSpace(void)
{
    ResetMenuExitTime();
    if(0 == pyBuf.pos)
    {
        return BackSpaceChar();
    }
    else
    {
        if(SELECT_HZ == t9Pinyin.step)
        {
            t9Pinyin.step = SELECT_PY;
        }
        else
        {
            pyBuf.buf[--pyBuf.pos] = 0;
        }

        if(0 == pyBuf.pos)
        {
            ClearPyIndex();
            SetInputDisMode(DROW_BACK);
            return (OK);
        }
        DisplaySelectPy(SearchPyIndex());

        return (OK);
    }
}

