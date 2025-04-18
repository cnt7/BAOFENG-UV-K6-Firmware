#include "includes.h"

const U8 iconMenuUse[1][32] = 
{
   {
       0x00,0x00,0x00,0xF0,0x08,0x08,0x88,0xC8,0xC8,0x88,0x08,0x08,0xF0,0x00,0x00,0x00,
       0x00,0x00,0x00,0x0F,0x10,0x10,0x11,0x13,0x13,0x11,0x10,0x10,0x0F,0x00,0x00,0x00,
   }
};
/*定义主界面使用的图标******************************************************/
const U8 iconBattEmpty[] =
{
0x3E,0x22,0x7F,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,
0x63,
};//17*7
const U8 iconBatt1[] =
{
0x3E,0x22,0x7F,0x41,0x5D,0x5D,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,
0x63,
};//17*7
const U8 iconBatt2[] =
{
0x3E,0x22,0x7F,0x41,0x5D,0x5D,0x41,0x5D,0x5D,0x41,0x41,0x41,0x41,0x41,0x41,0x41,
0x63,
};//17*7
const U8 iconBatt3[] =
{
0x3E,0x22,0x7F,0x41,0x5D,0x5D,0x41,0x5D,0x5D,0x41,0x5D,0x5D,0x41,0x41,0x41,0x41,
0x63,
};//17*7
const U8 iconBattFull[] =
{
0x3E,0x22,0x7F,0x41,0x5D,0x5D,0x41,0x5D,0x5D,0x41,0x5D,0x5D,0x41,0x5D,0x5D,0x41,
0x63,
};//17*7

const U8 iconSingalTx[] =
{
0x01,0x01,0x01,0x7F,0x01,0x01,0x01,0x00,0x63,0x22,0x14,0x08,0x14,0x22,0x63,
};//15*7
const U8 iconSingalRx[] =
{
0x7F,0x09,0x09,0x09,0x19,0x29,0x46,0x00,0x63,0x22,0x14,0x08,0x14,0x22,0x63,
};//15*7
const U8 iconSingalEmpty[] =
{
0x03,0x05,0x7F,0x05,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
};//22*7

const U8 iconSingal1[] =
{
0x03,0x05,0x7F,0x05,0x03,0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
};//22*7

const U8 iconSingal2[] =
{
0x03,0x05,0x7F,0x05,0x03,0x60,0x60,0x00,0x70,0x70,0x00,0x78,0x78,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
};//22*7

const U8 iconSingal3[] =
{
0x03,0x05,0x7F,0x05,0x03,0x60,0x60,0x00,0x70,0x70,0x00,0x78,0x78,0x00,0x7C,0x7C,
0x00,0x7E,0x7E,0x00,0x00,0x00,
};//22*7

const U8 iconSingalFull[] =
{
0x03,0x05,0x7F,0x05,0x03,0x60,0x60,0x00,0x70,0x70,0x00,0x78,0x78,0x00,0x7C,0x7C,
0x00,0x7E,0x7E,0x00,0x7F,0x7F,
};//22*7

const U8 iconDTMF[]=
{
0x00,0x30,0x60,0x3F,0x01,0x02,0x00,
};//7*7

const U8 iconRight[]=
{
0x01,0x82,0x44,0x28,0x10,0x01,0x00,0x00,0x00,0x00,
};//5*9

const U8 iconPowerL[] =
{
0x7F,0x40,0x40,0x40,0x40,0x40,
};//6*7

const U8 iconPowerH[] =
{
0x7F,0x08,0x08,0x08,0x08,0x7F,
};//6*7

const U8 iconDT[] =
{
  0xFE,0x82,0x82,0x44,0x38,0x02,0x02,0xFE,0x02,0x02
};//10*8

const U8 iconDcs[] =
{
0x7F,0x41,0x41,0x41,0x3E,0x00,0x3E,0x41,0x41,0x41,0x22,0x00,0x26,0x49,0x49,0x49,
0x32,
};//17*7

