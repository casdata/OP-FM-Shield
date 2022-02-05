#include "ScreenActivity.h"


ScreenActivity::ScreenActivity(){
    modeScreenA = new ModeScreenA();
    modeScreenB = new ModeScreenB();
    modeScreenC = new ModeScreenC();
    operatorOp = new OperatorOp();
    channel = new Channel();
    saveInstrument = new SaveInstrument();

    startScreenTime = 0;

    currentScreen = IDLE;
    previousScreen = IDLE;
       
}

Screen ScreenActivity::getScreen(){
    return currentScreen;
}

Screen ScreenActivity::getPreviousScreen(){
    return previousScreen;
}

void ScreenActivity::setPreviousScreen(Screen previousScreen){
    this->previousScreen = previousScreen;
}

void ScreenActivity::update(Adafruit_ST7735 *tft){

    switch(currentScreen){
        case NO_SD_CARD:
        case SAVED_VOICE:
        case SAVED_ACCESS:
            currentTime = millis() - startScreenTime;
            
            if(currentTime > 3000){
                startScreenTime = 0;
                setScreen(tft, previousScreen);
            }
        break;
        case LOADED_ACCESS:
            currentTime = millis() - startScreenTime;
            
            if(currentTime > 1000){
                startScreenTime = 0;
                setScreen(tft, previousScreen);
            }
        break;
        /*
        case SAVED_VOICE:
            
            currentTime = millis() - startScreenTime;

            
            if(currentTime > 3000){
                startScreenTime = 0;
                setScreen(tft, previousScreen);
            }
            
        break;
        */
    }
}

void ScreenActivity::updateGenVoiceMessage(Adafruit_ST7735 *tft, int vgmSize, int vgmProgress, int16_t currentVgmVoices){
    tft->setTextColor(BLACK, YELLOW);
    tft->setCursor(68, 52);
    tft->print(vgmProgress);

    tft->setCursor(68, 66);
    tft->print(vgmSize);

    tft->setCursor(68, 80);
    tft->print(currentVgmVoices);
}

void ScreenActivity::render(Adafruit_ST7735 *tft){
  
    switch(currentScreen){
            case MODE_A:
                modeScreenA->print(tft, false);
            break;
            case MODE_B:
                modeScreenB->print(tft, false);
            break;
            case MODE_C:
                modeScreenC->print(tft, false);
            break;
            case CHANNEL:
                channel->print(tft, false);
            break;
            case OPERATOR:
                operatorOp->print(tft, false);
            break;
            case SAVE_INSTRUMENT:
                saveInstrument->print(tft, false);
            break;
        }//END switch(currentScreen)
}


void ScreenActivity::setScreen(Adafruit_ST7735 *tft, Screen newScreen){
    if(newScreen != currentScreen){
        previousScreen = currentScreen;
        currentScreen = newScreen; 
        
        switch(currentScreen){
            case MODE_A:
                modeScreenA->print(tft, true);
            break;
            case MODE_B:
                modeScreenB->print(tft, true);
            break;
            case MODE_C:
                modeScreenC->print(tft, true);
            break;
            case CHANNEL:
                channel->print(tft, true);
            break;
            case OPERATOR:
                operatorOp->print(tft, true);
            break;
            case SAVE_INSTRUMENT:
                saveInstrument->print(tft, true);
            break;
            case NO_SD_CARD:
                noSDCardMessage(tft);
                startScreenTime = millis();
            break;
            case SAVE_MESSAGE:
                saveMessage(tft);
            break;
            case SAVED_VOICE:
                savedMessage(tft);
                startScreenTime = millis();
            break;
            case OVERWRITE_VOICE:
                overWriteMessage(tft);
            break;
            case EXP_VOICES:
                genVoicesMessage(tft);
            break;
            case LOADED_ACCESS:
                loadedAccessMessage(tft);
                startScreenTime = millis();
            break;
            case SAVED_ACCESS:
                savedAccessMessage(tft);
                startScreenTime = millis();
            break; 
        }//END switch(currentScreen)
        
    }//END if(newScreen != currentScreen)
}

void ScreenActivity::noSDCardMessage(Adafruit_ST7735 *tft){
    tft->fillRect(2, 36, 124, 48, RED);
    tft->drawRect(2, 36, 124, 48, BLACK);

    tft->setTextSize(2);
    tft->setTextColor(WHITE, RED);
    tft->setCursor(4, 42);
    tft->print("NO SD CARD");
    tft->setCursor(12, 62);
    tft->print("DETECTED!");
    tft->setTextSize(1);
}

void ScreenActivity::saveMessage(Adafruit_ST7735 *tft){
    tft->fillRect(2, 26, 124, 88, RED);
    tft->drawRect(2, 26, 124, 88, BLACK);

    tft->setTextSize(2);
    tft->setTextColor(WHITE, RED);
    tft->setCursor(12, 32);
    tft->print("SAVE");
    tft->setCursor(12, 52);
    tft->print("CHANGES?");
    tft->setTextColor(BLUE, RED);
    tft->setCursor(12, 72);
    tft->print("A: YES");
    tft->setCursor(12, 92);
    tft->print("B: NO");

    tft->setTextSize(1);
}

