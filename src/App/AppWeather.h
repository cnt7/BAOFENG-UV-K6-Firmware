#ifndef __APPWEATHER_H
    #define __APPWEATHER_H

extern STR_FREQINFO Weatherfreq;
extern const U32 TAB_WEATHER[];

extern void WeatherInit(U8 num);
extern void EnterWeatherMode(void);
extern void ExitWeatherMode(void);
extern U32 Weather_GetChFreq(U8 num);
extern void WeatherScanNextChannel(void);
extern void WeatherScanTask(void);
extern void KeyProcess_Weather(U8 keyEvent);

#endif
