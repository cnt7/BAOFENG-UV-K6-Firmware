#include "includes.h"
#include "i2c.h"

#define     YACK                    0
#define     NACK                    1

U8 RDA5807_BUFF[8] = {
    0xD2,0x65,    // 02H
    0x00,0x00,    // 03H
    0x04,0x00,    // 04H   
    0x88,0x8f,    // 05H
};

U8 RDA5807_SeriesWrite(U8 num)
{
    U8 i,Acknowledge;
    
    I2C_Start();
    Acknowledge = I2C_Write(0x20);
    if(Acknowledge == YACK)
    {
        for(i = 0;i < num;i++)
        {
            Acknowledge = I2C_Write(RDA5807_BUFF[i]);
            if(Acknowledge == NACK)
            {
                break;
            }
        }
    }
    
    I2C_Stop();
    return(Acknowledge);
}

U8 RDA5802E_SeriesRead(void)
{
    U8 Acknowledge;
    I2C_Start();
    Acknowledge = I2C_Write(0x21);
    if( Acknowledge== YACK)
    {
        I2C_ReadBuffer(RDA5807_BUFF,4);
    }

    I2C_Stop();
    return(Acknowledge);
}


extern void RDA5807_PowerOn(void)
{

}

extern void RDA5807_PowerOff(void)
{
    RDA5807_BUFF[0] &= 0xBF;
    RDA5807_BUFF[1] &= 0xFE;
    RDA5807_SeriesWrite(2);
}

void RDA5807_FrequencyToTune(void)
{
    U16 FM_frequency;
    
    FM_frequency = fmInfo.freq-650;
    RDA5807_BUFF[0] = 0xD2;
    RDA5807_BUFF[1] = 0x65;

    if(FM_frequency >= 110)                    // 判断频率范围  76M~108M / 0.1M
    {
        RDA5807_BUFF[3] = 0x18;
        FM_frequency -= 110;
        fmInfo.band = 0;
    }
    else                                      // 65M~76M
    {
        RDA5807_BUFF[3] = 0x1C;               // Band Select
        fmInfo.band = 1;
    }

    RDA5807_BUFF[2] = (FM_frequency >> 2)&0xFF;
    RDA5807_BUFF[3] |= ((FM_frequency<<6)&0xFF);
    RDA5807_SeriesWrite(8);    
    SpeakerSwitch(ON);
}


void RDA5807_Seek(void)
{
    RDA5807_BUFF[0] = 0xD3;               // 
    RDA5807_BUFF[1] = 0x65;
    RDA5807_SeriesWrite(2);
}

U8 RDA5807_STC(void)
{
    U8 FalseStation = 0;
    U16 tempFreq = 760;
    
    if(RDA5802E_SeriesRead()== YACK)
    {
 
        if((RDA5807_BUFF[0]&0x40)==0)             // STC
        {
            return FalseStation;
        }
        
        FalseStation = 1;
        if(RDA5807_BUFF[2]&0x01)           // FM_TURE
        {
            //切换频段
            if(fmInfo.band)
            {
                tempFreq = 650;
            }
        
            FalseStation = 2;
            
            fmInfo.freq = (RDA5807_BUFF[0]&0x03)*256;  
            fmInfo.freq = (fmInfo.freq+RDA5807_BUFF[1])+tempFreq;
            return FalseStation; 
        }
    }
    return FalseStation;
}


