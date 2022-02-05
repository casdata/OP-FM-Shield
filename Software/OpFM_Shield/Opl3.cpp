#include "Opl3.h"



Opl3::Opl3(){

    this->tempReg = 0;

    this->speedExpression = false;

    noteFNumberOPL3[0] = 345;   //C4
    noteFNumberOPL3[1] = 365;   
    noteFNumberOPL3[2] = 387;   //D4
    noteFNumberOPL3[3] = 410;
    noteFNumberOPL3[4] = 435;   //E4
    noteFNumberOPL3[5] = 460;   //F4
    noteFNumberOPL3[6] = 488;
    noteFNumberOPL3[7] = 517;   
    noteFNumberOPL3[8] = 547;
    noteFNumberOPL3[9] = 580;
    noteFNumberOPL3[10] = 614;
    noteFNumberOPL3[11] = 651;

    lowNoteFNumberOPL3[0] = 326;
    lowNoteFNumberOPL3[1] = 307;

    highNoteFNumberOPL3[0] = 690;
    highNoteFNumberOPL3[1] = 731;

    noteFNumberOPL2[0] = 363;   //C4
    noteFNumberOPL2[1] = 385;   
    noteFNumberOPL2[2] = 408;   //D4
    noteFNumberOPL2[3] = 432;
    noteFNumberOPL2[4] = 458;   //E4
    noteFNumberOPL2[5] = 485;   //F4
    noteFNumberOPL2[6] = 514;
    noteFNumberOPL2[7] = 544;   
    noteFNumberOPL2[8] = 577;
    noteFNumberOPL2[9] = 611;
    noteFNumberOPL2[10] = 647;
    noteFNumberOPL2[11] = 686;


    for(int i = 0; i < 18; i++){
        chConnections[i] = 0;
        chTlOp1[i] = 0;
        chTlOp2[i] = 0;
        chTlOp3[i] = 0;
        chTlOp4[i] = 0;
    }

}

void Opl3::init(bool op4Mode){   

    pinMode(IC, OUTPUT);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(WR, OUTPUT);
    pinMode(CS, OUTPUT);

    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);


    digitalWrite(IC, HIGH);
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(WR, HIGH);
    digitalWrite(CS, HIGH);

    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
    
    reStart(op4Mode);
    
}

void Opl3::reStart(bool op4Mode){
    reset();

    op4ModeChip = op4Mode;

    if(op4Mode)
        enableOPL3Features(true);
    else
        enableOPL3Features(false);

    setNTS(true);

    int tempAddr = 0;

    for(int i = 0; i < 9; i++){
        tempAddr = (0xC0 + i);

        write(tempAddr, 0x30);
        registersA[tempAddr] = 0x30;

        PIOC->PIO_SODR = PIO_PC19;

        write(tempAddr, 0x30);
        registersB[tempAddr] = 0x30;
    }

}

void Opl3::setSpeedExpression(bool enable){
    this->speedExpression = enable;
}

bool Opl3::getSpeedExpression(){
    return this->speedExpression;
}

void Opl3::reset(){
    digitalWrite(IC, LOW);
    delay(1);
    digitalWrite(IC, HIGH);
    
    for(int i = 0; i < 246; i++){
        registersA[i] = 0;
        registersB[i] = 0;
    }

    for(int i = 0; i < 6; i++)
        op4ChannelsA[i] = false;
    

}

void Opl3::fakeWrite(uint8_t reg, uint8_t data, bool moduleB){
    if(moduleB)
        registersB[reg] = data;
    else
        registersA[reg] = data;
}

void Opl3::write(uint8_t reg, uint8_t data, bool moduleB){
    if(moduleB)
        PIOC->PIO_SODR = PIO_PC19;

    write(reg, data);
}


void Opl3::write(uint8_t reg, uint8_t data){

    //write reg address

    PIOA->PIO_CODR = PIO_PA19;

    reg2Pins(reg);

    PIOC->PIO_CODR = PIO_PC15;
    PIOC->PIO_CODR = PIO_PC17;
    
    delayMicroseconds(2);   
    
    PIOC->PIO_SODR = PIO_PC15;
    PIOC->PIO_SODR = PIO_PC17;


    delayMicroseconds(4);   
    
    //write data 
    PIOA->PIO_SODR = PIO_PA19;

    reg2Pins(data);

    PIOC->PIO_CODR = PIO_PC15;
    PIOC->PIO_CODR = PIO_PC17;

    delayMicroseconds(2);   

    PIOC->PIO_SODR = PIO_PC15;
    PIOC->PIO_SODR = PIO_PC17;

    PIOC->PIO_CODR = PIO_PC19;

}


void Opl3::reg2Pins(uint8_t data){
    if((data & (BIT0)) == BIT0)
        PIOA->PIO_SODR = PIO_PA15;
    else
        PIOA->PIO_CODR = PIO_PA15;
        
        
    if((data & (BIT1)) == BIT1)    
        PIOD->PIO_SODR = PIO_PD1;
    else
        PIOD->PIO_CODR = PIO_PD1;
        
        
    if((data & (BIT2)) == BIT2)    
        PIOD->PIO_SODR = PIO_PD3;
    else
        PIOD->PIO_CODR = PIO_PD3;
        
        
    if((data & (BIT3)) == BIT3)    
        PIOD->PIO_SODR = PIO_PD9;
    else
        PIOD->PIO_CODR = PIO_PD9;
        

    if((data & (BIT4)) == BIT4)    
        PIOD->PIO_SODR = PIO_PD10;
    else
        PIOD->PIO_CODR = PIO_PD10;

        
    if((data & (BIT5)) == BIT5)    
        PIOC->PIO_SODR = PIO_PC2;
    else
        PIOC->PIO_CODR = PIO_PC2;
        

    if((data & (BIT6)) == BIT6)    
        PIOC->PIO_SODR = PIO_PC4;
    else
        PIOC->PIO_CODR = PIO_PC4;
    

    if((data & (BIT7)) == BIT7)    
        PIOC->PIO_SODR = PIO_PC6;
    else
        PIOC->PIO_CODR = PIO_PC6;

    
}



uint8_t Opl3::getRegisterAt(int i, bool a1Flag){
    uint8_t registerValue = 0;
    
    if(a1Flag)
        registerValue = registersB[i];
    else
        registerValue = registersA[i];

    return registerValue;    
}


uint8_t Opl3::getRegisterAtAddress(uint8_t addressSector, uint8_t channel, uint8_t opIndex){
    
    uint8_t tempAddr = getAddress(addressSector, channel, opIndex);

    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
  
    if(isRegisterA)
        preRegister = registersA[tempAddr];   
    else
        preRegister = registersB[tempAddr];  

    return preRegister; 

}


void Opl3::setAM(bool enable, uint8_t channel, uint8_t opIndex){
    uint8_t tempBit = 0;
    
    uint8_t tempAddr = getAddress(0x20, channel, opIndex);

    if(enable)
        tempBit = 0x80;
       
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
  
    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0x7f;   
    else
        preRegister = registersB[tempAddr] & 0x7f;   
        
    preRegister += tempBit;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }            

    write(tempAddr, preRegister);
     
}

bool Opl3::getAM(uint8_t channel, uint8_t opIndex){

    bool temp = false;
    
    uint8_t tempAddr = getAddress(0x20, channel, opIndex);

    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0x80;   
    else
        preRegister = registersB[tempAddr] & 0x80;   
    
    if(preRegister == 0x80)
        temp = true;


    return temp;     
}

void Opl3::setVIB(bool enable, uint8_t channel, uint8_t opIndex){
    uint8_t tempBit = 0;
    
    uint8_t tempAddr = getAddress(0x20, channel, opIndex);
    
    if(enable)
        tempBit = 0x40;
       
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xbf;   
    else
        preRegister = registersB[tempAddr] & 0xbf;   
        
    preRegister += tempBit;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }            

    write(tempAddr, preRegister);
 
}

bool Opl3::getVIB(uint8_t channel, uint8_t opIndex){

    bool temp = false;
    
    uint8_t tempAddr = getAddress(0x20, channel, opIndex);
           
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0x40;   
    else
        preRegister = registersB[tempAddr] & 0x40;   
        
    if(preRegister == 0x40)
        temp = true;

    return temp;
}


void Opl3::setEGT(bool enable, uint8_t channel, uint8_t opIndex){
    uint8_t tempBit = 0;
    
    uint8_t tempAddr = getAddress(0x20, channel, opIndex);
    
    if(enable)
        tempBit = 0x20;
       
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xdf;   
    else
        preRegister = registersB[tempAddr] & 0xdf;   
        
    preRegister += tempBit;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }            

    write(tempAddr, preRegister);
       
}