void ScreenActivity::savedMessage(Adafruit_ST7735 *tft){
    tft->fillRect(2, 36, 124, 48, GREEN);
    tft->drawRect(2, 36, 124, 48, BLACK);

    tft->setTextSize(2);
    tft->setTextColor(BLACK, GREEN);
    tft->setCursor(11, 42);
    tft->print("VOICE WAS");
    tft->setCursor(34, 62);
    tft->print("SAVED!");
    tft->setTextSize(1);
}

void ScreenActivity::overWriteMessage(Adafruit_ST7735 *tft){
    tft->fillRect(2, 26, 124, 88, RED);
    tft->drawRect(2, 26, 124, 88, BLACK);

    tft->setTextSize(2);
    tft->setTextColor(WHITE, RED);
    tft->setCursor(12, 32);
    tft->print("OVERWRITE");
    tft->setCursor(12, 52);
    tft->print("VOICE?");
    tft->setTextColor(BLUE, RED);
    tft->setCursor(12, 72);
    tft->print("A: YES");
    tft->setCursor(12, 92);
    tft->print("B: NO");

    tft->setTextSize(1);
}

void ScreenActivity::genVoicesMessage(Adafruit_ST7735 *tft){
    tft->fillRect(2, 26, 124, 88, YELLOW);
    tft->drawRect(2, 26, 124, 88, BLACK);
    

    tft->setTextSize(1);
    tft->setTextColor(RED, YELLOW);
    tft->setCursor(12, 32);
    tft->print("GENERATING VOICES");
    tft->setTextColor(BLUE, YELLOW);
    tft->setCursor(12, 52);
    tft->print("Progress:");
    //tft->setCursor(68, 52);
    //tft->print("999999999");
    tft->setCursor(12, 66);
    tft->print("VGM Size: ");
    tft->setCursor(24, 80);
    tft->print("Voices:");
    tft->setTextColor(BLACK, YELLOW);
    tft->setCursor(18, 100);
    tft->print("PLEASE WAIT....");

}

 void ScreenActivity::loadedAccessMessage(Adafruit_ST7735 *tft){
    tft->fillRect(2, 36, 124, 48, GREEN);
    tft->drawRect(2, 36, 124, 48, BLACK);

    tft->setTextSize(2);
    tft->setTextColor(BLACK, GREEN);
    tft->setCursor(11, 42);
    tft->print("ACCESS");
    tft->setCursor(11, 62);
    tft->print("LOADED!");
    tft->setTextSize(1);
 }

 void ScreenActivity::savedAccessMessage(Adafruit_ST7735 *tft){
    tft->fillRect(2, 36, 124, 48, GREEN);
    tft->drawRect(2, 36, 124, 48, BLACK);

    tft->setTextSize(2);
    tft->setTextColor(BLACK, GREEN);
    tft->setCursor(11, 42);
    tft->print("ACCESS");
    tft->setCursor(11, 62);
    tft->print("SAVED!");
    tft->setTextSize(1);
 }

void ScreenActivity::setModeScreenAExtraData(String fileNameList[], int currentPage, int totalPage, int currentVoice, int currentVoiceInPage, OpMode voiceOp, bool pagerA, MemState mState, bool speedE){
    modeScreenA->setExtraData(fileNameList, currentPage, totalPage, currentVoice, currentVoiceInPage, voiceOp, pagerA, mState, speedE, false, false);
}

void ScreenActivity::setOperatorOpExtraData(String voiceName, int currentPage, int currentPositionInPage, int selectedOp, int attack, int decay, int sustain, int release, bool tremolo, bool vibrato, bool envelopeType, bool keyScaleRate, int waveform, int totalLevel, int freMult, int keyScaleLevel){
    operatorOp->setExtraData(voiceName, currentPage, currentPositionInPage, selectedOp, attack, decay, sustain, release, tremolo, vibrato, envelopeType, keyScaleRate, waveform, totalLevel, freMult, keyScaleLevel);
}

void ScreenActivity::setChannelExtraData(String voiceName, int currentPositionInPage, int algorithm, int feedback){
    channel->setExtraData(voiceName, currentPositionInPage, algorithm, feedback);
}

void ScreenActivity::setModeScreenBExtraData(String fileNameList[], String fileNameListB[], int currentPage, int currentPageB, int totalPage, int totalPageB, int currentVoice, int currentVoiceB, int currentVoiceInPage, int currentVoiceInPageB, OpMode voiceOp, OpMode voiceOpB, bool pagerA, bool pagerB, bool op4op2Set, MemState mState, bool localOrSD2, int notepoint, bool setNotepoint){
    modeScreenB->setExtraData(fileNameList, fileNameListB, currentPage, currentPageB, totalPage,totalPageB, currentVoice, currentVoiceB, currentVoiceInPage, currentVoiceInPageB, voiceOp, voiceOpB, pagerA, pagerB, op4op2Set, mState, localOrSD2, false, false, notepoint, setNotepoint);
}

void ScreenActivity::setSaveInsExtraData(String instrumentName, int8_t cursorX){
    saveInstrument->setExtraData(instrumentName, cursorX);
}

void ScreenActivity::setModeScreenCExtraData(String fileNameList[], int currentPage, int totalPage, int currentVgm, int currentVgmInPage, bool pager, bool play, bool pause){
    modeScreenC->setExtraData(fileNameList, currentPage, totalPage, currentVgm, currentVgmInPage, pager, play, pause);
}
