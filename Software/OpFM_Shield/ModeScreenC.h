#ifndef MODESCREENC_H
#define MODESCREENC_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> 
#include "LandSpace.h"



class ModeScreenC {
public:
    ModeScreenC();
    void print(Adafruit_ST7735 *tft, bool preScreen);
    void setExtraData(String fileNameList[], int currentPage, int totalPage, int currentVgm, int currentVgmInPage, bool pager, bool play, bool pause);


private: 
    void drawPreScreen(Adafruit_ST7735 *tft);
    void drawScreen(Adafruit_ST7735 *tft);

    void printShortName(Adafruit_ST7735 *tft, int tempIndex);
    
    String fileNameList[4];
    
    bool updateCurrentVgm;
    bool updateCurrentVgmInPage;

    bool updatePlayPause;

    bool playBtn;
    bool pauseBtn;

    bool updatePage;

    int currentVgm;
    int currentVgmInPage;
    int previousVgmInPage;    
    int currentPage;
    int totalPage;

    bool currentPager;

    int16_t x1, y1;
    uint16_t w, h;
    
};

#endif