const U8 iconCtcss[] =
{
0x00,0x00,0x00,0x3E,0x41,0x41,0x41,0x22,0x00,0x01,0x01,0x7F,0x01,0x01,0x00,0x00,
0x00,
};//17*7

const U8 iconAM[] =
{
0x7E,0x09,0x09,0x09,0x09,0x09,0x7E,0x00,0x7F,0x02,0x04,0x08,0x04,0x02,0x7F,
};//15*7

const U8 iconDual[] =
{
0x7F,0x41,0x41,0x41,0x3E,0x00,0x7F,0x20,0x18,0x20,0x7F,
};//11*7

const U8 iconKill[] =
{
 0x7F,0x41,0x6B,0x49,0x7F,0x41,0x7F,0x41,0x5F,0x7F,0x41,0x5F,0x7F
};//13*7

const U8 iconVox[] =
{
0x03,0x0C,0x30,0x40,0x30,0x0C,0x03,0x00,0x3E,0x41,0x41,0x41,0x41,0x41,0x3E,0x00,
0x63,0x22,0x14,0x08,0x14,0x22,0x63,
};//6*8

const U8 iconAdd[] =
{
0x18,0x18,0x7E,0x7E,0x7E,0x18,0x18,
};//7*7

const U8 iconSub[] =
{
0x18,0x18,0x18,0x18,0x18,0x18,0x18,
};//7*7

const U8 iconAddSub[] =
{
 0x90,0x90,0xFE,0x90,0x90
};//5*8

const U8 iconReverse[] =
{
0x7F,0x09,0x09,0x09,0x19,0x29,0x46,
};//7*7 R

const U8 iconUnNet[] =
{
0x01,0x01,0x01,0x7F,0x01,0x01,0x01,
};//7*7 T

const U8 iconNarrow[] =
{
0x7F,0x02,0x04,0x08,0x10,0x20,0x7F,
};//7*7 N

const U8 iconLock[] =
{
0x7C,0x46,0x45,0x45,0x45,0x45,0x46,0x7C,
};//8*7

const U8 iconScan[] =
{
 0x18,0x24,0x42,0x47,0x42,0x00,0x42,0xE2,0x42,0x24,0x18
};//11*8

const U8 iconFhss[] =
{
0x7F,0x09,0x09,0x09,0x09,0x01,0x01,
};//7*7

const U8 iconScr[] =
{
0x26,0x49,0x49,0x49,0x32,0x00,0x3E,0x41,0x41,0x41,0x22,0x00,0x7F,0x09,0x19,0x29,
0x46,
};//17*7

const U8 iconQT_DTMF[] = 
{
    0x7F,0x41,0x41,0x3E,0x00,0x01,0x01,0x7F,0x01,0x01,0x00,0x7F,0x02,0x0C,0x02,0x7F,
    0x00,0x7F,0x09,0x09,0x01,
};

const U8 iconBt[] =
{
  0x82,0x44,0x28,0xFF,0x11,0xAA,0x44,0x00,0x00,0x00,0x01,0x01,0x00,0x00
};//7*9


const U8 iconSearchFreq[] = 
{
 0x00,0xF8,0x06,0x02,0x04,0x08,0x10,0x21,0x65,0x85,0x09,0x32,0x04,0x78,
 0x60,0x70,0x6B,0x44,0x08,0x08,0x10,0x10,0x10,0x10,0x11,0x0A,0x0C,0x00
};//14*15


const U8 iconChArrow[] =
{
 0x04,0x06,0x07,0x06,0x04
};//5*3

const U8 iconArrowA[] =
{
0x7F,0x7F,0x3E,0x3E,0x1C,0x1C,0x08,
};//7*7

const U8 iconArrowB[] =
{
0x7F,0x7F,0x3E,0x3E,0x1C,0x1C,0x08,
};//7*7

const U8 iconMenuSel[] =
{
 0x01,0x83,0xC6,0x6C,0x38,0x10,0x01,0x01,0x00,0x00,0x00,0x00,
};//6*9

