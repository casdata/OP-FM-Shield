#ifndef OPL3_H
#define OPL3_H

#include <Arduino.h>

#define IC 40
#define A0 42
#define A1 44
#define WR 46
#define CS 48

#define D0 24
#define D1 26
#define D2 28
#define D3 30
#define D4 32
#define D5 34
#define D6 36
#define D7 38


#define BIT0 0x1
#define BIT1 0x2
#define BIT2 0x4 
#define BIT3 0x8
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80 

class Opl3{
    public:
        Opl3();  

        //Opl3(uint8_t icPin, uint8_t a0Pin, uint8_t a1Pin, uint8_t );
        void init(bool op4Mode);
        void reset();

        void reStart(bool op4Mode);

        void setSpeedExpression(bool enable);
        bool getSpeedExpression();

        void write(uint8_t reg, uint8_t data);

        void fakeWrite(uint8_t reg, uint8_t data, bool moduleB);

        void write(uint8_t reg, uint8_t data, bool moduleB);

        void loadAllChannelsOpl2(uint8_t addressSector, uint8_t data, uint8_t opIndex);
        void loadLeftChannelsOpl2(uint8_t addressSector, uint8_t data, uint8_t opIndex);
        void loadRightChannelsOpl2(uint8_t addressSector, uint8_t data, uint8_t opIndex);

        void loadAllChannelsOpl3(uint8_t addressSector, uint8_t data, uint8_t opIndex);
        void loadLeftChannelsOpl3(uint8_t addressSector, uint8_t data, uint8_t opIndex);

        void loadVRegAllChannelsOpl2();
        void loadVRegLeftChannelsOpl2();
        void loadVRegRightChannelsOpl2();

        void loadVRegAllChannelsOpl3();
        void loadVRegLeftChannelsOp3();

        void loadVRegAllChannelsOpl2(uint8_t opIndex, bool tlMode, bool cntMode);
        void loadVRegAllChannelsOpl3(uint8_t opIndex, bool tlMode, bool cntMode);
        
        void setChannel4OpMode(bool enable, uint8_t channel);                               //0x4,  A1=H

        bool getChannel4OpMode(uint8_t channel);

        void setDAM(bool enable);                                                           //0xBD, A1=L
        void setDVB(bool enable);                                                           //0xBD, A1=L   

        bool getDAM();
        bool getDVB();
        
        void setAM(bool enable, uint8_t channel, uint8_t opIndex);                          //0x20 - 0x35,  A1=L A1=H
        void setVIB(bool enable, uint8_t channel, uint8_t opIndex);                         //0x20 - 0x35,  A1=L A1=H
        void setEGT(bool enable, uint8_t channel, uint8_t opIndex);                         //0x20 - 0x35,  A1=L A1=H
        void setKSR(bool enable, uint8_t channel, uint8_t opIndex);                         //0x20 - 0x35,  A1=L A1=H
        void setMULT(uint8_t multiple, uint8_t channel, uint8_t opIndex);                   //0x20 - 0x35,  A1=L A1=H

        bool getAM(uint8_t channel, uint8_t opIndex);
        bool getVIB(uint8_t channel, uint8_t opIndex);
        bool getEGT(uint8_t channel, uint8_t opIndex);
        bool getKSR(uint8_t channel, uint8_t opindex);
        uint8_t getMULT(uint8_t channel, uint8_t opIndex);
         
        void setKSL(uint8_t keyScaleLevel, uint8_t channel, uint8_t opIndex);               //0x40 - 0x55,  A1=L A1=H
        void setTL(uint8_t totalLevel, uint8_t channel, uint8_t opIndex, bool initMode);                   //0x40 - 0x55,  A1=L A1=H

        uint8_t getKSL(uint8_t channel, uint8_t opIndex);
        uint8_t getTL(uint8_t channel, uint8_t opIndex);
        
