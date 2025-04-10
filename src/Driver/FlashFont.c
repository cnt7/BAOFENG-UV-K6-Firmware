#include "includes.h"
#include "NorFlash.h"

/*********************************************************************/
void Font_Read_16x16_CN( uint8_t  *pString, uint8_t  *pdat )
{
    uint16_t MSB;
	uint16_t LSB;
	uint32_t addr = 0x010000;

	MSB = pString[0];
	LSB = pString[1];

	if(MSB >= 0xA4 && MSB <= 0XA8 && LSB >= 0XA1)
	{
        addr += 0;
	}
	else if(MSB >= 0XA1 && MSB <= 0XA9 && LSB >= 0XA1)
	{
        addr += ((MSB - 0XA1) * 94 + (LSB - 0XA1)) << 5;
	}
	else if(MSB >= 0XB0 && MSB <= 0XF7 && LSB >= 0XA1)
	{
        addr += ((MSB - 0XB0) * 94 + (LSB - 0XA1) + 846) << 5;
	}

	SpiFlash_ReadBytes( addr, pdat , 32);
}

void Font_Read_12x12_CN( uint8_t  *pString, uint8_t  *pdat )
{
    uint16_t MSB;
	uint16_t LSB;
	uint32_t addr = 0X04CF80;

	MSB = pString[0];
	LSB = pString[1];

	if(MSB >= 0xA1 && MSB <= 0XA3 && LSB >= 0XA1)
	{
        addr = ( (MSB - 0XA1) * 94 + (LSB - 0XA1)) * 24 + addr;
	}
	else if(MSB == 0XA9 && LSB >= 0XA1)
	{
        addr = (282 + (LSB - 0XA1)) * 24 + addr;
	}
	else if(MSB >= 0XB0 && MSB <= 0XF7 && LSB >= 0XA1)
	{
        addr = ((MSB - 0XB0) * 94 + (LSB - 0XA1) + 376) * 24 + addr;
	}

	SpiFlash_ReadBytes( addr, pdat, 24 );
}


void Font_Read_8x16_ASCII( uint8_t  *pString, uint8_t  *pdat )
{
    uint16_t ASCIICode;
	uint32_t addr = 0x04B7C0;

	ASCIICode = pString[0];
	if( ASCIICode >= 0x20 && ASCIICode <= 0x7E )
	{
        addr = addr + ((ASCIICode - 0x20) << 4);
	}

	SpiFlash_ReadBytes( addr, pdat, 16);
}

void Font_Read_6x12_ASCII( uint8_t  *pString, uint8_t  *pdat )
{
    uint16_t ASCIICode;
	uint32_t addr = 0x076d40;

	ASCIICode = pString[0];

	if( ASCIICode >= 0x20 && ASCIICode <= 0x7E )
	{
        addr = addr + ((ASCIICode - 0x20) * 12);
	}

	SpiFlash_ReadBytes( addr, pdat, 12 );
}


void Font_Read_5x7_ASCII( uint8_t  *pString, uint8_t  *pdat )
{
    uint16_t ASCIICode;
	uint32_t addr = 0x04bfc0;

	ASCIICode = pString[0];

	if( ASCIICode >= 0x20 && ASCIICode <= 0x7E )
	{
        addr = addr + ((ASCIICode - 0x20) << 3);
	}

	SpiFlash_ReadBytes( addr, pdat, 5 );
}