const U8 iconProgrom[] = 
{
 0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
 0x20,0x20,0x20,0x20,0x20,0x2E,0xEE,0x0E,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xFE,0xFE,
 0xFF,0x00,0x00,0x00,0x15,0x15,0x15,0x15,0x15,0x15,0x14,0x14,0x10,0x10,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0xD8,0xD8,0xD8,0xD8,0xD8,0xD8,0xFF,0xFF,
 0x07,0x04,0x04,0x04,0x04,0x04,0x04,0xC4,0xC4,0xE4,0xFC,0xC4,0xFC,0xE4,0xC4,0xC4,
 0x04,0x04,0x44,0xA4,0x04,0xF4,0xF7,0xF0,0xFF,0xF1,0xB5,0xF1,0xFF,0xFE,0xFF,0xFF,
 0x00,0x00,0x7C,0x6C,0x54,0x6C,0x54,0x6C,0x54,0x6C,0x54,0x6C,0x54,0x6C,0x54,0x6C,
 0x54,0x6E,0x55,0x6C,0x7C,0x00,0x00,0x70,0x58,0x70,0x00,0x00,0x00,0x00,0x00,0x00,
};//32*32

const U8 iconWorkWeather[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x38,
    0x78,0x70,0x80,0xC0,0xC0,0x60,0x6F,0x6F,0x66,0xC0,0xC0,0x80,0x20,0x70,0x38,0x10,
    0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0xE0,0x38,0x1C,0x0E,0x06,0x03,0x03,0x03,0x03,
    0x03,0x03,0x07,0x07,0x07,0x1D,0x38,0xE0,0x80,0x80,0x80,0x80,0x80,0xC3,0xFF,0x3C,
    0x00,0x18,0x18,0x18,0xFC,0xFF,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x03,0x03,
    0x0E,0xFC,0xF0,0x00,0x00,0x00,0x00,0x03,0x07,0x0E,0x0C,0x18,0x18,0x18,0x18,0x18,
    0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
    0x0C,0x0E,0x07,0x03,0x00,0x00,0x00,0x00
};


