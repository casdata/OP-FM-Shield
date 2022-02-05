#include "ModeScreenC.h"
#include "Arduino.h"


ModeScreenC::ModeScreenC(){

    updateCurrentVgm = false;
    updateCurrentVgmInPage = false;


    currentVgm = 1;
    currentVgmInPage = 1;


    previousVgmInPage = -1;

    currentPager = false;

    updatePlayPause = false;

    playBtn = false;
    pauseBtn = true;

    currentPage = 0;
    totalPage = 0;


    for(int i = 0; i < 4; i++){
        fileNameList[i] = " ";
    }

}


void ModeScreenC::setExtraData(String fileNameList[], int currentPage, int totalPage, int currentVgm, int currentVgmInPage, bool pager, bool play, bool pause){


    if(this->playBtn != play){
        this->playBtn = play;
        updatePlayPause = true;
    }

    if(this->pauseBtn != pause){
        this->pauseBtn = pause;
        updatePlayPause = true;
    }

    if(this->currentVgm != currentVgm){
        this->currentVgm = currentVgm;
        updateCurrentVgm = true;
    }

    if(this->currentVgmInPage != currentVgmInPage){
        this->previousVgmInPage = this->currentVgmInPage;
        this->currentVgmInPage = currentVgmInPage;
        updateCurrentVgmInPage = true;
    }


    if(this->currentPage != currentPage){
        this->currentPage = currentPage;
        this->totalPage = totalPage;

        for(int i = 0; i < 4; i++)
            this->fileNameList[i] = fileNameList[i];

        updatePage = true;
    }


    if(this->currentPager != pager){
        this->currentPager = pager;
        updatePage = true;
    }

}

void ModeScreenC::drawPreScreen(Adafruit_ST7735 *tft){

    tft->fillScreen(BLACK);
    
    tft->fillRect(0,0, 128, 10, BLUE);

    //tft->fillRect(0, 58, 128, 12, BLUE);

    tft->fillRect(0, 118, 128, 10, BLUE);

    tft->setTextColor(WHITE, BLUE);

    tft->getTextBounds("VGM", 40, 2, &x1, &y1, &w, &h);

    int tempX =  64 - (w/2);


    tft->setCursor(4, 1);
    tft->print("0");

    tft->setCursor(tempX, 1);
    tft->print("VGM");

    /*
    tft->setCursor(1, 58);
    tft->print("A: EXIT");

    tft->setCursor(1, 70);
    tft->print("B: PLAY/PAUSE");
    */

   tft->setTextColor(YELLOW, BLACK);

    tft->setCursor(1, 82);
    tft->print("A: EXIT");
    
    tft->setCursor(1, 94);
    tft->print("B: PLAY");

    tft->setCursor(1, 106);
    tft->print("C: STOP");

    tft->setCursor(56, 82);
    tft->print("D: AUTO-PLAY");

    tft->setCursor(56, 94);
    tft->print("E: GEN-VOICE");

    
    updatePage = true;

    updateCurrentVgm = true;
    updateCurrentVgmInPage = true;

    updatePlayPause = true;

}


void ModeScreenC::drawScreen(Adafruit_ST7735 *tft){    

    
    if(updatePage){
        updatePage = false;

        tft->setTextColor(RED, WHITE);
        
        uint8_t yOffset = 0;

        for(int i = 0; i < 4; i++){

            yOffset = 10 + (i * 12);

            tft->fillRect(0, yOffset, 128, 12, WHITE);

            yOffset = 12 + (i * 12);

            tft->setCursor(1, yOffset);
            printShortName(tft, i);
        }

        tft->setTextColor(WHITE, BLUE);

        tft->fillRect(78, 0, 50, 10, BLUE);

        String a = String(currentPage);
        a += '/';
        a += totalPage;

        tft->getTextBounds(a, 40, 2, &x1, &y1, &w, &h);
        
        int tempX = 128 - w - 1;
        tft->setCursor(tempX, 1);
        tft->print(a);

        tempX = tempX - 8;
        if(currentPager)
            tft->fillCircle(tempX, 4, 3, RED);
        else
            tft->drawCircle(tempX, 4, 3, RED);

        updateCurrentVgmInPage = true;

        updateCurrentVgm = true;
    }


    if(updateCurrentVgm){
        updateCurrentVgm = false;

        tft->fillRect(4, 1, 20, 9, BLUE);

        tft->setTextColor(WHITE,  BLUE);
        tft->setCursor(4, 1);
        tft->print(currentVgm);
    }


    if(updateCurrentVgmInPage){
        updateCurrentVgmInPage = false;

        uint8_t yOffset = 0;

        if(previousVgmInPage >= 0){
            yOffset = 10 + (previousVgmInPage * 12);

            tft->fillRect(0, yOffset, 128, 12, WHITE);

            yOffset = 12 + (previousVgmInPage * 12);

            tft->setTextColor(RED, WHITE);
            tft->setCursor(1, yOffset);
            //tft->print(fileNameList[previousVgmInPage]);
            printShortName(tft, previousVgmInPage);
        }

        yOffset = 10 + (currentVgmInPage * 12);

        tft->fillRect(0, yOffset, 128, 12, RED);

        yOffset = 12 + (currentVgmInPage * 12);

        tft->setCursor(1, yOffset);
        tft->setTextColor(WHITE, RED);
        //tft->print(fileNameList[currentVgmInPage]);
        printShortName(tft, currentVgmInPage);

        
        tft->fillRect(0,58, 128, 20, LIGHT_BLUE);

        tft->setTextColor(BLACK, LIGHT_BLUE);
        tft->setCursor(1, 60);                        //58
        tft->print(fileNameList[currentVgmInPage]);
        
    }

    if(updatePlayPause){
        updatePlayPause = false;

        tft->setTextColor(YELLOW, BLACK);

        if(playBtn){
            tft->setCursor(1, 82);
            tft->print("A: NEXT");

            tft->setTextColor(DARK_GRAY, BLACK);
        }
        else{
            tft->setCursor(1, 82);
            tft->print("A: EXIT");
        }        

        tft->setCursor(56, 94);
        tft->print("E: GEN-VOICE");


        tft->setTextColor(YELLOW, BLACK);

        if(playBtn)
            tft->setTextColor(DARK_GRAY, BLACK);
    
        tft->setCursor(1, 94);
        tft->print("B: PLAY");
        

        tft->setCursor(56, 82);
        tft->print("D: AUTO-PLAY");


        tft->setTextColor(YELLOW, BLACK);

        if(pauseBtn)
            tft->setTextColor(DARK_GRAY, BLACK);

        tft->setCursor(1, 106);
        tft->print("C: STOP");

        
        tft->setTextColor(YELLOW, BLACK);
    }


}

void ModeScreenC::print(Adafruit_ST7735 *tft, bool preScreen){
    
    if(preScreen)
        drawPreScreen(tft);
    else
        drawScreen(tft);
    
}


void ModeScreenC::printShortName(Adafruit_ST7735 *tft, int tempIndex){

    String tempName = "";

    int tempLength = fileNameList[tempIndex].length();
    int countIt = 0;

    if(tempLength > 0){

        bool copyIt = true;

        while(copyIt){

            tempName += fileNameList[tempIndex].charAt(countIt);
        
            countIt++;
        
            if(countIt >= tempLength)
                copyIt = false;

            if(countIt >= 21)
                copyIt = false;

        }
    }

    tft->print(tempName);
}
