#include "SaveInstrument.h"
#include "Arduino.h"

SaveInstrument::SaveInstrument(){

    updateName = false;

    cursorX = 0;

    instrumentName = "";
}

void SaveInstrument::setExtraData(String instrumentName, int8_t cursorX){
    
    if(!this->instrumentName.equals(instrumentName)){
        this->instrumentName = instrumentName;
        updateName = true;
    }

    if(this->cursorX != cursorX){

        this->cursorX = cursorX;
        updateName = true;
    }

}


void SaveInstrument::drawPreScreen(Adafruit_ST7735 *tft){

    tft->fillScreen(WHITE);
    
    tft->fillRect(0,0, 128, 10, DARK_GRAY);

    tft->fillRect(0, 118, 128, 10, DARK_GRAY);

    tft->setTextColor(WHITE, DARK_GRAY);
    tft->setCursor(1,1);
    tft->print("SAVE VOICE");

    tft->setTextColor(BLUE, WHITE);

    tft->setCursor(1, 24);       

    tft->print("NAME:");

    tft->setCursor(1, 58);
    tft->print("A: CLEAR ALL");

    tft->setCursor(1, 70);
    tft->print("B: CLEAR LETTER");

    tft->setCursor(1, 82);
    tft->print("C: GO TO VOICE LIST");
    
    tft->setCursor(1, 94);
    tft->print("D: SAVE");


    updateName = true;

}


void SaveInstrument::drawScreen(Adafruit_ST7735 *tft){    
    
    if(updateName){
        updateName = false;

        tft->fillRect(0,34, 128, 12, RED);
        
        tft->setTextColor(WHITE, RED);
        
        //tft->setCursor(1,36);
        //tft->print(instrumentName);

        int tempLenght = instrumentName.length();

        if(tempLenght > 0){
            tft->setCursor(1,36);
            for(int i = 0; i < tempLenght; i++){
                if(i == cursorX)
                    tft->setTextColor(GREEN, BLACK);
                else
                    tft->setTextColor(WHITE, RED);

                tft->print(instrumentName.charAt(i));
            }
        }

    }



}

void SaveInstrument::print(Adafruit_ST7735 *tft, bool preScreen){
    
    if(preScreen)
        drawPreScreen(tft);
    else
        drawScreen(tft);
    
}