bool Opl3::getEGT(uint8_t channel, uint8_t opIndex){
    bool temp = false;
    
    uint8_t tempAddr = getAddress(0x20, channel, opIndex);

    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0x20;   
    else
        preRegister = registersB[tempAddr] & 0x20;   
        
    if(preRegister == 0x20)
        temp = true;

    return temp;       
}

void Opl3::setKSR(bool enable, uint8_t channel, uint8_t opIndex){
    uint8_t tempBit = 0;
    
    uint8_t tempAddr = getAddress(0x20, channel, opIndex);
    
    if(enable)
        tempBit = 0x10;
       
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xef;   
    else
        preRegister = registersB[tempAddr] & 0xef;   
        
    preRegister += tempBit;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }            

    write(tempAddr, preRegister);     
}

bool Opl3::getKSR(uint8_t channel, uint8_t opIndex){
    bool temp = false;
    
    uint8_t tempAddr = getAddress(0x20, channel, opIndex);
       
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0x10;   
    else
        preRegister = registersB[tempAddr] & 0x10;   
        
    if(preRegister == 0x10)
        temp = true;

    return temp;   
}


void Opl3::setMULT(uint8_t multiple, uint8_t channel, uint8_t opIndex){
    uint8_t postData = multiple & 0xf;
    
    uint8_t tempAddr = getAddress(0x20, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xf0;   
    else
        preRegister = registersB[tempAddr] & 0xf0;   
        
    preRegister += postData;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }  

    write(tempAddr, preRegister);     
}