        void setAR(uint8_t attackRate, uint8_t channel, uint8_t opIndex);                   //0x60 - 0x75,  A1=L A1=H     
        void setDR(uint8_t decayRate, uint8_t channel, uint8_t opIndex);                    //0x60 - 0x75,  A1=L A1=H   

        uint8_t getAR(uint8_t channel, uint8_t opIndex);
        uint8_t getDR(uint8_t channel, uint8_t opIndex);
        
        void setSL(uint8_t sustainLevel, uint8_t channel, uint8_t opIndex);                 //0x80 - 0x95,  A1=L A1=H
        void setRR(uint8_t releaseRate, uint8_t channel, uint8_t opIndex);                  //0x80 - 0x95,  A1=L A1=H   

        uint8_t getSL(uint8_t channel, uint8_t opIndex);
        uint8_t getRR(uint8_t channel, uint8_t opIndex);
        
        void setWS(uint8_t waveSelect, uint8_t channel, uint8_t opIndex);                   //0xE0 - 0xF5,  A1=L A1=H 

        uint8_t getWS(uint8_t channel, uint8_t opIndex);

        void setFNumber(uint8_t note, uint8_t channel);                                     //0xA0 - 0xA8 / 0xB0 - 0xB8,  A1=L A1=H

        void setBlock(uint8_t block, uint8_t channel);                                      //0xB0 - 0xB8,  A1=L A1=H
        void setKON(bool enable, uint8_t channel);                                          //0xB0 - 0xB8,  A1=L A1=H
                
        void setAudioBalance(uint8_t balance, uint8_t channel);                             //0xC0 - 0xC8,  A1=L A1=H 
        void setFB(uint8_t feedback, uint8_t channel, uint8_t opIndex);                                      //0xC0 - 0xC8,  A1=L A1=H  
        void setCNT(uint8_t connection, uint8_t channel);                                   //0xC0 - 0xC8,  A1=L A1=H 

        uint8_t getAudioBalance(uint8_t channel);
        uint8_t getFB(uint8_t channel, uint8_t opIndex);
        uint8_t getCNT(uint8_t channel);

        void rythm(bool enable);
        void bass(bool play);
        void snare(bool play);
        void tom(bool play);
        void cymbal(bool play);
        void hihat(bool play);
        
        void playNote(uint8_t channel, uint8_t block, uint8_t note, uint8_t bendData, bool rightBend, uint8_t speed);
        void releaseNote(uint8_t channel);

        void setSpeedTL(uint8_t channel);
        void setSpeedTL(uint8_t channel, uint8_t speed);
        int volSpeedEqu(int min, int max, int speed);
        
        void bendNote(uint8_t channel, uint8_t note, uint8_t bendData, bool rightBend);
        
        uint8_t getRegisterAt(int i, bool a1Flag);
        uint8_t getRegisterAtAddress(uint8_t addressSector, uint8_t channel, uint8_t opIndex);
        
        bool getOpMode();
        

        void reg2Pins(uint8_t data);

        
    private:
        void enableOPL3Features(bool enable);                                               //0x5,  A1=H
    
        void setNTS(bool enable);                                                           //0x8,  A1=L
        
        bool isRegisterB(uint8_t channel);
        uint8_t getAddress(uint8_t addressSector, uint8_t channel, uint8_t opIndex);


        uint8_t registersA[246];
        uint8_t registersB[246];
        
        bool op4ChannelsA[6];

        unsigned int noteFNumberOPL3[12];
        unsigned int noteFNumberOPL2[12];

        unsigned int lowNoteFNumberOPL3[2];
        unsigned int highNoteFNumberOPL3[2];
        
        //int8_t addr2;
        bool isOp4Channel;
        
        bool op4ModeChip;   

        int chConnections[18];

        int chTlOp1[18];
        int chTlOp2[18];
        int chTlOp3[18];
        int chTlOp4[18];

        bool speedExpression;        
        
        uint32_t tempReg;
};


#endif