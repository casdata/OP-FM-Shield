#include "ModeScreenB.h"
#include "Arduino.h"


ModeScreenB::ModeScreenB(){


    updateCurrentVoice = false;
    updateCurrentVoiceInPage = false;

    updateCurrentVoiceB = false;
    updateCurrentVoiceInPageB = false;

    currentVoice = 1;
    currentVoiceInPage = 1;

    currentVoiceB = 1;
    currentVoiceInPageB = 1;

    previousVoiceInPage = -1;
    previousVoiceInPageB = -1;

    updateOpType = false;
    updateOpTypeB = false;

    currentPagerA = false;
    currentPagerB = false;

    op4op2Set = true;
    setNotepoint = false;
    notePoint = 60;
    updateOp4Op2Set = false;
    updateNotePoint = false;

    arp = false;
    hold = false;

    currentOpMode = OP2;
    currentOpModeB = OP2;

    currentPage = 0;
    totalPage = 0;

    currentPageB = 0;
    totalPageB = 0;

    mState = LOCAL_FOLDER;
    updateLocalOrSD = false;

    localOrSD2 = true;
    updateLocalOrSD2 = false;

    for(int i = 0; i < 4; i++){
        fileNameList[i] = " ";
        fileNameListB[i] = " ";
    }

}


void ModeScreenB::setExtraData(String fileNameList[], String fileNameListB[], int currentPage, int currentPageB, int totalPage, int totalPageB, int currentVoice, int currentVoiceB, int currentVoiceInPage, int currentVoiceInPageB, OpMode voiceOp, OpMode voiceOpB, bool pagerA, bool pagerB, bool op4op2Set, MemState mState, bool localOrSD2, bool arp, bool hold, int notePoint, bool setNotepoint){

    if(this->mState != mState){
        this->mState = mState;
        updateLocalOrSD = true;
        this->totalPage = totalPage;

        for(int i = 0; i < 4; i++)
            this->fileNameList[i] = fileNameList[i];

        updatePage = true;

        updateCurrentVoice = true;
        updateCurrentVoiceInPage = true;

        updateOpType = true;

    }

    if(this->localOrSD2 != localOrSD2){
        this->localOrSD2 = localOrSD2;
        updateLocalOrSD2 = true;
        this->totalPageB = totalPageB;

        for(int i = 0; i < 4; i++)
            this->fileNameListB[i] = fileNameListB[i];

        updatePageB = true;

        updateCurrentVoiceB = true;
        updateCurrentVoiceInPageB = true;

        updateOpTypeB = true;

    }

    if(this->setNotepoint != setNotepoint){
        this->setNotepoint = setNotepoint;
        updateOp4Op2Set = true;
    }

    if(this->notePoint != notePoint){
        this->notePoint = notePoint;
        updateOp4Op2Set = true;
    }

    if(this->op4op2Set != op4op2Set){
        this->op4op2Set = op4op2Set;
        updateOp4Op2Set = true;
    }

    if(this->arp != arp){
        this->arp = arp;
        updateOp4Op2Set = true;
    }

    if(this->hold != hold){
        this->hold = hold;
        updateOp4Op2Set = true;
    }

    if(this->currentVoice != currentVoice){
        this->currentVoice = currentVoice;
        updateCurrentVoice = true;
    }

    if(this->currentVoiceB != currentVoiceB){
        this->currentVoiceB = currentVoiceB;
        updateCurrentVoiceB = true;
    }

    if(this->currentVoiceInPage != currentVoiceInPage){
        this->previousVoiceInPage = this->currentVoiceInPage;
        this->currentVoiceInPage = currentVoiceInPage;
        updateCurrentVoiceInPage = true;
    }

    if(this->currentVoiceInPageB != currentVoiceInPageB){
        this->previousVoiceInPageB = this->currentVoiceInPageB;
        this->currentVoiceInPageB = currentVoiceInPageB;
        updateCurrentVoiceInPageB = true;
    }

    if(this->currentPage != currentPage){
        this->currentPage = currentPage;
        this->totalPage = totalPage;

        for(int i = 0; i < 4; i++)
            this->fileNameList[i] = fileNameList[i];

        updatePage = true;
    }

    if(this->currentPageB != currentPageB){
        this->currentPageB = currentPageB;
        this->totalPageB = totalPageB;

        for(int i = 0; i < 4; i++)
            this->fileNameListB[i] = fileNameListB[i];

        updatePageB = true;
    }

    if(this->currentOpMode != voiceOp){
        this->currentOpMode = voiceOp;

        updateOpType = true;
    }

    if(this->currentOpModeB != voiceOpB){
        this->currentOpModeB = voiceOpB;

        updateOpTypeB = true;
    }

    if(this->currentPagerA != pagerA){
        this->currentPagerA = pagerA;
        updatePage = true;
    }

    if(this->currentPagerB != pagerB){
        this->currentPagerB = pagerB;
        updatePageB = true;
    }

}