uint8_t Opl3::getMULT(uint8_t channel, uint8_t opIndex){    
    uint8_t tempAddr = getAddress(0x20, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xf;   
    else
        preRegister = registersB[tempAddr] & 0xf;   
          
    return preRegister; 
}

void Opl3::setKSL(uint8_t keyScaleLevel, uint8_t channel, uint8_t opIndex){
    uint8_t postData = (keyScaleLevel & 0x3) << 6;
    
    uint8_t tempAddr = getAddress(0x40, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0x3f;   
    else
        preRegister = registersB[tempAddr] & 0x3f;   
        
    preRegister += postData;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }  

    write(tempAddr, preRegister); 
}

uint8_t Opl3::getKSL(uint8_t channel, uint8_t opIndex){
    
    uint8_t tempAddr = getAddress(0x40, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xC0;   
    else
        preRegister = registersB[tempAddr] & 0xC0;   
        
    preRegister >>= 6;

    return preRegister;
}

void Opl3::setTL(uint8_t totalLevel, uint8_t channel, uint8_t opIndex, bool initMode){
    uint8_t postData = totalLevel & 0x3f;


    if(initMode){
        switch(opIndex){
            case 1:
                chTlOp1[(channel - 1)] = totalLevel;
            break;
            case 2:
                chTlOp2[(channel - 1)] = totalLevel;
            break;
            case 3:
                chTlOp3[(channel - 1)] = totalLevel;
            break;
            case 4:
                chTlOp4[(channel - 1)] = totalLevel;
            break;
        }
    }
    
    uint8_t tempAddr = getAddress(0x40, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xc0;   
    else
        preRegister = registersB[tempAddr] & 0xc0;   
        
    preRegister += postData;


    if(initMode){
        if(isRegisterA)
            registersA[tempAddr] = preRegister;
        else{
            registersB[tempAddr] = preRegister;
            PIOC->PIO_SODR = PIO_PC19;
        }  
    }
    else{
        if(!isRegisterA)
            PIOC->PIO_SODR = PIO_PC19;
    }
    

    write(tempAddr, preRegister);    
}

uint8_t Opl3::getTL(uint8_t channel, uint8_t opIndex){
    
    uint8_t tempAddr = getAddress(0x40, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0x3F;   
    else
        preRegister = registersB[tempAddr] & 0x3F;   

    return preRegister; 
}


void Opl3::setAR(uint8_t attackRate, uint8_t channel, uint8_t opIndex){
    uint8_t postData = (attackRate & 0xf) << 4;
    
    uint8_t tempAddr = getAddress(0x60, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xf;   
    else
        preRegister = registersB[tempAddr] & 0xf;   
        
    preRegister += postData;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }  

    write(tempAddr, preRegister);
}

uint8_t Opl3::getAR(uint8_t channel, uint8_t opIndex){
    uint8_t tempAddr = getAddress(0x60, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xF0;   
    else
        preRegister = registersB[tempAddr] & 0xF0;   

    preRegister >>= 4;

    return preRegister;
}
        
        
void Opl3::setDR(uint8_t decayRate, uint8_t channel, uint8_t opIndex){
    uint8_t postData = decayRate & 0xf;
    
    uint8_t tempAddr = getAddress(0x60, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xf0;   
    else
        preRegister = registersB[tempAddr] & 0xf0;   
        
    preRegister += postData;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }  

    write(tempAddr, preRegister);
}

uint8_t Opl3::getDR(uint8_t channel, uint8_t opIndex){
    
    uint8_t tempAddr = getAddress(0x60, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xF;   
    else
        preRegister = registersB[tempAddr] & 0xF;   

    return preRegister;
}

void Opl3::setSL(uint8_t sustainLevel, uint8_t channel, uint8_t opIndex){
    uint8_t postData = (sustainLevel & 0xf) << 4;
    
    uint8_t tempAddr = getAddress(0x80, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xf;   
    else
        preRegister = registersB[tempAddr] & 0xf;   
        
    preRegister += postData;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }  

    write(tempAddr, preRegister);      
}

uint8_t Opl3::getSL(uint8_t channel, uint8_t opIndex){
    
    uint8_t tempAddr = getAddress(0x80, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
        
    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xF0;   
    else
        preRegister = registersB[tempAddr] & 0xF0;   

    preRegister >>= 4;  

    return preRegister;  
}

void Opl3::setRR(uint8_t releaseRate, uint8_t channel, uint8_t opIndex){
    uint8_t postData = releaseRate & 0xf;
    
    uint8_t tempAddr = getAddress(0x80, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xf0;   
    else
        preRegister = registersB[tempAddr] & 0xf0;   
        
    preRegister += postData;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }  

    write(tempAddr, preRegister);   
}

uint8_t Opl3::getRR(uint8_t channel, uint8_t opIndex){    
    uint8_t tempAddr = getAddress(0x80, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xF;   
    else
        preRegister = registersB[tempAddr] & 0xF;   
        
    return preRegister;
}

void Opl3::setWS(uint8_t waveSelect, uint8_t channel, uint8_t opIndex){
    uint8_t tempAddr = getAddress(0xE0, channel, opIndex);
    
    uint8_t preRegister = waveSelect & 0x7;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }  

    write(tempAddr, preRegister);
}

uint8_t Opl3::getWS(uint8_t channel, uint8_t opIndex){
    uint8_t tempAddr = getAddress(0xE0, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0x7;
    else
        preRegister = registersB[tempAddr] & 0x7;

    return preRegister;
}

void Opl3::setAudioBalance(uint8_t balance, uint8_t channel){
    uint8_t postData = 3;   //center
    
    switch(balance){
        case 0:             
            postData = 2;   //left    
        break;
        case 2:             
            postData = 1;   //right
        break;   
    }
    
    postData = postData << 4;
    
    uint8_t tempAddr = getAddress(0xC0, channel, 1);
    uint8_t tempAddr2 = getAddress(0xC0, channel, 3);
    
    uint8_t preRegister = 0;
    uint8_t preRegister2 = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isOp4Channel){
        if(isRegisterA){
            preRegister = registersA[tempAddr] & 0xcf;
            preRegister2 = registersA[tempAddr2] & 0xcf;
        }
        else{
            preRegister = registersB[tempAddr] & 0xcf;
            preRegister2 = registersB[tempAddr2] & 0xcf;
        }

        preRegister += postData;
        preRegister2 += postData;

        if(isRegisterA){
            registersA[tempAddr] = preRegister;
            registersA[tempAddr2] = preRegister2;
        }
        else{
            registersB[tempAddr] = preRegister;
            registersB[tempAddr2] = preRegister2;
            PIOC->PIO_SODR = PIO_PC19;
        }  

        write(tempAddr, preRegister);

        if(!isRegisterA)
            PIOC->PIO_SODR = PIO_PC19;

        write(tempAddr2, preRegister2);
    }
    else{
        if(isRegisterA)
            preRegister = registersA[tempAddr] & 0xcf;
        else
            preRegister = registersB[tempAddr] & 0xcf;

        preRegister += postData;

        if(isRegisterA)
            registersA[tempAddr] = preRegister;
        else{
            registersB[tempAddr] = preRegister;
            PIOC->PIO_SODR = PIO_PC19;
        }  

        write(tempAddr, preRegister);
    }          
}

uint8_t Opl3::getAudioBalance(uint8_t channel){

    bool temp = false;
    
    uint8_t tempAddr = getAddress(0xC0, channel, 1);
       
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0x30;   
    else
        preRegister = registersB[tempAddr] & 0x30;   

    preRegister >> 4;

    switch(preRegister){
        case 1:                             //right
            preRegister = 2;
        break;
        case 2:                             //left
            preRegister = 0;
        break;
        case 3:                             //center
            preRegister = 1;
        break;
    }

    return temp;           
}

void Opl3::setFB(uint8_t feedback, uint8_t channel, uint8_t opIndex){
    uint8_t postData = (feedback & 0x7) << 1;
    
    uint8_t tempAddr = getAddress(0xC0, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xf1;
    else
        preRegister = registersB[tempAddr] & 0xf1;

    preRegister += postData;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }  

    write(tempAddr, preRegister);        
}

uint8_t Opl3::getFB(uint8_t channel, uint8_t opIndex){
    uint8_t tempAddr = getAddress(0xC0, channel, opIndex);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xE;
    else
        preRegister = registersB[tempAddr] & 0xE;

    preRegister >>= 1;

    return preRegister;     
}

void Opl3::setCNT(uint8_t connection, uint8_t channel){
    uint8_t postData = 0;
    uint8_t postDataB = 0;


    switch(connection){
        case 1:
        case 3:
            postData = 1;
            postDataB = 0;
        break;
        case 4:
            postData = 0;
            postDataB = 1;
        break;
        case 5:
            postData = 1;
            postDataB = 1;
        break;  
    }
    
    chConnections[(channel - 1)] = connection;

    uint8_t tempAddr = getAddress(0xC0, channel, 1);
    uint8_t tempAddr2 = getAddress(0xC0, channel, 3);
    
    uint8_t preRegister = 0;
    uint8_t preRegister2 = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;
    
    if(isOp4Channel){
        if(isRegisterA){
            preRegister = registersA[tempAddr] & 0xfe;
            preRegister2 = registersA[tempAddr2] & 0xfe;
        }
        else{
            preRegister = registersB[tempAddr] & 0xfe;
            preRegister2 = registersB[tempAddr2] & 0xfe;
        }

        preRegister += postData;
        preRegister2 += postDataB; 

        if(isRegisterA){
            registersA[tempAddr] = preRegister;
            registersA[tempAddr2] = preRegister2;
        }
        else{
            registersB[tempAddr] = preRegister;
            registersB[tempAddr2] = preRegister2;
            PIOC->PIO_SODR = PIO_PC19;
        }  

        write(tempAddr, preRegister);

        if(!isRegisterA)
            PIOC->PIO_SODR = PIO_PC19;

        write(tempAddr2, preRegister2);
    }
    else{
        if(isRegisterA)
            preRegister = registersA[tempAddr] & 0xfe;
        else
            preRegister = registersB[tempAddr] & 0xfe;

        preRegister += postData;

        if(isRegisterA)
            registersA[tempAddr] = preRegister;
        else{
            registersB[tempAddr] = preRegister;
            PIOC->PIO_SODR = PIO_PC19;
        }  

        write(tempAddr, preRegister);
    }            
}

uint8_t Opl3::getCNT(uint8_t channel){

    uint8_t tempAddr = getAddress(0xC0, channel, 1);
    uint8_t tempAddr2 = getAddress(0xC0, channel, 3);
    
    uint8_t preRegister = 0;
    uint8_t preRegister2 = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isOp4Channel){
        if(isRegisterA){
            preRegister = registersA[tempAddr] & 0x1;
            preRegister2 = registersA[tempAddr2] & 0x1;
        }
        else{
            preRegister = registersB[tempAddr] & 0x1;
            preRegister2 = registersB[tempAddr2] & 0x1;
        }     

        if((preRegister == 0) && (preRegister2 == 0))
            preRegister = 2;
        else if((preRegister == 1) && (preRegister2 == 0))
            preRegister = 3;
        else if((preRegister == 0) && (preRegister2 == 1))
            preRegister = 4;
        else if((preRegister == 1) && (preRegister2 == 1))
            preRegister = 5;

    }
    else{
        if(isRegisterA)
            preRegister = registersA[tempAddr] & 0x1;
        else
            preRegister = registersB[tempAddr] & 0x1;

    }

    return preRegister;              
}

void Opl3::rythm(bool enable){
    uint8_t tempBit = 0;
        
    if(enable)       
        tempBit = 0x20;
        
    uint8_t preRegister = 0;
    
    preRegister = registersA[0xBD] & 0xDF;
        
    preRegister += tempBit;
    registersA[0xBD] = preRegister;
    
    write(0xBD, preRegister);    
}

void Opl3::bass(bool play){
    uint8_t tempBit = 0;
        
    if(play)       
        tempBit = 0x10;
        
    uint8_t preRegister = 0;
    
    preRegister = registersA[0xBD] & 0xEF;
        
    preRegister += tempBit;
    registersA[0xBD] = preRegister;
    
    write(0xBD, preRegister);  
}

void Opl3::snare(bool play){
    uint8_t tempBit = 0;
        
    if(play)       
        tempBit = 0x8;
        
    uint8_t preRegister = 0;
    
    preRegister = registersA[0xBD] & 0xF7;
        
    preRegister += tempBit;
    registersA[0xBD] = preRegister;
    
    write(0xBD, preRegister);  
}

void Opl3::tom(bool play){
    uint8_t tempBit = 0;
        
    if(play)       
        tempBit = 0x4;
        
    uint8_t preRegister = 0;
    
    preRegister = registersA[0xBD] & 0xFB;
        
    preRegister += tempBit;
    registersA[0xBD] = preRegister;
    
    write(0xBD, preRegister);  
}

void Opl3::cymbal(bool play){
    uint8_t tempBit = 0;
        
    if(play)       
        tempBit = 0x2;
        
    uint8_t preRegister = 0;
    
    preRegister = registersA[0xBD] & 0xFD;
        
    preRegister += tempBit;
    registersA[0xBD] = preRegister;
    
    write(0xBD, preRegister);  
}

void Opl3::hihat(bool play){
    uint8_t tempBit = 0;
        
    if(play)       
        tempBit = 0x1;
        
    uint8_t preRegister = 0;
    
    preRegister = registersA[0xBD] & 0xFE;
        
    preRegister += tempBit;
    registersA[0xBD] = preRegister;
    
    write(0xBD, preRegister);  
}

void Opl3::setKON(bool enable, uint8_t channel){
    uint8_t postData = 0;

    if(enable)
        postData = 0x20;

    uint8_t tempAddr = getAddress(0xB0, channel, 1);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xdf;   
    else
        preRegister = registersB[tempAddr] & 0xdf;   
        
    preRegister += postData;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }  

    write(tempAddr, preRegister);
    
}

void Opl3::setBlock(uint8_t block, uint8_t channel){         
    uint8_t postData = (block & 0x7) << 2;
    
    uint8_t tempAddr = getAddress(0xB0, channel, 1);
    
    uint8_t preRegister = 0;

    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xe3;   
    else
        preRegister = registersB[tempAddr] & 0xe3;   
        
    preRegister += postData;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }  

    write(tempAddr, preRegister);  
    
}

void Opl3::setFNumber(uint8_t note, uint8_t channel){
    uint8_t ld = 0;
    uint8_t hd = 0;

    if(op4ModeChip){
        ld = noteFNumberOPL3[note] & 0xff;
        hd = (noteFNumberOPL3[note] & 0x300) >> 8;
    }
    else{
        ld = noteFNumberOPL2[note] & 0xff;
        hd = (noteFNumberOPL2[note] & 0x300) >> 8;
    }
    
    uint8_t tempAddr = getAddress(0xA0, channel, 1);
    
    uint8_t preRegister = 0;
    
    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        registersA[tempAddr] = ld;
    else{
        registersB[tempAddr] = ld;
        PIOC->PIO_SODR = PIO_PC19;
    }
        
    write(tempAddr, ld);

    
    tempAddr = getAddress(0xB0, channel, 1); 

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xfc;   
    else
        preRegister = registersB[tempAddr] & 0xfc;   
        
    preRegister += hd;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }  

    write(tempAddr, preRegister);  
 
}

void Opl3::playNote(uint8_t channel, uint8_t block, uint8_t note, uint8_t bendData, bool rightBend, uint8_t speed){  

    releaseNote(channel);

    if(speedExpression)
        setSpeedTL(channel, speed);
    else
        setSpeedTL(channel);
    

    setBlock(block, channel);
    //setFNumber(note, channel);
    bendNote(channel, note, bendData, rightBend);
    setKON(true, channel);
}

void Opl3::releaseNote(uint8_t channel){
    setKON(false, channel);
}

void Opl3::setSpeedTL(uint8_t channel){
    int tempChannel = channel - 1;
    int tempConnection = chConnections[tempChannel];
    int maxVolA = 0;
    int maxVolB = 0;
    int maxVolC = 0;


    switch(tempConnection){
        case 0:
            maxVolA = chTlOp2[tempChannel];

            setTL(maxVolA, channel, 2, false);

        break;
        case 1:
            maxVolA = chTlOp1[tempChannel];
            maxVolB = chTlOp2[tempChannel];

            setTL(maxVolA, channel, 1, false);

            setTL(maxVolB, channel, 2, false);

        break;
        case 2:
            maxVolA = chTlOp4[tempChannel];

            setTL(maxVolA, channel, 4, false);
        break;
        case 3:
            maxVolA = chTlOp1[tempChannel];
            maxVolB = chTlOp4[tempChannel];

            setTL(maxVolA, channel, 1, false);

            setTL(maxVolB, channel, 4, false);
        break;
        case 4:
            maxVolA = chTlOp2[tempChannel];
            maxVolB = chTlOp4[tempChannel];

            setTL(maxVolA, channel, 2, false);

            setTL(maxVolB, channel, 4, false);

        break;
        case 5:
            maxVolA = chTlOp1[tempChannel];
            maxVolB = chTlOp3[tempChannel];
            maxVolC = chTlOp4[tempChannel];

            setTL(maxVolA, channel, 1, false);

            setTL(maxVolB, channel, 3, false);

            setTL(maxVolC, channel, 4, false);

        break;
    }
}

void Opl3::setSpeedTL(uint8_t channel, uint8_t speed){
    int tempChannel = channel - 1;
    int tempConnection = chConnections[tempChannel];
    int maxVolA = 0;
    int maxVolB = 0;
    int maxVolC = 0;

    int minVolA = 1;
    int minVolB = 1;
    int minVolC = 1;

    int newVol = 0;

    int delta = 20;

    if(speed > 100)
        speed = 100;

    switch(tempConnection){
        case 0:
            maxVolA = chTlOp2[tempChannel];
            minVolA = maxVolA + delta;

            newVol = volSpeedEqu(minVolA, maxVolA, speed);

            setTL(newVol, channel, 2, false);


        break;
        case 1:
            maxVolA = chTlOp1[tempChannel];
            maxVolB = chTlOp2[tempChannel];
            minVolA = maxVolA + delta;
            minVolB = maxVolB + delta;

            newVol = volSpeedEqu(minVolA, maxVolA, speed);
            setTL(newVol, channel, 1, false);


            newVol = volSpeedEqu(minVolB, maxVolB, speed);
            setTL(newVol, channel, 2, false);
            

        break;
        case 2:
            maxVolA = chTlOp4[tempChannel];
            minVolA = maxVolA + delta;

            newVol = volSpeedEqu(minVolA, maxVolA, speed);
            setTL(newVol, channel, 4, false);


        break;
        case 3:
            maxVolA = chTlOp1[tempChannel];
            maxVolB = chTlOp4[tempChannel];
            minVolA = maxVolA + delta;
            minVolB = maxVolB + delta;

            newVol = volSpeedEqu(minVolA, maxVolA, speed);
            setTL(newVol, channel, 1, false);

            newVol = volSpeedEqu(minVolB, maxVolB, speed);
            setTL(newVol, channel, 4, false);

        break;
        case 4:
            maxVolA = chTlOp2[tempChannel];
            maxVolB = chTlOp4[tempChannel];
            minVolA = maxVolA + delta;
            minVolB = maxVolB + delta;

            newVol = volSpeedEqu(minVolA, maxVolA, speed);
            setTL(newVol, channel, 2, false);


            newVol = volSpeedEqu(minVolB, maxVolB, speed);
            setTL(newVol, channel, 4, false);


        break;
        case 5:
            maxVolA = chTlOp1[tempChannel];
            maxVolB = chTlOp3[tempChannel];
            maxVolC = chTlOp4[tempChannel];
            minVolA = maxVolA + delta;
            minVolB = maxVolB + delta;
            minVolC = maxVolC + delta;

            newVol = volSpeedEqu(minVolA, maxVolA, speed);
            setTL(newVol, channel, 1, false);

            newVol = volSpeedEqu(minVolB, maxVolB, speed);
            setTL(newVol, channel, 3, false);

            newVol = volSpeedEqu(minVolC, maxVolC, speed);
            setTL(newVol, channel, 4, false);

        break;
    }
}

int Opl3::volSpeedEqu(int minV, int maxV, int speed){
    int tempVol;

    if(minV > 63)
        minV = 63;

    tempVol = minV - (((speed - 1)*(minV - maxV))/99);


    
    return tempVol;
}


void Opl3::bendNote(uint8_t channel, uint8_t note, uint8_t bendData, bool rightBend){

    uint8_t ld = 0;
    uint8_t hd = 0;

    int minF = 0;
    int maxF = 1;

    minF = noteFNumberOPL3[note];

    int bendNote = 0;

    if(rightBend){
        switch(note){
            case 10:
                maxF = highNoteFNumberOPL3[0];
            break;
            case 11:
                maxF = highNoteFNumberOPL3[1];
            break;
            default:
                maxF = noteFNumberOPL3[(note + 2)];
            break;
        }  
        bendNote = minF + ((bendData * (maxF - minF))/63);
    }
    else{
        switch(note){
            case 0:
                maxF = lowNoteFNumberOPL3[1];
            break;
            case 1:
                maxF = lowNoteFNumberOPL3[0];
            break;
            default:
                maxF = noteFNumberOPL3[(note - 2)];
            break;
        }
        bendNote = minF - ((bendData * (minF - maxF))/ 63);
    }


    ld = bendNote & 0xff;
    hd = (bendNote & 0x300) >> 8;

    uint8_t tempAddr = getAddress(0xA0, channel, 1);
    
    uint8_t preRegister = 0;
    
    bool isRegisterA = true;

    if(isRegisterB(channel))
        isRegisterA = false;

    if(isRegisterA)
        registersA[tempAddr] = ld;
    else{
        registersB[tempAddr] = ld;
        PIOC->PIO_SODR = PIO_PC19;
    }
        
    write(tempAddr, ld);
    
    tempAddr = getAddress(0xB0, channel, 1); 

    if(isRegisterA)
        preRegister = registersA[tempAddr] & 0xfc;   
    else
        preRegister = registersB[tempAddr] & 0xfc;   
        
    preRegister += hd;

    if(isRegisterA)
        registersA[tempAddr] = preRegister;
    else{
        registersB[tempAddr] = preRegister;
        PIOC->PIO_SODR = PIO_PC19;
    }  

    write(tempAddr, preRegister);  

}

void Opl3::setDAM(bool enable){   
    uint8_t tempBit = 0;
        
    if(enable)       
        tempBit = 0x80;
        
    uint8_t preRegister = 0;
    
    preRegister = registersA[0xBD] & 0x7f;
        
    preRegister += tempBit;
    registersA[0xBD] = preRegister;
    
    write(0xBD, preRegister);        
}

bool Opl3::getDAM(){  

    bool temp = false;

    uint8_t preRegister = registersA[0xBD] & 0x80;

    if(preRegister == 0x80)
        temp = true;

    return temp;
}


void Opl3::setDVB(bool enable){
    uint8_t tempBit = 0;
    
    if(enable == 1)
        tempBit = 0x40;
        
    uint8_t preRegister = 0;
    
    preRegister = registersA[0xBD] & 0xBF;
        
    preRegister += tempBit;
    registersA[0xBD] = preRegister;
        
    write(0xBD, preRegister);        
}

bool Opl3::getDVB(){

    bool temp = false;
        
    uint8_t preRegister = registersA[0xBD] & 0x40;

    if(preRegister == 0x40)      
        temp = true;

    return temp;
}

void Opl3::setChannel4OpMode(bool enable, uint8_t channel){

    uint8_t tempBit = 0;
    uint8_t maskWord = 0;

    uint8_t preRegister = 0;

    switch(channel){
        case 1:
            maskWord = 0x3e;
            op4ChannelsA[0] = false;
        break;
        case 2:
            maskWord = 0x3d;
            op4ChannelsA[1] = false;
        break;
        case 3:
            maskWord = 0x3b;
            op4ChannelsA[2] = false;
      
        break;
        case 4:
            maskWord = 0x37;
            op4ChannelsA[3] = false;
        break;
        case 5:
            maskWord = 0x2f;
            op4ChannelsA[4] = false;
        break;
        case 6:
            maskWord = 0x1f;
            op4ChannelsA[5] = false;
        break;

    }

    if(enable){
        switch(channel){
            case 1:
                tempBit = 0x1;
                op4ChannelsA[0] = true;
            break;
            case 2:
                tempBit = 0x2;
                op4ChannelsA[1] = true;
            break;
            case 3:
                tempBit = 0x4;
                op4ChannelsA[2] = true;
            break;
            case 4:
                tempBit = 0x8;
                op4ChannelsA[3] = true;
            break;
            case 5:
                tempBit = 0x10;
                op4ChannelsA[4] = true;
            break;
            case 6:
                tempBit = 0x20;
                op4ChannelsA[5] = true;
            break;
        }
    }

    preRegister = registersB[0x4] & maskWord;
    preRegister += tempBit;
    registersB[0x4] = preRegister;

    PIOC->PIO_SODR = PIO_PC19;
    write(0x4, preRegister);
}

bool Opl3::getChannel4OpMode(uint8_t channel){
    
    bool temp = false;

    uint8_t preRegister = 0;

    switch(channel){
        case 1:
            preRegister = registersB[0x4] & 0x1;
        break;
        case 2:
            preRegister = registersB[0x4] & 0x2;
            preRegister >>= 1;
        break;
        case 3:
            preRegister = registersB[0x4] & 0x4;
            preRegister >>= 2;         
        break;
        case 4:
            preRegister = registersB[0x4] & 0x8;
            preRegister >>= 3; 
        break;
        case 5:
            preRegister = registersB[0x4] & 0x10;
            preRegister >>= 4; 
        break;
        case 6:
            preRegister = registersB[0x4] & 0x20;
            preRegister >>= 5; 
        break;
    }

    if(preRegister == 1)
        temp = true;

    return temp;
}

void Opl3::enableOPL3Features(bool enable){ 
    if(enable){
        registersB[0x5] = 1;
        registersA[0x1] = 0;
    }
    else{
        registersB[0x5] = 0;
        registersA[0x1] = 0x20;
    }

    PIOC->PIO_SODR = PIO_PC19;
    write(0x5, registersB[0x5]);

    write(0x1, registersA[0x1]);

}

void Opl3::setNTS(bool enable){                                                  //keyboard split selection

    if(enable)
        registersA[0x8] = 64;
    else
        registersA[0x8] = 0;

    write(0x8, registersA[8]);
}

void Opl3::loadAllChannelsOpl3(uint8_t addressSector, uint8_t data, uint8_t opIndex){
    uint8_t tempAddr = 0;

    for(int i = 1; i < 4; i++){
        tempAddr = getAddress(addressSector, i, opIndex);
        registersA[tempAddr] = data;
        write(tempAddr, data);
    }

    for(int i = 10; i <13; i++){
        tempAddr = getAddress(addressSector, i, opIndex);
        registersB[tempAddr] = data;
        PIOC->PIO_SODR = PIO_PC19;
        write(tempAddr, data);
    }
}

void Opl3::loadLeftChannelsOpl3(uint8_t addressSector, uint8_t data, uint8_t opIndex){
    uint8_t tempAddr = 0;

    for(int i = 1; i < 4; i++){
        tempAddr = getAddress(addressSector, i, opIndex);
        registersA[tempAddr] = data;
        write(tempAddr, data);
    }

    for(int i = 10; i <12; i++){
        tempAddr = getAddress(addressSector, i, opIndex);
        registersB[tempAddr] = data;
        PIOC->PIO_SODR = PIO_PC19;
        write(tempAddr, data);
    }
}

void Opl3::loadAllChannelsOpl2(uint8_t addressSector, uint8_t data, uint8_t opIndex){

    uint8_t tempAddr = 0;
    
    for(int i = 1; i < 7; i++){
        tempAddr = getAddress(addressSector, i, opIndex);
        registersA[tempAddr] = data;
        write(tempAddr, data);
    }

    for(int i = 10; i < 16; i++){
        tempAddr = getAddress(addressSector, i, opIndex);
        registersB[tempAddr] = data;
        PIOC->PIO_SODR = PIO_PC19;
        write(tempAddr, data);
    }   

}

void Opl3::loadLeftChannelsOpl2(uint8_t addressSector, uint8_t data, uint8_t opIndex){
    uint8_t tempAddr = 0;

    for(int i = 1; i < 6; i++){
        tempAddr = getAddress(addressSector, i, opIndex);
        registersA[tempAddr] = data;
        write(tempAddr, data);
    }
}

void Opl3::loadRightChannelsOpl2(uint8_t addressSector, uint8_t data, uint8_t opIndex){
    uint8_t tempAddr = 0;

    tempAddr = getAddress(addressSector, 12, opIndex);
    registersB[tempAddr] = data;
    PIOC->PIO_SODR = PIO_PC19;
    write(tempAddr, data);

    for(int i = 15; i < 19; i++){
        tempAddr = getAddress(addressSector, i, opIndex);
        registersB[tempAddr] = data;
        PIOC->PIO_SODR = PIO_PC19;
        write(tempAddr, data);
    }
}

void Opl3::loadVRegAllChannelsOpl2(uint8_t opIndex, bool tlMode, bool cntMode){
    uint8_t tempData = 0;
    
    for(int i = 1; i < 7; i++){

        if(cntMode)
            chConnections[(i - 1)] = getCNT(i);

        if(tlMode){
            switch(opIndex){
                case 1:
                    chTlOp1[(i - 1)] = getTL(i, 1);
                break;
                case 2:
                    chTlOp2[(i - 1)] = getTL(i, 2);  
                break;
            }
        }            
    }

    for(int i = 10; i < 16; i++){
        if(cntMode)
            chConnections[(i - 1)] = getCNT(i);

        if(tlMode){
            switch(opIndex){
                case 1:
                    chTlOp1[(i - 1)] = getTL(i, 1);
                break;
                case 2:
                    chTlOp2[(i - 1)] = getTL(i, 2);  
                break;
            }
        }  
    } 
}

void Opl3::loadVRegAllChannelsOpl3(uint8_t opIndex, bool tlMode, bool cntMode){
    for(int i = 1; i < 4; i++){
        if(cntMode)
            chConnections[(i - 1)] = getCNT(i);

        if(tlMode){
            switch(opIndex){
                case 1:
                    chTlOp1[(i - 1)] = getTL(i, 1);
                break;
                case 2:
                    chTlOp2[(i - 1)] = getTL(i, 2);  
                break;
                case 3:
                    chTlOp3[(i - 1)] = getTL(i, 3);
                break;
                case 4:
                    chTlOp4[(i - 1)] = getTL(i, 4);
                break;
            }
        }          
    }

    for(int i = 10; i <13; i++){
        if(cntMode)
            chConnections[(i - 1)] = getCNT(i);

        if(tlMode){
            switch(opIndex){
                case 1:
                    chTlOp1[(i - 1)] = getTL(i, 1);
                break;
                case 2:
                    chTlOp2[(i - 1)] = getTL(i, 2);  
                break;
                case 3:
                    chTlOp3[(i - 1)] = getTL(i, 3);
                break;
                case 4:
                    chTlOp4[(i - 1)] = getTL(i, 4);
                break;
            }
        }   
    }
}

void Opl3::loadVRegAllChannelsOpl2(){
    uint8_t tempData = 0;
    
    for(int i = 1; i < 7; i++){
        chConnections[(i - 1)] = getCNT(i);

        chTlOp1[(i - 1)] = getTL(i, 1);
        chTlOp2[(i - 1)] = getTL(i, 2);  
              
    }

    for(int i = 10; i < 16; i++){
        chConnections[(i - 1)] = getCNT(i);

        chTlOp1[(i - 1)] = getTL(i, 1);
        chTlOp2[(i - 1)] = getTL(i, 2);
    }   
}



void Opl3::loadVRegLeftChannelsOpl2(){
    for(int i = 1; i < 6; i++){
        chConnections[(i - 1)] = getCNT(i);

        chTlOp1[(i - 1)] = getTL(i, 1);
        chTlOp2[(i - 1)] = getTL(i, 2);
        
    }
}

void Opl3::loadVRegRightChannelsOpl2(){             //here
    chConnections[11] = getCNT(12);

    chTlOp1[11] = getTL(12, 1);
    chTlOp2[11] = getTL(12, 2);

    for(int i = 15; i < 19; i++){
        chConnections[(i - 1)] = getCNT(i);

        chTlOp1[(i - 1)] = getTL(i, 1);
        chTlOp2[(i - 1)] = getTL(i, 2);
    }
}

void Opl3::loadVRegAllChannelsOpl3(){
    for(int i = 1; i < 4; i++){
        chConnections[(i - 1)] = getCNT(i);
        
        chTlOp1[(i - 1)] = getTL(i, 1);
        chTlOp2[(i - 1)] = getTL(i, 2);
        chTlOp3[(i - 1)] = getTL(i, 3);
        chTlOp4[(i - 1)] = getTL(i, 4);
        
    }

    for(int i = 10; i <13; i++){
        chConnections[(i - 1)] = getCNT(i);

        chTlOp1[(i - 1)] = getTL(i, 1);
        chTlOp2[(i - 1)] = getTL(i, 2);
        chTlOp3[(i - 1)] = getTL(i, 3);
        chTlOp4[(i - 1)] = getTL(i, 4);
    }
}

        
void Opl3::loadVRegLeftChannelsOp3(){               //here
    for(int i = 1; i < 4; i++){
        chConnections[(i - 1)] = getCNT(i);

        chTlOp1[(i - 1)] = getTL(i, 1);
        chTlOp2[(i - 1)] = getTL(i, 2);
        chTlOp3[(i - 1)] = getTL(i, 3);
        chTlOp4[(i - 1)] = getTL(i, 4);
    }

    for(int i = 10; i < 12; i++){
        chConnections[(i - 1)] = getCNT(i);

        chTlOp1[(i - 1)] = getTL(i, 1);
        chTlOp2[(i - 1)] = getTL(i, 2);
        chTlOp3[(i - 1)] = getTL(i, 3);
        chTlOp4[(i - 1)] = getTL(i, 4);
    }
}

bool Opl3::isRegisterB(uint8_t channel){
    bool registerB = false;

    if(channel >= 10)
        registerB = true;

    switch(channel){
        case 4:
            if(op4ChannelsA[3])
                registerB = true;
            
        break;
        case 5:
            if(op4ChannelsA[4])
                registerB = true;
        break;
        case 6:
            if(op4ChannelsA[5])
                registerB = true;
        break;
    }

    return registerB;
}

uint8_t Opl3::getAddress(uint8_t addressSector, uint8_t channel, uint8_t opIndex){
    uint8_t addr = 0;
    bool op4State = false;

    isOp4Channel = false;

    if(channel < 1)
        channel = 1;
    
    if(channel > 18)
        channel = 18;

    if(opIndex < 1)
        opIndex = 1;
    
    if(opIndex > 4)
        opIndex = 4;

    switch(addressSector){
        case 0x20:
            switch(channel){
                case 1:
                case 10:
                    switch(opIndex){
                        case 1:
                            addr = 0x20; 
                        break;
                        case 2:
                            addr = 0x23;
                        break;
                        case 3:
                            addr = 0x28;
                        break;
                        case 4:
                            addr = 0x2b;
                        break;
                    }
                break;
                case 2:
                case 11:
                    switch(opIndex){
                        case 1:
                            addr = 0x21;
                        break;
                        case 2:
                            addr = 0x24;
                        break;
                        case 3:
                            addr = 0x29;
                        break;
                        case 4:
                            addr = 0x2c;
                        break;
                    }
                break;
                case 3:
                case 12:
                    switch(opIndex){
                        case 1:
                            addr = 0x22;
                        break;
                        case 2:
                            addr = 0x25;
                        break;
                        case 3:
                            addr = 0x2a;
                        break;
                        case 4:
                            addr = 0x2d;
                        break;
                    }
                break;
                case 4:
                    op4State = op4ChannelsA[3];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0x20; 
                            break;
                            case 2:
                                addr = 0x23;
                            break;
                            case 3:
                                addr = 0x28;
                            break;
                            case 4:
                                addr = 0x2b;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0x28;
                            break;
                            case 2:
                                addr = 0x2b;
                            break;
                        }
                    }
                break;
                case 5:
                    op4State = op4ChannelsA[4];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0x21;
                            break;
                            case 2:
                                addr = 0x24;
                            break;
                            case 3:
                                addr = 0x29;
                            break;
                            case 4:
                                addr = 0x2c;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0x29;
                            break;
                            case 2:
                                addr = 0x2c;
                            break;
                        }
                    }
                break;
                case 6:
                    op4State = op4ChannelsA[5];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0x22;
                            break;
                            case 2:
                                addr = 0x25;
                            break;
                            case 3:
                                addr = 0x2a;
                            break;
                            case 4:
                                addr = 0x2d;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0x2a;
                            break;
                            case 2:
                                addr = 0x2d;
                            break;
                        }
                    }
                break;
                case 7:
                case 16:
                    switch(opIndex){
                        case 1:
                            addr = 0x30;
                        break;
                        case 2:
                            addr = 0x33;
                        break;
                    }
                break;
                case 8:
                case 17:
                    switch(opIndex){
                        case 1:
                            addr = 0x31;
                        break;
                        case 2:
                            addr = 0x34;
                        break;
                    }
                break;
                case 9:
                case 18:
                    switch(opIndex){
                        case 1:
                            addr = 0x32;
                        break;
                        case 2:
                            addr = 0x35;
                        break;
                    }
                break;
                case 13:
                    switch(opIndex){
                        case 1:
                            addr = 0x28;
                        break;
                        case 2:
                            addr = 0x2b;
                        break;
                    }
                break;
                case 14:
                    switch(opIndex){
                        case 1:
                            addr = 0x29;
                        break;
                        case 2:
                            addr = 0x2c;
                        break;
                    }
                break;
                case 15:
                    switch(opIndex){
                        case 1:
                            addr = 0x2a;
                        break;
                        case 2:
                            addr = 0x2d;
                        break;
                    }
                break;
            
            }
        break;
        case 0x40:
            switch(channel){
                case 1:
                case 10:
                    switch(opIndex){
                        case 1:
                            addr = 0x40; 
                        break;
                        case 2:
                            addr = 0x43;
                        break;
                        case 3:
                            addr = 0x48;
                        break;
                        case 4:
                            addr = 0x4b;
                        break;
                    }
                break;
                case 2:
                case 11:
                    switch(opIndex){
                        case 1:
                            addr = 0x41;
                        break;
                        case 2:
                            addr = 0x44;
                        break;
                        case 3:
                            addr = 0x49;
                        break;
                        case 4:
                            addr = 0x4c;
                        break;
                    }
                break;
                case 3:
                case 12:
                    switch(opIndex){
                        case 1:
                            addr = 0x42;
                        break;
                        case 2:
                            addr = 0x45;
                        break;
                        case 3:
                            addr = 0x4a;
                        break;
                        case 4:
                            addr = 0x4d;
                        break;
                    }
                break;
                case 4:
                    op4State = op4ChannelsA[3];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0x40; 
                            break;
                            case 2:
                                addr = 0x43;
                            break;
                            case 3:
                                addr = 0x48;
                            break;
                            case 4:
                                addr = 0x4b;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0x48;
                            break;
                            case 2:
                                addr = 0x4b;
                            break;
                        }
                    }
                break;
                case 5:
                    op4State = op4ChannelsA[4];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0x41;
                            break;
                            case 2:
                                addr = 0x44;
                            break;
                            case 3:
                                addr = 0x49;
                            break;
                            case 4:
                                addr = 0x4c;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0x49;
                            break;
                            case 2:
                                addr = 0x4c;
                            break;
                        }
                    }
                break;
                case 6:
                    op4State = op4ChannelsA[5];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0x42;
                            break;
                            case 2:
                                addr = 0x45;
                            break;
                            case 3:
                                addr = 0x4a;
                            break;
                            case 4:
                                addr = 0x4d;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0x4a;
                            break;
                            case 2:
                                addr = 0x4d;
                            break;
                        }
                    }
                break;
                case 7:
                case 16:
                    switch(opIndex){
                        case 1:
                            addr = 0x50;
                        break;
                        case 2:
                            addr = 0x53;
                        break;
                    }
                break;
                case 8:
                case 17:
                    switch(opIndex){
                        case 1:
                            addr = 0x51;
                        break;
                        case 2:
                            addr = 0x54;
                        break;
                    }
                break;
                case 9:
                case 18:
                    switch(opIndex){
                        case 1:
                            addr = 0x52;
                        break;
                        case 2:
                            addr = 0x55;
                        break;
                    }
                break;
                case 13:
                    switch(opIndex){
                        case 1:
                            addr = 0x48;
                        break;
                        case 2:
                            addr = 0x4b;
                        break;
                    }
                break;
                case 14:
                    switch(opIndex){
                        case 1:
                            addr = 0x49;
                        break;
                        case 2:
                            addr = 0x4c;
                        break;
                    }
                break;
                case 15:
                    switch(opIndex){
                        case 1:
                            addr = 0x4a;
                        break;
                        case 2:
                            addr = 0x4d;
                        break;
                    }
                break;
            
            }
        break;
        case 0x60:
            switch(channel){
                case 1:
                case 10:
                    switch(opIndex){
                        case 1:
                            addr = 0x60; 
                        break;
                        case 2:
                            addr = 0x63;
                        break;
                        case 3:
                            addr = 0x68;
                        break;
                        case 4:
                            addr = 0x6b;
                        break;
                    }
                break;
                case 2:
                case 11:
                    switch(opIndex){
                        case 1:
                            addr = 0x61;
                        break;
                        case 2:
                            addr = 0x64;
                        break;
                        case 3:
                            addr = 0x69;
                        break;
                        case 4:
                            addr = 0x6c;
                        break;
                    }
                break;
                case 3:
                case 12:
                    switch(opIndex){
                        case 1:
                            addr = 0x62;
                        break;
                        case 2:
                            addr = 0x65;
                        break;
                        case 3:
                            addr = 0x6a;
                        break;
                        case 4:
                            addr = 0x6d;
                        break;
                    }
                break;
                case 4:
                    op4State = op4ChannelsA[3];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0x60; 
                            break;
                            case 2:
                                addr = 0x63;
                            break;
                            case 3:
                                addr = 0x68;
                            break;
                            case 4:
                                addr = 0x6b;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0x68;
                            break;
                            case 2:
                                addr = 0x6b;
                            break;
                        }
                    }
                break;
                case 5:
                    op4State = op4ChannelsA[4];
                
                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0x61;
                            break;
                            case 2:
                                addr = 0x64;
                            break;
                            case 3:
                                addr = 0x69;
                            break;
                            case 4:
                                addr = 0x6c;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0x69;
                            break;
                            case 2:
                                addr = 0x6c;
                            break;
                        }
                    }
                break;
                case 6:
                    op4State = op4ChannelsA[5];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0x62;
                            break;
                            case 2:
                                addr = 0x65;
                            break;
                            case 3:
                                addr = 0x6a;
                            break;
                            case 4:
                                addr = 0x6d;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0x6a;
                            break;
                            case 2:
                                addr = 0x6d;
                            break;
                        }
                    }
                break;
                case 7:
                case 16:
                    switch(opIndex){
                        case 1:
                            addr = 0x70;
                        break;
                        case 2:
                            addr = 0x73;
                        break;
                    }
                break;
                case 8:
                case 17:
                    switch(opIndex){
                        case 1:
                            addr = 0x71;
                        break;
                        case 2:
                            addr = 0x74;
                        break;
                    }
                break;
                case 9:
                case 18:
                    switch(opIndex){
                        case 1:
                            addr = 0x72;
                        break;
                        case 2:
                            addr = 0x75;
                        break;
                    }
                break;
                case 13:
                    switch(opIndex){
                        case 1:
                            addr = 0x68;
                        break;
                        case 2:
                            addr = 0x6b;
                        break;
                    }
                break;
                case 14:
                    switch(opIndex){
                        case 1:
                            addr = 0x69;
                        break;
                        case 2:
                            addr = 0x6c;
                        break;
                    }
                break;
                case 15:
                    switch(opIndex){
                        case 1:
                            addr = 0x6a;
                        break;
                        case 2:
                            addr = 0x6d;
                        break;
                    }
                break;
            
            }
        break;
        case 0x80:
            switch(channel){
                case 1:
                case 10:
                    switch(opIndex){
                        case 1:
                            addr = 0x80; 
                        break;
                        case 2:
                            addr = 0x83;
                        break;
                        case 3:
                            addr = 0x88;
                        break;
                        case 4:
                            addr = 0x8b;
                        break;
                    }
                break;
                case 2:
                case 11:
                    switch(opIndex){
                        case 1:
                            addr = 0x81;
                        break;
                        case 2:
                            addr = 0x84;
                        break;
                        case 3:
                            addr = 0x89;
                        break;
                        case 4:
                            addr = 0x8c;
                        break;
                    }
                break;
                case 3:
                case 12:
                    switch(opIndex){
                        case 1:
                            addr = 0x82;
                        break;
                        case 2:
                            addr = 0x85;
                        break;
                        case 3:
                            addr = 0x8a;
                        break;
                        case 4:
                            addr = 0x8d;
                        break;
                    }
                break;
                case 4:
                    op4State = op4ChannelsA[3];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0x80; 
                            break;
                            case 2:
                                addr = 0x83;
                            break;
                            case 3:
                                addr = 0x88;
                            break;
                            case 4:
                                addr = 0x8b;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0x88;
                            break;
                            case 2:
                                addr = 0x8b;
                            break;
                        }
                    }
                break;
                case 5:
                    op4State = op4ChannelsA[4];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0x81;
                            break;
                            case 2:
                                addr = 0x84;
                            break;
                            case 3:
                                addr = 0x89;
                            break;
                            case 4:
                                addr = 0x8c;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0x89;
                            break;
                            case 2:
                                addr = 0x8c;
                            break;
                        }
                    }
                break;
                case 6:
                    op4State = op4ChannelsA[5];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0x82;
                            break;
                            case 2:
                                addr = 0x85;
                            break;
                            case 3:
                                addr = 0x8a;
                            break;
                            case 4:
                                addr = 0x8d;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0x8a;
                            break;
                            case 2:
                                addr = 0x8d;
                            break;
                        }
                    }
                break;
                case 7:
                case 16:
                    switch(opIndex){
                        case 1:
                            addr = 0x90;
                        break;
                        case 2:
                            addr = 0x93;
                        break;
                    }
                break;
                case 8:
                case 17:
                    switch(opIndex){
                        case 1:
                            addr = 0x91;
                        break;
                        case 2:
                            addr = 0x94;
                        break;
                    }
                break;
                case 9:
                case 18:
                    switch(opIndex){
                        case 1:
                            addr = 0x92;
                        break;
                        case 2:
                            addr = 0x95;
                        break;
                    }
                break;
                case 13:
                    switch(opIndex){
                        case 1:
                            addr = 0x88;
                        break;
                        case 2:
                            addr = 0x8b;
                        break;
                    }
                break;
                case 14:
                    switch(opIndex){
                        case 1:
                            addr = 0x89;
                        break;
                        case 2:
                            addr = 0x8c;
                        break;
                    }
                break;
                case 15:
                    switch(opIndex){
                        case 1:
                            addr = 0x8a;
                        break;
                        case 2:
                            addr = 0x8d;
                        break;
                    }
                break;
            
            }
        break;
        case 0xA0:
            switch(channel){
                case 1:
                case 10:
                    switch(opIndex){
                        case 1:
                        case 2:
                            addr = 0xa0;
                        break;
                        case 3:
                        case 4:
                            addr = 0xa3;
                        break;
                    }
                break;
                case 2:
                case 11:
                    switch(opIndex){
                        case 1:
                        case 2:
                            addr = 0xa1;
                        break;
                        case 3:
                        case 4:
                            addr = 0xa4;
                        break;
                    }
                break;
                case 3:
                case 12:
                    switch(opIndex){
                        case 1:
                        case 2:
                            addr = 0xa2;
                        break;
                        case 3:
                        case 4:
                            addr = 0xa5;
                        break;
                    }
                break;
                case 4:
                    op4State = op4ChannelsA[3];

                    switch(opIndex){
                        case 1:
                        case 2:
                            if(op4State)
                                addr = 0xa0;
                            else
                                addr = 0xa3;
                        break;
                        case 3:
                        case 4:
                            addr = 0xa3;
                        break;
                    }
                break;
                case 5:

                    op4State = op4ChannelsA[4];

                    switch(opIndex){
                        case 1:
                        case 2:
                            if(op4State)
                                addr = 0xa1;
                            else
                                addr = 0xa4;
                        break;
                        case 3:
                        case 4:
                            addr = 0xa4;
                        break;
                    }
                break;
                case 6:
                    op4State = op4ChannelsA[5];

                    switch(opIndex){
                        case 1:
                        case 2:
                            if(op4State)
                                addr = 0xa2;
                            else
                                addr = 0xa5;
                        break;
                        case 3:
                        case 4:
                            addr = 0xa5;
                        break;
                    }
                break;
                case 7:
                case 16:
                    addr = 0xa6;
                break;
                case 8:
                case 17:
                    addr = 0xa7;
                break;
                case 9:
                case 18:
                    addr = 0xa8;
                break;
                case 13:
                    addr = 0xa3;
                break;
                case 14:
                    addr = 0xa4;
                break;
                case 15:
                    addr = 0xa5;
                break;
            
            }
        break;
        case 0xB0:
            switch(channel){
                case 1:
                case 10:
                    switch(opIndex){
                        case 1:
                        case 2:
                            addr = 0xb0;
                        break;
                        case 3:
                        case 4:
                            addr = 0xb3;
                        break;
                    }
                break;
                case 2:
                case 11:
                    switch(opIndex){
                        case 1:
                        case 2:
                            addr = 0xb1;
                        break;
                        case 3:
                        case 4:
                            addr = 0xb4;
                        break;
                    }
                break;
                case 3:
                case 12:
                    switch(opIndex){
                        case 1:
                        case 2:
                            addr = 0xb2;
                        break;
                        case 3:
                        case 4:
                            addr = 0xb5;
                        break;
                    }
                break;
                case 4:
                    op4State = op4ChannelsA[3];

                    switch(opIndex){
                        case 1:
                        case 2:
                            if(op4State)
                                addr = 0xb0;
                            else
                                addr = 0xb3;
                        break;
                        case 3:
                        case 4:
                            addr = 0xb3;
                        break;
                    }

                    
                break;
                case 5:
                    op4State = op4ChannelsA[4];

                    switch(opIndex){
                        case 1:
                        case 2:
                            if(op4State)
                                addr = 0xb1;
                            else
                                addr = 0xb4;
                        break;
                        case 3:
                        case 4:
                            addr = 0xb4;
                        break;
                    }

                break;
                case 6:
                    op4State = op4ChannelsA[5];

                    switch(opIndex){
                        case 1:
                        case 2:
                            if(op4State)
                                addr = 0xb2;
                            else
                                addr = 0xb5;
                        break;
                        case 3:
                        case 4:
                            addr = 0xb5;
                        break;
                    }
                break;
                case 7:
                case 16:
                    addr = 0xb6;
                break;
                case 8:
                case 17:
                    addr = 0xb7;
                break;
                case 9:
                case 18:
                    addr = 0xb8;
                break;
                case 13:
                    addr = 0xb3;
                break;
                case 14:
                    addr = 0xb4;
                break;
                case 15:
                    addr = 0xb5;
                break;
            
            }
        break;
        case 0xC0:
            switch(channel){
                case 1:
                case 10:
                    switch(opIndex){
                        case 1:
                        case 2:
                            addr = 0xc0;
                        break;
                        case 3:
                        case 4:
                            addr = 0xc3;
                        break;
                    }

                    isOp4Channel = op4ChannelsA[0];

                break;
                case 2:
                case 11:
                    switch(opIndex){
                        case 1:
                        case 2:
                            addr = 0xc1;
                        break;
                        case 3:
                        case 4:
                            addr = 0xc4;
                        break;
                    }

                    isOp4Channel = op4ChannelsA[1];
                break;
                case 3:
                case 12:
            
                    switch(opIndex){
                        case 1:
                        case 2:
                            addr = 0xc2;
                        break;
                        case 3:
                        case 4:
                            addr = 0xc5;
                        break;
                    }
                    
                    isOp4Channel = op4ChannelsA[2];
                break;
                case 4:
                    op4State = op4ChannelsA[3];

                    if(op4State){
                        //addr = 0xc0;
                        //addr2 = 0xc3;

                        switch(opIndex){
                            case 1:
                            case 2:
                                addr = 0xc0;
                            break;
                            case 3:
                            case 4:
                                addr = 0xc3;
                            break;
                        }

                        isOp4Channel = true;
                    }
                    else
                        addr = 0xc3;
                break;
                case 5:
                    op4State = op4ChannelsA[4];

                    if(op4State){
                        //addr = 0xc1;
                        //addr2 = 0xc4;

                        switch(opIndex){
                            case 1:
                            case 2:
                                addr = 0xc1;
                            break;
                            case 3:
                            case 4:
                                addr = 0xc4;
                            break;
                        }

                        isOp4Channel = true;
                    }
                    else
                        addr = 0xc4;
                break;
                case 6:
                    op4State = op4ChannelsA[5];

                    if(op4State){
                        //addr = 0xc2;
                        //addr2 = 0xc5;

                        switch(opIndex){
                            case 1:
                            case 2:
                                addr = 0xc2;
                            break;
                            case 3:
                            case 4:
                                addr = 0xc5;
                            break;
                        }

                        isOp4Channel = true;
                    }
                    else
                        addr = 0xc5;
                break;
                case 7:
                case 16:
                    addr = 0xc6;
                break;
                case 8:
                case 17:
                    addr = 0xc7;
                break;
                case 9:
                case 18:
                    addr = 0xc8;
                break;
                case 13:
                    addr = 0xc3;
                break;
                case 14:
                    addr = 0xc4;
                break;
                case 15:
                    addr = 0xc5;
                break;
            
            }
        break;
        case 0xE0:
            switch(channel){
                case 1:
                case 10:
                    switch(opIndex){
                        case 1:
                            addr = 0xe0; 
                        break;
                        case 2:
                            addr = 0xe3;
                        break;
                        case 3:
                            addr = 0xe8;
                        break;
                        case 4:
                            addr = 0xeb;
                        break;
                    }
                break;
                case 2:
                case 11:
                    switch(opIndex){
                        case 1:
                            addr = 0xe1;
                        break;
                        case 2:
                            addr = 0xe4;
                        break;
                        case 3:
                            addr = 0xe9;
                        break;
                        case 4:
                            addr = 0xec;
                        break;
                    }
                break;
                case 3:
                case 12:
                    switch(opIndex){
                        case 1:
                            addr = 0xe2;
                        break;
                        case 2:
                            addr = 0xe5;
                        break;
                        case 3:
                            addr = 0xea;
                        break;
                        case 4:
                            addr = 0xed;
                        break;
                    }
                break;
                case 4:
                    op4State = op4ChannelsA[3];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0xe0; 
                            break;
                            case 2:
                                addr = 0xe3;
                            break;
                            case 3:
                                addr = 0xe8;
                            break;
                            case 4:
                                addr = 0xeb;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0xe8;
                            break;
                            case 2:
                                addr = 0xeb;
                            break;
                        }
                    }
                break;
                case 5:
                    op4State = op4ChannelsA[4];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0xe1;
                            break;
                            case 2:
                                addr = 0xe4;
                            break;
                            case 3:
                                addr = 0xe9;
                            break;
                            case 4:
                                addr = 0xec;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0xe9;
                            break;
                            case 2:
                                addr = 0xec;
                            break;
                        }
                    }
                break;
                case 6:
                    op4State = op4ChannelsA[5];

                    if(op4State){
                        switch(opIndex){
                            case 1:
                                addr = 0xe2;
                            break;
                            case 2:
                                addr = 0xe5;
                            break;
                            case 3:
                                addr = 0xea;
                            break;
                            case 4:
                                addr = 0xed;
                            break;
                        }
                    }
                    else{
                        switch(opIndex){
                            case 1:
                                addr = 0xea;
                            break;
                            case 2:
                                addr = 0xed;
                            break;
                        }
                    }
                break;
                case 7:
                case 16:
                    switch(opIndex){
                        case 1:
                            addr = 0xf0;
                        break;
                        case 2:
                            addr = 0xf3;
                        break;
                    }
                break;
                case 8:
                case 17:
                    switch(opIndex){
                        case 1:
                            addr = 0xf1;
                        break;
                        case 2:
                            addr = 0xf4;
                        break;
                    }
                break;
                case 9:
                case 18:
                    switch(opIndex){
                        case 1:
                            addr = 0xf2;
                        break;
                        case 2:
                            addr = 0xf5;
                        break;
                    }
                break;
                case 13:
                    switch(opIndex){
                        case 1:
                            addr = 0xe8;
                        break;
                        case 2:
                            addr = 0xeb;
                        break;
                    }
                break;
                case 14:
                    switch(opIndex){
                        case 1:
                            addr = 0xe9;
                        break;
                        case 2:
                            addr = 0xec;
                        break;
                    }
                break;
                case 15:
                    switch(opIndex){
                        case 1:
                            addr = 0xea;
                        break;
                        case 2:
                            addr = 0xed;
                        break;
                    }
                break;
            
            }
        break;

    }


    return addr;
}


