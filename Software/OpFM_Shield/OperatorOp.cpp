#include "OperatorOp.h"
#include "Arduino.h"


OperatorOp::OperatorOp(){
    /*
    operatorListA[0] = "ATTACK:";
    operatorListA[1] = "DECAY:";
    operatorListA[2] = "SUSTAIN:";
    operatorListA[3] = "RELEASE:";
    operatorListA[4] = "TREMOLO:";
    operatorListA[5] = "VIBRATO:";
    operatorListA[6] = "ENVELOPE TYPE:";
    operatorListA[7] = "KEY SCALE RATE:";
    operatorListA[8] = "WAVEFORM: ";

    operatorListB[0] = "TOTAL LEVEL:";
    operatorListB[1] = "FREQUENCY MULTI:";
    operatorListB[2] = "KEY SCALE LEVEL:";
    */

    currentPage = 1;
    currentPositionInPage = 1;
    previousPositionInPage = -1;
    selectedOp = 1;

    updatePage = false;
    updateSelectedObject = false;
    updateOp = false;

    attack = 0;
    decay = 0;
    sustain = 0;
    release = 0;
    preAttack = 0;
    preDecay = 0;
    preSustain = 0;
    preRelease = 0;
    tremolo = false;
    vibrato = false;
    envelopeType = false;
    keyScaleRate = false;
    waveform = 0;
    totalLevel = 0;
    preTotalLevel = 0;
    freMult = 0;
    keyScaleLevel = 0;

    updateAttack = false;
    updateDecay = false;
    updateSustain = false;
    updateRelease = false;
    updateTremolo = false;
    updateVibrato = false;
    updateEnvelopeType = false;
    updateKeyScaleRate = false;
    updateWaveform = false;
    updateTotalLevel = false;
    updateFreMult = false;
    updateKeyScaleLevel = false;

    voiceName = " ";
    updateVoiceName = false;

    selectIt = 1;
    previousSelectIt = 0;

    opColorBar = CYAN;
    opColorTextBar = BLACK;
}


void OperatorOp::update(bool decVocie, bool incVoice){

   
}

void OperatorOp::setOpColor(){
    switch(selectedOp){
        case 1:
            opColorBar = CYAN;
            opColorTextBar = BLACK;
        break;
        case 2:
            opColorBar = YELLOW;
            opColorTextBar = BLACK;
        break;
        case 3:
            opColorBar = MAGENTA;
            opColorTextBar = WHITE;
        break;
        case 4:
            opColorBar = DARK_GREEN;
            opColorTextBar = WHITE;
        break;
    }
}

void OperatorOp::drawBars(Adafruit_ST7735 *tft){

    tft->fillRect(0,0, 128, 10, opColorBar);

    tft->fillRect(0, 118, 128, 10, opColorBar);
}

void OperatorOp::drawPreScreen(Adafruit_ST7735 *tft){

    tft->fillScreen(WHITE);
    
    drawBars(tft);

    tft->setTextColor(opColorTextBar, opColorBar);

    tft->setCursor(1,1);
    tft->print("OP1");

    previousSelectIt = 0;

    updatePage = true;
    updateSelectedObject = true;
    updateOp = true;
    updateAttack = true;
    updateDecay = true;
    updateSustain = true;
    updateRelease = true;
    updateTremolo = true;
    updateVibrato = true;
    updateEnvelopeType = true;
    updateKeyScaleRate = true;
    updateWaveform = true;
    updateTotalLevel = true;
    updateFreMult = true;
    updateKeyScaleLevel = true;
    updateVoiceName = true;

}

