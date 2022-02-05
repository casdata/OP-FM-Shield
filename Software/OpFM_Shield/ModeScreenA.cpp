#include "ModeScreenA.h"
#include "Arduino.h"


ModeScreenA::ModeScreenA(){
    updateCurrentVoice = false;
    updateCurrentVoiceInPage = false;

    currentVoice = 1;
    currentVoiceInPage = 1;

    previousVoiceInPage = -1;

    updateOpType = false;

    currentPagerA = false;

    mState = LOCAL_FOLDER;
    updateLocalOrSD = false;

    updateArp = false;

    arp = false;
    hold = false;

    currentOpMode = OP2;

    currentPage = 0;
    totalPage = 0;

    speedE = false;

    for(int i = 0; i < 9; i++)
        fileNameList[i] = " ";

}

void ModeScreenA::update(bool updateScreen){

    if(updateScreen){
        updatePage = true;

        updateSpeedE = true;

        updateArp = true;

        updateCurrentVoice = true;
        updateCurrentVoiceInPage = true;

        updateOpType = true;

        updateLocalOrSD = true;
    }
    
}

void ModeScreenA::setExtraData(String fileNameList[], int currentPage, int totalPage, int currentVoice, int currentVoiceInPage, OpMode voiceOp, bool pagerA, MemState mState, bool speedE, bool arp, bool hold){


    bool updateList = false;

    for(int i = 0; i < 9; i++){
        if(!this->fileNameList[i].equals(fileNameList[i]))
            updateList = true;
    }
      

    if(updateList){
        for(int i = 0; i < 9; i++)
            this->fileNameList[i] = fileNameList[i];


        updatePage = true;
    }


    if(this->mState != mState){
        this->mState = mState;
        updateLocalOrSD = true;
        this->totalPage = totalPage;

        //for(int i = 0; i < 9; i++)
          //  this->fileNameList[i] = fileNameList[i];

        updatePage = true;

        updateCurrentVoice = true;
        updateCurrentVoiceInPage = true;

        updateOpType = true;
    }

    if(this->currentVoice != currentVoice){
        this->currentVoice = currentVoice;
        updateCurrentVoice = true;
    }

    if(this->currentVoiceInPage != currentVoiceInPage){
        this->previousVoiceInPage = this->currentVoiceInPage;
        this->currentVoiceInPage = currentVoiceInPage;
        updateCurrentVoiceInPage = true;
    }

    if(this->currentPage != currentPage){
        this->currentPage = currentPage;
        this->totalPage = totalPage;

        //for(int i = 0; i < 9; i++)
          //  this->fileNameList[i] = fileNameList[i];

        updatePage = true;
    }

    if(this->currentOpMode != voiceOp){
        this->currentOpMode = voiceOp;

        updateOpType = true;
    }

    if(this->currentPagerA != pagerA){
        this->currentPagerA = pagerA;
        updatePage = true;
    }

    if(this->speedE != speedE){
        this->speedE = speedE;
        updateSpeedE = true;
    }

    if(this->arp != arp){
        this->arp = arp;
        updateArp = true;
    }

    if(this->hold != hold){
        this->hold = hold;
        updateArp = true;
    }

}

void ModeScreenA::drawPreScreen(Adafruit_ST7735 *tft){
    
    tft->fillRect(0,0, 128, 10, BLUE);

    tft->fillRect(0, 118, 128, 10, BLUE);

    tft->setTextColor(WHITE, BLUE);

    tft->getTextBounds("LOCAL", 40, 2, &x1, &y1, &w, &h);

    int tempX =  64 - (w/2);


    tft->setCursor(tempX, 1);
    tft->print("LOCAL");

    tft->setCursor(1,1);
    tft->print("OPL2");

    tft->setCursor(4, 120);
    tft->print("0");

    updatePage = true;

    updateCurrentVoice = true;
    updateCurrentVoiceInPage = true;

    updateSpeedE = true;

    updateOpType = true;

    updateLocalOrSD = true;

}


