#ifndef MODESCREENB_H
#define MODESCREENB_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "LandSpace.h"



class ModeScreenB {
public:
    ModeScreenB();
    void print(Adafruit_ST7735 *tft, bool preScreen);
    void setExtraData(String fileNameList[], String fileNameListB[], int currentPage, int currentPageB, int totalPage, int totalPageB, int currentVoice, int currentVoiceB, int currentVoiceInPage, int currentVoiceInPageB, OpMode voiceOp, OpMode voiceOpB, bool pagerA, bool pagerB, bool op4op2Set, MemState mState, bool localOrSD2, bool arp, bool hold, int notePoint, bool setNotepoint);


private: 
    void drawPreScreen(Adafruit_ST7735 *tft);
    void drawScreen(Adafruit_ST7735 *tft);
    
    String fileNameList[4];
    String fileNameListB[4];
    
    bool updateCurrentVoice;
    bool updateCurrentVoiceInPage;

    bool updateCurrentVoiceB;
    bool updateCurrentVoiceInPageB;

    bool updatePage;
    bool updatePageB;

    bool updateOpType;
    bool updateOpTypeB;

    OpMode currentOpMode;
    OpMode currentOpModeB;

    int currentVoice;
    int currentVoiceInPage;
    int previousVoiceInPage;    
    int currentPage;
    int totalPage;

    int currentVoiceB;
    int currentVoiceInPageB;
    int previousVoiceInPageB;
    int currentPageB;
    int totalPageB;

    bool currentPagerA;
    bool currentPagerB;

    bool op4op2Set;
    int notePoint;
    bool setNotepoint;

    bool updateOp4Op2Set;
    bool updateNotePoint;

    MemState mState;
    bool updateLocalOrSD;

    bool arp;
    bool hold;
    bool localOrSD2;
    bool updateLocalOrSD2;

    int16_t x1, y1;
    uint16_t w, h;
    
};

#endif