void OperatorOp::setExtraData(String voiceName, int currentPage, int currentPositionInPage, int selectedOp, int attack, int decay, int sustain, int release, bool tremolo, bool vibrato, bool envelopeType, bool keyScaleRate, int waveform, int totalLevel, int freMult, int keyScaleLevel){

    if(!this->voiceName.equals(voiceName)){
        this->voiceName = voiceName;
        updateVoiceName = true;
        
    }

    if(this->currentPage != currentPage){
        this->currentPage = currentPage;
        updatePage = true;
    }

    if(this->currentPositionInPage != currentPositionInPage){
        this->previousPositionInPage = this->currentPositionInPage;
        this->currentPositionInPage = currentPositionInPage;
        updateSelectedObject = true;
    }

    if(this->selectedOp != selectedOp){
        this->selectedOp = selectedOp;

        setOpColor();

        updateOp = true;
        updateVoiceName = true;
        updatePage = true;
    }

    if(this->attack != attack){
        this->preAttack = this->attack;
        this->attack = attack;
        updateAttack = true;
    }

    if(this->decay != decay){
        this->preDecay = this->decay;
        this->decay = decay;
        updateDecay = true;
    }

    if(this->sustain != sustain){
        this->preSustain = this->sustain;
        this->sustain = sustain;
        updateSustain = true;
    }

    if(this->release != release){
        this->preRelease = this->release;
        this->release = release;
        updateRelease = true;
    }

    if(this->tremolo != tremolo){
        this->tremolo = tremolo;
        updateTremolo = true;
    }

    if(this->vibrato != vibrato){
        this->vibrato = vibrato;
        updateVibrato = true;
    }

    if(this->envelopeType != envelopeType){
        this->envelopeType = envelopeType;
        updateEnvelopeType = true;
    }

    if(this->keyScaleRate != keyScaleRate){
        this->keyScaleRate = keyScaleRate;
        updateKeyScaleRate = true;
    }

    if(this->waveform != waveform){
        this->waveform = waveform;
        updateWaveform = true;
    }
    
    if(this->totalLevel != totalLevel){
        this->preTotalLevel = this->totalLevel;
        this->totalLevel = totalLevel;
        updateTotalLevel = true;
    }

    if(this->freMult != freMult){
        this->freMult = freMult;
        updateFreMult = true;
    }

    if(this->keyScaleLevel != keyScaleLevel){
        this->keyScaleLevel = keyScaleLevel;
        updateKeyScaleLevel = true;
    }

}