const U8 num_12X13_0[] = {
0xF0,0xFC,0xFE,0x0F,0x03,0x03,0x03,0x03,0x0F,0xFE,0xFC,0xF8,0x01,0x07,0x0F,0x1E,
0x18,0x18,0x18,0x18,0x1E,0x0F,0x07,0x03,
};
const U8 num_12X13_1[] = {
0x00,0x00,0x00,0x0C,0x0C,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x00,
};
const U8 num_12X13_2[] = {
0x04,0x0E,0x0E,0x07,0x83,0x83,0xC3,0xC3,0xFF,0x7E,0x3C,0x00,0x1C,0x1E,0x1F,0x1F,
0x1B,0x19,0x19,0x19,0x18,0x18,0x18,0x00,
};
const U8 num_12X13_3[] = {
0x04,0x06,0x06,0x67,0x63,0x63,0x63,0x63,0x73,0xFE,0xFE,0x9C,0x0C,0x0C,0x0C,0x1C,
0x18,0x18,0x18,0x18,0x1C,0x0F,0x0F,0x07,
};
const U8 num_12X13_4[] = {
0x00,0x80,0xC0,0xE0,0x70,0x38,0x1C,0x0E,0xFF,0xFF,0xFF,0x00,0x07,0x07,0x07,0x07,
0x06,0x06,0x06,0x06,0x1F,0x1F,0x1F,0x06,
};
const U8 num_12X13_5[] = {
0x3F,0x3F,0x33,0x33,0x33,0x33,0x33,0x73,0xF3,0xE3,0xC3,0x00,0x06,0x0C,0x1C,0x18,
0x18,0x18,0x18,0x1C,0x1E,0x0F,0x07,0x00,
};
const U8 num_12X13_6[] = {
0xF0,0xFC,0xFE,0xCE,0x67,0x63,0x63,0x63,0xE3,0xE7,0xCE,0x8C,0x03,0x07,0x0F,0x1C,
0x18,0x18,0x18,0x18,0x1C,0x0C,0x0F,0x07,
};
const U8 num_12X13_7[] = {
0x03,0x03,0x03,0x03,0x03,0x03,0xC3,0xE3,0xFB,0x3F,0x0F,0x07,0x00,0x00,0x10,0x18,
0x1E,0x1F,0x07,0x01,0x00,0x00,0x00,0x00,
};
const U8 num_12X13_8[] = {
0x80,0xDE,0xFE,0xF7,0x63,0x63,0x63,0x63,0xF7,0xFE,0xDE,0x80,0x07,0x0F,0x0F,0x1C,
0x18,0x18,0x18,0x18,0x1C,0x0F,0x0F,0x07,
};
const U8 num_12X13_9[] = {
0x7C,0xFE,0xEE,0xC7,0x83,0x83,0x83,0x83,0xC7,0xEE,0xFC,0xF8,0x04,0x0C,0x1D,0x19,
0x19,0x19,0x19,0x1D,0x0C,0x0F,0x07,0x01,
};
const U8 num_12X13_Point[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x00,0x00,
};
const U8 num_12X13_c[] = {
0xFC,0xFE,0xFF,0x07,0x03,0x03,0x03,0x03,0x07,0x06,0x1E,0x1C,0x07,0x0F,0x1F,0x1C,
0x18,0x18,0x18,0x18,0x1C,0x0C,0x0F,0x07,
};
const U8 num_12X13_h[] = {
0xFF,0xFF,0xFF,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xFF,0xFF,0xFF,0x1F,0x1F,0x1F,0x00,
0x00,0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,
};
const U8 num_12X13_heng[] = {
0x00,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
const U8 *iconNumTable_12X13[] =
{
    num_12X13_0, num_12X13_1, num_12X13_2, num_12X13_3, num_12X13_4,
  	num_12X13_5, num_12X13_6, num_12X13_7, num_12X13_8, num_12X13_9, 
    num_12X13_Point, num_12X13_c, num_12X13_h, num_12X13_heng
};


const U8 num_6X7_0[] = {
0x3E,0x41,0x41,0x41,0x41,0x3E,
};
const U8 num_6X7_1[] = {
0x00,0x00,0x42,0x7F,0x40,0x00,
};
const U8 num_6X7_2[] = {
0x62,0x51,0x51,0x49,0x49,0x46,
};
const U8 num_6X7_3[] = {
0x22,0x41,0x49,0x49,0x4D,0x32,
};
const U8 num_6X7_4[] = {
0x18,0x14,0x12,0x11,0x7F,0x10,
};
const U8 num_6X7_5[] = {
0x27,0x45,0x45,0x45,0x45,0x39,
};
const U8 num_6X7_6[] = {
0x3E,0x49,0x49,0x49,0x49,0x30,
};
const U8 num_6X7_7[] = {
0x00,0x01,0x71,0x09,0x05,0x03,
};
const U8 num_6X7_8[] = {
0x36,0x49,0x49,0x49,0x49,0x36,
};
const U8 num_6X7_9[] = {
0x46,0x49,0x49,0x49,0x29,0x1E,
};
const U8 num_6X7_m[] = {
0x7F,0x02,0x04,0x18,0x04,0x02,0x7F,
};
const U8 num_6X7_v[] = {
0x03,0x0C,0x30,0x40,0x30,0x0C,0x03,
};
const U8 num_6X7_f[] = {
0x7F,0x09,0x09,0x09,0x09,0x01,0x01,
};
const U8 num_6X7_o[] = {
0x3E,0x41,0x41,0x41,0x41,0x41,0x3E,
};
const U8 num_6X7_heng[] = {
0x08,0x08,0x08,0x08,0x08,0x08,
};

const U8 num_6X7_point[] = {
0x00,0x00,0x30,0x30,0x00,0x00,
};

const U8 *iconNumTable_6X7[] =
{
    num_6X7_0, num_6X7_1, num_6X7_2, num_6X7_3, num_6X7_4,
  	num_6X7_5, num_6X7_6, num_6X7_7, num_6X7_8, num_6X7_9, 
    num_6X7_m, num_6X7_v, num_6X7_f, num_6X7_o, num_6X7_heng,
    num_6X7_point
};
