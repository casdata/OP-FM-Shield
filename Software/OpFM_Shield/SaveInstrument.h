#ifndef SAVEINSTRUMENT_H
#define SAVEINSTRUMENT_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> 
#include "LandSpace.h"



class SaveInstrument {
public:
    SaveInstrument();
    void print(Adafruit_ST7735 *tft, bool preScreen);
    void setExtraData(String instrumentName, int8_t cursorX);

private: 
    void drawPreScreen(Adafruit_ST7735 *tft);
    void drawScreen(Adafruit_ST7735 *tft);
    
    
    String instrumentName;

    int8_t cursorX;

    bool updateName;

    int16_t x1, y1;
    uint16_t w, h;
    
};

#endif