void OperatorOp::drawScreen(Adafruit_ST7735 *tft){    

    if(updateOp){
        updateOp = false;
        
        drawBars(tft);

        tft->setTextColor(opColorTextBar, opColorBar);

        tft->setCursor(1,1);

        switch(selectedOp){
            case 1:
                tft->print("OP1");
            break;
            case 2:
                tft->print("OP2");
            break;
            case 3:
                tft->print("OP3");
            break;
            case 4:
                tft->print("OP4");
            break;
        }

    }
    
    if(updateVoiceName){
        updateVoiceName = false;

        tft->fillRect(0, 118, 128, 10, opColorBar);

        tft->setTextColor(opColorTextBar, opColorBar);
        tft->setCursor(4, 120);

        tft->print(voiceName);
    }

    if(updatePage){
        updatePage = false;

        tft->setTextColor(opColorTextBar, opColorBar);

        tft->fillRect(78, 0, 50, 10, opColorBar);

        String a;

        if(currentPage == 1)
            a = "1/2";
        else
            a = "2/2";

    
        tft->getTextBounds(a, 40, 2, &x1, &y1, &w, &h);
        
        int tempX = 128 - w - 1;
        tft->setCursor(tempX, 1);
        tft->print(a);

        if(currentPage == 1){
            updateAttack = true;
            updateDecay = true;
            updateSustain = true;
            updateRelease = true;
            updateTremolo = true;
            updateVibrato = true;
            updateEnvelopeType = true;
            updateKeyScaleRate = true;
            updateWaveform = true;
        }
        else{
            updateTotalLevel = true;
            updateFreMult = true;
            updateKeyScaleLevel = true;

            int yOffset = 0;

            for(int i = 3; i < 9; i++){
                yOffset = 10 + (i * 12);
                tft->fillRect(0, yOffset, 128, 12, WHITE);
            }

        }


    }

    if(updateSelectedObject){
        updateSelectedObject = false;

        selectIt = currentPositionInPage;
    

        switch(currentPositionInPage){
            case 1:
                if(currentPage == 1)
                    updateAttack = true;  
                else
                    updateTotalLevel = true;
            
            break;
            case 2:
                if(currentPage == 1)
                    updateDecay = true;
                else
                    updateFreMult = true;
            break;
            case 3:
                if(currentPage == 1)
                    updateSustain = true;
                else
                    updateKeyScaleLevel = true;
            break;
            case 4:
                updateRelease = true;
            break;
            case 5:
                updateTremolo = true;
            break;
            case 6:
                updateVibrato = true;
            break;
            case 7:
                updateEnvelopeType = true;
            break;
            case 8:
                updateKeyScaleRate = true;
            break;
            case 9:
                updateWaveform = true;
            break;
        }

        if(previousPositionInPage > 0){
            switch(previousPositionInPage){
                case 1:
                    if(currentPage == 1)
                        updateAttack = true;  
                    else
                        updateTotalLevel = true;
            
                break;
                case 2:
                    if(currentPage == 1)
                        updateDecay = true;
                    else
                        updateFreMult = true;
                break;
                case 3:
                    if(currentPage == 1)
                        updateSustain = true;
                    else
                        updateKeyScaleLevel = true;
                break;
                case 4:
                    updateRelease = true;
                break;
                case 5:
                    updateTremolo = true;
                break;
                case 6:
                    updateVibrato = true;
                break;
                case 7:
                    updateEnvelopeType = true;
                break;
                case 8:
                    updateKeyScaleRate = true;
                break;
                case 9:
                    updateWaveform = true;
                break;
            }
        }

    }

    if(updateAttack){
        updateAttack = false;

        if(currentPage == 1){

            uint16_t tempColorText = RED;
            uint16_t tempColorBack = WHITE;
            uint16_t tempColorText2 = BLACK;
            uint16_t tempColorText3 = RED;

            if(selectIt == 1){
                tempColorText = WHITE;
                tempColorBack = RED;
                tempColorText2 = GREEN;
                tempColorText3 = DARK_RED;
            }

            tft->setTextColor(tempColorText, tempColorBack);

            uint8_t yOffset = 10;


            if(selectIt != 1)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            else{
                if(selectIt != previousSelectIt)
                    tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            }


            yOffset = 12;
            tft->setCursor(1, yOffset);
            tft->print("ATTACK:");

            tft->fillRect(112, yOffset, 14, 8, tempColorBack);

            String a = "";
            a += attack;

            tft->getTextBounds(a, 100, yOffset, &x1, &y1, &w, &h);

            int tempX = 128 - w - 1;

            tft->setTextColor(tempColorText2, tempColorBack);

            tft->setCursor(tempX, yOffset);
            tft->print(attack);

            if(preAttack == -1)
                preAttack = 0;
            
            drawVarBar(tft, attack, preAttack, 0, 15, 58, 12, 32, 7, GREEN, tempColorText3);

        }  

    }

    if(updateDecay){
        updateDecay = false;
        if(currentPage == 1){

            uint16_t tempColorText = RED;
            uint16_t tempColorBack = WHITE;
            uint16_t tempColorText2 = BLACK;
            uint16_t tempColorText3 = RED;

            if(selectIt == 2){
                tempColorText = WHITE;
                tempColorBack = RED;
                tempColorText2 = GREEN;
                tempColorText3 = DARK_RED;
            }

            tft->setTextColor(tempColorText, tempColorBack);

            uint8_t yOffset = 22;

            if(selectIt != 2)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            else{
                if(selectIt != previousSelectIt)
                    tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            }


            yOffset = 24;
            tft->setCursor(1, yOffset);
            tft->print("DECAY:");

            tft->fillRect(112, yOffset, 14, 8, tempColorBack);

            String a = "";
            a += decay;

            tft->getTextBounds(a, 100, yOffset, &x1, &y1, &w, &h);

            int tempX = 128 - w - 1;

            tft->setTextColor(tempColorText2, tempColorBack);

            tft->setCursor(tempX, yOffset);
            tft->print(decay);

            if(preDecay == -1)
                preDecay = 0;
            
            drawVarBar(tft, decay, preDecay, 0, 15, 58, 24, 32, 8, GREEN, tempColorText3);

        }
    }

    if(updateSustain){
        updateSustain = false;
        if(currentPage == 1){

            uint16_t tempColorText = RED;
            uint16_t tempColorBack = WHITE;
            uint16_t tempColorText2 = BLACK;
            uint16_t tempColorText3 = RED;

            if(selectIt == 3){
                tempColorText = WHITE;
                tempColorBack = RED;
                tempColorText2 = GREEN;
                tempColorText3 = DARK_RED;
            }

            tft->setTextColor(tempColorText, tempColorBack);

            uint8_t yOffset = 34;

            if(selectIt != 3)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            else{
                if(selectIt != previousSelectIt)
                    tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            }

            yOffset = 36;
            tft->setCursor(1, yOffset);
            tft->print("SUSTAIN:");

            tft->fillRect(112, yOffset, 14, 8, tempColorBack);

            String a = "";
            a += sustain;

            tft->getTextBounds(a, 100, yOffset, &x1, &y1, &w, &h);

            int tempX = 128 - w - 1;

            tft->setTextColor(tempColorText2, tempColorBack);

            tft->setCursor(tempX, yOffset);
            tft->print(sustain);

            if(preSustain == -1)
                preSustain = 0;
            
            drawVarBar(tft, sustain, preSustain, 0, 15, 58, 36, 32, 8, GREEN, tempColorText3);
        }
    }

    if(updateRelease){
        updateRelease = false;
        if(currentPage == 1){

            uint16_t tempColorText = RED;
            uint16_t tempColorBack = WHITE;
            uint16_t tempColorText2 = BLACK;
            uint16_t tempColorText3 = RED;

            if(selectIt == 4){
                tempColorText = WHITE;
                tempColorBack = RED;
                tempColorText2 = GREEN;
                tempColorText3 = DARK_RED;
            }

            tft->setTextColor(tempColorText, tempColorBack);

            uint8_t yOffset = 46;
            if(selectIt != 4)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            else{
                if(selectIt != previousSelectIt)
                    tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            }

            yOffset = 48;
            tft->setCursor(1, yOffset);
            tft->print("RELEASE:");

            tft->fillRect(112, yOffset, 14, 8, tempColorBack);

            String a = "";
            a += release;

            tft->getTextBounds(a, 100, yOffset, &x1, &y1, &w, &h);

            int tempX = 128 - w - 1;

            tft->setTextColor(tempColorText2, tempColorBack);

            tft->setCursor(tempX, yOffset);
            tft->print(release);

            if(preRelease == -1)
                preRelease = 0;
            
            drawVarBar(tft, release, preRelease, 0, 15, 58, 48, 32, 8, GREEN, tempColorText3);
        }
    }

    if(updateTremolo){
        updateTremolo = false;
        if(currentPage == 1){

            uint16_t tempColorText = RED;
            uint16_t tempColorBack = WHITE;
            uint16_t tempColorText2 = BLACK;

            if(selectIt == 5){
                tempColorText = WHITE;
                tempColorBack = RED;
                tempColorText2 = GREEN;
            }

            tft->setTextColor(tempColorText, tempColorBack);

            uint8_t yOffset = 58;
            if(selectIt != 5)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            else{
                if(selectIt != previousSelectIt)
                    tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            }

            yOffset = 60;
            tft->setCursor(1, yOffset);
            tft->print("TREMOLO:");

            tft->fillRect(102, yOffset, 22, 8, tempColorBack);

            String a = "";

            if(tremolo)
                a += "ON";
            else
                a += "OFF";

            tft->getTextBounds(a, 100, yOffset, &x1, &y1, &w, &h);

            int tempX = 128 - w - 1;

            tft->setTextColor(tempColorText2, tempColorBack);

            tft->setCursor(tempX, yOffset);
            tft->print(a);
        }
    }

    if(updateVibrato){
        updateVibrato = false;
        if(currentPage == 1){

            uint16_t tempColorText = RED;
            uint16_t tempColorBack = WHITE;
            uint16_t tempColorText2 = BLACK;

            if(selectIt == 6){
                tempColorText = WHITE;
                tempColorBack = RED;
                tempColorText2 = GREEN;
            }

            tft->setTextColor(tempColorText, tempColorBack);

            uint8_t yOffset = 70;
            if(selectIt != 6)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            else{
                if(selectIt != previousSelectIt)
                    tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            }

            yOffset = 72;
            tft->setCursor(1, yOffset);
            tft->print("VIBRATO:");

            tft->fillRect(102, yOffset, 22, 8, tempColorBack);

            String a = "";

            if(vibrato)
                a += "ON";
            else
                a += "OFF";

            tft->getTextBounds(a, 100, yOffset, &x1, &y1, &w, &h);

            int tempX = 128 - w - 1;

            tft->setTextColor(tempColorText2, tempColorBack);

            tft->setCursor(tempX, yOffset);
            tft->print(a);
        }
    }

    if(updateEnvelopeType){
        updateEnvelopeType = false;
        if(currentPage == 1){

            uint16_t tempColorText = RED;
            uint16_t tempColorBack = WHITE;
            uint16_t tempColorText2 = BLACK;

            if(selectIt == 7){
                tempColorText = WHITE;
                tempColorBack = RED;
                tempColorText2 = GREEN;
            }

            tft->setTextColor(tempColorText, tempColorBack);

            uint8_t yOffset = 82;
            if(selectIt != 7)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            else{
                if(selectIt != previousSelectIt)
                    tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            }

            yOffset = 84;
            tft->setCursor(1, yOffset);
            tft->print("SUSTAIN MODE:");

            tft->fillRect(102, yOffset, 22, 8, tempColorBack);

            String a = "";

            if(envelopeType)
                a += "ON";
            else
                a += "OFF";

            tft->getTextBounds(a, 100, yOffset, &x1, &y1, &w, &h);

            int tempX = 128 - w - 1;

            tft->setTextColor(tempColorText2, tempColorBack);

            tft->setCursor(tempX, yOffset);
            tft->print(a);
        }
    }

    if(updateKeyScaleRate){
        updateKeyScaleRate = false;
        if(currentPage == 1){

            uint16_t tempColorText = RED;
            uint16_t tempColorBack = WHITE;
            uint16_t tempColorText2 = BLACK;

            if(selectIt == 8){
                tempColorText = WHITE;
                tempColorBack = RED;
                tempColorText2 = GREEN;
            }

            tft->setTextColor(tempColorText, tempColorBack);

            uint8_t yOffset = 94;
            if(selectIt != 8)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            else{
                if(selectIt != previousSelectIt)
                    tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            }

            yOffset = 96;
            tft->setCursor(1, yOffset);
            tft->print("KEY SCALE RATE:");

            tft->fillRect(102, yOffset, 22, 8, tempColorBack);

            String a = "";

            if(keyScaleRate)
                a += "ON";
            else
                a += "OFF";

            tft->getTextBounds(a, 100, yOffset, &x1, &y1, &w, &h);

            int tempX = 128 - w - 1;

            tft->setTextColor(tempColorText2, tempColorBack);

            tft->setCursor(tempX, yOffset);
            tft->print(a);
        }
    }

    if(updateWaveform){
        updateWaveform = false;
        if(currentPage == 1){

            uint16_t tempColorText = RED;
            uint16_t tempColorBack = WHITE;
            uint16_t tempColorText2 = BLACK;

            if(selectIt == 9){
                tempColorText = WHITE;
                tempColorBack = RED;
                tempColorText2 = GREEN;
            }

            tft->setTextColor(tempColorText, tempColorBack);

            uint8_t yOffset = 106;
            if(selectIt != 9)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            else{
                if(selectIt != previousSelectIt)
                    tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            }

            yOffset = 108;
            tft->setCursor(1, yOffset);
            tft->print("WAVE:");

            tft->fillRect(40, yOffset, 87, 8, tempColorBack);

            String a = "";

            switch(waveform){
                case 0:
                    a += "SINE";
                break;
                case 1:
                    a += "HALF_SINE";
                break;
                case 2:
                    a += "ABS-SINE";
                break;
                case 3:
                    a += "PULSE-SINE";
                break;
                case 4:
                    a += "SINE (EPO)";
                break;
                case 5:
                    a += "ABS_SIN (EPO)";
                break;
                case 6:
                    a += "SQUARE";
                break;
                case 7:
                    a += "DERIVED SQUARE";
                break;
            }

            tft->getTextBounds(a, 10, yOffset, &x1, &y1, &w, &h);

            int tempX = 128 - w - 1;

            tft->setTextColor(tempColorText2, tempColorBack);

            tft->setCursor(tempX, yOffset);
            tft->print(a);
        }
    }

    if(updateTotalLevel){
        updateTotalLevel = false;
        if(currentPage == 2){

            uint16_t tempColorText = RED;
            uint16_t tempColorBack = WHITE;
            uint16_t tempColorText2 = BLACK;
            uint16_t tempColorText3 = RED;

            if(selectIt == 1){
                tempColorText = WHITE;
                tempColorBack = RED;
                tempColorText2 = GREEN;
                tempColorText3 = DARK_RED;
            }

            tft->setTextColor(tempColorText, tempColorBack);

            uint8_t yOffset = 10;
            if(selectIt != 1)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            else{
                if(selectIt != previousSelectIt)
                    tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            }

            yOffset = 12;
            tft->setCursor(1, yOffset);
            tft->print("T LEVEL:");

            tft->fillRect(102, yOffset, 22, 8, tempColorBack);

            String a = "";
            a += totalLevel;

            tft->getTextBounds(a, 100, yOffset, &x1, &y1, &w, &h);

            int tempX = 128 - w - 1;

            tft->setTextColor(tempColorText2, tempColorBack);

            tft->setCursor(tempX, yOffset);
            tft->print(totalLevel);

            if(preTotalLevel == -1)
                preTotalLevel = 0;
            
            drawVarBar(tft, totalLevel, preTotalLevel, 0, 63, 58, 12, 32, 8, GREEN, tempColorText3);

        }
    }

    if(updateFreMult){
        updateFreMult = false;
        if(currentPage == 2){

            uint16_t tempColorText = RED;
            uint16_t tempColorBack = WHITE;
            uint16_t tempColorText2 = BLACK;

            if(selectIt == 2){
                tempColorText = WHITE;
                tempColorBack = RED;
                tempColorText2 = GREEN;
            }

            tft->setTextColor(tempColorText, tempColorBack);

            uint8_t yOffset = 22;
            if(selectIt != 2)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            else{
                if(selectIt != previousSelectIt)
                    tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            }


            yOffset = 24;
            tft->setCursor(1, yOffset);
            tft->print("FREQUENCY MULTI:");

            tft->fillRect(102, yOffset, 22, 8, tempColorBack);

            String a = "";
            a += freMult;

            tft->getTextBounds(a, 100, yOffset, &x1, &y1, &w, &h);

            int tempX = 128 - w - 1;

            tft->setTextColor(tempColorText2, tempColorBack);

            tft->setCursor(tempX, yOffset);
            tft->print(freMult);

        }
    }

    if(updateKeyScaleLevel){
        updateKeyScaleLevel = false;
        if(currentPage == 2){

            uint16_t tempColorText = RED;
            uint16_t tempColorBack = WHITE;
            uint16_t tempColorText2 = BLACK;

            if(selectIt == 3){
                tempColorText = WHITE;
                tempColorBack = RED;
                tempColorText2 = GREEN;
            }

            tft->setTextColor(tempColorText, tempColorBack);

            uint8_t yOffset = 34;
            if(selectIt != 3)
                tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            else{
                if(selectIt != previousSelectIt)
                    tft->fillRect(0, yOffset, 128, 12, tempColorBack);
            }

            yOffset = 36;
            tft->setCursor(1, yOffset);
            tft->print("KEY SCALE LEVEL:");

            tft->fillRect(102, yOffset, 22, 8, tempColorBack);

            String a = "";
            a += keyScaleLevel;      

            tft->getTextBounds(a, 100, yOffset, &x1, &y1, &w, &h);

            int tempX = 128 - w - 1;

            tft->setTextColor(tempColorText2, tempColorBack);

            tft->setCursor(tempX, yOffset);
            tft->print(a);

        }
    }

    //if(selectIt != 0)
        //selectIt = 0;

    previousSelectIt = selectIt;

}

void OperatorOp::print(Adafruit_ST7735 *tft, bool preScreen){
    
    if(preScreen)
        drawPreScreen(tft);
    else
        drawScreen(tft);
    
}
