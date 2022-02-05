#ifndef MODESCREEN_H
#define MODESCREEN_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> 
#include "LandSpace.h"



class ModeScreenA {
public:
    ModeScreenA();
    void update(bool updateScreen);
    void print(Adafruit_ST7735 *tft, bool preScreen);
    void setExtraData(String fileNameList[], int currentPage, int totalPage, int currentVoice, int currentVoiceInPage, OpMode voiceOp, bool pagerA, MemState mState, bool speedE, bool arp, bool hold);

private: 
    void drawPreScreen(Adafruit_ST7735 *tft);
    void drawScreen(Adafruit_ST7735 *tft);
    
    
    String fileNameList[9];
    

    bool updateCurrentVoice;
    bool updateCurrentVoiceInPage;

    bool updatePage;
    bool updateSpeedE;

    bool updateArp;

    bool updateOpType;

    OpMode currentOpMode;

    int currentVoice;
    int currentVoiceInPage;
    int previousVoiceInPage;    
    int currentPage;
    int totalPage;

    bool currentPagerA;
    
    bool speedE;

    bool arp;
    bool hold;

    MemState mState;
    bool updateLocalOrSD;

    int16_t x1, y1;
    uint16_t w, h;
    
};

#endif
