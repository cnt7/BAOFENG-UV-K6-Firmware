#include "includes.h"

#define GPIOA_PIN_I2C_SCL  GPIO_Pin_4 
#define GPIOA_PIN_I2C_SDA  GPIO_Pin_12 

/******************************************************************************************/
void I2C_Start(void)
{
	GPIOA->BSRR = GPIOA_PIN_I2C_SDA;
	DelayUs(10);
	GPIOA->BSRR = GPIOA_PIN_I2C_SCL;
	DelayUs(10);
	GPIOA->BRR = GPIOA_PIN_I2C_SDA;
	DelayUs(10);
	GPIOA->BRR = GPIOA_PIN_I2C_SCL;
	DelayUs(10);
}

void I2C_Stop(void)
{
	GPIOA->BRR = GPIOA_PIN_I2C_SCL;
	DelayUs(10);
	GPIOA->BRR = GPIOA_PIN_I2C_SDA;
	DelayUs(10);
	GPIOA->BSRR = GPIOA_PIN_I2C_SCL;
	DelayUs(10);
	GPIOA->BSRR = GPIOA_PIN_I2C_SDA;
	DelayUs(10);
	GPIOA->BRR = GPIOA_PIN_I2C_SCL;
	GPIOA->BRR = GPIOA_PIN_I2C_SDA;
}

uint8_t I2C_Read(bool bFinal)
{
	uint8_t i, Data;

	GpioModeSwitch( GPIOA,GPIOA_PIN_I2C_SDA,0);
    DelayUs(20);
	Data = 0;
	for (i = 0; i < 8; i++)
	{
		GPIOA->BSRR = GPIOA_PIN_I2C_SCL;
		DelayUs(10);
		Data <<= 1;
		if (GPIOA->IDR & GPIOA_PIN_I2C_SDA)
		{
			Data |= 1U;
		}
		GPIOA->BRR = GPIOA_PIN_I2C_SCL;
		DelayUs(10);
	}

	GpioModeSwitch( GPIOA,GPIOA_PIN_I2C_SDA,1);
	DelayUs(20);
	if (bFinal)
	{
		GPIOA->BSRR = GPIOA_PIN_I2C_SDA;
	}
	else
	{
		GPIOA->BRR = GPIOA_PIN_I2C_SDA;
	}
	DelayUs(10);
	GPIOA->BSRR = GPIOA_PIN_I2C_SCL;
	DelayUs(10);
	GPIOA->BRR = GPIOA_PIN_I2C_SCL;
	DelayUs(10);

	return Data;
}

uint8_t I2C_Write(uint8_t Data)
{
	uint8_t i;
	uint8_t ret = 1;

	GPIOA->BRR = GPIOA_PIN_I2C_SCL;
	DelayUs(10);
	for (i = 0; i < 8; i++)
	{
		if ((Data & 0x80) == 0)
		{
			GPIOA->BRR = GPIOA_PIN_I2C_SDA;
		}
		else
		{
			GPIOA->BSRR = GPIOA_PIN_I2C_SDA;
		}
		Data <<= 1;
		DelayUs(10);
		GPIOA->BSRR = GPIOA_PIN_I2C_SCL;
		DelayUs(10);
		GPIOA->BRR = GPIOA_PIN_I2C_SCL;
		DelayUs(10);
	}
    GPIOA->BRR = GPIOA_PIN_I2C_SDA;
    DelayUs(10);
	GpioModeSwitch( GPIOA,GPIOA_PIN_I2C_SDA,0);
	DelayUs(10);
	GPIOA->BSRR = GPIOA_PIN_I2C_SCL;
	DelayUs(10);

	for (i = 0; i < 100; i++)
	{
		if ((GPIOA->IDR & GPIOA_PIN_I2C_SDA) == 0)
		{
			ret = 0;
			break;
		}
	}
	GPIOA->BRR = GPIOA_PIN_I2C_SCL;
	DelayUs(10);
	GpioModeSwitch( GPIOA,GPIOA_PIN_I2C_SDA,1);
	GPIOA->BSRR = GPIOA_PIN_I2C_SDA;

	return ret;
}

uint8_t I2C_ReadBuffer(void *pBuffer, uint8_t Size)
{
	uint8_t *pData = (uint8_t*) pBuffer;
	uint8_t i;

	if (Size == 1)
	{
		*pData = I2C_Read(TRUE);
		return 1;
	}

	for (i = 0; i < Size - 1; i++)
	{
		DelayUs(10);
		pData[i] = I2C_Read(FALSE);
	}

	DelayUs(10);
	pData[i++] = I2C_Read(TRUE);

	return Size;
}

uint8_t I2C_WriteBuffer(const void *pBuffer, uint8_t Size)
{
	const uint8_t *pData = (const uint8_t*) pBuffer;
	uint8_t i;

	for (i = 0; i < Size; i++)
	{
		if (I2C_Write(*pData++) == 1)
		{
			return 1;
		}
	}

	return 0;
}

