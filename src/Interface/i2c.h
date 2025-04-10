#ifndef DRIVER_I2C_H
    #define DRIVER_I2C_H

enum {
	I2C_WRITE = 0U,
	I2C_READ = 1U,
};

void I2C_Start(void);
void I2C_Stop(void);

uint8_t I2C_Read(bool bFinal);
uint8_t I2C_Write(uint8_t Data);

uint8_t I2C_ReadBuffer(void *pBuffer, uint8_t Size);
uint8_t I2C_WriteBuffer(const void *pBuffer, uint8_t Size);

#endif

