#ifndef __SC5260_H
    #define __SC5260_H

extern U8 gLcdBuffer[8][128];

extern void SC5260_Init(void);
extern void SC5620_SetContpastRatio(U8 level);

extern void SC5260_DisplaySmallArea( U8 posY, U8 posX, U8 length,U8 wide,const U8 *pdat, U8 flagClear,U8 flagInvert);
extern void SC5260_DisplayArea( U8 posY, U8 posX, U8 length,U8 wide,const U8 *pdat,U8 flagInvert);
extern void SC5260_ClearArea( U8 posY, U8 posX, U8 length,U8 wide,U8 fillData);

extern void LCD_UpdateFullScreen(void);
extern void LCD_UpdateStateBar(void);
extern void LCD_UpdateWorkAre(void);

extern void LCD_ClearStateBar(void);
extern void LCD_ClearWorkArea(void);
extern void LCD_ClearFullBuf(void);

#endif