void ModeScreenA::drawScreen(Adafruit_ST7735 *tft){    
    
    if(updatePage){
        updatePage = false;

        tft->setTextColor(RED, WHITE);
        
        uint8_t yOffset = 0;

        for(int i = 0; i < 9; i++){

            yOffset = 10 + (i * 12);

            tft->fillRect(0, yOffset, 128, 12, WHITE);

            yOffset = 12 + (i * 12);

            tft->setCursor(1, yOffset);
            tft->print(fileNameList[i]);
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
        if(currentPagerA)
            tft->fillCircle(tempX, 4, 3, RED);
        else
            tft->drawCircle(tempX, 4, 3, RED);

        updateCurrentVoiceInPage = true;
        updateOpType = true;
        updateCurrentVoice = true;
    }

    if(updateCurrentVoice){
        updateCurrentVoice = false;

        tft->fillRect(4,120, 20, 10, BLUE);

        tft->setTextColor(WHITE,  BLUE);
        tft->setCursor(4, 120);
        tft->print(currentVoice);
    }

    if(updateSpeedE){
        updateSpeedE = false;

        //tft->fillRect()

        if(speedE){
            tft->setTextColor(WHITE, BLUE);
            tft->setCursor(112, 120);
            tft->print("SE");
        }
        else{
            tft->fillRect(112, 120, 12, 10, BLUE);
        }
    }

    if(updateArp){
        updateArp = false;

        if(arp){

            tft->fillRect(42, 120, 40, 8, BLUE);

            tft->setTextColor(WHITE, BLUE);
        
            String a = "";

            if(hold){
                tft->setTextColor(YELLOW, BLUE);
                a += "HARP";
            }
            else
                a += "ARP";
            
            tft->getTextBounds(a, 40, 2, &x1, &y1, &w, &h);

            int tempX =  64 - (w/2);


            tft->setCursor(tempX, 120);
            tft->print(a);

        }
        else
            tft->fillRect(42, 120, 40, 8, BLUE);
    }

    if(updateCurrentVoiceInPage){
        updateCurrentVoiceInPage = false;

        uint8_t yOffset = 0;

        if(previousVoiceInPage >= 0){
            yOffset = 10 + (previousVoiceInPage * 12);

            tft->fillRect(0, yOffset, 128, 12, WHITE);

            yOffset = 12 + (previousVoiceInPage * 12);

            tft->setTextColor(RED, WHITE);
            tft->setCursor(1, yOffset);
            tft->print(fileNameList[previousVoiceInPage]);
        }

        yOffset = 10 + (currentVoiceInPage * 12);

        tft->fillRect(0, yOffset, 128, 12, RED);

        yOffset = 12 + (currentVoiceInPage * 12);

        tft->setCursor(1, yOffset);
        tft->setTextColor(WHITE, RED);
        tft->print(fileNameList[currentVoiceInPage]);

    }
    
    if(updateOpType){
        updateOpType = false;

        tft->setTextColor(WHITE, BLUE);
        tft->setCursor(1,1);

        if(currentOpMode == OP2)
            tft->print("OPL2");
        else
            tft->print("OPL3");

    }

    if(updateLocalOrSD){
        updateLocalOrSD = false;

        tft->fillRect(42, 1, 40, 8, BLUE);

        tft->setTextColor(WHITE, BLUE);
        
        String a = "";

        switch(mState){
            case LOCAL_FOLDER:
                a += "LOCAL";
            break;
            case SD_FOLDER:
                a += "SD";
            break;
            case TEMP_FOLDER:
                a += "SD_TEMP";
            break;
        }
            

        tft->getTextBounds(a, 40, 2, &x1, &y1, &w, &h);

        int tempX =  64 - (w/2);


        tft->setCursor(tempX, 1);
        tft->print(a);

    }
}

void ModeScreenA::print(Adafruit_ST7735 *tft, bool preScreen){
    
    if(preScreen)
        drawPreScreen(tft);
    else
        drawScreen(tft);
    
}