void ModeScreenB::drawPreScreen(Adafruit_ST7735 *tft){
    
    tft->fillRect(0,0, 128, 10, BLUE);

    tft->fillRect(0, 58, 128, 12, BLUE);

    tft->fillRect(0, 118, 128, 10, BLUE);

    tft->setTextColor(WHITE, BLUE);

    tft->getTextBounds("LOCAL", 40, 2, &x1, &y1, &w, &h);

    int tempX =  64 - (w/2);


    tft->setCursor(1,1);
    tft->print("OPL2");

    tft->setCursor(1, 120);
    tft->print("OPL2");

    tft->setCursor(4, 72);
    tft->print("0");

    tft->setCursor(tempX, 1);
    tft->print("LOCAL");


    tft->setCursor(tempX, 120);
    tft->print("LOCAL");
    
    updatePage = true;

    updateCurrentVoice = true;
    updateCurrentVoiceInPage = true;

    updateOpType = true;
    
    updatePageB = true;
    
    updateCurrentVoiceB = true;
    updateCurrentVoiceInPageB = true;

    updateOpTypeB = true;

    //updateNotePoint = true;
    updateOp4Op2Set = true;

    updateLocalOrSD = true;
    updateLocalOrSD2 = true;

}


void ModeScreenB::drawScreen(Adafruit_ST7735 *tft){    

    
    
    if(updatePage){
        updatePage = false;

        tft->setTextColor(RED, WHITE);
        
        uint8_t yOffset = 0;

        for(int i = 0; i < 4; i++){

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

    if(updatePageB){
        updatePageB = false;

        tft->setTextColor(GREEN, BLACK);
        
        uint8_t yOffset = 0;

        for(int i = 0; i < 4; i++){

            yOffset = 70 + (i * 12);

            tft->fillRect(0, yOffset, 128, 12, BLACK);

            yOffset = 72 + (i * 12);

            tft->setCursor(1, yOffset);
            tft->print(fileNameListB[i]);
        }

        tft->setTextColor(WHITE, BLUE);

        tft->fillRect(78, 118, 50, 10, BLUE);

        String a = String(currentPageB);
        a += '/';
        a += totalPageB;

        tft->getTextBounds(a, 40, 2, &x1, &y1, &w, &h);
        
        int tempX = 128 - w - 1;
        tft->setCursor(tempX, 120);
        tft->print(a);

        tempX = tempX - 8;
        if(currentPagerB)
            tft->fillCircle(tempX, 122, 3, GREEN);
        else
            tft->drawCircle(tempX, 122, 3, GREEN);

        updateCurrentVoiceInPageB = true;
        updateOpTypeB = true;
        updateCurrentVoiceB = true;
    }

    if(updateCurrentVoice){
        updateCurrentVoice = false;

        tft->fillRect(4,60, 20, 10, BLUE);

        tft->setTextColor(WHITE,  BLUE);
        tft->setCursor(4, 60);
        tft->print(currentVoice);
    }

    if(updateCurrentVoiceB){
        updateCurrentVoiceB = false;

        String a = "";
        a += currentVoiceB;

        tft->getTextBounds(a, 40, 2, &x1, &y1, &w, &h);
        
        int tempX = 128 - w - 4;

        tft->fillRect(102, 59, 20, 10, BLUE);

        tft->setCursor(tempX, 60);
 
        tft->setTextColor(WHITE,  BLUE);

        tft->print(currentVoiceB);
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

    if(updateCurrentVoiceInPageB){
        updateCurrentVoiceInPageB = false;

        uint8_t yOffset = 0;

        if(previousVoiceInPageB >= 0){
            yOffset = 70 + (previousVoiceInPageB * 12);

            tft->fillRect(0, yOffset, 128, 12, BLACK);

            yOffset = 72 + (previousVoiceInPageB * 12);

            tft->setTextColor(GREEN, BLACK);
            tft->setCursor(1, yOffset);
            tft->print(fileNameListB[previousVoiceInPageB]);
        }

        yOffset = 70 + (currentVoiceInPageB * 12);

        tft->fillRect(0, yOffset, 128, 12, GREEN);

        yOffset = 72 + (currentVoiceInPageB * 12);

        tft->setCursor(1, yOffset);
        tft->setTextColor(BLACK, GREEN);
        tft->print(fileNameListB[currentVoiceInPageB]);

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

    if(updateOpTypeB){
        updateOpTypeB = false;

        tft->setTextColor(WHITE, BLUE);
        tft->setCursor(1, 120);

        if(currentOpModeB == OP2)
            tft->print("OPL2");
        else
            tft->print("OPL3");
    }

    if(updateOp4Op2Set){
        updateOp4Op2Set = false;
        

        if(setNotepoint)
            tft->setTextColor(BLACK, LIGHT_BLUE);
        else
            tft->setTextColor(WHITE, BLACK);
        

        String a = "";

        uint8_t selectedOctave = (uint8_t)((notePoint / 12)-1);
        uint8_t selectedNote = (uint8_t)(notePoint % 12);

        switch(selectedNote){
            case 0:
                a += "C";
            break;
            case 1:
                a += "C#";
            break;
            case 2:
                a += "D";
            break;
            case 3:
                a += "D#";
            break;
            case 4:
                a += "E";
            break;
            case 5:
                a += "F";
            break;
            case 6:
                a += "F#";
            break;
            case 7:
                a += "G";
            break;
            case 8:
                a += "G#";
            break;
            case 9:
                a += "A";
            break;
            case 10:
                a += "A#";
            break;
            case 11:
                a += "B";
            break;
        }

        a += String(selectedOctave);

        tft->getTextBounds(a, 40, 2, &x1, &y1, &w, &h);


        int tempX = 64 - (w/2);
        int tempPosX = tempX - 1;
        int tempPosX2 = 64 + (w/2) + 1;


        tft->fillRect(36, 59, 55, 10, BLUE);
        //tft->fillRect(tempPosX - 17, 59, (w + 35), 10, BLACK);

        
        if(setNotepoint)
            tft->fillRect(tempPosX, 59, w + 1, h + 2, LIGHT_BLUE);
        else
            tft->fillRect(tempPosX, 59, w + 1, h + 2, BLACK);

        tft->setCursor(tempX, 61);
        tft->print(a);



        if(op4op2Set){
            tft->fillRect(tempPosX - 16, 59, 15, 10, RED);
            tft->fillRect(tempPosX2, 59, 15, 10, GREEN);
        }
        else{
            tft->fillRect(tempPosX - 16, 59, 15, 10, GREEN);
            tft->fillRect(tempPosX2, 59, 15, 10, RED);
        }

       
       for(int i = 0; i < 5; i++){
            int j = (tempPosX - 17) + (i*4);
            tft->drawLine(j, 59, j, 68, BLACK);

            j = (tempPosX2 - 1) + (i*4);
            tft->drawLine(j, 59, j, 68, BLACK);
        }
    
        int i = tempPosX - 17;
        tft->drawLine(i + 3, 59, i + 3, 63, BLACK);
        tft->drawLine(i + 5, 59, i + 5, 63, BLACK);

        tft->drawLine(i + 7, 59, i + 7, 63, BLACK);
        tft->drawLine(i + 9, 59, i + 9, 63, BLACK);

        tft->drawLine(i + 15, 59, i + 15, 63, BLACK);

        i = tempPosX2 - 1;
        tft->drawLine(i + 3, 59, i + 3, 63, BLACK);
        tft->drawLine(i + 5, 59, i + 5, 63, BLACK);

        tft->drawLine(i + 7, 59, i + 7, 63, BLACK);
        tft->drawLine(i + 9, 59, i + 9, 63, BLACK);

        tft->drawLine(i + 15, 59, i + 15, 63, BLACK);

        tft->drawLine(tempPosX - 17, 59, tempPosX2 + 15, 59, BLACK);

    }

    if(updateNotePoint){
        updateNotePoint = false;
    
        if(setNotepoint)
            tft->setTextColor(BLACK, LIGHT_BLUE);
        else
            tft->setTextColor(WHITE, BLACK);

        String a = "";

        uint8_t selectedOctave = (uint8_t)((notePoint / 12)-1);
        uint8_t selectedNote = (uint8_t)(notePoint % 12);

        switch(selectedNote){
            case 0:
                a += "C";
            break;
            case 1:
                a += "C#";
            break;
            case 2:
                a += "D";
            break;
            case 3:
                a += "D#";
            break;
            case 4:
                a += "E";
            break;
            case 5:
                a += "F";
            break;
            case 6:
                a += "F#";
            break;
            case 7:
                a += "G";
            break;
            case 8:
                a += "G#";
            break;
            case 9:
                a += "A";
            break;
            case 10:
                a += "A#";
            break;
            case 11:
                a += "B";
            break;
        }

        a += String(selectedOctave);

        tft->getTextBounds(a, 40, 2, &x1, &y1, &w, &h);


        int tempX = 64 - (w/2);

        if(setNotepoint)
            tft->drawRect(tempX - 1, 60, w + 1, h, LIGHT_BLUE);
        else
            tft->drawRect(tempX - 1, 60, w + 1, h, BLACK);

        tft->setCursor(tempX, 60);
        tft->print(a);
    }

    if(updateLocalOrSD){
        updateLocalOrSD = false;

        tft->fillRect(44, 1, 36, 8, BLUE);

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

    if(updateLocalOrSD2){
        updateLocalOrSD2 = false;

        tft->fillRect(44, 119, 36, 8, BLUE);

        tft->setTextColor(WHITE, BLUE);
        
        String a = "";

        if(localOrSD2)
            a += "LOCAL";
        else
            a += "SD";

        tft->getTextBounds(a, 40, 2, &x1, &y1, &w, &h);

        int tempX =  64 - (w/2);


        tft->setCursor(tempX, 119);
        tft->print(a);

    }

}

void ModeScreenB::print(Adafruit_ST7735 *tft, bool preScreen){
    
    if(preScreen)
        drawPreScreen(tft);
    else
        drawScreen(tft);
    
}
