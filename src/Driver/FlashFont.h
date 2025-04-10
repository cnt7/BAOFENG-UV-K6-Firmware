#ifndef _FLASHFONT_H
    #define _FLASHFONT_H

#define FONT_12X24NUM_ADDR   (0x070000 + FLASH_FNT_BASIC_ADDR)
#define ADDR_PY_INDEX_BASE   (0x078400 + FLASH_FNT_BASIC_ADDR)

#define FONTSIZE_12x12    12
#define FONTSIZE_16x16    16
/*******************************************************************************/
extern void Font_Read_16x16_CN( uint8_t *pString, uint8_t *pdat );
extern void Font_Read_12x12_CN( uint8_t *pString, uint8_t *pdat );
extern void Font_Read_8x16_ASCII( uint8_t *pString, uint8_t *pdat );
extern void Font_Read_6x12_ASCII( uint8_t *pString, uint8_t *pdat );
extern void Font_Read_5x7_ASCII( uint8_t *pString, uint8_t *pdat );
#endif
