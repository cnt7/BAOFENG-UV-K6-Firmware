#include <stdint.h>

extern uint16_t CRC_ValidationCalc(uint8_t *buf,uint16_t len)
{
    uint16_t CrcCnt,CrcRes = 0,CrcTemp;
    uint8_t CrcI;

    for(CrcCnt = 0; CrcCnt < len; CrcCnt++)
    {
        CrcTemp = buf[CrcCnt];

        CrcI = 8;
        CrcRes = CrcRes^(CrcTemp<<8);

        for(CrcI = 0; CrcI < 8; CrcI++)
        {
            if(CrcRes & 0x8000)
            {
                CrcRes = (CrcRes<<1) ^ 0x1021;
            }
            else
            {
                CrcRes = CrcRes<< 1;
            }
        }
    }

    return CrcRes;
}

