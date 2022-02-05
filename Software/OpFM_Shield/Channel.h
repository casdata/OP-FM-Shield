#ifndef CHANNEL_H
#define CHANNEL_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> 
#include "LandSpace.h"


class Channel{
  public:
    Channel();
    
    void print(Adafruit_ST7735 *tft, bool preScreen);
    void setExtraData(String voiceName, int currentPositionInPage, int algorithm, int feedback);

private: 
    void drawPreScreen(Adafruit_ST7735 *tft);
    void drawScreen(Adafruit_ST7735 *tft);

    void drawBars(Adafruit_ST7735 *tft);

    bool updatePage;
    bool updateSelectedObject;
    bool updateVoiceName;


    int currentPositionInPage;
    int previousPositionInPage;

    int algorithm;
    int feedback;

    bool updateAlgorithm;
    bool updateFeedback;

    String voiceName;


    int selectIt;
    int previousSelectIt;

    
    int16_t x1, y1;
    uint16_t w, h;
    
};

#endif
