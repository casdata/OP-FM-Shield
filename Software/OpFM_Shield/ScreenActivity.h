#ifndef SCREENACTIVITY_H
#define SCREENACTIVITY_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> 
#include "LandSpace.h"
#include "ModeScreenA.h"
#include "ModeScreenB.h"
#include "ModeScreenC.h"
#include "OperatorOp.h"
#include "Channel.h"
#include "SaveInstrument.h"



class ScreenActivity{
  public:
    ScreenActivity();
    
    void update(Adafruit_ST7735 *tft);
    void updateGenVoiceMessage(Adafruit_ST7735 *tft, int vgmSize, int vgmProgress, int16_t currentVgmVoices);

    void render(Adafruit_ST7735 *tft);
    void setScreen(Adafruit_ST7735 *tft, Screen newScreen);
    Screen getScreen();
    Screen getPreviousScreen();
    void setPreviousScreen(Screen previousScreen);

    void setModeScreenAExtraData(String fileNameList[], int currentPage, int totalPage, int currentVoice, int currentVoiceInPage, OpMode voiceOp, bool pagerA, MemState mState, bool speedE);
    void setModeScreenBExtraData(String fileNameList[], String fileNameListB[], int currentPage, int currentPageB, int totalPage, int totalPageB, int currentVoice, int currentVoiceB, int currentVoiceInPage, int currentVoiceInPageB, OpMode voiceOp, OpMode voiceOpB, bool pagerA, bool pagerB, bool op4op2Set, MemState mState, bool localOrSD2, int notepoint, bool setNotepoint);
    void setModeScreenCExtraData(String fileNameList[], int currentPage, int totalPage, int currentVgm, int currentVgmInPage, bool pager, bool play, bool pause);
    void setOperatorOpExtraData(String voiceName, int currentPage, int currentPositionInPage, int selectedOp, int attack, int decay, int sustain, int release,
                      bool tremolo, bool vibrato, bool envelopeType, bool keyScaleRate, int waveform, int totalLevel, int freMult, int keyScaleLevel);
    void setChannelExtraData(String voiceName, int currentPositionInPage, int algorithm, int feedback);
    void setSaveInsExtraData(String instrumentName, int8_t cursorX);

  
  private:  

    void noSDCardMessage(Adafruit_ST7735 *tft);
    void saveMessage(Adafruit_ST7735 *tft);
    void savedMessage(Adafruit_ST7735 *tft);
    void overWriteMessage(Adafruit_ST7735 *tft);
    void genVoicesMessage(Adafruit_ST7735 *tft);
    void loadedAccessMessage(Adafruit_ST7735 *tft);
    void savedAccessMessage(Adafruit_ST7735 *tft);


    Screen currentScreen;
    Screen previousScreen;
    
    ModeScreenA* modeScreenA;
    ModeScreenB* modeScreenB;
    ModeScreenC* modeScreenC;
    OperatorOp* operatorOp;
    Channel* channel;
    SaveInstrument* saveInstrument;

    unsigned long startScreenTime;

    unsigned long currentTime;
   
    
};

#endif
