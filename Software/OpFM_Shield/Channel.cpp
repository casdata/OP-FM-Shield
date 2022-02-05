#include "Channel.h"
#include "Arduino.h"


Channel::Channel(){
 
    currentPositionInPage = 1;
    previousPositionInPage = -1;

    updatePage = false;
    updateSelectedObject = false;
    updateVoiceName = false;

    algorithm = 0;
    feedback = 0;

    updateAlgorithm = false;
    updateFeedback = false;

    voiceName = " ";
    updateVoiceName = false;

    selectIt = 1;
    previousSelectIt = 0;


}


void Channel::drawPreScreen(Adafruit_ST7735 *tft){

    tft->fillScreen(WHITE);
    
    tft->fillRect(0,0, 128, 10, BLACK);

    tft->fillRect(0, 118, 128, 10, BLACK);

    tft->setTextColor(WHITE, BLACK);

    tft->setCursor(1,1);
    tft->print("OPL2");

    previousSelectIt = 0;

    updatePage = true;
    updateSelectedObject = true;
    updateVoiceName = true;

    updateAlgorithm = true;
    updateFeedback = true;

}

void Channel::setExtraData(String voiceName, int currentPositionInPage, int algorithm, int feedback){

    if(!this->voiceName.equals(voiceName)){
        this->voiceName = voiceName;
        updateVoiceName = true;
    }

    if(this->currentPositionInPage != currentPositionInPage){
        this->previousPositionInPage = this->currentPositionInPage;
        this->currentPositionInPage = currentPositionInPage;
        updateSelectedObject = true;
    }

    if(this->algorithm != algorithm){
        this->algorithm = algorithm;
        updateAlgorithm = true;
    }

    if(this->feedback != feedback){
        this->feedback = feedback;
        updateFeedback = true;
    }

}

void Channel::drawScreen(Adafruit_ST7735 *tft){    

    
    if(updateVoiceName){
        updateVoiceName = false;

        tft->fillRect(0, 118, 128, 10, BLACK);

        tft->setTextColor(YELLOW, BLACK);
        tft->setCursor(4, 120);

        tft->print(voiceName);
    }

    if(updatePage){
        updatePage = false;

        tft->setTextColor(WHITE, BLACK);

        tft->fillRect(78, 0, 50, 10, BLACK);

    
        tft->getTextBounds("1/1", 40, 2, &x1, &y1, &w, &h);
        
        int tempX = 128 - w - 1;
        tft->setCursor(tempX, 1);
        tft->print("1/1");

        /*
        tft->setTextSize(2);
        tft->setTextColor(RED, WHITE);
        tft->setCursor(1, 50);
        tft->print("FM-AM");
        tft->setTextSize(1);
        */

        /*
        int yOffset = 0;

            for(int i = 3; i < 9; i++){
                yOffset = 10 + (i * 12);
                tft->fillRect(0, yOffset, 128, 12, WHITE);
            }

        */

    }

    if(updateSelectedObject){
        updateSelectedObject = false;

        selectIt = currentPositionInPage;
        switch(currentPositionInPage){
            case 1:
                updateAlgorithm = true;
            break;
            case 2:
                updateFeedback = true;
            break;
        }

        if(previousPositionInPage > 0){
            switch(previousPositionInPage){
                case 1:
                    updateAlgorithm = true;            
                break;
                case 2:
                    updateFeedback = true;
                break;
            }
        }

    }


    if(updateAlgorithm){
        updateAlgorithm = false;

        uint16_t tempColorText = RED;
        uint16_t tempColorBack = WHITE;
        uint16_t tempColorText2 = BLACK;

        if(selectIt == 1){
            tempColorText = WHITE;
            tempColorBack = RED;
            tempColorText2 = GREEN;
        }

        tft->setTextColor(tempColorText, tempColorBack);

        uint8_t yOffset = 10;


        if(selectIt != 1)
            tft->fillRect(0, yOffset, 128, 96, tempColorBack);
        else{
            if(selectIt != previousSelectIt)
                tft->fillRect(0, yOffset, 128, 96, tempColorBack);
        }

        yOffset = 12;
        tft->setCursor(1, yOffset);
        tft->print("ALGORITHM:");

        tft->fillRect(94, yOffset, 32, 8, tempColorBack);

        String a = "";
        bool opl3M = true;

        switch(algorithm){
            case 0:
                a += "FM";
                opl3M = false;
            break;
            case 1:
                a += "AM";
                opl3M = false;
            break;
            case 2:
                a += "FM-FM";
            break;
            case 3:
                a += "AM-FM";
            break;
            case 4:
                a += "FM-AM";
            break;
            case 5:
                a += "AM-AM";
            break;
        }

        tft->getTextBounds(a, 64, yOffset, &x1, &y1, &w, &h);

        int tempX = 128 - w - 1;

        tft->setTextColor(tempColorText2, tempColorBack);

        tft->setCursor(tempX, yOffset);
        tft->print(a);

        drawAlgorithm(tft, 3, 26, algorithm);

        tft->setTextColor(WHITE, BLACK);
        tft->setCursor(1,1);
        if(opl3M)
            tft->print("OPL3");
        else
            tft->print("OPL2");

        //drawVarBar(tft, attack, preAttack, 0, 15, 58, 12, 32, 7, GREEN, tempColorText3);

    }

    if(updateFeedback){
        updateFeedback = false;

        uint16_t tempColorText = RED;
        uint16_t tempColorBack = WHITE;
        uint16_t tempColorText2 = BLACK;

        if(selectIt == 2){
            tempColorText = WHITE;
            tempColorBack = RED;
            tempColorText2 = GREEN;
        }

        tft->setTextColor(tempColorText, tempColorBack);

        uint8_t yOffset = 106;

        if(selectIt != 2)
            tft->fillRect(0, yOffset, 128, 12, tempColorBack);
        else{
            if(selectIt != previousSelectIt)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
        }

        yOffset = 108;
        tft->setCursor(1, yOffset);
        tft->print("FEEDBACK:");

        tft->fillRect(112, yOffset, 14, 8, tempColorBack);

        String a = "";

        a += feedback;

        tft->getTextBounds(a, 100, yOffset, &x1, &y1, &w, &h);

        int tempX = 128 - w - 1;

        tft->setTextColor(tempColorText2, tempColorBack);

        tft->setCursor(tempX, yOffset);
        tft->print(feedback);

    }


    previousSelectIt = selectIt;

}

void Channel::print(Adafruit_ST7735 *tft, bool preScreen){
    
    if(preScreen)
        drawPreScreen(tft);
    else
        drawScreen(tft);
    
}
