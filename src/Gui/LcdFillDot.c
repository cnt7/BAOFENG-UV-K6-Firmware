#include "includes.h"

void LCD_DisplayText(U8 posY, U8 posX,  U8 *pString,U8 fontSize,U8 flagInvert)
{
    U8 i = 0;
	U8 bufDat[64];
    U8 changeFlag = 0;
    U16 revBackFlag;

    revBackFlag = flagInvert;

	for( i = 0; ; )
	{
        if(pString[i] == '\0' )
        {
            return;
		}
		
        if(pString[i] >= 0xA1)
        {
            if( fontSize == FONTSIZE_16x16 )
            {
				Font_Read_16x16_CN(pString+i, bufDat);
			}
			else if( fontSize == FONTSIZE_12x12 )
			{
                Font_Read_12x12_CN(pString+i, bufDat);
			}
            SC5260_DisplayArea( posY, posX, fontSize, fontSize, bufDat, flagInvert);
            
			posX += fontSize;
			i += 2;
            if(changeFlag == 1)
            {
                flagInvert = revBackFlag;
                changeFlag = 0;
            }
		}
	    else if(pString[i] == 0x08)
	    {
	        if(i != 0)
	        {
                SC5260_ClearArea( posY, posX, fontSize>>1, fontSize, 0);
                posX += (fontSize>>1);
            }
	        
	        changeFlag = 1;
	        flagInvert = 1;
            
	        i += 1;
	    }
		else
		{
            if( pString[i] >= 0x20 && pString[i] <= 0x7E )
            {
                if( fontSize == FONTSIZE_16x16)
                {
				    Font_Read_8x16_ASCII(pString+i, bufDat);

				}
				else if( fontSize == FONTSIZE_12x12 )
				{
                    Font_Read_6x12_ASCII(pString+i, bufDat);
				}
			    
                SC5260_DisplayArea( posY, posX, fontSize>>1, fontSize, bufDat, flagInvert);

				posX += (fontSize>>1);	
			}
			i += 1;

            if(changeFlag == 1)
            {
                flagInvert = revBackFlag;
                changeFlag = 0;
            }
		}
	}
}

void LCD_DisplayNumber( U8 posY, U8 posX, U8 *pString,U8 flagInvert)
{
    U8 i = 0;
	U8 bufDat[5];

	for( i = 0; ; )
	{
        if(pString[i] == '\0' )
        {
            return;
		}
		
        Font_Read_5x7_ASCII(pString+i, bufDat);
        SC5260_DisplayArea( posY, posX, 5, 8, bufDat ,flagInvert);
        
		posX += 6;
		i += 1;
	}
}

void LCD_DrawRectangle( U8 posY, U8 posX, U8 length, U8 wide, U8 flagFill )
{
    SC5260_ClearArea(posY, posX, length, wide, flagFill);
    if( flagFill == 0 )
    {
        if( length > 2 && wide > 2 )
        {
            SC5260_ClearArea( posY+1, posX+1, length-2, wide-2, 0);
        }
    }
}

void LCD_DisplayPicture( U8  posY, 
                         U8  posX, 
                         U8  length,
                         U8  wide,
                         const U8  *pdat,
                         U8  flagInvert)
{
    U8  i = 0;
    U16 j = 0;
    

	if( wide == 0 || length == 0 )
	{
        return;
	}

    j = 0;
	for( i = wide / 8 + 1; i > 0; i-- )
	{
        if( i > 1 )
        {
            SC5260_DisplaySmallArea( posY, posX, length, 8, &pdat[j], 0, flagInvert);
		}
		else
		{
            SC5260_DisplaySmallArea( posY, posX, length, wide % 8,  &pdat[j], 0, flagInvert);
		}

		posY += 8;
		j += length;
	}
}


void LCD_DisplayBoldNum6X7( U8 posY, U8 posX, U8 *pString)
{
    U8 bufDat[7] = {0};
    U8 i,wide;

    for( i = 0; ; )
    {
        if(pString[i] == '\0' )
        {
            return;
        }
        
        wide = 6;
        if( pString[i] >= '0' && pString[i] <= '9' )
        {
            memcpy( bufDat, iconNumTable_6X7[pString[i]-0x30], 6);
        }
        else if( pString[i] == 'M' )
        {
            memcpy( bufDat, iconNumTable_6X7[10], 7);
            wide = 7;
        }
        else if( pString[i] == 'V' )
        {
            memcpy( bufDat, iconNumTable_6X7[11], 7);
            wide = 7;
        }
        else if( pString[i] == 'F' )
        {
            memcpy( bufDat, iconNumTable_6X7[12], 7);
            wide = 7;
        }
        else if( pString[i] == 'O' )
        {
            memcpy( bufDat, iconNumTable_6X7[13], 7);
            wide = 7;
        }
        else if( pString[i] == '-' )
        {
            memcpy( bufDat, iconNumTable_6X7[14], 6);
            wide = 6;
        }
        else if( pString[i] == '.' )
        {
            memcpy( bufDat, iconNumTable_6X7[15], 6);
  			wide = 6;
        }
        else
        {//清除显示
            memcpy( bufDat, iconNumTable_6X7[0], 6);
            wide = 6;
        }
        
        SC5260_DisplayArea( posY, posX, wide, 7, bufDat, 0);
        posX += wide;
        
        LCD_ClearArea( posY, posX, 1, 7);
        posX += 1;
        i++;
    }
}

void LCD_DisplayBoldNum12X13( U8 posY, U8 posX, U8 *pString)
{
    U8 bufDat[24] = {0};
    U8 i,wide;

    for( i = 0; ; )
    {
        if(pString[i] == '\0' )
        {
            return;
        }

        wide = 12;
        if( pString[i] >= '0' && pString[i] <= '9' )
        {
            memcpy( bufDat, iconNumTable_12X13[pString[i]-0x30], 24);
    	}
    	else if( pString[i] == '.' )
     	{
    	    memcpy( bufDat, iconNumTable_12X13[10], 12);
    	    wide = 6;
    	}
    	else if( pString[i] == 'C' )
     	{
    	    memcpy( bufDat, iconNumTable_12X13[11], 24);
    	}
    	else if( pString[i] == 'H' )
    	{
    	    memcpy( bufDat, iconNumTable_12X13[12], 24);
    	}
        else if( pString[i] == '-' )
        {
            memcpy( bufDat, iconNumTable_12X13[13], 24);
        }
    	else
    	{//清除显示
    	    memset(bufDat,0x00,24);
    	}

        SC5260_DisplayArea( posY, posX, wide, 13, bufDat, 0);
		posX += wide;
	
        LCD_ClearArea( posY, posX, 1, 13);
        posX += 1;
        i++;
	}
}



