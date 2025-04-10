#include "includes.h"

/***************************   IO 接口  *****************************/
#define LCD_CS_H                GPIOB->BSRR = GPIO_Pin_12
#define LCD_CS_L                GPIOB->BRR  = GPIO_Pin_12

#define LCD_RS_H                GPIOC->BSRR = GPIO_Pin_15
#define LCD_RS_L                GPIOC->BRR  = GPIO_Pin_15

#define LCD_RST_H               GPIOB->BSRR = GPIO_Pin_8
#define LCD_RST_L               GPIOB->BRR  = GPIO_Pin_8
/*********************************************************************/
U8 gLcdBuffer[8][128];
/*********************************************************************/
void SC5260_delay( U8  i )
{
    while( i-- )
    {
        // __nop();
        // __nop();
		__ASM volatile("nop");
		__ASM volatile("nop");
	}
}

void SC5260_writeCmd( U8  dat )
{
    LCD_CS_L;
	LCD_RS_L;
    SC5260_delay(10);
    
    while(RESET == SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
    SPI_SendData8(SPI2, dat);
    while(SET == SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY)); 
    SC5260_delay(10);
    LCD_CS_H;
	SC5260_delay(10);
}	

void SC5260_writeDat( U8  dat )
{
    while(RESET == SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
    SPI_SendData8(SPI2, dat);
    while(SET == SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY));    
}

void SC5260_SetStartPosition( U8 Line, U8 column )
{
    SC5260_writeCmd( 0xB0 | (Line & 0x0f) );
    SC5260_writeCmd( 0x10 | ((column >> 4)&0x0f) );
	SC5260_writeCmd( column & 0x0f );
}

extern void SC5260_FillScreen(U8 value)
{
    int i,j;

    for( i = 0; i < 8; i++ )
	{
        SC5260_SetStartPosition(i,0);

        LCD_CS_L;
    	LCD_RS_H;
    	SC5260_delay(10);
		for( j = 0; j < 132; j++ )
		{
            SC5260_writeDat( value );
		}
	}
	SC5260_delay(10);
    LCD_CS_H;
}

extern void SC5260_Init(void)
{
    LCD_RST_L;
    DelayMs(10);
    LCD_RST_H;

    DelayMs(100);
    SC5260_writeCmd( 0xE2 );
    DelayMs(100);

    SC5260_writeCmd( 0x2c );
	DelayMs(10);
	SC5260_writeCmd( 0x2e );
	DelayMs(10);
	SC5260_writeCmd( 0x2f );
	DelayMs(10);

	SC5260_writeCmd( 0x23 );
	SC5260_writeCmd( 0x81 );  
	SC5260_writeCmd( 0x26 );
    SC5260_writeCmd( 0xA2 );
    SC5260_writeCmd( 0xA1 ); 
    SC5260_writeCmd( 0xC0 );       
	SC5260_writeCmd( 0x40 );     
	SC5260_writeCmd( 0xAE );  

    SC5260_FillScreen(0x00);

	SC5260_writeCmd( 0xAF );
}

//设置液晶对比度
extern void SC5620_SetContpastRatio(U8 level)
{
    const U8 setLevel[5] = {37,41,45,48,51};

    if(level > 4)
    {
        level = 4;
    }

    SC5260_writeCmd( 0x81 );  
    SC5260_writeCmd( setLevel[level] ); // 对比度细调
}


