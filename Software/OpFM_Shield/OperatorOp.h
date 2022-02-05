#ifndef OPERATOROP_H
#define OPERATOROP_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> 
#include "LandSpace.h"


class OperatorOp{
  public:
    OperatorOp();
    
    void update(bool decVocie, bool incVoice);
    void print(Adafruit_ST7735 *tft, bool preScreen);
    void setExtraData(String voiceName, int currentPage, int currentPositionInPage, int selectedOp, int attack, int decay, int sustain, int release,
                      bool tremolo, bool vibrato, bool envelopeType, bool keyScaleRate, int waveform, int totalLevel, int freMult, int keyScaleLevel);

private: 
    void drawPreScreen(Adafruit_ST7735 *tft);
    void drawScreen(Adafruit_ST7735 *tft);

    void drawBars(Adafruit_ST7735 *tft);

    void setOpColor();

    bool updatePage;
    bool updateSelectedObject;
    bool updateOp;

    int currentPage;
    int currentPositionInPage;
    int previousPositionInPage;
    int selectedOp;

    bool updateAttack;
    bool updateDecay;
    bool updateSustain;
    bool updateRelease;
    bool updateTremolo;
    bool updateVibrato;
    bool updateEnvelopeType;
    bool updateKeyScaleRate;
    bool updateWaveform;
    bool updateTotalLevel;
    bool updateFreMult;
    bool updateKeyScaleLevel;
    bool updateVoiceName;

    String voiceName;

    int attack;
    int decay;
    int sustain;
    int release;
    bool tremolo;
    bool vibrato;
    bool envelopeType;
    bool keyScaleRate;
    int waveform;
    int totalLevel;
    int freMult;
    int keyScaleLevel;

    int preAttack;
    int preDecay;
    int preSustain;
    int preRelease;

    int preTotalLevel;

    int selectIt;
    int previousSelectIt;

    uint16_t opColorBar;
    uint16_t opColorTextBar;
    
    int16_t x1, y1;
    uint16_t w, h;
    
};

#endif
