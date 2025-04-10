#ifndef __LCDFILLDOT_H
    #define __LCDFILLDOT_H

enum
{
    LCD_DIS_NORMAL=0, LCD_DIS_INVERT
};

#define LCD_ClearArea(y,x,len,high)     SC5260_ClearArea(y,x,len,high,0)

extern void LCD_DisplayText( U8 posY, U8 posX, U8 *pString,U8 fontSize,U8 flagInvert);
extern void LCD_DisplayNumber( U8 posY, U8 posX, U8 *pString,U8 flagInvert);                      
extern void LCD_DrawRectangle( U8 posY, U8 posX, U8 length, U8 hight, U8 flagFill );
extern void LCD_DisplayPicture( U8 posY, U8 posX, U8 length,U8 wide,const U8 *pdat,U8 flagInvert);
extern void LCD_DisplayBoldNum6X7( U8 posY, U8 posX, U8 *pString);
extern void LCD_DisplayBoldNum12X13( U8 posY, U8 posX, U8 *pString);

#endif