void SC5260_DisplaySmallArea( U8 posY, U8 posX, U8 length,U8 wide,const U8 *pdat, U8 flagClear,U8 flagInvert)
{
    U8 maskLowerBits1 = 0x00;
	U8 maskHigherBits1 = 0x00;
	U8 maskLowerBits2 = 0x00;
	U8 maskHigherBits2 = 0x00;
    U8 remainRows1 = 0;
	U8 remainRows2 = 0;
	U8 curPage = 0;
    U8 bufDat1 = 0;
	U8 bufDat2 = 0;
	U8 bufDat3 = 0;
    U8 i,j;

	if( wide == 0 || length == 0 )
	{
        return;
	}
    
    remainRows1 = posY % 8;
	curPage = posY / 8;
	maskLowerBits1 =  ( 1 << remainRows1 ) - 1;
	maskHigherBits1 = ( 1 << (8 - remainRows1)) - 1;

    for( i = 0; i < length; i++ )
    {
        if( flagClear == 0 )
        {
            bufDat3 = pdat[i];
		}
		else
		{
            bufDat3 = 0x00;
		}

		if( flagInvert == 1 )
		{
            bufDat3 =~ bufDat3;
		}

		if(posX+i == 128)
		{
		    return;
		}
		
	    if(wide <  (8-remainRows1))
		{
		    bufDat1 = gLcdBuffer[curPage][posX+i];

		    for(j = 0;j<wide;j++)
		    {
		        bufDat1 &= ~(1 << (j + remainRows1));
		    }
		}
		else
		{
            bufDat1 = gLcdBuffer[curPage][posX+i] & maskLowerBits1 ;
		}
        
		bufDat2 = (bufDat3 & maskHigherBits1);
		if( wide < (8-remainRows1) )
		{
           bufDat2 = bufDat2 & ((1 << wide) - 1);
		}
		bufDat2 <<= remainRows1;
		
		gLcdBuffer[curPage][posX+i] = bufDat1 | bufDat2;
		
		if( wide > (8-remainRows1) && (curPage < 7))
		{
            remainRows2 = wide - (8-remainRows1);
			maskLowerBits2 = (1 << remainRows2) - 1;
			maskHigherBits2 = (1 << wide) - 1;
			bufDat1 = (bufDat3 & maskHigherBits2) >> (8 - remainRows1);
			bufDat2 = gLcdBuffer[curPage+1][posX+i] & (~maskLowerBits2);
			gLcdBuffer[curPage+1][posX+i] = bufDat1 | bufDat2;						
		}
	}
}

void SC5260_ClearArea( U8 posY, U8 posX, U8 length,U8 wide,U8 fillData)
{
    U8 i = 0;
    U8 *pdat;
    

	if( wide == 0 || length == 0 )
	{
        return;
	}

	for( i = wide / 8 + 1; i > 0; i-- )
	{
        if( i > 1 )
        {
            SC5260_DisplaySmallArea( posY, posX, length, 8, pdat, 1 , fillData);
		}
		else
		{
            SC5260_DisplaySmallArea( posY, posX, length, wide % 8,  pdat, 1 , fillData);
		}

		posY += 8;
	}
}


void SC5260_DisplayArea( U8 posY, U8 posX, U8 length,U8 wide,const U8 *pdat,U8 flagInvert)
{
    U8 i = 0;
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

//全屏更新
extern void LCD_UpdateFullScreen(void)
{
    U8 i,j;

	for( i = 0; i < 8; i++ )
	{
	    SC5260_SetStartPosition(i, 4);

	    LCD_CS_L;
    	LCD_RS_H;
    	SC5260_delay(10);
        for( j = 0; j < 128; j++ )
        {
     	    if(g_radioInform.DisplayStyles)
     	    {    
     	        SC5260_writeDat(~gLcdBuffer[i][j] );
     	    }
     	    else
     	    {
     	        SC5260_writeDat(gLcdBuffer[i][j] );
     	    } 
		}
	}
	SC5260_delay(10);
    LCD_CS_H;
}

//更新状态栏
extern void LCD_UpdateStateBar(void)
{
    U8 j;
    
    SC5260_SetStartPosition(0, 4);

    LCD_CS_L;
	LCD_RS_H;
	SC5260_delay(10);
    for( j = 0; j < 128; j++ )
    {
 	    if(g_radioInform.DisplayStyles)
 	    {    
 	        SC5260_writeDat(~gLcdBuffer[0][j] );
 	    }
 	    else
 	    {
 	        SC5260_writeDat(gLcdBuffer[0][j] );
 	    } 
	}
	SC5260_delay(10);
    LCD_CS_H;
}

//更新工作区域
extern void LCD_UpdateWorkAre(void)
{
    U8 i,j;

	for( i = 1; i < 8; i++ )
	{
	    SC5260_SetStartPosition(i, 4);

	    LCD_CS_L;
    	LCD_RS_H;
    	SC5260_delay(10);
        for( j = 0; j < 128; j++ )
        {
     	    if(g_radioInform.DisplayStyles)
     	    {    
     	        SC5260_writeDat(~gLcdBuffer[i][j] );
     	    }
     	    else
     	    {
     	        SC5260_writeDat(gLcdBuffer[i][j] );
     	    } 
		}
	}
	SC5260_delay(10);
    LCD_CS_H;
}

extern void LCD_ClearStateBar(void)
{
    memset(gLcdBuffer,0x00,128);
}

extern void LCD_ClearWorkArea(void)
{
    memset(&gLcdBuffer[1][0],0x00,128*7);
}

//清除液晶缓存
extern void LCD_ClearFullBuf(void)
{
    memset(gLcdBuffer,0x00,sizeof(gLcdBuffer));
}

