#ifndef __DISPLAYINPUTBOX_H
    #define __DISPLAYINPUTBOX_H

#define T9PY_INDEX_LENGTH                    16 //所以存储为16个字节

#define IN_EN_L        0
#define IN_EN_U        1
#define IN_NUMBER      2
#define IN_PINYIN      3
#define IME_MAXMODE    4
#define IME_ENMAX      3

extern U16 SJCharIndex,SJPriorChar,SJTimeout;

typedef struct
{
    U32 t9PY;
    U8  pyStr[9];   
    U16 hzAddr;    
    U8  strLen;     
}__attribute__( ( packed ) ) T9PY_Index;

//--------------输入阶段设置----------------------------------------------------
#define SELECT_PY         0
#define SELECT_HZ         1 
#define SELECT_BD         2 

//--------------定义显示模式----------------------------------------------------
#define DROW_NORMAL       0 
#define DROW_INPUT        1 
#define DROW_BACK         2 

typedef struct
{
    U8  disMode; 
    U8  step;    
	U8  firstNo;   
	U8  hzIndex;  
	U8  mblen;      
	U8  mbtimes;       
	U32 t9py;		  
    U32 t9PyAddr; 
    T9PY_Index headIndex;
}__attribute__( ( packed ) )T9PY_Struct;

typedef struct
{
    U8 pos;
    U8 buf[7];
}PyBuf_Struct;

extern PyBuf_Struct pyBuf;
extern String lcdDispBuf[2][18]; 
//-------------------------------------------------------------------------------------------
extern void ChkFirstInput(void);
extern U8 CharInput(String ch);
extern U8 NumToChar(U8 ch);
extern U8 NumInput(U8 ch);
extern void ClearInputBuffer(void);
extern U8 InsertChar(String  ch);
extern void ChangePos(U8 flag);
extern void ChangeInputType(void);
extern U8 BackSpaceChar(void);
extern void CheckSjTimeout(void);
extern void ClearPyIndex(void);
extern void SetInputDisMode(U8 mode);
extern U8 BackSpace(void);
extern U8 InputPinYin(String ch);
extern void EnterSelectMode(void);
extern U8 T9Select(U8 direction);
extern U8 DrowInputWindow(void);

#endif
