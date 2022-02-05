#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <MIDI.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Adafruit_ST7735.h> 
#include <Encoder.h>
#include <Thread.h>
#include <DueTimer.h>
#include "Opl3.h"
#include "ScreenActivity.h"
#include "LandSpace.h"
#include "midi_voices.h"
#include "logo.h"


Thread screenThread = Thread();

Adafruit_ST7735 tft = Adafruit_ST7735(__CS, 8,__DC);


MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);


Opl3 *opl3;
File voicesFolder;
File vgmFolder;
File tempFolder;

Screen currentScreen;
Screen beforeSaveScreen = OPERATOR;

ScreenActivity *screenActivity;

KeyMode currentKeyMode;
KeyMode previousKeyMode;
Instrument currentInstrument;

String mainInstrumentVoice = "testName";
String cleanMainInstrumentVoice = "";
String preMainInstrumentVoice = "";
OpMode mainInstrumentOpMode = OP2;

OpMode instrumentAOpMode = OP2;
OpMode instrumentBOpMode = OP2;

//unsigned long cycleTime = 0;

bool internalBtnState = false;
bool nextSong = false;
bool autoPlayIt = false;
bool vgmPlay = false;
bool vgmPause = true;
bool vgmAutoPlay = false;
bool vgmGenVoice = false;
 
uint32_t vgmPC = 0;

uint8_t opType = 3;

bool op4op2Set = true;

MemState memState = LOCAL_FOLDER;
//bool localOrSD1 = true;
bool localOrSD2 = true;
bool setNotepoint = false;
int notepoint = 60;

bool newInstrument = false;
bool setChanges = false;
bool overwriteIt = false;
bool onlySave = false;
bool presetChange = false;

int8_t saveCursorX = 0;
int letterSelect = 65;

bool rightBend = true;

//OPERATOR DATA
int opCurrentPage = 1;
int opCurrentPosition = 1;
int opCurrentPositionInPage = 1;

int selectedOp = 1;

int opAttack = 0;
int opDecay = 0;
int opSustain = 0;
int opRelease = 0;

bool opTremolo = false;
bool opVibrato = false;
bool opEnvelopeType = false;
bool opKeyScaleRate = false;

int opWaveform = 0;
int opTotalLevel = 0;
int opFreMult = 0;
int opKeyScaleLevel = 0;
//END OPERATOR DATA

//CHANNEL DATA
int chCurrentPosition = 1;

int chAlgorithm = 0;
int chFeedback = 0;
//END CHANNEL DATA


Encoder knobA(KNOB_A_CLK, KNOB_A_DT);
Encoder knobB(KNOB_B_CLK, KNOB_B_DT);

/*
Encoder knobA(KNOB_A_DT, KNOB_A_CLK);
Encoder knobB(KNOB_B_DT, KNOB_B_CLK);
*/

int buttonACurrentState;
int buttonBCurrentState;
int buttonCCurrentState;
int buttonDCurrentState;
int buttonECurrentState;
int buttonFCurrentState;

int buttonAPreviousState = 1;
int buttonBPreviousState = 1;
int buttonCPreviousState = 1;
int buttonDPreviousState = 1;
int buttonEPreviousState = 1;
int buttonFPreviousState = 1;

int switchACurrentState;
int switchBCurrentState;

int switchAPreviousState = 1;
int switchBPreviousState = 1;

bool btA = false;
bool btB = false;
bool btC = false;
bool btD = false;
bool btE = false;
bool btF = false;

bool shiftBtn = false;
bool saveAccesFlag = false;
bool accesLoaded = false;

bool ledFState = false;

unsigned long currentTimeLedF = 0;

unsigned long currentTimeBtA = 0;
unsigned long currentTimeBtB = 0;
unsigned long currentTimeBtC = 0;
unsigned long currentTimeBtD = 0;
unsigned long currentTimeBtE = 0;
unsigned long currentTimeBtF = 0;

bool lockBtA = false;
bool lockBtB = false;
bool lockBtC = false;
bool lockBtD = false;
bool lockBtE = false;
bool lockBtF = false;

int knobACount = 0;
int knobBCount = 0;

bool knobARight = false;
bool knobALeft = false;
bool knobBRight = false;
bool knobBLeft = false;

bool swA = false;
bool swB = false;

bool holdSwA = false;
bool holdSwB = false;

unsigned long currentTimeSwA = 0;
unsigned long currentTimeSwB = 0;

bool lockSwA = false;
bool lockSwB = false;

bool sdCard = false;

int16_t tlVoices = 127;     //129
int16_t tl3Voices = 127;



int16_t localVoices = 0;
int16_t localVoicesPages = 0;
int16_t currentLocalVoice = 1;
int16_t currentLocalVoiceInPage = 0;
int16_t currentLocalPage = 1;

int16_t localVoicesPagesA = 0;
int16_t currentLocalVoiceInPageA = 0;
int16_t currentLocalVoiceA = 1;
int16_t currentLocalPageA = 1;

uint16_t localVoicesB = 0;
int16_t localVoicesPagesB  = 0;
int16_t currentLocalVoiceInPageB = 0;
int16_t currentLocalVoiceB = 1;
int16_t currentLocalPageB = 1;


bool voicePagerA = false;
bool voicePagerB = false;

bool vgmPager = false;

int16_t tempVoices = 0;
int16_t tempVoicesPages = 0;
int16_t currentTempVoice = 1;
int16_t currentTempVoiceInPage = 0;
int16_t currentTempPage = 1;

int16_t sdVoices = 0;
int16_t sdVoicesPages = 0;
int16_t currentSdVoice = 1;
int16_t currentSdVoiceInPage = 0;
int16_t currentSdPage = 1;

int16_t sdVoicesPagesA = 0;
int16_t currentSdVoiceInPageA = 0;
int16_t currentSdVoiceA = 1;
int16_t currentSdPageA = 1;

uint16_t sdVoicesB = 0;
int16_t sdVoicesPagesB = 0;
int16_t currentSdVoiceInPageB = 0;
int16_t currentSdVoiceB = 1;
int16_t currentSdPageB = 1;

int16_t sdVgms = 0;
int16_t sdVgmPages = 0;
int16_t currentSdVgm = 1;
int16_t currentSdVgmInPage = 0;
int16_t currentSdVgmPage = 1;

uint8_t voicesPages[9];

uint16_t voices = 0;
uint16_t voices3 = 0;

uint16_t tVoices = 0;
uint16_t tVoices3 = 0;

uint16_t vgms = 0;
/*
uint16_t voicePages = 0; 
uint16_t currentVoice = 0;
uint16_t currentVoicePage = 0;
*/


String fileNameList[9];
String fileSDNameList[9];
String fileTEMPNameList[9];

String fileNameListA[4];
String fileNameListB[4];

String fileSDNameListA[4];
String fileSDNameListB[4];

String vgmNameList[4];
String vgmSDNameList[4];

bool wait4Samples = false;
int samplesCounter = 0;

//MIDI VARIABLES 
bool ledState;

int singlePC = -1;
int singlePCA = -1;
int singlePCB = -1;

bool overrideSend2FM = false;
bool notes2FM = false;


uint8_t selectedNote = 0;
uint8_t selectedOctave = 0;

int mod = 0;
bool modTrigger = false;

int pitch = 0;
bool pitchTrigger = false;

bool pedalSustain = false;
bool pedalSustainTrigger = false;


uint8_t notes[12] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
uint8_t notesSpeed[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bool notesTrigger[12] = {false, false, false, false, false, false, false, false, false ,false, false, false};
bool sustainRelease[12] = {false, false, false, false, false, false, false, false, false ,false, false, false};

uint8_t notesA[5] = {255, 255, 255, 255, 255};
uint8_t notesSpeedA[5] = {0, 0, 0, 0, 0};
bool notesTriggerA[5] =  {false, false, false, false, false};
bool sustainReleaseA[5] = {false, false, false, false, false};

uint8_t notesB[5] = {255, 255, 255, 255, 255};
uint8_t notesSpeedB[5] = {0, 0, 0, 0, 0};
bool notesTriggerB[5] =  {false, false, false, false, false};
bool sustainReleaseB[5] = {false, false, false, false, false};

uint8_t preNotes[12] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
uint8_t preNotesA[5] = {255, 255, 255, 255, 255};
uint8_t preNotesB[5] = {255, 255, 255, 255, 255}; 
//END MIDI VARIABLES

bool chEnableA[9] = {true, true, true, true, true, true, true, true, true};
bool chEnableB[9] = {true, true, true, true, true, true, true, true, true};


void initAll();
bool initSD();
void getLocalVoicePageList(uint16_t page);
void getLocalVoicePageListA(uint16_t page);
void getLocalVoicePageListB(uint16_t page);
void loadLocalVoice(uint16_t voiceIndex);
void loadTEMPorSDVoice(uint16_t voiceIndex, int opt, bool sdVo);
void loadVoice2OPL2(uint8_t addressSector, uint8_t data, uint8_t opIndex);
void loadVoice2OPL3(uint8_t addressSector, uint8_t data, uint8_t opIndex);
void checkNative4OPChannels(bool opl2);

void readMIDIKeyboard();
void convertMidiToNote(int myNote);
void sendNotes2FM();
void cleanSynthNotes();
void cleanANotes();
void cleanSplitNotes();
void stateChecker();
void readKnobsButtons();
void readButtons();
void checkUserInput();
void accessBtnsUserInput();
void modeAUserInput();
void modeBUserInput();
void modeCUserInput();
void operatorUserInput();
void channelUserInput();
void saveInsUserInput();
void saveMessageUserInput();
void overwriteSaveUserInput();
void getChannelOpType();
void loadChannelData();
void loadOperatorData();
void checkLimitOfKnob(int selectedKnob);
void updateKnobs();
void updateExtraData();
void resetKnobSwitchs();
void setScreen(Screen newScreen);
String selectVoice2Page(uint8_t voiceIndex, uint8_t nameListIndex);
uint8_t getVoiceProgmemAt(int voiceIndex, int byteIndex);
void setLocalVoicesTotals();
void setSDVoicesTotals();
String getVoiceNameSDorTEMP(String entryName, bool sdVo);
//String getVoiceNameSD(String entryName);
String getVgmNameSD(String entryName);
void getTEMPVoicePageList(uint16_t page);
void getSDVoicePageList(uint16_t page);
void getVGMPageList(uint16_t page);
void getSDVoicePageListA(uint16_t page);
void getSDVoicePageListB(uint16_t page);
bool isOp2(String entryName);
void createOP2Voice();
void createOP4Voice();
void updateCurrentSDInPage(int16_t temp);
void updateCurrentTEMPInPage(int16_t temp);
void updateCurrentSdVgmInPage(int16_t temp);
void updateCurrentSDInPageA(int16_t temp);
void updateCurrentSDInPageB(int16_t temp);
void updateCurrentLocalInPage(int16_t temp);
void updateCurrentLocalInPageA(int16_t temp);
void updateCurrentLocalInPageB(int16_t temp);
void saveInstrument();
void saveTempVoice(String vgmStr, int tempPC, int tempCh, bool isOPl3);
bool isAOp4Voice(int tempCh);
bool dontExistTempVoice(int tempCh, bool isOPl3);
void voiceNameToSaveScreen();
bool checkIfVoiceExist();
void findSavedFile(String fileName);
int tlFix(int tlData);
void loadVoiceVolumeRegistersOPL2();
void loadVoiceVolumeRegistersOPL3();
bool isCompatibleVGMFile();
void incVgmSong();
void vgmPlayer();
void generateOP12Voices();
bool isKOn(int tempData);
void clearTEMPFolder();
void startUpSequence();
void releaseShiftBtn();
bool saveBtnAccess(uint8_t selectedBtn);
bool loadBtnAccess(uint8_t selectedBtn);
bool findSDVoice(String vName);
bool findSDVoiceA(String vName);
bool findSDVoiceB(String vName);
void blinkFLed();
void swapMidiLed();
void waitSamples(int nSamples);



void debugReadThis(){
  //Serial.print("S: ");
        
  /*
  for(int i = 0; i < 12; i++){
    Serial.print(notes[i]);              
    Serial.print(" ");
  }   
  Serial.println(""); 
  Serial.println("");
  */

  /*
  for(int i = 0; i < 5; i++){
    Serial.print(notesA[i]);              
    Serial.print(" ");
  }   
  Serial.println(""); 
  Serial.println("");
  */
  
}

/*
void tempoCount(){
  if(bpmCount == 4294967295)
    bpmCount = 0;
  else
    bpmCount++;

  if(newBpm){
    newBpm = false;
    Timer4.stop();
    Timer4.start(bpm);
  }
}
*/

void renderScreen(){
  screenActivity->render(&tft);
}

void countSample(){
  if(wait4Samples){
    samplesCounter--;
    if(samplesCounter < 0)
      wait4Samples = false;
  }
}


void setup() {

  setLocalVoicesTotals();

  getLocalVoicePageList(currentLocalPage);
  getLocalVoicePageListA(currentLocalPageA);
  getLocalVoicePageListB(currentLocalPageB);

  initAll();


  opl3->setDAM(true);
  opl3->setDVB(true);

  loadLocalVoice(1);

  screenThread.onRun(renderScreen);
  screenThread.setInterval(1);

  Timer7.attachInterrupt(countSample);
  Timer7.setFrequency(44100);

}

void loop() {
  // put your main code here, to run repeatedly:

  //Serial.println((micros() - cycleTime));
  //cycleTime = micros();

  readKnobsButtons();
  readButtons();
  updateKnobs();
  
  readMIDIKeyboard();

  screenActivity->update(&tft);

  updateExtraData();

  if(saveAccesFlag)
    blinkFLed();

  if(notes2FM)
    sendNotes2FM();
  
  if(vgmPlay){

    Timer7.start();

    do{
      screenActivity->update(&tft);
      updateExtraData();
      screenActivity->render(&tft);

      vgmPlayer();

    } while (autoPlayIt || nextSong);

    Timer7.stop();

  }
  
  stateChecker();

  checkUserInput();

  resetKnobSwitchs();

  if(screenThread.shouldRun())
    screenThread.run();
   
}

void createOP2Voice(){
  //localOrSD1 = false;
  memState = SD_FOLDER;

  opl3->reStart(true);

  newInstrument = true;

  mainInstrumentVoice = "UNTITLED_OP2";
  mainInstrumentOpMode = OP2;
  
  chAlgorithm = 0;

  checkNative4OPChannels(true);
  
  opl3->setCNT(chAlgorithm, 1);
  uint8_t tempByte = opl3->getRegisterAtAddress(0xC0, 1, 1);
  opl3->loadAllChannelsOpl2(0xC0, tempByte, 1);

  setScreen(CHANNEL);
}

void createOP4Voice(){

  //localOrSD1 = false;
  memState = SD_FOLDER;

  opl3->reStart(true);

  newInstrument = true;
  
  mainInstrumentVoice = "UNTITLED_OP4";
  mainInstrumentOpMode = OP4;

  chAlgorithm = 2;

  checkNative4OPChannels(false);
  
  opl3->setCNT(chAlgorithm, 1);
  uint8_t tempByte = opl3->getRegisterAtAddress(0xC0, 1, 1);
  opl3->loadAllChannelsOpl3(0xC0, tempByte, 1);
          
  tempByte = opl3->getRegisterAtAddress(0xC0, 1, 3);
  opl3->loadAllChannelsOpl3(0xC0, tempByte, 3);

  setScreen(CHANNEL);
}

void getTEMPVoicePageList(uint16_t page){
  if(page > tempVoicesPages)
    page = tempVoicesPages;
  if(page < 1)
    page = 1;

  page--;

  int startPoint = (0 + (page * 9));
  int endPoint = (8 + (page * 9));

  int currentIndex = 0;
  uint8_t internalIndex = 0;  

  for(int i = 0; i < 9; i++){
    fileNameList[i] = " ";
    fileTEMPNameList[i] = " ";
  }

  tempFolder.rewindDirectory();
  while(true){
    File entry = tempFolder.openNextFile();
    if(!entry)
      break;

    if(currentIndex > endPoint)
      break;
    
    if(!entry.isDirectory()){
      if((currentIndex >= startPoint) && (currentIndex <= endPoint)){
        fileTEMPNameList[internalIndex] = String(entry.name());
        fileNameList[internalIndex] = getVoiceNameSDorTEMP(String(entry.name()), false);

        internalIndex++;
      }
    }

    currentIndex++;
  }

}

void getSDVoicePageList(uint16_t page){
  if(page > sdVoicesPages)
    page = sdVoicesPages;
  if(page < 1)
    page = 1;

  page--;

  int startPoint = (0 + (page * 9));
  int endPoint = (8 + (page * 9));

  int currentIndex = 0;
  uint8_t internalIndex = 0;  

  for(int i = 0; i < 9; i++){
    fileNameList[i] = " ";
    fileSDNameList[i] = " ";
  }

  voicesFolder.rewindDirectory();
  while(true){
    File entry = voicesFolder.openNextFile();
    if(!entry)
      break;

    if(currentIndex > endPoint)
      break;
    
    if(!entry.isDirectory()){
      if((currentIndex >= startPoint) && (currentIndex <= endPoint)){
        fileSDNameList[internalIndex] = String(entry.name());
        fileNameList[internalIndex] = getVoiceNameSDorTEMP(String(entry.name()), true);

        internalIndex++;
      }
    }

    currentIndex++;
  }

}

void getVGMPageList(uint16_t page){
  if(page > sdVgmPages)
    page = sdVgmPages;
  if(page < 1)
    page = 1;

  page--;

  int startPoint = (0 + (page * 4));
  int endPoint = (3 + (page * 4));

  int currentIndex = 0;
  uint8_t internalIndex = 0;  

  for(int i = 0; i < 4; i++){
    vgmNameList[i] = " ";
    vgmSDNameList[i] = " ";
  }
  

  vgmFolder.rewindDirectory();
  while(true){
    File entry = vgmFolder.openNextFile();
    if(!entry)
      break;

    if(currentIndex > endPoint)
      break;
    
    if(!entry.isDirectory()){
      if((currentIndex >= startPoint) && (currentIndex <= endPoint)){
        vgmSDNameList[internalIndex] = String(entry.name());
        vgmNameList[internalIndex] = getVgmNameSD(String(entry.name()));
    
        internalIndex++;
      }
    }

    currentIndex++;
  }
  
}

void getSDVoicePageListA(uint16_t page){
  if(page > sdVoicesPagesA)
    page = sdVoicesPagesA;
  if(page < 1)
    page = 1;

  page--;

  int startPoint = (0 + (page * 4));
  int endPoint = (3 + (page * 4));

  int currentIndex = 0;
  uint8_t internalIndex = 0;  

  for(int i = 0; i < 4; i++){
    fileNameListA[i] = " ";
    fileSDNameListA[i] = " ";
  }

  voicesFolder.rewindDirectory();
  while(true){
    File entry = voicesFolder.openNextFile();
    if(!entry)
      break;

    if(currentIndex > endPoint)
      break;
    
    if(!entry.isDirectory()){
      if((currentIndex >= startPoint) && (currentIndex <= endPoint)){
        fileSDNameListA[internalIndex] = String(entry.name());
        fileNameListA[internalIndex] = getVoiceNameSDorTEMP(String(entry.name()), true);

        internalIndex++;
      }
    }

    currentIndex++;
  }

}

void getSDVoicePageListB(uint16_t page){
  if(page > sdVoicesPagesB)
    page = sdVoicesPagesB;
  if(page < 1)
    page = 1;

  page--;

  int startPoint = (0 + (page * 4));
  int endPoint = (3 + (page * 4));

  int currentIndex = 0;
  uint8_t internalIndex = 0;  

  for(int i = 0; i < 4; i++){
    fileNameListB[i] = " ";
    fileSDNameListB[i] = " ";
  }

  voicesFolder.rewindDirectory();
  while(true){
    File entry = voicesFolder.openNextFile();
    if(!entry)
      break;

    if(currentIndex > endPoint)
      break;
    
    if(!entry.isDirectory()){
      if((currentIndex >= startPoint) && (currentIndex <= endPoint)){
        
        if(isOp2(entry.name())){
          fileSDNameListB[internalIndex] = String(entry.name());
          fileNameListB[internalIndex] = getVoiceNameSDorTEMP(String(entry.name()), true);
        }
        else{
          fileSDNameListB[internalIndex] = "null";
          fileNameListB[internalIndex] = "<OP4 NO COMPATIBLE>";
        }
        internalIndex++;
      }
    }

    currentIndex++;
  }

}

bool isOp2(String entryName){
  bool temp = false;

  File selectedFile = SD.open("/voices/"+entryName);

  if(selectedFile){
    if(selectedFile.available()){
      int selectedByte = selectedFile.read();

      if(selectedByte == 0x3C)
        temp = true;
    }
  }

  selectedFile.close();
  
  return temp;
}

String getVoiceNameSDorTEMP(String entryName, bool sdVo){
  String a = "";

  File selectedFile;

  if(sdVo)
    selectedFile = SD.open("/voices/"+entryName);
  else
    selectedFile = SD.open("/temp/"+entryName);

  if(selectedFile){

    int selectedByte = 0;
    int minPoint = 0;
    int i = 1;

    if(selectedFile.available())
      selectedByte = selectedFile.read();

    if(selectedByte == 0x3C)                             //OP2
      minPoint = 12;
    
    else if(selectedByte == 0x78)                        //OP3
      minPoint = 23;
    

    if(minPoint > 0){
      while(selectedFile.available()){
        selectedByte = selectedFile.read();

        if(i >= minPoint){
          if(selectedByte != 0)
            a += (char)selectedByte;
        }
        i++;
      }
    }
  }

  selectedFile.close();

  return a;
}


String getVgmNameSD(String entryName){
  String a = "";

  File selectedFile = SD.open("/vgm/"+entryName);

  if(selectedFile){

    int selectedByte = 0;
    int countIt = 0;

    uint32_t tempSize = selectedFile.size();

    if(tempSize > 0){
      uint32_t tempTarget = tempSize - 1000;

      if(tempTarget < 0)
        tempTarget = 0;

      if(selectedFile.seek(tempTarget)){

        bool lockA = false;
        bool lockB = false;
        bool lockC = false;

        while(selectedFile.available()){
          selectedByte = selectedFile.read();

          if(lockA && lockB && lockC){
            if(countIt > 41)  //
              break;
            else{
              if(selectedByte != 0 && selectedByte < 0x7F && selectedByte >= 0x20){
                a += (char)selectedByte;
                countIt++;
              }
            }
            
          }//END if(lockA && lockB && lockC)
          else{
            if(selectedByte == 0x47)
              lockA = true;
            else if(selectedByte == 0x64)
              lockB = true;
            else if(selectedByte == 0x33){
              lockC = true;
              selectedByte = selectedFile.read();
            }
            else{
              lockA = false;
              lockB = false;
              lockC = false;
            }
          }//END else
          
        }//END while()

      }//END if

    }
  }

  selectedFile.close();

  return a;
}


void getLocalVoicePageList(uint16_t page){
  if(page > localVoicesPages)
    page = localVoicesPages;
  if(page < 1)
    page = 1;

  page--;

  int startPoint = (1 + (page * 9));
  int endPoint = (9 + (page * 9));

  uint8_t internalIndex = 0;

  for(int i = 0; i < 9; i++)
    fileNameList[i] = " ";

  for(int i = 1; i <= localVoices; i++){
    if((i >= startPoint) && (i <= endPoint)){
      fileNameList[internalIndex] = selectVoice2Page(i, internalIndex);
      internalIndex++;
    }

    if(i > endPoint)
      break;
  }
}

void getLocalVoicePageListA(uint16_t page){
  if(page > localVoicesPagesA)
    page = localVoicesPagesA;
  if(page < 1)
    page = 1;

  page--;

  int startPoint = (1 + (page * 4));
  int endPoint = (4 + (page * 4));

  uint8_t internalIndex = 0;

  for(int i = 0; i < 4; i++)
    fileNameListA[i] = " ";

  for(int i = 1; i <= localVoices; i++){
    if((i >= startPoint) && (i <= endPoint)){
      fileNameListA[internalIndex] = selectVoice2Page(i, internalIndex);
      internalIndex++;
    }

    if(i > endPoint)
      break;
  }
}



void getLocalVoicePageListB(uint16_t page){
  if(page > localVoicesPagesB)
    page = localVoicesPagesB;
  if(page < 1)
    page = 1;

  page--;

  int startPoint = (1 + (page * 4));
  int endPoint = (4 + (page * 4));

  uint8_t internalIndex = 0;

  for(int i = 0; i < 4; i++)
    fileNameListB[i] = " ";

  for(int i = 1; i <= localVoicesB; i++){
    if((i >= startPoint) && (i <= endPoint)){
      fileNameListB[internalIndex] = selectVoice2Page(i, internalIndex);
      internalIndex++;
    }

    if(i > endPoint)
      break;
  }
}


String selectVoice2Page(uint8_t voiceIndex, uint8_t nameListIndex){
  voicesPages[nameListIndex] = voiceIndex;

  String tempString = "";

  uint8_t tempData = getVoiceProgmemAt(voiceIndex, 0);

  if(tempData == 0x3C){                                                    //OPL2

    for(int i = 12; i < 30; i++){
      tempData = getVoiceProgmemAt(voiceIndex, i);

      if(tempData == 0x0)
        break;

      tempString += (char)tempData;

    }
  }
  else if(tempData == 0x78){
    for(int i = 23; i < 41; i++){
      tempData = getVoiceProgmemAt(voiceIndex, i);

      if(tempData == 0x0)
        break;
      
      tempString += (char)tempData;
    }
  }

  return tempString;

}

void loadLocalVoice(uint16_t voiceIndex){
  if(voiceIndex < 1)
    voiceIndex = 1;

  uint8_t tempData = getVoiceProgmemAt(voiceIndex, 0);

  if(tempData == 0x3C){                                                   //OPL2

    if(currentKeyMode == SINGLE){

      mainInstrumentOpMode = OP2;

      if(mainInstrumentOpMode == OP4)
        cleanSynthNotes();
    }
    else{
      if(currentInstrument == A)
        instrumentAOpMode = OP2;
      else
        instrumentBOpMode = OP2;
    }

    checkNative4OPChannels(true);

    tempData = getVoiceProgmemAt(voiceIndex, 1);
    loadVoice2OPL2(0x20, tempData, 1);
    tempData = getVoiceProgmemAt(voiceIndex, 2);
    loadVoice2OPL2(0x40, tempData, 1);
    tempData = getVoiceProgmemAt(voiceIndex, 3);
    loadVoice2OPL2(0x60, tempData, 1);
    tempData = getVoiceProgmemAt(voiceIndex, 4);
    loadVoice2OPL2(0x80, tempData, 1);
    tempData = getVoiceProgmemAt(voiceIndex, 5);
    loadVoice2OPL2(0xE0, tempData, 1);

    tempData = getVoiceProgmemAt(voiceIndex, 6);
    loadVoice2OPL2(0x20, tempData, 2);
    tempData = getVoiceProgmemAt(voiceIndex, 7);
    loadVoice2OPL2(0x40, tempData, 2);
    tempData = getVoiceProgmemAt(voiceIndex, 8);
    loadVoice2OPL2(0x60, tempData, 2);
    tempData = getVoiceProgmemAt(voiceIndex, 9);
    loadVoice2OPL2(0x80, tempData, 2);
    tempData = getVoiceProgmemAt(voiceIndex, 10);
    loadVoice2OPL2(0xE0, tempData, 2);
    
    tempData = getVoiceProgmemAt(voiceIndex, 11);
    loadVoice2OPL2(0xC0, tempData, 1);

    loadVoiceVolumeRegistersOPL2();
  }
  else if(tempData == 0x78){

    if(currentKeyMode == SINGLE){
      mainInstrumentOpMode = OP4;

      if(mainInstrumentOpMode == OP2)
        cleanSynthNotes();
    }
    else{
      if(currentInstrument == A)
        instrumentAOpMode = OP4;
    }


    checkNative4OPChannels(false);

    tempData = getVoiceProgmemAt(voiceIndex, 1);
    loadVoice2OPL3(0x20, tempData, 1);
    tempData = getVoiceProgmemAt(voiceIndex, 2);
    loadVoice2OPL3(0x40, tempData, 1);
    tempData = getVoiceProgmemAt(voiceIndex, 3);
    loadVoice2OPL3(0x60, tempData, 1);
    tempData = getVoiceProgmemAt(voiceIndex, 4);
    loadVoice2OPL3(0x80, tempData, 1);
    tempData = getVoiceProgmemAt(voiceIndex, 5);
    loadVoice2OPL3(0xE0, tempData, 1);

    tempData = getVoiceProgmemAt(voiceIndex, 6);
    loadVoice2OPL3(0x20, tempData, 2);
    tempData = getVoiceProgmemAt(voiceIndex, 7);
    loadVoice2OPL3(0x40, tempData, 2);
    tempData = getVoiceProgmemAt(voiceIndex, 8);
    loadVoice2OPL3(0x60, tempData, 2);
    tempData = getVoiceProgmemAt(voiceIndex, 9);
    loadVoice2OPL3(0x80, tempData, 2);
    tempData = getVoiceProgmemAt(voiceIndex, 10);
    loadVoice2OPL3(0xE0, tempData, 2);
    
    tempData = getVoiceProgmemAt(voiceIndex, 11);
    loadVoice2OPL3(0xC0, tempData, 1);

    tempData = getVoiceProgmemAt(voiceIndex, 12);
    loadVoice2OPL3(0x20, tempData, 3);
    tempData = getVoiceProgmemAt(voiceIndex, 13);
    loadVoice2OPL3(0x40, tempData, 3);
    tempData = getVoiceProgmemAt(voiceIndex, 14);
    loadVoice2OPL3(0x60, tempData, 3);
    tempData = getVoiceProgmemAt(voiceIndex, 15);
    loadVoice2OPL3(0x80, tempData, 3);
    tempData = getVoiceProgmemAt(voiceIndex, 16);
    loadVoice2OPL3(0xE0, tempData, 3);

    tempData = getVoiceProgmemAt(voiceIndex, 17);
    loadVoice2OPL3(0x20, tempData, 4);
    tempData = getVoiceProgmemAt(voiceIndex, 18);
    loadVoice2OPL3(0x40, tempData, 4);
    tempData = getVoiceProgmemAt(voiceIndex, 19);
    loadVoice2OPL3(0x60, tempData, 4);
    tempData = getVoiceProgmemAt(voiceIndex, 20);
    loadVoice2OPL3(0x80, tempData, 4);
    tempData = getVoiceProgmemAt(voiceIndex, 21);
    loadVoice2OPL3(0xE0, tempData, 4);
    
    tempData = getVoiceProgmemAt(voiceIndex, 22);
    loadVoice2OPL3(0xC0, tempData, 3);

    loadVoiceVolumeRegistersOPL3();

  }

}


void loadTEMPorSDVoice(uint16_t voiceIndex, int opt, bool sdVo){  

  String tempName = "";

  File selectedFile;

  if(sdVo){
    switch(opt){
      case 0:
        tempName += fileSDNameList[voiceIndex];
      break;
      case 1:
        tempName += fileSDNameListA[voiceIndex];
      break;
      case 2:
        tempName += fileSDNameListB[voiceIndex];
      break;
    }

    selectedFile = SD.open("/voices/" + tempName);
  }
  else{
    tempName += fileTEMPNameList[voiceIndex];

    selectedFile = SD.open("/temp/" + tempName);
  }

  

  if(selectedFile){
    uint8_t selectedByte = 0;
    
    if(selectedFile.available())
      selectedByte = selectedFile.read();

    if(selectedByte == 0x3C){                                           //OP2

      if(currentKeyMode == SINGLE){

        mainInstrumentOpMode = OP2;

        if(mainInstrumentOpMode == OP4)
          cleanSynthNotes();
      }
      else{
        if(currentInstrument == A)
          instrumentAOpMode = OP2;
        else
          instrumentBOpMode = OP2;
      }

      checkNative4OPChannels(true);  

      int k = 0;

      for(int i = 0; i < 4; i++){
        k = 0x20 + (i * 0x20);
        if(selectedFile.available()){
          selectedByte = selectedFile.read();
          loadVoice2OPL2(k, selectedByte, 1);

        }
      }

      if(selectedFile.available()){
        selectedByte = selectedFile.read();
        loadVoice2OPL2(0xE0, selectedByte, 1);

      }
      
      for(int i = 0; i < 4; i++){
        k = 0x20 + (i * 0x20);
        if(selectedFile.available()){
          selectedByte = selectedFile.read();
          loadVoice2OPL2(k, selectedByte, 2);

        }
      }

      if(selectedFile.available()){
        selectedByte = selectedFile.read();
        loadVoice2OPL2(0xE0, selectedByte, 2);

      }

      if(selectedFile.available()){
        selectedByte = selectedFile.read();
        loadVoice2OPL2(0xC0, selectedByte, 1);

      }

      loadVoiceVolumeRegistersOPL2();

    }
    else if(selectedByte == 0x78){                                      //OP4


      if(currentKeyMode == SINGLE){
        mainInstrumentOpMode = OP4;

        if(mainInstrumentOpMode == OP2)
          cleanSynthNotes();
      }
      else{
        if(currentInstrument == A)
          instrumentAOpMode = OP4;
        else
          instrumentBOpMode = OP4;
      }


      checkNative4OPChannels(false);

      int k = 0;

      for(int i = 0; i < 4; i++){
        k = 0x20 + (i * 0x20);
        if(selectedFile.available()){
          selectedByte = selectedFile.read();
          loadVoice2OPL3(k, selectedByte, 1);

        }
      }

      if(selectedFile.available()){
        selectedByte = selectedFile.read();
        loadVoice2OPL3(0xE0, selectedByte, 1);

      }
      
      for(int i = 0; i < 4; i++){
        k = 0x20 + (i * 0x20);
        if(selectedFile.available()){
          selectedByte = selectedFile.read();
          loadVoice2OPL3(k, selectedByte, 2);

        }
      }

      if(selectedFile.available()){
        selectedByte = selectedFile.read();
        loadVoice2OPL3(0xE0, selectedByte, 2);

      }

      if(selectedFile.available()){
        selectedByte = selectedFile.read();
        loadVoice2OPL3(0xC0, selectedByte, 1);

      }


      for(int i = 0; i < 4; i++){
        k = 0x20 + (i * 0x20);
        if(selectedFile.available()){
          selectedByte = selectedFile.read();
          loadVoice2OPL3(k, selectedByte, 3);

        }
      }

      if(selectedFile.available()){
        selectedByte = selectedFile.read();
        loadVoice2OPL3(0xE0, selectedByte, 3);

      }
      
      for(int i = 0; i < 4; i++){
        k = 0x20 + (i * 0x20);
        if(selectedFile.available()){
          selectedByte = selectedFile.read();
          loadVoice2OPL3(k, selectedByte, 4);

        }
      }

      if(selectedFile.available()){
        selectedByte = selectedFile.read();
        loadVoice2OPL3(0xE0, selectedByte, 4);

      }

      if(selectedFile.available()){
        selectedByte = selectedFile.read();
        loadVoice2OPL3(0xC0, selectedByte, 3);

      }

      loadVoiceVolumeRegistersOPL3();

    }
  }

  selectedFile.close();

}

void loadVoiceVolumeRegistersOPL2(){
  if(currentKeyMode == SINGLE)
      opl3->loadVRegAllChannelsOpl2();
  else{
    if((instrumentAOpMode == OP2) && (instrumentBOpMode == OP2)){
      if(currentInstrument == A)
        opl3->loadVRegLeftChannelsOpl2();
      else
        opl3->loadVRegRightChannelsOpl2();
    }
    else{
      opl3->loadVRegRightChannelsOpl2();
    }
  }
}

void loadVoiceVolumeRegistersOPL3(){
  if(currentKeyMode == SINGLE)
    opl3->loadVRegAllChannelsOpl3();
  else
    opl3->loadVRegLeftChannelsOp3();
}


void loadVoice2OPL2(uint8_t addressSector, uint8_t data, uint8_t opIndex){
  if(currentKeyMode == SINGLE)
      opl3->loadAllChannelsOpl2(addressSector, data, opIndex);
  else{
    if((instrumentAOpMode == OP2) && (instrumentBOpMode == OP2)){
      if(currentInstrument == A)
        opl3->loadLeftChannelsOpl2(addressSector, data, opIndex);
      else
        opl3->loadRightChannelsOpl2(addressSector, data, opIndex);
    }
    else{
      opl3->loadRightChannelsOpl2(addressSector, data, opIndex);
    }
  }
}

void loadVoice2OPL3(uint8_t addressSector, uint8_t data, uint8_t opIndex){
  if(currentKeyMode == SINGLE)
    opl3->loadAllChannelsOpl3(addressSector, data, opIndex);
  else{
    opl3->loadLeftChannelsOpl3(addressSector, data, opIndex);
  }
}

void checkNative4OPChannels(bool opl2){
  if(currentKeyMode == SINGLE){
    if(opl2){
      for(int i = 1; i < 7; i++)                                                  //disable all the 4 op channels
        opl3->setChannel4OpMode(false, i); 
    }
    else{
      for(int i = 1; i < 7; i++)                                                  //enable all the 4 op channels
        opl3->setChannel4OpMode(true, i); 
    }
  }
  else{
    if((instrumentAOpMode == OP2) && (instrumentBOpMode == OP2)){
      for(int i = 1; i < 7; i++)                                                  //disable all the 4 op channels
        opl3->setChannel4OpMode(false, i);
    }
    else{
      for(int i = 1; i < 6; i++)
        opl3->setChannel4OpMode(true, i);

      opl3->setChannel4OpMode(false, 6);
    }
  }
}

void saveInstrument(){

  presetChange = false;

  String tempName = "";
  String shortName = "";

  char tempChar;

  for(int i = 0; i < cleanMainInstrumentVoice.length(); i++){
    tempChar = cleanMainInstrumentVoice.charAt(i);

    if(tempChar != ' ' && tempChar != '.')
      tempName += tempChar;
  }

  int tempLength = tempName.length();

  if(tempLength > 8){    
    for(int i = 0; i < 5; i++)
      shortName += tempName.charAt(i);

    shortName += tempName.charAt(tempLength - 3);
    shortName += tempName.charAt(tempLength - 2);
    shortName += tempName.charAt(tempLength - 1);
  }
  else
    shortName = tempName;


  if(mainInstrumentOpMode == OP4){
    shortName += ".OP4";

    bool createFile = true;

    if(overwriteIt)
      createFile = SD.remove("/voices/" + shortName);
    

    if(createFile){
      File newFile = SD.open("/voices/" + shortName, FILE_WRITE);

      if(newFile){
        newFile.write(0x78);
      
        int k = 0;

        for(int i = 0; i < 4; i++){
          k = 0x20 + (i * 0x20);
          newFile.write(opl3->getRegisterAtAddress(k, 1, 1));
          delay(10);
        }

        newFile.write(opl3->getRegisterAtAddress(0xE0, 1, 1));
        delay(10);

        for(int i = 0; i < 4; i++){
          k = 0x20 + (i * 0x20);
          newFile.write(opl3->getRegisterAtAddress(k, 1, 2));
          delay(10);
        }

        newFile.write(opl3->getRegisterAtAddress(0xE0, 1, 2));
        delay(10);

        newFile.write(opl3->getRegisterAtAddress(0xC0, 1, 1));
        delay(10);

        
        //
        for(int i = 0; i < 4; i++){
          k = 0x20 + (i * 0x20);
          newFile.write(opl3->getRegisterAtAddress(k, 1, 3));
          delay(10);
        }

        newFile.write(opl3->getRegisterAtAddress(0xE0, 1, 3));
        delay(10);

        for(int i = 0; i < 4; i++){
          k = 0x20 + (i * 0x20);
          newFile.write(opl3->getRegisterAtAddress(k, 1, 4));
          delay(10);
        }

        newFile.write(opl3->getRegisterAtAddress(0xE0, 1, 4));
        delay(10);

        newFile.write(opl3->getRegisterAtAddress(0xC0, 1, 3));
        delay(10);


        for(int i = 0; i < cleanMainInstrumentVoice.length(); i++){
          if(i < 18)
            newFile.print(cleanMainInstrumentVoice.charAt(i));
        }
      }
      else
        Serial.println("ERROR SAVING FILE");

      newFile.close();
    }
    
  }
  else{
    shortName += ".OP2";

    bool createFile = true;

    if(overwriteIt)
      createFile = SD.remove("/voices/" + shortName);
    

    if(createFile){
      File newFile = SD.open("/voices/" + shortName, FILE_WRITE);

      if(newFile){
        newFile.write(0x3C);
      
        int k = 0;

        for(int i = 0; i < 4; i++){
          k = 0x20 + (i * 0x20);
          newFile.write(opl3->getRegisterAtAddress(k, 1, 1));
          delay(10);
        }

        newFile.write(opl3->getRegisterAtAddress(0xE0, 1, 1));
        delay(10);

        for(int i = 0; i < 4; i++){
          k = 0x20 + (i * 0x20);
          newFile.write(opl3->getRegisterAtAddress(k, 1, 2));
          delay(10);
        }

        newFile.write(opl3->getRegisterAtAddress(0xE0, 1, 2));
        delay(10);

        newFile.write(opl3->getRegisterAtAddress(0xC0, 1, 1));
        delay(10);


        for(int i = 0; i < cleanMainInstrumentVoice.length(); i++){
          if(i < 18)
            newFile.print(cleanMainInstrumentVoice.charAt(i));
        }

      }
      else
        Serial.println("ERROR SAVING FILE");

      newFile.close();
    }

    newInstrument = false;
  }

  preMainInstrumentVoice = cleanMainInstrumentVoice;

  initSD();

  setScreen(SAVED_VOICE);

  if(overwriteIt){
    overwriteIt = false;
    //screenActivity->setPreviousScreen(SAVE_INSTRUMENT);
  }

  if(onlySave){
    onlySave = false;
    screenActivity->setPreviousScreen(MODE_A);

    if(initSD()){
        //localOrSD1 = false;
        memState = SD_FOLDER;
        setSDVoicesTotals();  

        findSavedFile(shortName);

        updateCurrentSDInPage(currentSdVoice);
        getSDVoicePageList(currentSdPage);
        loadTEMPorSDVoice(currentSdVoiceInPage, 0, true);

        
      }
      else{
        //localOrSD1 = true;
        memState = LOCAL_FOLDER;
        getLocalVoicePageList(currentLocalPage);
        loadLocalVoice(currentLocalVoice);  
        setScreen(NO_SD_CARD);
      }
  }  

}

void saveTempVoice(String vgmStr, int tempPC, int tempCh, bool isOPl3){

  String tempName = "";
  String shortName = "";
  String tempPCStr = String(tempPC, DEC);

  uint8_t tempByte;

  char tempChar;

  for(int i = 0; i < 10; i++){
    tempChar = vgmStr.charAt(i);
    if(tempChar != ' ' && tempChar != '.')
      tempName += tempChar;
  }

  tempName += "Voice";
  tempName += tempPCStr;

  int tempLength = tempName.length();

  if(tempLength > 8){    
    for(int i = 0; i < 5; i++)        
      shortName += tempName.charAt(i);

    shortName += tempName.charAt(tempLength - 3);
    shortName += tempName.charAt(tempLength - 2);
    shortName += tempName.charAt(tempLength - 1);
  }
  else
    shortName = tempName;


  if(isOPl3){
    shortName += ".OP4";


    File newFile = SD.open("/temp/" + shortName, FILE_WRITE);

    if(newFile){
      newFile.write(0x78);
      
      int k = 0;

      for(int i = 0; i < 4; i++){
        k = 0x20 + (i * 0x20);
        newFile.write(opl3->getRegisterAtAddress(k, tempCh, 1));
        delay(10);
      }

      newFile.write(opl3->getRegisterAtAddress(0xE0, tempCh, 1));
      delay(10);

      for(int i = 0; i < 4; i++){
        k = 0x20 + (i * 0x20);
        newFile.write(opl3->getRegisterAtAddress(k, tempCh, 2));
        delay(10);
      }

      newFile.write(opl3->getRegisterAtAddress(0xE0, tempCh, 2));
      delay(10);

      tempByte = opl3->getRegisterAtAddress(0xC0, tempCh, 1);
      tempByte &= 0xF;
      tempByte += 0x30;

      newFile.write(tempByte);
      delay(10);


      for(int i = 0; i < 4; i++){
        k = 0x20 + (i * 0x20);
        newFile.write(opl3->getRegisterAtAddress(k, tempCh, 3));
        delay(10);
      }

      newFile.write(opl3->getRegisterAtAddress(0xE0, tempCh, 3));
      delay(10);

      for(int i = 0; i < 4; i++){
        k = 0x20 + (i * 0x20);
        newFile.write(opl3->getRegisterAtAddress(k, tempCh, 4));
        delay(10);
      }

      newFile.write(opl3->getRegisterAtAddress(0xE0, tempCh, 4));
      delay(10);

      tempByte = opl3->getRegisterAtAddress(0xC0, tempCh, 3);
      tempByte &= 0xF;
      tempByte += 0x30;

      newFile.write(tempByte);
      delay(10);


      for(int i = 0; i < tempName.length(); i++){
        if(i < 18)
          newFile.print(tempName.charAt(i));
      }
    }
    else
      Serial.println("ERROR SAVING FILE");

    newFile.close();
    
  }//ENF if(isOPL3)
  else{
    shortName += ".OP2";

    bool createFile = true;    

    if(createFile){

      File newFile = SD.open("/temp/" + shortName, FILE_WRITE);

      if(newFile){
        newFile.write(0x3C);
      
        int k = 0;

        for(int i = 0; i < 4; i++){
          k = 0x20 + (i * 0x20);
          newFile.write(opl3->getRegisterAtAddress(k, tempCh, 1));
          delay(10);
        }

        newFile.write(opl3->getRegisterAtAddress(0xE0, tempCh, 1));
        delay(10);

        for(int i = 0; i < 4; i++){
          k = 0x20 + (i * 0x20);
          newFile.write(opl3->getRegisterAtAddress(k, tempCh, 2));
          delay(10);
        }

        newFile.write(opl3->getRegisterAtAddress(0xE0, tempCh, 2));
        delay(10);

        tempByte = opl3->getRegisterAtAddress(0xC0, tempCh, 1);
        tempByte &= 0xF;
        tempByte += 0x30;

        newFile.write(tempByte);
        delay(10);


        for(int i = 0; i < tempName.length(); i++){
          if(i < 18)
            newFile.print(tempName.charAt(i));
        }

      }
      else
        Serial.println("ERROR SAVING FILE");

      newFile.close();
    }

  }//END else

}

bool isAOp4Voice(int tempCh){
  bool tempReturn = false;

  int tempByte = opl3->getRegisterAt(0x04, true);

  switch(tempCh){
    case 1:
      tempByte = tempByte & 0x1;
      if(tempByte == 0x1)
        tempReturn = true;
    break;
    case 2:
      tempByte = tempByte & 0x2;
      if(tempByte == 0x2)
        tempReturn = true;
    break;
    case 3:
      tempByte = tempByte & 0x4;
      if(tempByte == 0x4)
        tempReturn = true;
    break;
    case 4: 
      tempByte = tempByte & 0x8;
      if(tempByte == 0x8)
        tempReturn = true;
    break;
    case 5:
      tempByte = tempByte & 0x10;
      if(tempByte == 0x10)
        tempReturn = true;
    break;
    case 6:
      tempByte = tempByte & 0x20;
      if(tempByte == 0x20)
        tempReturn = true;
    break;
  }


  return tempReturn;
}

bool dontExistTempVoice(int tempCh, bool isOPl3){
  bool tempReturn = true;

  uint8_t tempByte = 0;
  bool areEqual = true;

  tempFolder.rewindDirectory();

  if(isOPl3){

    while(true){

      areEqual = true;

      File entry = tempFolder.openNextFile();
      if(!entry){
        areEqual = false;
        break;
      }

      if(!entry.isDirectory()){
        
        int k = 0;

        if(entry.available()){
          if(entry.read() == 0x78){

            for(int i = 0; i < 4; i++){
              k = 0x20 + (i * 0x20);
              
              if(opl3->getRegisterAtAddress(k, tempCh, 1) != entry.read())
                areEqual = false;
            }

            if(opl3->getRegisterAtAddress(0xE0, tempCh, 1) != entry.read())
              areEqual = false;

            for(int i = 0; i < 4; i++){
              k = 0x20 + (i * 0x20);

              if(opl3->getRegisterAtAddress(k, tempCh, 2) != entry.read())
                areEqual = false;
            }

            if(opl3->getRegisterAtAddress(0xE0, tempCh, 2) != entry.read())
              areEqual = false;

            tempByte = opl3->getRegisterAtAddress(0xC0, tempCh, 1);
            tempByte &= 0xF;
            tempByte += 0x30;

            if(tempByte != entry.read())
              areEqual = false;


            for(int i = 0; i < 4; i++){
              k = 0x20 + (i * 0x20);
              
              if(opl3->getRegisterAtAddress(k, tempCh, 3) != entry.read())
                areEqual = false;
            }

            if(opl3->getRegisterAtAddress(0xE0, tempCh, 3) != entry.read())
              areEqual = false;

            for(int i = 0; i < 4; i++){
              k = 0x20 + (i * 0x20);

              if(opl3->getRegisterAtAddress(k, tempCh, 4) != entry.read())
                areEqual = false;
            }

            if(opl3->getRegisterAtAddress(0xE0, tempCh, 4) != entry.read())
              areEqual = false;

            tempByte = opl3->getRegisterAtAddress(0xC0, tempCh, 3);
            tempByte &= 0xF;
            tempByte += 0x30;

            if(tempByte != entry.read())
              areEqual = false;

          }//END if(entry.read() == 0x78)
          else
            areEqual = false;
        }//END if(entry.available())
        else
          areEqual = false;

      }

      entry.close();

      if(areEqual)
        break;
   
    }//END while(true)

    if(areEqual)
      tempReturn = false;

  }
  else{

    while(true){

      areEqual = true;

      File entry = tempFolder.openNextFile();
      if(!entry){
        areEqual = false;
        break;
      }

      
      if(!entry.isDirectory()){
        
        int k = 0;

        if(entry.available()){
          if(entry.read() == 0x3C){

            for(int i = 0; i < 4; i++){
              k = 0x20 + (i * 0x20);
              
              if(opl3->getRegisterAtAddress(k, tempCh, 1) != entry.read())
                areEqual = false;
            }

            if(opl3->getRegisterAtAddress(0xE0, tempCh, 1) != entry.read())
              areEqual = false;

            for(int i = 0; i < 4; i++){
              k = 0x20 + (i * 0x20);

              if(opl3->getRegisterAtAddress(k, tempCh, 2) != entry.read())
                areEqual = false;
            }

            if(opl3->getRegisterAtAddress(0xE0, tempCh, 2) != entry.read())
              areEqual = false;

            tempByte = opl3->getRegisterAtAddress(0xC0, tempCh, 1);
            tempByte &= 0xF;
            tempByte += 0x30;

            if(tempByte != entry.read())
              areEqual = false;

          }//END if(entry.read() == 0x3C)
          else
            areEqual = false;
        }//END if(entry.available())
        else
          areEqual = false;

      }

      entry.close();

      if(areEqual)
        break;
   
    }

    if(areEqual)
      tempReturn = false;
    

  }//END else

  return tempReturn;
}


void findSavedFile(String fileName){

  int indexFlow = 0;
  int selectedIndex = -1;

  voicesFolder.rewindDirectory();

  while(true){
    File entry = voicesFolder.openNextFile();
    if(!entry)
      break;
      
    if(!entry.isDirectory()){

      if(fileName.equals(entry.name()))
        selectedIndex = indexFlow;  

    }
    entry.close();

    indexFlow++;
  }

  
  if(selectedIndex >= 0){
    currentSdVoice = (selectedIndex + 1);
    currentSdPage = ((int) (selectedIndex / 9)) + 1;

    updateCurrentSDInPage(currentSdVoice);
    getSDVoicePageList(currentSdPage);
    loadTEMPorSDVoice(currentSdVoiceInPage, 0, true);
  }
  
}

bool isCompatibleVGMFile(){
  bool tempState = false;

  String tempName = vgmSDNameList[currentSdVgmInPage];

  File selectedFile = SD.open("/vgm/"+tempName);

  if(selectedFile){

    int selectedByte = 0;
    int countIt = 0;

    bool lockA = false;
    bool lockB = false;
    bool lockC = false;
    bool lockD = false;

    bool vgmType = false;
    bool opl2or3Type = false;

    while(selectedFile.available()){
      selectedByte = selectedFile.read();

      if(selectedByte == 0x56 && countIt == 0)
        lockA = true;
      else if(selectedByte == 0x67 && countIt == 1)
        lockB = true;
      else if(selectedByte == 0x6d && countIt == 2)
        lockC = true;
      else if(selectedByte == 0x20 && countIt == 3)
        lockD = true;
      
      if(countIt > 3)
        break;    

      countIt++;

    }

    if(lockA && lockB && lockC && lockD)
      vgmType = true;

    lockA = false;
    lockB = false;
    lockC = false;
    lockD = false;

    selectedFile.rewindDirectory();

    countIt = 0;

    if(selectedFile.seek(0x50)){                            //OPL2
      while(selectedFile.available()){
        selectedByte = selectedFile.read();

        if(selectedByte == 0x99 && countIt == 0)
          lockA = true;
        else if(selectedByte == 0x9e && countIt == 1)
          lockB = true;
        else if(selectedByte == 0x36 && countIt == 2)
          lockC = true;
      
        if(countIt > 2)
          break;    

        countIt++;

      }//END while
    }//END if()

    if(lockA && lockB && lockC){
      opl2or3Type = true;
      opType = 2;
    }


    if(!opl2or3Type){
      lockA = false;
      lockB = false;
      lockC = false;
      lockD = false;

      selectedFile.rewindDirectory();

      countIt = 0;

      if(selectedFile.seek(0x5c)){                            //OPL3
        while(selectedFile.available()){
          selectedByte = selectedFile.read();

          if(selectedByte == 0x64 && countIt == 0)
            lockA = true;
          else if(selectedByte == 0x7a && countIt == 1)
            lockB = true;
          else if(selectedByte == 0xda && countIt == 2)
            lockC = true;
      
          if(countIt > 2)
            break;    

          countIt++;

        }//END while
      }//END if()

      if(lockA && lockB && lockC){
        opl2or3Type = true;
        opType = 3;
      }

    }//if(!opl2or3Type)


    if(!opl2or3Type){
      lockA = false;
      lockB = false;
      lockC = false;
      lockD = false;

      selectedFile.rewindDirectory();

      countIt = 0;

      if(selectedFile.seek(0x54)){                            //OPL1
        while(selectedFile.available()){
          selectedByte = selectedFile.read();

          if(selectedByte == 0x00 && countIt == 0)
            lockA = true;
          else if(selectedByte == 0x9 && countIt == 1)
            lockB = true;
          else if(selectedByte == 0x3d && countIt == 2)
            lockC = true;
      
          if(countIt > 2)
            break;    

          countIt++;

        }//END while
      }//END if()

      if(lockA && lockB && lockC){
        opl2or3Type = true;
        opType = 1;
      }


    }//if(!opl2or3Type)

    if(vgmType && opl2or3Type)
      tempState = true;

  }

  return tempState;
}

/*
void readUsbMIDIKeyboard(){
  char buf[20];
  uint8_t bufMidi[64];
  uint16_t rcvd;

  if(Midi.RecvDatat(&rcvd, bufMidi) == 0){
    byte type = bufMidi[1]
    byte note = bufMidi[2];
    byte velocity = bufMidi[3];

    switch(){
      case 0x90:                                  //NOTE ON
        if(currentKeyMode == SINGLE){             //SYNTH
          if(mainInstrumentOpMode == OP2){
            while(doCycle){           
              singlePC++;
              if(singlePC > 11)
                singlePC = 0;
  
              if((notesSpeed[singlePC] == 0) || pedalSustain){
                sustainRelease[singlePC] = false;
                notes[singlePC] = note;
                notesSpeed[singlePC] = velocity;
                notesTrigger[singlePC] = true;
                doCycle = false;
                notes2FM = true;
              }
                            
              cycleCount++;
              if(cycleCount > 11)
                doCycle = false;            
                        
            }//END while(doCycle)
          }
          else{
            while(doCycle){           
              singlePC++;
              if(singlePC > 5)
                singlePC = 0;
                                   
              if((notesSpeed[singlePC] == 0) || pedalSustain){
                sustainRelease[singlePC] = false;
                notes[singlePC] = note;
                notesSpeed[singlePC] = velocity;
                notesTrigger[singlePC] = true;
                doCycle = false;
                notes2FM = true;
              }
                            
              cycleCount++;
              if(cycleCount > 5)
                doCycle = false;
                                   
            }//END while(doCycle)  
          }//END else
        }//END synthMode = true
        else if(currentKeyMode == DUAL){          //DUAL
          bool rightSide = false;

          if(setNotepoint){
            setNotepoint = false;
            notepoint = note; 
          }
          else{
            if(op4op2Set){
              if(note > (notepoint - 1))
                rightSide = true;
            }
            else{
              if(note < notepoint)
                rightSide = true;
            } 
     
            if(rightSide){                                    //NOTE 60 is the middle   RIGHT SIDE B  if(note > 59)
              while(doCycle){           
                singlePCB++;
                if(singlePCB > 4)
                  singlePCB = 0;
                        
                if((notesSpeedB[singlePCB] == 0) || pedalSustain){
                  sustainReleaseB[singlePCB] = false;
                  notesB[singlePCB] = note;
                  notesSpeedB[singlePCB] = velocity;
                  notesTriggerB[singlePCB] = true;
                  doCycle = false;
                  notes2FM = true;
                }
                            
                cycleCount++;
                if(cycleCount > 4)
                  doCycle = false;            
                        
              }//END while(doCycle)  
            }//END if(note > 59)
            else{                                             //LEFT SIDE A
              while(doCycle){           
                singlePCA++;
                if(singlePCA > 4)
                  singlePCA = 0;
                        
                if((notesSpeedA[singlePCA] == 0) || pedalSustain){
                  sustainReleaseA[singlePCA] = false;
                  notesA[singlePCA] = note;
                  notesSpeedA[singlePCA] = velocity;
                  notesTriggerA[singlePCA] = true;
                  doCycle = false;
                  notes2FM = true;  
                }
                            
                cycleCount++;
                if(cycleCount > 4)
                  doCycle = false;
                 
              }//END while(doCycle)
            }//END else    
          }//END else 
        }//END else DUAL
      break;
      case 0x80:                                  //NOTE OFF
        if(currentKeyMode == SINGLE){                                      //SINGLE                         
          if(mainInstrumentOpMode == OP2){
            for(int i = 0; i < 12; i++){
              if(notes[i] == note){

                if(pedalSustain)
                  sustainRelease[i] = true;
                else{
                  preNotes[i] = notes[i];
                  notes[i] = 255;
                  notesSpeed[i] = 0;
                  notesTrigger[i] = true;
                  notes2FM = true;
                }//END else 
              }
            }//END for
          }      
          else{
            for(int i = 0; i < 6; i++){
              if(notes[i] == note){

                if(pedalSustain)
                  sustainRelease[i] = true;
                else{
                  preNotes[i] = notes[i];
                  notes[i] = 255;
                  notesSpeed[i] = 0;
                  notesTrigger[i] = true;
                  notes2FM = true;
                }
              }
            }//END for
          }//END else
                        
        }//END if(synthMode)
        else if(currentKeyMode == DUAL){                                               //DUAL  
          bool rightSide = false;
          if(currentKeyMode == DUAL){                                                                             //DUAL
            if(op4op2Set){
              if(note > (notepoint - 1))
                rightSide = true;
            }
            else{
              if(note < notepoint)
                rightSide = true;
            }
          }
          else{                                                   //D_ARP
            if(op4op2Set)
              rightSide = false;
            else
              rightSide = true;
          }

          if(rightSide){                                    //NOTE 60 is the middle   RIGHT SIDE B
            for(int i = 0; i < 5; i++){
              if(notesB[i] == note){

                if(pedalSustain)
                  sustainReleaseB[i] = true;
                else{
                  preNotesB[i] = notesB[i];
                  notesB[i] = 255;
                  notesSpeedB[i] = 0;
                  notesTriggerB[i] = true;
                  notes2FM = true;
                }//END else   
              }//END if
            }//END for
             
          }//END if(note > 59)
          else{                                             //LEFT SIDE A
            for(int i = 0; i < 5; i++){
              if(notesA[i] == note){
                if(pedalSustain)
                  sustainReleaseA[i] = true;
                else{
                  preNotesA[i] = notesA[i];
                  notesA[i] = 255;
                  notesSpeedA[i] = 0;
                  notesTriggerA[i] = true;
                  notes2FM = true;         
                }//END else
              }//END if
            }//END for        
          }//END else LEFT SIDE A
        }//END else
        debugReadThis();
      break;
      case 224:                                   //PITCH

      break;
      case 176:                                   //OTHER

      break;
    }//END switch  
  }//END if(Midi.RecvDatat(&rcvd, bufMidi) == 0)
}//END readUsbMIDIKeyboard()
*/


void readMIDIKeyboard(){

  int type, note, velocity, channel, d1, d2, selectedNote;
  bool doCycle = true;
  int cycleCount = 0;

  bool exeRead = false;

  if(MIDI.read()){      
    type = MIDI.getType();
    note = MIDI.getData1();
    velocity = MIDI.getData2();

    swapMidiLed();
      
    exeRead = true;
  }//END if(MIDI.read())
  
  
  if(exeRead) {                   
    switch (type) {
      case midi::NoteOn:
        if(currentKeyMode == SINGLE){                                  //SYNTH         
          if(mainInstrumentOpMode == OP2){
            while(doCycle){           
              singlePC++;
              if(singlePC > 11)
                singlePC = 0;       
              
              if((notesSpeed[singlePC] == 0) || pedalSustain){
                sustainRelease[singlePC] = false;
                notes[singlePC] = note;
                notesSpeed[singlePC] = velocity;
                notesTrigger[singlePC] = true;
                doCycle = false;
                notes2FM = true;
              }
                                
              cycleCount++;
              if(cycleCount > 11)
                doCycle = false;               
                        
            }//END while(doCycle)
          }//END if
          else{
            while(doCycle){           
              singlePC++;
              if(singlePC > 5)
                singlePC = 0;
                        
              if((notesSpeed[singlePC] == 0) || pedalSustain){      
                sustainRelease[singlePC] = false;
                notes[singlePC] = note;
                notesSpeed[singlePC] = velocity;
                notesTrigger[singlePC] = true;
                doCycle = false;
                notes2FM = true;
              }
                            
              cycleCount++;
              if(cycleCount > 5)
                doCycle = false;       
                        
            }//END while(doCycle)  
          }//END else               
        }//END synthMode = true
        else if(currentKeyMode == DUAL){                                               //DUAL

          bool rightSide = false;

          if(setNotepoint){
            setNotepoint = false;
            notepoint = note; 
          }
          else{
            if(op4op2Set){
              if(note > (notepoint - 1))
                rightSide = true;
            }
            else{
              if(note < notepoint)
                rightSide = true;
            } 
          
            if(rightSide){                                    //NOTE 60 is the middle   RIGHT SIDE B  if(note > 59)
              while(doCycle){           
                singlePCB++;
                if(singlePCB > 4)
                  singlePCB = 0;
                
                if((notesSpeedB[singlePCB] == 0) || pedalSustain){
                  sustainReleaseB[singlePCB] = false;
                  notesB[singlePCB] = note;
                  notesSpeedB[singlePCB] = velocity;
                  notesTriggerB[singlePCB] = true;
                  doCycle = false;
                  notes2FM = true;
                }
                            
                cycleCount++;
                if(cycleCount > 4)
                  doCycle = false;        
                        
              }//END while(doCycle)  
            }//END if(note > 59)
            else{                                             //LEFT SIDE A
              while(doCycle){           
                singlePCA++;
                if(singlePCA > 4)
                  singlePCA = 0;
                        
                if((notesSpeedA[singlePCA] == 0) || pedalSustain){
                  sustainReleaseA[singlePCA] = false;
                  notesA[singlePCA] = note;
                  notesSpeedA[singlePCA] = velocity;
                  notesTriggerA[singlePCA] = true;
                  doCycle = false;
                  notes2FM = true;
                }
                            
                cycleCount++;
                if(cycleCount > 4)
                  doCycle = false;
                        
              }//END while(doCycle)
            }//END else    
          }//END else              
        }//END else if(currentKeyMode == DUAL)
        
        debugReadThis();
      break;
      case midi::NoteOff:
        if(currentKeyMode == SINGLE){                                      //SINGLE                         

          if(mainInstrumentOpMode == OP2){
            for(int i = 0; i < 12; i++){
              if(notes[i] == note){
                if(pedalSustain)
                  sustainRelease[i] = true;
                else{
                  preNotes[i] = notes[i];
                  notes[i] = 255;
                  notesSpeed[i] = 0;
                  notesTrigger[i] = true;
                  notes2FM = true;
                }//END else   
              }//END if
            }//END for
          }//END if
          else{
            for(int i = 0; i < 6; i++){
              if(notes[i] == note){

                if(pedalSustain)
                  sustainRelease[i] = true;
                else{
                  preNotes[i] = notes[i];
                  notes[i] = 255;
                  notesSpeed[i] = 0;
                  notesTrigger[i] = true;
                  notes2FM = true;
                }
              }//END if
            }//END for
          }//END else
                    
        
        }//END if(synthMode)
        else if(currentKeyMode == DUAL){                                               //DUAL  

          bool rightSide = false;

          if(currentKeyMode == DUAL){                                                                             //DUAL
            if(op4op2Set){
              if(note > (notepoint - 1))
                rightSide = true;
            }
            else{
              if(note < notepoint)
                rightSide = true;
            }
          }
          else{                                                   //D_ARP
            if(op4op2Set)
              rightSide = false;
            else
              rightSide = true;
          }

          if(rightSide){                                    //NOTE 60 is the middle   RIGHT SIDE B
            for(int i = 0; i < 5; i++){
              if(notesB[i] == note){
                if(pedalSustain)
                  sustainReleaseB[i] = true;
                else{
                  preNotesB[i] = notesB[i];
                  notesB[i] = 255;
                  notesSpeedB[i] = 0;
                  notesTriggerB[i] = true;
                  notes2FM = true;
                }
              }
            }//END for
             
          }//END if(note > 59)
          else{                                             //LEFT SIDE A
            for(int i = 0; i < 5; i++){
              if(notesA[i] == note){
                if(pedalSustain)
                  sustainReleaseA[i] = true;
                else{
                  preNotesA[i] = notesA[i];
                  notesA[i] = 255;
                  notesSpeedA[i] = 0;
                  notesTriggerA[i] = true;
                  notes2FM = true;
                }
              }
            }//END for     
          }//END else LEFT SIDE A
        }//END else
        debugReadThis();
      break;
      case 224:                         //PITCH

        d1 = note;
        d2 = velocity;

        if(currentKeyMode != VGM){
          pitch = d2;

          notes2FM = true;
          pitchTrigger = true;
        }
      break;
      case 176:                        //OTHER

        d1 = note;
        d2 = velocity;

        if(currentKeyMode != VGM){

          if(d1 == 1){                   //MODULATION
            mod = d2;
            notes2FM = true;
            modTrigger = true;
          }
          else if(d1 == 64){              //SUSTAIN PEDAL
            if(d2 == 0){
              pedalSustain = false;
              pedalSustainTrigger = true;
              notes2FM = true;
            }
            else
              pedalSustain = true;  
          }//END else if(d1 == 64)
        }
        
      break;
      default:
        d1 = note;
        d2 = velocity;
        
        //Serial.println(String("Message, type=") + type + ", data = " + d1 + " " + d2);
        
      break;
    }//END SWITCH
      
  }//END if(exe)
}

void convertMidiToNote(int myNote){
    selectedOctave = (uint8_t)((myNote / 12)-1);
    
    selectedNote = (uint8_t)(myNote % 12);
}

void sendNotes2FM(){
    notes2FM = false;

    if(pedalSustainTrigger){
      pedalSustainTrigger = false;

      if(!pedalSustain){
        if(currentKeyMode == SINGLE){                   //SINGLE
          for(int i = 0; i < 12; i++){
            if(sustainRelease[i]){
              sustainRelease[i] = false;
              preNotes[i] = notes[i];
              notes[i] = 255;
              notesSpeed[i] = 0;
              notesTrigger[i] = true;
            }
          }
        }//END if(currentKeyMode == SINGLE)
        else{                                           //DUAL
          for(int i = 0; i < 5; i++){

            if(sustainReleaseA[i]){
              sustainReleaseA[i] = false;
              preNotesA[i] = notesA[i];
              notesA[i] = 255;
              notesSpeedA[i] = 0;
              notesTriggerA[i] = true;
            }

            if(sustainReleaseB[i]){
              sustainReleaseB[i] = false;
              preNotesB[i] = notesB[i];
              notesB[i] = 255;
              notesSpeedB[i] = 0;
              notesTriggerB[i] = true;
            }
          
          }
        }//END DUAL
      }//END else

      

    }//END if(pedalSustainTrigger)
    
    if(currentKeyMode == SINGLE  || overrideSend2FM){                           //SINGLE
      if(mainInstrumentOpMode == OP2){
        for(int i = 0; i < 12; i++){                                            //12
            if(notesTrigger[i]){
                notesTrigger[i] = false;
            
                if(notes[i] != 255){                        //NOTE ON
                    /*
                    Serial.print("S-ON: ");
                    Serial.print(notes[i]);
                    Serial.print(" - "); 
                    Serial.println(notesSpeed[i]);
                    */
                
                    //SEND NOTE ON TO FM      
                  convertMidiToNote(notes[i]);

                  if(i < 6)
                    opl3->playNote((i + 1), selectedOctave, selectedNote, pitch, rightBend, notesSpeed[i]);
                  else
                    opl3->playNote((i + 4), selectedOctave, selectedNote, pitch, rightBend, notesSpeed[i]);      
                }//END if(notes[i] != 255)
                else{                                       //NOTE OFF
                    /*
                    Serial.print("S-OFF: ");
                    Serial.print(preNotes[i]);
                    Serial.println(" - 0"); 
                    */
                
                    //SEND NOTE OFF TO FM
                  convertMidiToNote(preNotes[i]);
                          
                  if(i < 6)
                    opl3->releaseNote((i + 1));
                  else
                    opl3->releaseNote((i + 4));
                }           
            
            }//END if(notesTrigger[i])
        }//END for(int i = 0; i < 12; i++)
      }
      else{                                                                     //OP4
        for(int i = 0; i < 6; i++){                                            //12
            if(notesTrigger[i]){
                notesTrigger[i] = false;
            
                if(notes[i] != 255){                        //NOTE ON
                    /*
                    Serial.print("OP4 S-ON: ");
                    Serial.print(notes[i]);
                    Serial.print(" - "); 
                    Serial.println(notesSpeed[i]);
                    */
                
                    //SEND NOTE ON TO FM      
                    
                  convertMidiToNote(notes[i]);
          
                  if(i < 3)
                    opl3->playNote((i + 1), selectedOctave, selectedNote, pitch, rightBend, notesSpeed[i]);
                  else
                    opl3->playNote((i + 7), selectedOctave, selectedNote, pitch, rightBend, notesSpeed[i]);

                }//END if(notes[i] != 255)
                else{                                       //NOTE OFF
                  /*
                    Serial.print("S-OFF: ");
                    Serial.print(preNotes[i]);
                    Serial.println(" - 0"); 
                    */
                
                    //SEND NOTE OFF TO FM
                    
                  convertMidiToNote(preNotes[i]);
                    
                  if(i < 3)
                    opl3->releaseNote((i + 1));
                  else
                    opl3->releaseNote((i + 7));
                }           
            
            }//END if(notesTrigger[i])
        }//END for(int i = 0; i < 12; i++)
      }
    
    }//END if(myData->getMode() == SYNTH)
    
    if(currentKeyMode == DUAL || overrideSend2FM){                     //DUAL
        for(int i = 0; i < 5; i++){
            if(notesTriggerA[i]){
                notesTriggerA[i] = false;
                
                if(notesA[i] != 255){                       //NOTE ON
                    /*
                    Serial.print("LA-ON: ");
                    Serial.print(notesA[i]);
                    Serial.print(" - ");
                    Serial.println(notesSpeedA[i]);
                    */
                    
                  convertMidiToNote(notesA[i]);
              
                    //SEND NOTE ON TO FM
                  if(instrumentAOpMode == OP4){
                    if(i < 3)
                      opl3->playNote((i + 1), selectedOctave, selectedNote, pitch, rightBend, notesSpeedA[i]);
                    else
                      opl3->playNote((i + 7), selectedOctave, selectedNote, pitch, rightBend, notesSpeedA[i]);
                  }
                  else
                    opl3->playNote((i + 1), selectedOctave, selectedNote, pitch, rightBend, notesSpeedA[i]);
                    
                }//END if(notesA[i] != 255)
                else{                                       //NOTE OFF
                  /*
                    Serial.print("LA OFF: ");
                    Serial.print(preNotesA[i]);
                    Serial.println(" - 0");   
                    */
                    
                    //SEND NOTE OFF TO FM
                  if(instrumentAOpMode == OP4){
                    if(i < 3)
                      opl3->releaseNote((i+1));
                    else
                      opl3->releaseNote((i+7));
                  }
                  else
                    opl3->releaseNote((i+1));         
                }
                
            }
            if(notesTriggerB[i]){
                notesTriggerB[i] = false;
                
                if(notesB[i] != 255){                       //NOTE ON
                /*
                    Serial.print("LB-ON: ");
                    Serial.print(notesB[i]);
                    Serial.print(" - ");
                    Serial.println(notesSpeedB[i]);
                    */

                  convertMidiToNote(notesB[i]);
        
                    //SEND NOTE ON TO FM       
                  if(i == 0)             
                    opl3->playNote(12, selectedOctave, selectedNote, pitch, rightBend, notesSpeedB[i]);
                  else
                    opl3->playNote((i + 14), selectedOctave, selectedNote, pitch, rightBend, notesSpeedB[i]);
                    
                }//END if(notesB[i] != 255)
                else{                                       //NOTE OFF
                  /*
                    Serial.print("LB OFF: ");
                    Serial.print(preNotesB[i]);
                    Serial.println(" - 0");
                    */
                  
                    //SEND NOTE OFF TO FM
                  if(i == 0)
                    opl3->releaseNote(12);
                  else
                    opl3->releaseNote((i + 14));           
                }
            }
        
            
        }//END for(int i = 0; i < 6; i++)
        
            
    }//END else if
    
    overrideSend2FM = false;
    
    
    if(modTrigger){
        modTrigger = false;
        //SEND BEND TO FM
        
    }//END if(bendTrigger)
    
    if(pitchTrigger){
      pitchTrigger = false;
        
      //SEND PITCH TO FM


      if(pitch >= 64){
        rightBend = true;
        pitch -= 64;
      }
      else if(pitch < 64){
        rightBend = false;
        pitch -= 63;
        pitch *= (-1);
      }


      if(currentKeyMode == SINGLE){                   //SINGLE
        if(mainInstrumentOpMode == OP2){              //OP2
          for(int i = 0; i < 12; i++){
            if(notes[i] != 255){
              convertMidiToNote(notes[i]);

              if(i < 6)
                opl3->bendNote((i + 1), selectedNote, pitch, rightBend);
              else
                opl3->bendNote((i + 4), selectedNote, pitch, rightBend);
            }
          }
        }//END OP2
        else{                                         //OP4
          for(int i = 0; i < 6; i++){
            if(notes[i] != 255){
              convertMidiToNote(notes[i]);

              if(i < 3)
                opl3->bendNote((i + 1), selectedNote, pitch, rightBend);
              else
                opl3->bendNote((i + 7), selectedNote, pitch, rightBend);
            }
          }
        }//END OP4
      }//END if(currentKeyMode == SINGLE)
      else{                                           //DUAL
        for(int i = 0; i < 5; i++){
          if(notesA[i] != 255){
            convertMidiToNote(notesA[i]);

            opl3->bendNote((i + 1), selectedNote, pitch, rightBend);
          }

          if(notesB[i] != 255){
            convertMidiToNote(notesB[i]);

            if(i == 0)
              opl3->bendNote(12, selectedNote, pitch, rightBend);
            else
              opl3->bendNote((i + 14), selectedNote, pitch, rightBend);
          }
        }
      }//END DUAL

        
    }//END if(pitchTirgger)
}

void cleanSynthNotes(){
    for(int i = 0; i < 12; i++){
      if(notesSpeed[i] != 0){
        sustainRelease[i] = false;
        notesSpeed[i] = 0;
        notesTrigger[i] = false;
        preNotes[i] = notes[i];
        notes[i] = 255;
      }
    }
    notes2FM = true;
    overrideSend2FM = true;
}

void cleanANotes(){
  for(int i = 0; i < 5; i++){
    if(notesSpeedA[i] != 0){
      sustainReleaseA[i] = false;
      notesSpeedA[i] = 0;
      notesTriggerA[i] = true;
      preNotesA[i] = notesA[i];
      notesA[i] = 255;
    }  
  }
}

void cleanSplitNotes(){
    for(int i = 0; i < 5; i++){
        if(notesSpeedA[i] != 0){
          sustainReleaseA[i] = false;
          notesSpeedA[i] = 0;
          notesTriggerA[i] = true;
          preNotesA[i] = notesA[i];
          notesA[i] = 255;
        }
        
        if(notesSpeedB[i] != 0){
          sustainReleaseB[i] = false;
          notesSpeedB[i] = 0;
          notesTriggerB[i] = true;
          preNotesB[i] = notesB[i];
          notesB[i] = 255;
        }
    }
    notes2FM = true;
    overrideSend2FM = true;
}

void stateChecker(){
  if((currentKeyMode == SINGLE) && (previousKeyMode == VGM)){
    previousKeyMode = SINGLE;
    cleanSplitNotes();

    if(accesLoaded)
      accesLoaded = false;
    else
      releaseShiftBtn();
  
  }
  else if((currentKeyMode == DUAL) && (previousKeyMode == SINGLE)){
    previousKeyMode = DUAL;
    cleanSynthNotes();
    
    if(accesLoaded)
      accesLoaded = false;
    else
      releaseShiftBtn();

  }
  else if((currentKeyMode == VGM) && (previousKeyMode == DUAL)){
    previousKeyMode = VGM;
    
    if(accesLoaded)
      accesLoaded = false;
    else
      releaseShiftBtn();

  }
  else{
    if(accesLoaded)
      accesLoaded = false;
  }
}

void readKnobsButtons(){

    switchACurrentState = ((PIOA->PIO_PDSR & PIO_PA23) == PIO_PA23);  //digitalRead(KNOB_A_BTN);
    switchBCurrentState = ((PIOA->PIO_PDSR & PIO_PA22) == PIO_PA22);  //digitalRead(KNOB_B_BTN);

    unsigned long tempTime = 0;

    if((switchACurrentState == 0) && (switchAPreviousState == 1)){
        if(!lockSwA){
            lockSwA = true;
            currentTimeSwA = millis();   
        }
    }
    if(lockSwA){
        tempTime = millis() - currentTimeSwA;
        if(tempTime > 200){
          if(switchACurrentState == 1){
            if(!(PIOA->PIO_PDSR & PIO_PA2))              //if(digitalRead(BUTTON_D) == 0)  
              holdSwA = true;
            else
              swA = true;
  
            lockSwA = false;
            delay(200);
          }
        }
    }

    if((switchBCurrentState == 0) && (switchBPreviousState == 1)){
        if(!lockSwB){
            lockSwB = true;
            currentTimeSwB = millis();
        }
    }
    if(lockSwB){
        tempTime = millis() - currentTimeSwB;
        if(tempTime > 200){
          if(switchBCurrentState == 1){
            if(!(PIOA->PIO_PDSR & PIO_PA2))              //if(digitalRead(BUTTON_D) == 0)
              holdSwB = true;
            else
              swB = true;

            lockSwB = false;
            delay(200);
          }
        }
    }

    
    switchAPreviousState = switchACurrentState;
    switchBPreviousState = switchBCurrentState;
}


void readButtons(){

    buttonACurrentState = ((PIOA->PIO_PDSR & PIO_PA6) == PIO_PA6);  //digitalRead(BUTTON_A);
    buttonBCurrentState = ((PIOA->PIO_PDSR & PIO_PA4) == PIO_PA4);  //digitalRead(BUTTON_B);
    buttonCCurrentState = ((PIOA->PIO_PDSR & PIO_PA3) == PIO_PA3);  //digitalRead(BUTTON_C);
    buttonDCurrentState = ((PIOA->PIO_PDSR & PIO_PA2) == PIO_PA2);  //digitalRead(BUTTON_D);
    buttonECurrentState = ((PIOB->PIO_PDSR & PIO_PB17) == PIO_PB17);  //digitalRead(BUTTON_E);
    buttonFCurrentState = ((PIOB->PIO_PDSR & PIO_PB18) == PIO_PB18);  //digitalRead(BUTTON_F);

    unsigned long tempTime = 0;

    if(lockBtA){
        tempTime = millis() - currentTimeBtA;
        if(tempTime > 200)
            lockBtA = false;
    }

    if((buttonACurrentState == 0) && (buttonAPreviousState == 1)){
        if(!lockBtA){
            btA = true;
            lockBtA = true;
            currentTimeBtA = millis();
        }
    }

    if(lockBtB){
        tempTime = millis() - currentTimeBtB;
        if(tempTime > 200)
            lockBtB = false;

    }

    if((buttonBCurrentState == 0) && (buttonBPreviousState == 1)){
        if(!lockBtB){
            btB = true;
            lockBtB = true;
            currentTimeBtB = millis();
        }
    }

    if(lockBtC){
        tempTime = millis() - currentTimeBtC;
        if(tempTime > 200)
            lockBtC = false;
    }

    if((buttonCCurrentState == 0) && (buttonCPreviousState == 1)){
        if(!lockBtC){
            btC = true;
            lockBtC = true;
            currentTimeBtC = millis();
        }
    }

    if(lockBtD){
        tempTime = millis() - currentTimeBtD;
        if(tempTime > 200)
            lockBtD = false;
    }

    if((buttonDCurrentState == 0) && (buttonDPreviousState == 1)){
        if(!lockBtD){
            btD = true;
            lockBtD = true;
            currentTimeBtD = millis();
        }
    }

    if(lockBtE){
        tempTime = millis() - currentTimeBtE;
        if(tempTime > 200)
            lockBtE = false;
    }

    if((buttonECurrentState == 0) && (buttonEPreviousState == 1)){
        if(!lockBtE){
            btE = true;
            lockBtE = true;
            currentTimeBtE = millis();
        }
    }

    if(lockBtF){
        tempTime = millis() - currentTimeBtF;
        if(tempTime > 200)
            lockBtF = false;
    }

    if((buttonFCurrentState == 0) && (buttonFPreviousState == 1)){
        if(!lockBtF){
            btF = true;
            lockBtF = true;
            currentTimeBtF = millis();
        }
    }



    buttonAPreviousState = buttonACurrentState;
    buttonBPreviousState = buttonBCurrentState;
    buttonCPreviousState = buttonCCurrentState;
    buttonDPreviousState = buttonDCurrentState;
    buttonEPreviousState = buttonECurrentState;
    buttonFPreviousState = buttonFCurrentState;

}

void checkUserInput(){
  
  if(btF){
    if(!(PIOA->PIO_PDSR & PIO_PA2)){                             //if(digitalRead(BUTTON_D) == 0)
      saveAccesFlag = true;
      currentTimeLedF = millis();
    }

    if(shiftBtn)
      releaseShiftBtn();
    else{
      shiftBtn = true;
      PIOD->PIO_CODR = PIO_PD7;                                 //digitalWrite(LED_F, LOW);   
    }
  }

  
  switch(screenActivity->getScreen()){
    case MODE_A:
      modeAUserInput();
    break;
    case MODE_B:
      if(!shiftBtn)
        modeBUserInput();
      else
        accessBtnsUserInput();
    break;
    case MODE_C:
      modeCUserInput();
    break;
    case OPERATOR:
      operatorUserInput();
    break;
    case CHANNEL:
      channelUserInput();
    break;
    case SAVE_INSTRUMENT:
      saveInsUserInput();
    break;
    case SAVE_MESSAGE:
      saveMessageUserInput();
    break;
    case OVERWRITE_VOICE:
      overwriteSaveUserInput();
    break;
  }
            
}

void accessBtnsUserInput(){
  if(btA){
    if(saveAccesFlag){
      releaseShiftBtn();
      if(saveBtnAccess(0))
        setScreen(SAVED_ACCESS);

    }
    else{
      releaseShiftBtn();
      if(loadBtnAccess(0)){
        PIOB->PIO_CODR = PIO_PB25;        //digitalWrite(LED_A, LOW);
        setScreen(LOADED_ACCESS);
        accesLoaded = true;
      }
    } 
  }
  else if(btB){
    if(saveAccesFlag){
      releaseShiftBtn();
      if(saveBtnAccess(1))
        setScreen(SAVED_ACCESS);
    }
    else{
      releaseShiftBtn();
      if(loadBtnAccess(1)){
        PIOC->PIO_CODR = PIO_PC28;        //digitalWrite(LED_B, LOW);
        setScreen(LOADED_ACCESS);
        accesLoaded = true;
      }
    }
  }
  else if(btC){
    if(saveAccesFlag){
      releaseShiftBtn();
      if(saveBtnAccess(2))
        setScreen(SAVED_ACCESS);
    }
    else{
      releaseShiftBtn();
      if(loadBtnAccess(2)){
        PIOC->PIO_CODR = PIO_PC25;        //digitalWrite(LED_C, LOW);
        setScreen(LOADED_ACCESS);
        accesLoaded = true;
      }
    }
  }
  else if(btD){
    if(saveAccesFlag){
      releaseShiftBtn();
      if(saveBtnAccess(3))
        setScreen(SAVED_ACCESS);
    }
    else{
      releaseShiftBtn();
      if(loadBtnAccess(3)){
        PIOC->PIO_CODR = PIO_PC24;      //digitalWrite(LED_D, LOW);
        setScreen(LOADED_ACCESS);
        accesLoaded = true;
      }
    }
  }
  else if(btE){
    if(saveAccesFlag){
      releaseShiftBtn();
      if(saveBtnAccess(4))
        setScreen(SAVED_ACCESS);
    }
    else{
      releaseShiftBtn();
      if(loadBtnAccess(4)){
        PIOC->PIO_CODR = PIO_PC23;      //digitalWrite(LED_E, LOW);
        setScreen(LOADED_ACCESS);
        accesLoaded = true;
      }
    }
  }
}

void modeAUserInput(){                                                  //MODE A

  if(!shiftBtn){
    if(knobALeft){
      releaseShiftBtn();
      if(voicePagerA){

        switch(memState){
          case LOCAL_FOLDER:
            currentLocalPage--;

            if(currentLocalPage < 1){
              currentLocalPage = localVoicesPages;
              currentLocalVoice = localVoices;  

              updateCurrentLocalInPage(localVoices);
   
              getLocalVoicePageList(currentLocalPage);
            }
            else{
              currentLocalVoice -= 9;

              updateCurrentLocalInPage(currentLocalVoice);

              getLocalVoicePageList(currentLocalPage);
            }

            loadLocalVoice(currentLocalVoice);
          break;
          case SD_FOLDER:
            currentSdPage--;

            if(currentSdPage < 1){
              currentSdPage = sdVoicesPages;
              currentSdVoice = sdVoices;  

              updateCurrentSDInPage(sdVoices);

              getSDVoicePageList(currentSdPage);
            }
            else{
              currentSdVoice -= 9;

              updateCurrentSDInPage(currentSdVoice);

              getSDVoicePageList(currentSdPage);
            }

            loadTEMPorSDVoice(currentSdVoiceInPage, 0, true);
          break;
          case TEMP_FOLDER:
            currentTempPage--;

            if(currentTempPage < 1){
              currentTempPage = tempVoicesPages;
              currentTempVoice = tempVoices;  

              updateCurrentTEMPInPage(tempVoices);

              getTEMPVoicePageList(currentTempPage);
            }
            else{
              currentTempVoice -= 9;

              updateCurrentTEMPInPage(currentTempVoice);

              getTEMPVoicePageList(currentTempPage);
            }

            loadTEMPorSDVoice(currentTempVoiceInPage, 0, false);
          break;

        }//END switch
      
      }
      else{

        switch(memState){
          case LOCAL_FOLDER:
            currentLocalVoiceInPage--;
            currentLocalVoice--;

            if(currentLocalVoice < 1){

              currentLocalVoice = localVoices;
              currentLocalPage = localVoicesPages;

              updateCurrentLocalInPage(localVoices);

              getLocalVoicePageList(currentLocalPage);

            }
            else{
              if(currentLocalVoiceInPage < 0){
                currentLocalPage--;
                currentLocalVoiceInPage = 8;

                getLocalVoicePageList(currentLocalPage);
              }
            }

            loadLocalVoice(currentLocalVoice);
          break;
          case SD_FOLDER:
            currentSdVoiceInPage--;
            currentSdVoice--;

            if(currentSdVoice < 1){

              currentSdVoice = sdVoices;
              currentSdPage = sdVoicesPages;

              updateCurrentSDInPage(sdVoices);

              getSDVoicePageList(currentSdPage);
            }
            else{
              if(currentSdVoiceInPage < 0){
                currentSdPage--;
                currentSdVoiceInPage = 8;

                getSDVoicePageList(currentSdPage);
              }
            }

            loadTEMPorSDVoice(currentSdVoiceInPage, 0, true);
          break;
          case TEMP_FOLDER:
            currentTempVoiceInPage--;
            currentTempVoice--;

            if(currentTempVoice < 1){

              currentTempVoice = tempVoices;
              currentTempPage = tempVoicesPages;

              updateCurrentTEMPInPage(tempVoices);

              getTEMPVoicePageList(currentTempPage);
            }
            else{
              if(currentTempVoiceInPage < 0){
                currentTempPage--;
                currentTempVoiceInPage = 8;

                getTEMPVoicePageList(currentTempPage);
              }
            }

            loadTEMPorSDVoice(currentTempVoiceInPage, 0, false);
          break;
        }

      }//END else

    }
    else if(knobARight){
      releaseShiftBtn();
      if(voicePagerA){

        switch(memState){
          case LOCAL_FOLDER:
            currentLocalPage++;

            if(currentLocalPage > localVoicesPages){
              currentLocalPage = 1;
              currentLocalVoice = 1;
              currentLocalVoiceInPage = 0;

              getLocalVoicePageList(currentLocalPage);

            }
            else{
              currentLocalVoice += 9;

              if(currentLocalVoice > localVoices)
                currentLocalVoice = localVoices;

              updateCurrentLocalInPage(currentLocalVoice);

              getLocalVoicePageList(currentLocalPage);
            }

            loadLocalVoice(currentLocalVoice);
          break;
          case SD_FOLDER:
            currentSdPage++;

            if(currentSdPage > sdVoicesPages){
              currentSdPage = 1;
              currentSdVoice = 1;
              currentSdVoiceInPage = 0;

              getSDVoicePageList(currentSdPage);
            }
            else{
              currentSdVoice += 9;

              if(currentSdVoice > sdVoices)
                currentSdVoice = sdVoices;

              updateCurrentSDInPage(currentSdVoice);

              getSDVoicePageList(currentSdPage);
            }

            loadTEMPorSDVoice(currentSdVoiceInPage, 0, true);
          break;
          case TEMP_FOLDER:
            currentTempPage++;

            if(currentTempPage > tempVoicesPages){
              currentTempPage = 1;
              currentTempVoice = 1;
              currentTempVoiceInPage = 0;

              getTEMPVoicePageList(currentTempPage);
            }
            else{
              currentTempVoice += 9;

              if(currentTempVoice > tempVoices)
                currentTempVoice = tempVoices;

              updateCurrentTEMPInPage(currentTempVoice);

              getTEMPVoicePageList(currentTempPage);
            }

            loadTEMPorSDVoice(currentTempVoiceInPage, 0, false);
          break;
        }

      
      }
      else{

        switch(memState){
          case LOCAL_FOLDER:
            currentLocalVoiceInPage++;
            currentLocalVoice++;

            if(currentLocalVoice > localVoices){

              currentLocalVoice = 1;  
              currentLocalPage = 1;

              currentLocalVoiceInPage = 0;

              getLocalVoicePageList(currentLocalPage);

            }
            else{
              if(currentLocalVoiceInPage > 8){
                currentLocalPage++;
                currentLocalVoiceInPage = 0;

                getLocalVoicePageList(currentLocalPage);
              }
            }//END else

            loadLocalVoice(currentLocalVoice);
          break;
          case SD_FOLDER:
            currentSdVoiceInPage++;
            currentSdVoice++;

            if(currentSdVoice > sdVoices){

              currentSdVoice = 1;  
              currentSdPage = 1;

              currentSdVoiceInPage = 0;

              getSDVoicePageList(currentSdPage);

            }
            else{
              if(currentSdVoiceInPage > 8){
                currentSdPage++;
                currentSdVoiceInPage = 0;

                getSDVoicePageList(currentSdPage);
              }
            }//END else

            loadTEMPorSDVoice(currentSdVoiceInPage, 0, true);
          break;
          case TEMP_FOLDER:
            currentTempVoiceInPage++;
            currentTempVoice++;

            if(currentTempVoice > tempVoices){

              currentTempVoice = 1;  
              currentTempPage = 1;

              currentTempVoiceInPage = 0;

              getTEMPVoicePageList(currentTempPage);

            }
            else{
              if(currentTempVoiceInPage > 8){
                currentTempPage++;
                currentTempVoiceInPage = 0;

                getTEMPVoicePageList(currentTempPage);
              }
            }//END else

            loadTEMPorSDVoice(currentTempVoiceInPage, 0, false);
          break;
        }
      
      }//END else if(knobARight)
    }

      

    if(btA){
      releaseShiftBtn();

      cleanSynthNotes();

   
      memState = LOCAL_FOLDER;
      localOrSD2 = true;

      currentKeyMode = DUAL;

      currentInstrument = A;
  
      if(memState == LOCAL_FOLDER){
        getLocalVoicePageListA(currentLocalPageA);
        loadLocalVoice(currentLocalVoiceA);
      }

      currentInstrument = B;
      if(localOrSD2){
        getLocalVoicePageListB(currentLocalPageB);
        loadLocalVoice(currentLocalVoiceB);
      }


      setScreen(MODE_B);
    }

    if(btB){
      releaseShiftBtn();
      setScreen(CHANNEL);
      loadChannelData();
    }

/*
    if(btC){     
      releaseShiftBtn();
      selectedOp = 1;
      setScreen(OPERATOR);
      getChannelOpType();
      loadOperatorData();
    }
    */


    if(btE){
      if(opl3->getSpeedExpression())
        opl3->setSpeedExpression(false);
      else
        opl3->setSpeedExpression(true);
    }
    
    if(swA){
      if(voicePagerA)
        voicePagerA = false;
      else
        voicePagerA = true;
    }  

    if(holdSwA){
      releaseShiftBtn();

      bool changeStorage = false;

      initSD();

      if(sdCard)
        changeStorage = true;

      if(changeStorage){

        switch(memState){
          case LOCAL_FOLDER:
            memState = SD_FOLDER;
          break;
          case SD_FOLDER:
            memState = TEMP_FOLDER;
          break;
          case TEMP_FOLDER:
            memState = LOCAL_FOLDER;
          break;
        }

        switch(memState){
          case LOCAL_FOLDER:
            getLocalVoicePageList(currentLocalPage);

            loadLocalVoice(currentLocalVoice);
          break;
          case SD_FOLDER:
            setSDVoicesTotals();

            getSDVoicePageList(currentSdPage);

            loadTEMPorSDVoice(currentSdVoiceInPage, 0, true);
          break;
          case TEMP_FOLDER:
            setSDVoicesTotals();

            getTEMPVoicePageList(currentTempPage);

            loadTEMPorSDVoice(currentTempVoiceInPage, 0, false);
          break;
        }

      }
      else
        setScreen(NO_SD_CARD);
    
    }//END if(holdSwA)

  }//END if(!shiftBtn)
  else{
    if(swA){
      releaseShiftBtn();
      createOP2Voice();
    }
    else if(swB){
      releaseShiftBtn();
      createOP4Voice();
    }

    accessBtnsUserInput();

  }
      
}


void modeBUserInput(){                                                    //MODE B

  if(knobALeft){
    releaseShiftBtn();
    if(voicePagerA){

      switch(memState){
        case LOCAL_FOLDER:
          currentLocalPageA--;

          if(currentLocalPageA < 1){
            currentLocalPageA = localVoicesPagesA;
            currentLocalVoiceA = localVoices;

            updateCurrentLocalInPageA(localVoices);

            getLocalVoicePageListA(currentLocalPageA);
          }
          else{
            currentLocalVoiceA -= 4;

            updateCurrentLocalInPageA(currentLocalVoiceA);

            getLocalVoicePageListA(currentLocalPageA);
          }

          currentInstrument = A;

          loadLocalVoice(currentLocalVoiceA);
        break;
        case SD_FOLDER:
          currentSdPageA--;

          if(currentSdPageA < 1){
            currentSdPageA = sdVoicesPagesA;
            currentSdVoiceA = sdVoices;

            updateCurrentSDInPageA(sdVoices);

            getSDVoicePageListA(currentSdPageA);
          }
          else{
            currentSdVoiceA -= 4;

            updateCurrentSDInPageA(currentSdVoiceA);

            getSDVoicePageListA(currentSdPageA);
          } 

          currentInstrument = A;

          loadTEMPorSDVoice(currentSdVoiceInPageA, 1, true);
        break;
      }
  
    }
    else{

      switch(memState){
        case LOCAL_FOLDER:
            currentLocalVoiceInPageA--;
          currentLocalVoiceA--;

          if(currentLocalVoiceA < 1){

            currentLocalVoiceA = localVoices;
            currentLocalPageA = localVoicesPagesA;

            updateCurrentLocalInPageA(localVoices);

            getLocalVoicePageListA(currentLocalPageA);

          }
          else{
            if(currentLocalVoiceInPageA < 0){
              currentLocalPageA--;
              currentLocalVoiceInPageA = 3;

              getLocalVoicePageListA(currentLocalPageA);
            }
          }

          currentInstrument = A;

          loadLocalVoice(currentLocalVoiceA);
        break;
        case SD_FOLDER:
          currentSdVoiceInPageA--;
          currentSdVoiceA--;

          if(currentSdVoiceA < 1){
            currentSdVoiceA = sdVoices;
            currentSdPageA = sdVoicesPagesA;

            updateCurrentSDInPageA(sdVoices);

            getSDVoicePageListA(currentSdPageA);
          }
          else{
            if(currentSdVoiceInPageA < 0){
              currentSdPageA--;
              currentSdVoiceInPageA = 3;

              getSDVoicePageListA(currentSdPageA);
            }
          }

          currentInstrument = A;

          loadTEMPorSDVoice(currentSdVoiceInPageA, 1, true);
        break;
      }
      
    }//END else

  }
  else if(knobARight){
    releaseShiftBtn();
    if(voicePagerA){

      switch(memState){
        case LOCAL_FOLDER:
          currentLocalPageA++;

          if(currentLocalPageA > localVoicesPagesA){
            currentLocalPageA = 1;
            currentLocalVoiceA = 1;
            currentLocalVoiceInPageA = 0;

            getLocalVoicePageListA(currentLocalPageA);

          }
          else{
            currentLocalVoiceA += 4;

            if(currentLocalVoiceA > localVoices)
              currentLocalVoiceA = localVoices;

            updateCurrentLocalInPageA(currentLocalVoiceA);

            getLocalVoicePageListA(currentLocalPageA);

          }
          currentInstrument = A;
          loadLocalVoice(currentLocalVoiceA);
        break;
        case SD_FOLDER:
          currentSdPageA++;

          if(currentSdPageA > sdVoicesPagesA){
            currentSdPageA = 1;
            currentSdVoiceA = 1;
            currentSdVoiceInPageA = 0;

            getSDVoicePageListA(currentSdPageA);
          }
          else{
            currentSdVoiceA += 4;

            if(currentSdVoiceA > sdVoices)
              currentSdVoiceA = sdVoices;

            updateCurrentSDInPageA(currentSdVoiceA);

            getSDVoicePageListA(currentSdPageA);
          }

          currentInstrument = A;

          loadTEMPorSDVoice(currentSdVoiceInPageA, 1, true);
        break;
      }
      
    }
    else{

      switch(memState){
        case LOCAL_FOLDER:
          currentLocalVoiceInPageA++;
          currentLocalVoiceA++;

          if(currentLocalVoiceA > localVoices){

            currentLocalVoiceA = 1;  
            currentLocalPageA = 1;

            currentLocalVoiceInPageA = 0;

            getLocalVoicePageListA(currentLocalPageA);

          }
          else{
            if(currentLocalVoiceInPageA > 3){
              currentLocalPageA++;
              currentLocalVoiceInPageA = 0;

              getLocalVoicePageListA(currentLocalPageA);  

            }
          }//END else

          currentInstrument = A;

          loadLocalVoice(currentLocalVoiceA);
        break;
        case SD_FOLDER:
          currentSdVoiceInPageA++;
          currentSdVoiceA++;

          if(currentSdVoiceA > sdVoices){

            currentSdVoiceA = 1;  
            currentSdPageA = 1;

            currentSdVoiceInPageA = 0;

            getSDVoicePageListA(currentSdPageA);

          }
          else{
            if(currentSdVoiceInPageA > 3){
              currentSdPageA++;
              currentSdVoiceInPageA = 0;

              getSDVoicePageListA(currentSdPageA);
            }
          }//END else

          currentInstrument = A;

          loadTEMPorSDVoice(currentSdVoiceInPageA, 1, true);
        break;
      }
    
    }//END else if(knobARight)
  }
      

  if(knobBLeft){
    releaseShiftBtn();
    if(voicePagerB){

      if(localOrSD2){
        currentLocalPageB--;

        if(currentLocalPageB < 1){
          currentLocalPageB = localVoicesPagesB;
          currentLocalVoiceB = localVoicesB;

          updateCurrentLocalInPageB(localVoicesB);

          getLocalVoicePageListB(currentLocalPageB);

        }
        else{
          currentLocalVoiceB -= 4;

          updateCurrentLocalInPageB(currentLocalVoiceB);

          getLocalVoicePageListB(currentLocalPageB);

        }
        currentInstrument = B;
        loadLocalVoice(currentLocalVoiceB);

      }//END if(localOrSD2)
      else{
        currentSdPageB--;

        if(currentSdPageB < 1){
          currentSdPageB = sdVoicesPagesB;
          currentSdVoiceB = sdVoicesB;

          updateCurrentSDInPageB(sdVoicesB);

          getSDVoicePageListB(currentSdPageB);
        }
        else{
          currentSdVoiceB -= 4;

          updateCurrentSDInPageB(currentSdVoiceB);

          getSDVoicePageListB(currentSdPageB);
        }

        currentInstrument = B;

        loadTEMPorSDVoice(currentSdVoiceInPageB, 2, true);
      }//END else
  
    }
    else{

      if(localOrSD2){
        currentLocalVoiceInPageB--;
        currentLocalVoiceB--;

        if(currentLocalVoiceB < 1){

          currentLocalVoiceB = localVoicesB;
          currentLocalPageB = localVoicesPagesB;

          updateCurrentLocalInPageB(localVoicesB);

          getLocalVoicePageListB(currentLocalPageB);

        }
        else{
          if(currentLocalVoiceInPageB < 0){
            currentLocalPageB--;
            currentLocalVoiceInPageB = 3;

            getLocalVoicePageListB(currentLocalPageB);

          }
        }

        currentInstrument = B;

        loadLocalVoice(currentLocalVoiceB);

      }//END if(localOrSD2)
      else{
        currentSdVoiceInPageB--;
        currentSdVoiceB--;

        if(currentSdVoiceB < 1){

          currentSdVoiceB = sdVoicesB;
          currentSdPageB = sdVoicesPagesB;

          updateCurrentSDInPageB(sdVoicesB);

          getSDVoicePageListB(currentSdPageB);
        }
        else{
          if(currentSdVoiceInPageB < 0){
            currentSdPageB--;
            currentSdVoiceInPageB = 3;

            getSDVoicePageListB(currentSdPageB);
          }
        }

        currentInstrument = B;

        loadTEMPorSDVoice(currentSdVoiceInPageB, 2, true);
      }//END else

      
    }//END else

  }
  else if(knobBRight){
    releaseShiftBtn();
    if(voicePagerB){

      if(localOrSD2){
        currentLocalPageB++;

        if(currentLocalPageB > localVoicesPagesB){
          currentLocalPageB = 1;
          currentLocalVoiceB = 1;
          currentLocalVoiceInPageB = 0;

          getLocalVoicePageListB(currentLocalPageB);
        }
        else{
          currentLocalVoiceB += 4;

          if(currentLocalVoiceB > localVoicesB)
            currentLocalVoiceB = localVoicesB;

          updateCurrentLocalInPageB(currentLocalVoiceB);

          getLocalVoicePageListB(currentLocalPageB);
        }

        currentInstrument = B;

        loadLocalVoice(currentLocalVoiceB);

      }//END if(localOrSD2)
      else{
        currentSdPageB++;

        if(currentSdPageB > sdVoicesPagesB){
          currentSdPageB = 1;
          currentSdVoiceB = 1;
          currentSdVoiceInPageB = 0;

          getSDVoicePageListB(currentSdPageB);
        }
        else{
          currentSdVoiceB += 4;

          if(currentSdVoiceB > sdVoicesB)
            currentSdVoiceB = sdVoicesB;

          updateCurrentSDInPageB(currentSdVoiceB);

          getSDVoicePageListB(currentSdPageB);
        }

        currentInstrument = B;

        loadTEMPorSDVoice(currentSdVoiceInPageB, 2, true);
      }//END else
    }
    else{

      if(localOrSD2){
        currentLocalVoiceInPageB++;
        currentLocalVoiceB++;

        if(currentLocalVoiceB > localVoicesB){

          currentLocalVoiceB = 1;  
          currentLocalPageB = 1;

          currentLocalVoiceInPageB = 0;

          getLocalVoicePageListB(currentLocalPageB);

        }
        else{
          if(currentLocalVoiceInPageB > 3){
            currentLocalPageB++;
            currentLocalVoiceInPageB = 0;

            getLocalVoicePageListB(currentLocalPageB);

          }
        }//END else

        currentInstrument = B;

        loadLocalVoice(currentLocalVoiceB);
      }//END if(localOrSD2)
      else{
        currentSdVoiceInPageB++;
        currentSdVoiceB++;

        if(currentSdVoiceB > sdVoicesB){

          currentSdVoiceB = 1;  
          currentSdPageB = 1;

          currentSdVoiceInPageB = 0;

          getSDVoicePageListB(currentSdPageB);

        }
        else{
          if(currentSdVoiceInPageB > 3){
            currentSdPageB++;
            currentSdVoiceInPageB = 0;

            getSDVoicePageListB(currentSdPageB);
          }
        }//END else

        currentInstrument = B;

        loadTEMPorSDVoice(currentSdVoiceInPageB, 2, true);
      }//END else
      
    }//END else if(knobARight)
  }

      
  if(swA){
    if(voicePagerA)
      voicePagerA = false;
    else
      voicePagerA = true;
  }  

  if(swB){
    if(voicePagerB)
      voicePagerB = false;
    else
      voicePagerB = true; 
  }


  if(btA){
    cleanSplitNotes();

    currentKeyMode = VGM;

    if(initSD()){
  
      setSDVoicesTotals();

      getVGMPageList(currentSdVgmPage);
    }


    setScreen(MODE_C);
  }

  if(btB){
    cleanSplitNotes();

    if(op4op2Set){
      op4op2Set = false;

      if(currentKeyMode == DUAL)
        cleanANotes();
    }
    else
      op4op2Set = true;

  }

  if(btC)
    setNotepoint = true;

  /*
  if(btC){
      releaseShiftBtn();
      setScreen(ARPEGGIATOR);
  }
  */

  /*
  if(btC){
    Serial.println("A1=0");
    for(int i = 0; i < 246; i++){
      Serial.print(i, HEX);
      Serial.print(" ");
      Serial.println(opl3->getRegisterAt(i, false), HEX);
    }
    Serial.println("");

    Serial.println("A1=1");
    for(int i = 0; i < 246; i++){
      Serial.print(i, HEX);
      Serial.print(" ");
      Serial.println(opl3->getRegisterAt(i, true), HEX);
    }

    Serial.println("");
    Serial.println("");
  }
  */

  if(btE){
      if(opl3->getSpeedExpression())
        opl3->setSpeedExpression(false);
      else
        opl3->setSpeedExpression(true);
    }

  if(holdSwA){

    bool changeStorage = false;

    if(sdCard)
      changeStorage = true;

    if(!changeStorage)
      initSD();

    if(sdCard)
      changeStorage = true;

    if(changeStorage){

      switch(memState){
        case LOCAL_FOLDER:
          memState = SD_FOLDER;
        break;
        case SD_FOLDER:
          memState = LOCAL_FOLDER;
        break;
      }

      currentInstrument = A;

      switch(memState){
        case LOCAL_FOLDER:
          getLocalVoicePageListA(currentLocalPageA);
          loadLocalVoice(currentLocalVoiceA);
        break;
        case SD_FOLDER:
          setSDVoicesTotals();

          getSDVoicePageListA(currentSdPageA);
          loadTEMPorSDVoice(currentSdVoiceInPageA, 1, true);
        break;
      }
      
    }
    else
      setScreen(NO_SD_CARD);
  }//END if(holdSwA)

  if(holdSwB){
    bool changeStorage = false;

    if(sdCard)
      changeStorage = true;

    if(!changeStorage)
      initSD();

    if(sdCard)
      changeStorage = true;

    if(changeStorage){
      if(localOrSD2)
        localOrSD2 = false;
      else
        localOrSD2 = true;


      currentInstrument = B;
      if(localOrSD2){
        getLocalVoicePageListB(currentLocalPageB);
        loadLocalVoice(currentLocalVoiceB);
      }
      else{
        setSDVoicesTotals();

        getSDVoicePageListB(currentSdPageB);
        loadTEMPorSDVoice(currentSdVoiceInPageB, 2, true);
      }
    }
    else
      setScreen(NO_SD_CARD);
  }//END if(holdSwB)

}


void modeCUserInput(){                                                      //MODE C

  if(vgmPause){

    if(knobALeft){
      if(vgmPager){
        currentSdVgmPage--;

        if(currentSdVgmPage < 1){
          currentSdVgmPage  = sdVgmPages;
          currentSdVgm = sdVgms;      

          updateCurrentSdVgmInPage(sdVgms);

          getVGMPageList(currentSdVgmPage); 
        }
        else{
          currentSdVgm -= 4;

          updateCurrentSdVgmInPage(currentSdVgm);

          getVGMPageList(currentSdVgmPage);
        }
  
      }
      else{
        currentSdVgmInPage--;
        currentSdVgm--;

        if(currentSdVgm < 1){

          currentSdVgm = sdVgms;
          currentSdVgmPage  = sdVgmPages;              

          updateCurrentSdVgmInPage(sdVgms);

          getVGMPageList(currentSdVgmPage);
        }
        else{
          if(currentSdVgmInPage < 0){
            currentSdVgmPage--;
            currentSdVgmInPage = 3;

            getVGMPageList(currentSdVgmPage);
          }
        }

      }//END else

    }
    else if(knobARight){

      if(vgmPager){
        currentSdVgmPage++;

        if(currentSdVgmPage  > sdVgmPages){
          currentSdVgmPage  = 1;
          currentSdVgm = 1;
          currentSdVgmInPage = 0;

          getVGMPageList(currentSdVgmPage);
        }
        else{
          currentSdVgm += 4;

          if(currentSdVgm > sdVgms)
            currentSdVgm = sdVgms;

          updateCurrentSdVgmInPage(currentSdVgm);

          getVGMPageList(currentSdVgmPage);
        }

      }
      else{
        incVgmSong();
      }//END else if(knobARight)
    }
  }//END if(vgmPause)

  if(btA){
    //cleanSplitNotes();

    if(vgmPause){

      opl3->reStart(true);

      currentKeyMode = SINGLE;

      memState = LOCAL_FOLDER;

      if(memState == LOCAL_FOLDER){
        getLocalVoicePageList(currentLocalPage);
        loadLocalVoice(currentLocalVoice);
      }

      setScreen(MODE_A);
    }
  }

  if(btB){
    if(isCompatibleVGMFile()){
      vgmPlay = true;
      vgmPause = false;
      vgmAutoPlay = true;
      vgmGenVoice = true;
    }
  }

  if(btD){
    if(isCompatibleVGMFile()){
      autoPlayIt = true;
      vgmPlay = true;
      vgmPause = false;
      vgmAutoPlay = true;
      vgmGenVoice = true;
    }
  }

  if(btE){
    if(isCompatibleVGMFile()){

      setScreen(EXP_VOICES);

      clearTEMPFolder();

      generateOP12Voices();

      setScreen(MODE_C);
      

    }
  }


  if(swA){
    if(vgmPager)
      vgmPager = false;
    else
      vgmPager = true;
  } 

}

void operatorUserInput(){                                                           //OPERATOR USER INPUT

  bool updateData2Chip = false;

  if(knobALeft){
    opCurrentPositionInPage--;
    opCurrentPosition--;

    if(opCurrentPosition < 1){

      opCurrentPosition = 12;
      opCurrentPage = 2;

      opCurrentPositionInPage = 3; 

      //getLocalVoicePageList(currentLocalPage);
    }
    else{
      if(opCurrentPositionInPage < 1){
        opCurrentPage--;
        opCurrentPositionInPage = 9;
        //getLocalVoicePageList(currentLocalPage);
      }
    }

    //loadLocalVoice(currentLocalVoice);
  }
  else if(knobARight){
    opCurrentPositionInPage++;
    opCurrentPosition++;

    if(opCurrentPosition > 12){
      opCurrentPosition = 1;
      opCurrentPage = 1;
      opCurrentPositionInPage = 1;

      //getLocalVoicePageList(currentLocalPage);

    }
    else{
      if(opCurrentPositionInPage > 9){
        opCurrentPage++;
        opCurrentPositionInPage = 1;

        //getLocalVoicePageList(currentLocalPage);
      }
    }//END else

    //loadLocalVoice(currentLocalVoice);
  }


  if(knobBLeft){

    if(memState == LOCAL_FOLDER)
      presetChange = true;

    updateData2Chip = true;

    switch(opCurrentPosition){
      case 1:                                   //ATTACK
        opAttack--;
        if(opAttack < 0)
          opAttack = 0;

      break;
      case 2:                                   //DECAY
        opDecay--;
        if(opDecay < 0)
          opDecay = 0;

      break;
      case 3:                                   //SUSTAIN
        opSustain--;
        if(opSustain < 0)
          opSustain = 0;

      break;
      case 4:                                   //RELEASE
        opRelease--;
        if(opRelease < 0)
          opRelease = 0;

      break;
      case 5:                                   //TREMOLO
        opTremolo = false;

      break;
      case 6:                                   //VIBRATO
        opVibrato = false;

      break;
      case 7:                                   //ENVELOPE TYPE
        opEnvelopeType = false;

      break;
      case 8:                                   //KEY SCALE RATE
        opKeyScaleRate = false;

      break;
      case 9:                                   //WAVEFORM
        opWaveform--;
        if(opWaveform < 0)
          opWaveform = 0;

      break;
      case 10:                                  //TOTAL LEVEL
        opTotalLevel--;
        if(opTotalLevel < 0)
          opTotalLevel = 0;

      break;
      case 11:                                  //FREQUENCY MULTIPLICATOR
        opFreMult--;
        if(opFreMult < 0)
          opFreMult = 0;

      break;
      case 12:                                  //KEY SCALE LEVEL
        opKeyScaleLevel--;
          if(opKeyScaleLevel < 0)
            opKeyScaleLevel = 0;

      break;
    }
  }

  if(knobBRight){

    if(memState == LOCAL_FOLDER)
      presetChange = true;

    updateData2Chip = true;

    switch(opCurrentPosition){
      case 1:                                   //ATTACK
        opAttack++;
        if(opAttack > 15)
          opAttack = 15;

      break;
      case 2:                                   //DECAY
        opDecay++;
        if(opDecay > 15)
          opDecay = 15;

      break;
      case 3:                                   //SUSTAIN
        opSustain++;
        if(opSustain > 15)
          opSustain = 15;

      break;
      case 4:                                   //RELEASE
        opRelease++;
        if(opRelease > 15)
          opRelease = 15;

      break;
      case 5:                                   //TREMOLO
        opTremolo = true;

      break;
      case 6:                                   //VIBRATO
        opVibrato = true;

      break;
      case 7:                                   //ENVELOPE TYPE
        opEnvelopeType = true;

      break;
      case 8:                                   //KEY SCALE RATE
        opKeyScaleRate = true;

      break;
      case 9:                                   //WAVEFORM
        opWaveform++;
        if(opWaveform > 7)
          opWaveform = 7;

      break;
      case 10:                                  //TOTAL LEVEL
        opTotalLevel++;
        if(opTotalLevel > 63)
          opTotalLevel = 63;

      break;
      case 11:                                  //FREQUENCY MULTIPLICATOR
        opFreMult++;
        if(opFreMult > 15)
          opFreMult = 15;

      break;
      case 12:                                  //KEY SCALE LEVEL
        opKeyScaleLevel++;
          if(opKeyScaleLevel > 3)
            opKeyScaleLevel = 3;
      break;
    }
  }  

  if(updateData2Chip){

    uint8_t tempByte = 0;

    switch(opCurrentPosition){
      case 1:                                   //ATTACK
        opl3->setAR(opAttack, 1, selectedOp);
        tempByte = opl3->getRegisterAtAddress(0x60, 1, selectedOp);
      
        if(mainInstrumentOpMode == OP4)
          opl3->loadAllChannelsOpl3(0x60, tempByte, selectedOp);
        else
          opl3->loadAllChannelsOpl2(0x60, tempByte, selectedOp);


      break;
      case 2:                                   //DECAY
        opl3->setDR(opDecay, 1, selectedOp);
        tempByte = opl3->getRegisterAtAddress(0x60, 1, selectedOp);
      
        if(mainInstrumentOpMode == OP4)
          opl3->loadAllChannelsOpl3(0x60, tempByte, selectedOp);
        else
          opl3->loadAllChannelsOpl2(0x60, tempByte, selectedOp);

      break;
      case 3:                                   //SUSTAIN
        opl3->setSL(opSustain, 1, selectedOp);
        tempByte = opl3->getRegisterAtAddress(0x80, 1, selectedOp);
      
        if(mainInstrumentOpMode == OP4)
          opl3->loadAllChannelsOpl3(0x80, tempByte, selectedOp);
        else
          opl3->loadAllChannelsOpl2(0x80, tempByte, selectedOp);

      break;
      case 4:                                   //RELEASE
        opl3->setRR(opRelease, 1, selectedOp);
        tempByte = opl3->getRegisterAtAddress(0x80, 1, selectedOp);
      
        if(mainInstrumentOpMode == OP4)
          opl3->loadAllChannelsOpl3(0x80, tempByte, selectedOp);
        else
          opl3->loadAllChannelsOpl2(0x80, tempByte, selectedOp);

      break;
      case 5:                                   //TREMOLO
        opl3->setAM(opTremolo, 1, selectedOp);
        tempByte = opl3->getRegisterAtAddress(0x20, 1, selectedOp);
      
        if(mainInstrumentOpMode == OP4)
          opl3->loadAllChannelsOpl3(0x20, tempByte, selectedOp);
        else
          opl3->loadAllChannelsOpl2(0x20, tempByte, selectedOp);

      break;
      case 6:                                   //VIBRATO
        opl3->setVIB(opVibrato, 1, selectedOp);
        tempByte = opl3->getRegisterAtAddress(0x20, 1, selectedOp);
      
        if(mainInstrumentOpMode == OP4)
          opl3->loadAllChannelsOpl3(0x20, tempByte, selectedOp);
        else
          opl3->loadAllChannelsOpl2(0x20, tempByte, selectedOp);

      break;
      case 7:                                   //ENVELOPE TYPE
        opl3->setEGT(opEnvelopeType, 1, selectedOp);
        tempByte = opl3->getRegisterAtAddress(0x20, 1, selectedOp);
      
        if(mainInstrumentOpMode == OP4)
          opl3->loadAllChannelsOpl3(0x20, tempByte, selectedOp);
        else
          opl3->loadAllChannelsOpl2(0x20, tempByte, selectedOp);

      break;
      case 8:                                   //KEY SCALE RATE
        opl3->setKSR(opKeyScaleRate, 1, selectedOp);
        tempByte = opl3->getRegisterAtAddress(0x20, 1, selectedOp);
      
        if(mainInstrumentOpMode == OP4)
          opl3->loadAllChannelsOpl3(0x20, tempByte, selectedOp);
        else
          opl3->loadAllChannelsOpl2(0x20, tempByte, selectedOp);

      break;
      case 9:                                   //WAVEFORM
        opl3->setWS(opWaveform, 1, selectedOp);
        tempByte = opl3->getRegisterAtAddress(0xE0, 1, selectedOp);
      
        if(mainInstrumentOpMode == OP4)
          opl3->loadAllChannelsOpl3(0xE0, tempByte, selectedOp);
        else
          opl3->loadAllChannelsOpl2(0xE0, tempByte, selectedOp);

      break;
      case 10:                                  //TOTAL LEVEL

        opl3->setTL(tlFix(opTotalLevel), 1, selectedOp, true);
        tempByte = opl3->getRegisterAtAddress(0x40, 1, selectedOp);
      
        if(mainInstrumentOpMode == OP4){
          opl3->loadAllChannelsOpl3(0x40, tempByte, selectedOp);
          opl3->loadVRegAllChannelsOpl3(selectedOp, true, false);
        }
        else{
          opl3->loadAllChannelsOpl2(0x40, tempByte, selectedOp);
          opl3->loadVRegAllChannelsOpl2(selectedOp, true, false);
        }

      break;
      case 11:                                  //FREQUENCY MULTIPLICATOR
        opl3->setMULT(opFreMult, 1, selectedOp);
        tempByte = opl3->getRegisterAtAddress(0x20, 1, selectedOp);
      
        if(mainInstrumentOpMode == OP4)
          opl3->loadAllChannelsOpl3(0x20, tempByte, selectedOp);
        else
          opl3->loadAllChannelsOpl2(0x20, tempByte, selectedOp);

      break;
      case 12:                                  //KEY SCALE LEVEL
        opl3->setKSL(opKeyScaleLevel, 1, selectedOp);
        tempByte = opl3->getRegisterAtAddress(0x40, 1, selectedOp);
      
        if(mainInstrumentOpMode == OP4)
          opl3->loadAllChannelsOpl3(0x40, tempByte, selectedOp);
        else
          opl3->loadAllChannelsOpl2(0x40, tempByte, selectedOp);
      break;
    }
  }

  if(btA){
    if(memState == LOCAL_FOLDER){
      if(presetChange)
        setScreen(SAVE_MESSAGE);
      else{
        loadLocalVoice(currentLocalVoice);
        setScreen(MODE_A);
      }
    }
    else
      setScreen(SAVE_MESSAGE);
  }

  if(btB){
    setScreen(CHANNEL);
    loadChannelData();
  }


  if(btC){
    selectedOp++;

    uint8_t limitOp = 2;

    if(mainInstrumentOpMode == OP4)
      limitOp = 4;


    if(selectedOp > limitOp)
      selectedOp = 1;


    loadOperatorData();
  }

  if(btE){                                              //SAVE
    beforeSaveScreen = OPERATOR;
    preMainInstrumentVoice = mainInstrumentVoice;
    voiceNameToSaveScreen();
    setScreen(SAVE_INSTRUMENT);
  }
}//END operatorUserInput();


void voiceNameToSaveScreen(){
  int tempLength = preMainInstrumentVoice.length();
  if(tempLength > 0){
    if(tempLength < 18){

      mainInstrumentVoice = "";

      for(int i = 0; i < 18; i++){
        if(i < tempLength)
          mainInstrumentVoice += preMainInstrumentVoice.charAt(i);
        else
          mainInstrumentVoice += " ";
      }
    }
  }
  else
    mainInstrumentVoice = "                  ";
}

void channelUserInput(){                                                                        //CHANNEL USER INPUT

  bool updateData2Chip = false;

  if(knobALeft){
    
    chCurrentPosition--;

    if(chCurrentPosition < 1)
      chCurrentPosition = 2; 

    //loadLocalVoice(currentLocalVoice);
  }
  else if(knobARight){
    chCurrentPosition++;
    
    if(chCurrentPosition > 2)
      chCurrentPosition = 1;

  }

  if(knobBLeft){

    if(memState == LOCAL_FOLDER)
      presetChange = true;

    updateData2Chip = true;

    switch(chCurrentPosition){
      case 1:                                   //ALGORITHM
        chAlgorithm--;

        if(mainInstrumentOpMode == OP4){
          if(chAlgorithm < 2)
            chAlgorithm = 2;
        }
        else{
          if(chAlgorithm < 0)
            chAlgorithm = 0;
        }
    
      break;
      case 2:                                   //FEEDBACK
        chFeedback--;
        if(chFeedback < 0)
          chFeedback = 0;
      break;
    }
  }

  if(knobBRight){

    if(memState == LOCAL_FOLDER)
      presetChange = true;

    updateData2Chip = true;

    switch(chCurrentPosition){
      case 1:                                   //ALGORITHM
        chAlgorithm++;
        if(chAlgorithm > 5)
          chAlgorithm = 5;

        if(mainInstrumentOpMode == OP4){
          if(chAlgorithm > 5)
            chAlgorithm = 5;
        }
        else{
          if(chAlgorithm > 1)
            chAlgorithm = 1;
        }

      break;
      case 2:                                   //FEEDBACK
        chFeedback++;
        if(chFeedback > 7)
          chFeedback = 7;
      break;
    }
  } 

  if(updateData2Chip){
    uint8_t tempByte = 0;

    switch(chCurrentPosition){
      case 1:
        opl3->setCNT(chAlgorithm, 1);
        tempByte = opl3->getRegisterAtAddress(0xC0, 1, 1);
      
        if(mainInstrumentOpMode == OP4){
          opl3->loadAllChannelsOpl3(0xC0, tempByte, 1);
          
          tempByte = opl3->getRegisterAtAddress(0xC0, 1, 3);
          opl3->loadAllChannelsOpl3(0xC0, tempByte, 3);

          opl3->loadVRegAllChannelsOpl3(0, false, true);
        }
        else{
          opl3->loadAllChannelsOpl2(0xC0, tempByte, 1);

          opl3->loadVRegAllChannelsOpl2(0, false, true);
        }

      break;
      case 2:
        opl3->setFB(chFeedback, 1,1);
        tempByte = opl3->getRegisterAtAddress(0xC0, 1,1);

        if(mainInstrumentOpMode == OP4)
          opl3->loadAllChannelsOpl3(0xC0, tempByte, 1);
        else
          opl3->loadAllChannelsOpl2(0xC0, tempByte, 1);
        
      break;
    }

  }

  if(btA){
    if(memState == LOCAL_FOLDER){
      if(presetChange)
        setScreen(SAVE_MESSAGE);
      else{
        loadLocalVoice(currentLocalVoice);
        setScreen(MODE_A);
      }
    }
    else
      setScreen(SAVE_MESSAGE);
  }

  if(btC){
    selectedOp = 1;
    setScreen(OPERATOR);
    getChannelOpType();
    loadOperatorData();  
  }

  if(btE){                                              //SAVE
    beforeSaveScreen = CHANNEL;
    preMainInstrumentVoice = mainInstrumentVoice;
    voiceNameToSaveScreen();
    setScreen(SAVE_INSTRUMENT);  
  }

}//END channelUserInput();


void saveInsUserInput(){

  if(knobALeft){

    saveCursorX--;

    if(saveCursorX < 0)
      saveCursorX = 0;

  }//END if(knobALeft)
  else if(knobARight){
    int tempLength = mainInstrumentVoice.length();

    if(tempLength > 0){
      saveCursorX++;

      if(saveCursorX >= tempLength)
        saveCursorX = (tempLength - 1);
    }
  }//END else if(knobARight)

  if((knobBLeft) || (knobBRight)){
    int8_t sLetter = mainInstrumentVoice.charAt(saveCursorX);

    if(sLetter == 32)
      sLetter = 64;

    if(knobBLeft){
      sLetter--;


      if(sLetter < 48)
        sLetter = 90;
      else if(sLetter < 65 && sLetter > 57)
        sLetter = 57;
      
    }
    else if(knobBRight){
      sLetter++;

      if(sLetter > 90)
        sLetter = 48;
      else if(sLetter > 57 && sLetter < 65)
        sLetter = 65;
    }

    mainInstrumentVoice.setCharAt(saveCursorX,(char)sLetter);

  }//END if((knobBLeft) || (knobBRight))

  if(btA){                                                      //CLEAR WORD
    mainInstrumentVoice = "                  ";
    saveCursorX = 0;
  }

  if(btB)                                                       //DELETE LETTER
    mainInstrumentVoice.setCharAt(saveCursorX,' ');  
  

  if(btC){                                                      //GO BACK
    mainInstrumentVoice = preMainInstrumentVoice;

    //setScreen(beforeSaveScreen);

      onlySave = false;

      if(presetChange){
        presetChange = false;
      }
      
      switch(memState){
        case LOCAL_FOLDER:
          getLocalVoicePageList(currentLocalPage);
          loadLocalVoice(currentLocalVoice);  
        break;
        case SD_FOLDER:
          if(initSD()){
            setSDVoicesTotals();    
            updateCurrentSDInPage(currentSdVoice);
            getSDVoicePageList(currentSdPage);
            loadTEMPorSDVoice(currentSdVoiceInPage, 0, true);
          }
          else{
            getLocalVoicePageList(currentLocalPage);
            loadLocalVoice(currentLocalVoice);  
          }
        break; 
        case TEMP_FOLDER:
          if(initSD()){
            setSDVoicesTotals();    
            updateCurrentTEMPInPage(currentTempVoice);
            getTEMPVoicePageList(currentTempPage);
            loadTEMPorSDVoice(currentTempVoiceInPage, 0, false);
          }
          else{
            getLocalVoicePageList(currentLocalPage);
            loadLocalVoice(currentLocalVoice);  
          }
        break;
      }

 
      setScreen(MODE_A);
    
  }

  if(btD){                                                      //SAVE

    int tempLength = mainInstrumentVoice.length();
    int lastLetterIndex = tempLength;

    bool someName = false;

    for(int i = 0; i < tempLength; i++){
      if(mainInstrumentVoice.charAt(i) != 32)
        someName = true;
    }

    if(someName){
      cleanMainInstrumentVoice = "";

      if(tempLength > 0){
        for(int i = (tempLength - 1); i > 0; i--){
          if(mainInstrumentVoice.charAt(i) != 32){
            lastLetterIndex = i;
            break;
          }
        }

        for(int i = 0; i <= lastLetterIndex; i++)
          cleanMainInstrumentVoice += mainInstrumentVoice.charAt(i);
      }
    

      if(initSD()){

        onlySave = true;

        if(checkIfVoiceExist())
          setScreen(OVERWRITE_VOICE);
        else
          saveInstrument();
      
      }
      else
        setScreen(NO_SD_CARD);

    }
  }
}//END saveInsUserInput()


void saveMessageUserInput(){
  if(btA){//YES
    //setChanges = true;

    if(newInstrument){
      newInstrument = false;
      if(memState != LOCAL_FOLDER){    //!localOrSD1

        onlySave = true;

        preMainInstrumentVoice = mainInstrumentVoice;
        voiceNameToSaveScreen();
        setScreen(SAVE_INSTRUMENT);
      }
    }
    else{

      if(initSD()){

        onlySave = true;
        cleanMainInstrumentVoice = mainInstrumentVoice;

        if(checkIfVoiceExist())
          setScreen(OVERWRITE_VOICE);
        else
          saveInstrument();
      }
      else
        setScreen(NO_SD_CARD);
    }

  }

  if(btB){//NO

    if(newInstrument){
      newInstrument = false;
      memState = LOCAL_FOLDER;
      getLocalVoicePageList(currentLocalPage);
      loadLocalVoice(currentLocalVoice); 
    }
    else{
      if(presetChange){
        presetChange = false;
        loadLocalVoice(currentLocalVoice);
      }
      else{
        if(initSD()){
          memState = SD_FOLDER;
          setSDVoicesTotals();    
          updateCurrentSDInPage(currentSdVoice);
          getSDVoicePageList(currentSdPage);
          loadTEMPorSDVoice(currentSdVoiceInPage, 0, true);
        }
        else{
          memState = LOCAL_FOLDER;
          getLocalVoicePageList(currentLocalPage);
          loadLocalVoice(currentLocalVoice);  
          setScreen(NO_SD_CARD);
        }
      }//END else
    }
    setScreen(MODE_A);
  }
}

void overwriteSaveUserInput(){
  if(btA){
    overwriteIt = true;
    saveInstrument();
  }
  else if(btB){

    setScreen(SAVE_INSTRUMENT);

    /*
    if(onlySave){
      onlySave = false;

      if(presetChange){
        presetChange = false;
      }
      
      if(localOrSD1){
        //localOrSD1 = true;
        getLocalVoicePageList(currentLocalPage);
        loadLocalVoice(currentLocalVoice);  
      }
      else{
        if(initSD()){
          setSDVoicesTotals();    
          updateCurrentSDInPage(currentSdVoice);
          getSDVoicePageList(currentSdPage);
          loadSDVoice(currentSdVoiceInPage, 0);
        }
        else{
          getLocalVoicePageList(currentLocalPage);
          loadLocalVoice(currentLocalVoice);  
        }
      }
 
      setScreen(MODE_A);
    }
    else
      setScreen(SAVE_INSTRUMENT);
      */
  }
}


void checkLimitOfKnob(int selectedKnob){

    switch(selectedKnob){
        case 0:
        if(knobACount < 0){
            knobACount = 0;
            knobA.write(0);
            knobALeft = true;
        }
        else if(knobACount > 0){
            knobACount = 0;
            knobA.write(0);
            knobARight = true;
            //knobA.write(maxLimit * 2);
        }
        break;
        case 1:
        if(knobBCount < 0){
            knobBCount = 0;
            knobB.write(0);
            knobBLeft = true;
        }
        else if(knobBCount > 0){
            knobBCount = 0;
            knobB.write(0);
            knobBRight = true;
            //knobB.write(maxLimit * 2);
        }
        break;
    }
}

void getChannelOpType(){

    uint8_t algorithm = opl3->getCNT(1);

    if(algorithm > 1)
      mainInstrumentOpMode = OP4;
    else
      mainInstrumentOpMode = OP2;

}

void loadChannelData(){

  switch(memState){
    case LOCAL_FOLDER:
      mainInstrumentVoice = fileNameList[currentLocalVoiceInPage];
    break;
    case SD_FOLDER:
      if(!newInstrument)
        mainInstrumentVoice = fileNameList[currentSdVoiceInPage];
    break;
    case TEMP_FOLDER:
      if(!newInstrument)
        mainInstrumentVoice = fileNameList[currentTempVoiceInPage];
    break;
  }


   chAlgorithm = opl3->getCNT(1);
   chFeedback = opl3->getFB(1,1);

}

void loadOperatorData(){

  switch(memState){
    case LOCAL_FOLDER:
      mainInstrumentVoice = fileNameList[currentLocalVoiceInPage];
    break;
    case SD_FOLDER:
      if(!newInstrument)
        mainInstrumentVoice = fileNameList[currentSdVoiceInPage];
    break;
    case TEMP_FOLDER:
      if(!newInstrument)
        mainInstrumentVoice = fileNameList[currentTempVoiceInPage];
    break;
  }


  opAttack = opl3->getAR(1, selectedOp);
  opDecay = opl3->getDR(1, selectedOp);

  opSustain = opl3->getSL(1, selectedOp);
  opRelease = opl3->getRR(1, selectedOp);

  opTremolo = opl3->getAM(1, selectedOp);
  opVibrato = opl3->getVIB(1, selectedOp);
  opEnvelopeType = opl3->getEGT(1, selectedOp);
  opKeyScaleRate = opl3->getKSR(1, selectedOp);
  
  opWaveform = opl3->getWS(1, selectedOp);

  //opTotalLevel = opl3->getTL(1, selectedOp);
  opTotalLevel = tlFix(opl3->getTL(1, selectedOp));


  opFreMult = opl3->getMULT(1, selectedOp);

  opKeyScaleLevel = opl3->getKSL(1, selectedOp);

}


void updateExtraData(){

    switch(screenActivity->getScreen()){
        case MODE_A:
          switch(memState){
            case LOCAL_FOLDER:
              screenActivity->setModeScreenAExtraData(fileNameList, currentLocalPage, localVoicesPages, currentLocalVoice, currentLocalVoiceInPage, mainInstrumentOpMode, voicePagerA, memState, opl3->getSpeedExpression());
            break;
            case SD_FOLDER:
              screenActivity->setModeScreenAExtraData(fileNameList, currentSdPage, sdVoicesPages, currentSdVoice, currentSdVoiceInPage, mainInstrumentOpMode, voicePagerA, memState, opl3->getSpeedExpression());
            break;
            case TEMP_FOLDER:
              screenActivity->setModeScreenAExtraData(fileNameList, currentTempPage, tempVoicesPages, currentTempVoice, currentTempVoiceInPage, mainInstrumentOpMode, voicePagerA, memState, opl3->getSpeedExpression());
            break;
          }
        break;
        case MODE_B:
          if(memState == LOCAL_FOLDER && localOrSD2)
            screenActivity->setModeScreenBExtraData(fileNameListA, fileNameListB, currentLocalPageA, currentLocalPageB, localVoicesPagesA, localVoicesPagesB, currentLocalVoiceA, currentLocalVoiceB, currentLocalVoiceInPageA, currentLocalVoiceInPageB, instrumentAOpMode, instrumentBOpMode, voicePagerA, voicePagerB, op4op2Set, memState, localOrSD2, notepoint, setNotepoint);
          else if(memState == SD_FOLDER && localOrSD2)
            screenActivity->setModeScreenBExtraData(fileNameListA, fileNameListB, currentSdPageA, currentLocalPageB, sdVoicesPagesA, localVoicesPagesB, currentSdVoiceA, currentLocalVoiceB, currentSdVoiceInPageA, currentLocalVoiceInPageB, instrumentAOpMode, instrumentBOpMode, voicePagerA, voicePagerB, op4op2Set, memState, localOrSD2, notepoint, setNotepoint);
          else if(memState == LOCAL_FOLDER && (!localOrSD2))
            screenActivity->setModeScreenBExtraData(fileNameListA, fileNameListB, currentLocalPageA, currentSdPageB, localVoicesPagesA, sdVoicesPagesB, currentLocalVoiceA, currentSdVoiceB, currentLocalVoiceInPageA, currentSdVoiceInPageB, instrumentAOpMode, instrumentBOpMode, voicePagerA, voicePagerB, op4op2Set, memState, localOrSD2, notepoint, setNotepoint);
          else
            screenActivity->setModeScreenBExtraData(fileNameListA, fileNameListB, currentSdPageA, currentSdPageB, sdVoicesPagesA, sdVoicesPagesB, currentSdVoiceA, currentSdVoiceB, currentSdVoiceInPageA, currentSdVoiceInPageB, instrumentAOpMode, instrumentBOpMode, voicePagerA, voicePagerB, op4op2Set, memState, localOrSD2, notepoint, setNotepoint);
        break;
        case MODE_C:
          screenActivity->setModeScreenCExtraData(vgmNameList, currentSdVgmPage, sdVgmPages, currentSdVgm, currentSdVgmInPage, vgmPager, vgmPlay, vgmPause);
        break;
        case CHANNEL:
          screenActivity->setChannelExtraData(mainInstrumentVoice, chCurrentPosition, chAlgorithm, chFeedback);
        break;
        case OPERATOR:
          screenActivity->setOperatorOpExtraData(mainInstrumentVoice, opCurrentPage, opCurrentPositionInPage, selectedOp, opAttack, opDecay, opSustain, opRelease, opTremolo, opVibrato, opEnvelopeType, opKeyScaleRate, opWaveform, opTotalLevel, opFreMult, opKeyScaleLevel);
        break;
        case SAVE_INSTRUMENT:
          screenActivity->setSaveInsExtraData(mainInstrumentVoice, saveCursorX);
        break;
    } 
}

void updateKnobs(){

  int newA, newB;
  newA = knobA.read();
  newB = knobB.read();

    if(newA != knobACount) {
        knobACount = newA;
        knobACount *= 0.25f;
        checkLimitOfKnob(0);
    }

    if(newB != knobBCount) {
        knobBCount = newB;
        knobBCount *= 0.25f;
        checkLimitOfKnob(1);
    }

}//END void

void resetKnobSwitchs(){
    btA = false;
    btB = false;
    btC = false;
    btD = false;
    btE = false;
    btF = false;

    swA = false;
    swB = false;

    holdSwA = false;
    holdSwB = false;

    knobARight = false;
    knobALeft = false;
    knobBRight = false;
    knobBLeft = false;
    
}

void updateCurrentSDInPage(int16_t temp){     
  int tempDiv = (temp % 9);

  if(tempDiv == 0)
    currentSdVoiceInPage = 8;
  else
    currentSdVoiceInPage = tempDiv - 1;
}

void updateCurrentTEMPInPage(int16_t temp){     
  int tempDiv = (temp % 9);

  if(tempDiv == 0)
    currentTempVoiceInPage = 8;
  else
    currentTempVoiceInPage = tempDiv - 1;
}

void updateCurrentSdVgmInPage(int16_t temp){
  int tempDiv = (temp % 4);

  if(tempDiv == 0)
    currentSdVgmInPage = 3;
  else
    currentSdVgmInPage = tempDiv - 1;
}

void updateCurrentSDInPageA(int16_t temp){
  int tempDiv = (temp % 4);

  if(tempDiv == 0)
    currentSdVoiceInPageA = 3;
  else
    currentSdVoiceInPageA = tempDiv - 1;
}

void updateCurrentSDInPageB(int16_t temp){
  int tempDiv = (temp % 4);

  if(tempDiv == 0)
    currentSdVoiceInPageB = 3;
else
    currentSdVoiceInPageB = tempDiv - 1;
}

void updateCurrentLocalInPage(int16_t temp){
  int tempDiv = (temp % 9);

  if(tempDiv == 0)
    currentLocalVoiceInPage = 8;
  else
    currentLocalVoiceInPage = tempDiv - 1;
}

void updateCurrentLocalInPageA(int16_t temp){
  int tempDiv = (temp % 4);

  if(tempDiv == 0)
    currentLocalVoiceInPageA = 3;
  else
    currentLocalVoiceInPageA = tempDiv - 1;
}

void updateCurrentLocalInPageB(int16_t temp){
  int tempDiv = (temp % 4);

  if(tempDiv == 0)
    currentLocalVoiceInPageB = 3;
  else
    currentLocalVoiceInPageB = tempDiv - 1;
}

bool checkIfVoiceExist(){
  bool temp = false;

  String tempName = "";
  String shortName = "";

  char tempChar;


  for(int i = 0; i < cleanMainInstrumentVoice.length(); i++){
    tempChar = cleanMainInstrumentVoice.charAt(i);

    if(tempChar != ' ' && tempChar != '.')
      tempName += tempChar;
  }

  int tempLength = tempName.length();

  if(tempLength > 8){    
    for(int i = 0; i < 5; i++)
      shortName += tempName.charAt(i);

    shortName += tempName.charAt(tempLength - 3);
    shortName += tempName.charAt(tempLength - 2);
    shortName += tempName.charAt(tempLength - 1);
  }
  else
    shortName = tempName;



  if(mainInstrumentOpMode == OP4)
    shortName += ".OP4";
  else
    shortName += ".OP2";

  
  voicesFolder.rewindDirectory();

  while(true){
    File entry = voicesFolder.openNextFile();
    if(!entry)
      break;
    
    if(!entry.isDirectory()){
      if(shortName.equals(entry.name()))
        temp = true;
    }
  }

  return temp;
}

int tlFix(int tlData){

  tlData -= 63;
  tlData *= (-1);

  return tlData;
}

uint8_t getVoiceProgmemAt(int voiceIndex, int byteIndex){
  switch(voiceIndex){
    case 1:
      return pgm_read_byte_near(VOICE1 + byteIndex);
    break;
    case 2:
      return pgm_read_byte_near(VOICE2 + byteIndex);
    break;
    case 3:
      return pgm_read_byte_near(VOICE3 + byteIndex);
    break;
    case 4:
      return pgm_read_byte_near(VOICE4 + byteIndex);
    break;
    case 5:
      return pgm_read_byte_near(VOICE5 + byteIndex);
    break;
    case 6:
      return pgm_read_byte_near(VOICE6 + byteIndex);
    break;
    case 7:
      return pgm_read_byte_near(VOICE7 + byteIndex);
    break;
    case 8:
      return pgm_read_byte_near(VOICE8 + byteIndex);
    break;
    case 9:
      return pgm_read_byte_near(VOICE9 + byteIndex);
    break;
    case 10:
      return pgm_read_byte_near(VOICE10 + byteIndex);
    break;
    case 11:
      return pgm_read_byte_near(VOICE11 + byteIndex);
    break;
    case 12:
      return pgm_read_byte_near(VOICE12 + byteIndex);
    break;
    case 13:
      return pgm_read_byte_near(VOICE13 + byteIndex);
    break;
    case 14:
      return pgm_read_byte_near(VOICE14 + byteIndex);
    break;
    case 15:
      return pgm_read_byte_near(VOICE15 + byteIndex);
    break;
    case 16:
      return pgm_read_byte_near(VOICE16 + byteIndex);
    break;
    case 17:
      return pgm_read_byte_near(VOICE17 + byteIndex);
    break;
    case 18:
      return pgm_read_byte_near(VOICE18 + byteIndex);
    break;
    case 19:
      return pgm_read_byte_near(VOICE19 + byteIndex);
    break;
    case 20:
      return pgm_read_byte_near(VOICE20 + byteIndex);
    break;

    case 21:
      return pgm_read_byte_near(VOICE21 + byteIndex);
    break;
    case 22:
      return pgm_read_byte_near(VOICE22 + byteIndex);
    break;
    case 23:
      return pgm_read_byte_near(VOICE23 + byteIndex);
    break;
    case 24:
      return pgm_read_byte_near(VOICE24 + byteIndex);
    break;
    case 25:
      return pgm_read_byte_near(VOICE25 + byteIndex);
    break;
    case 26:
      return pgm_read_byte_near(VOICE26 + byteIndex);
    break;
    case 27:
      return pgm_read_byte_near(VOICE27 + byteIndex);
    break;
    case 28:
      return pgm_read_byte_near(VOICE28 + byteIndex);
    break;
    case 29:
      return pgm_read_byte_near(VOICE29 + byteIndex);
    break;
    case 30:
      return pgm_read_byte_near(VOICE30 + byteIndex);
    break;
    case 31:
      return pgm_read_byte_near(VOICE31 + byteIndex);
    break;
    case 32:
      return pgm_read_byte_near(VOICE32 + byteIndex);
    break;
    case 33:
      return pgm_read_byte_near(VOICE33 + byteIndex);
    break;
    case 34:
      return pgm_read_byte_near(VOICE34 + byteIndex);
    break;
    case 35:
      return pgm_read_byte_near(VOICE35 + byteIndex);
    break;
    case 36:
      return pgm_read_byte_near(VOICE36 + byteIndex);
    break;
    case 37:
      return pgm_read_byte_near(VOICE37 + byteIndex);
    break;
    case 38:
      return pgm_read_byte_near(VOICE38 + byteIndex);
    break;
    case 39:
      return pgm_read_byte_near(VOICE39 + byteIndex);
    break;
    case 40:
      return pgm_read_byte_near(VOICE40 + byteIndex);
    break;

    case 41:
      return pgm_read_byte_near(VOICE41 + byteIndex);
    break;
    case 42:
      return pgm_read_byte_near(VOICE42 + byteIndex);
    break;
    case 43:
      return pgm_read_byte_near(VOICE43 + byteIndex);
    break;
    case 44:
      return pgm_read_byte_near(VOICE44 + byteIndex);
    break;
    case 45:
      return pgm_read_byte_near(VOICE45 + byteIndex);
    break;
    case 46:
      return pgm_read_byte_near(VOICE46 + byteIndex);
    break;
    case 47:
      return pgm_read_byte_near(VOICE47 + byteIndex);
    break;
    case 48:
      return pgm_read_byte_near(VOICE48 + byteIndex);
    break;
    case 49:
      return pgm_read_byte_near(VOICE49 + byteIndex);
    break;
    case 50:
      return pgm_read_byte_near(VOICE50 + byteIndex);
    break;
    case 51:
      return pgm_read_byte_near(VOICE51 + byteIndex);
    break;
    case 52:
      return pgm_read_byte_near(VOICE52 + byteIndex);
    break;
    case 53:
      return pgm_read_byte_near(VOICE53 + byteIndex);
    break;
    case 54:
      return pgm_read_byte_near(VOICE54 + byteIndex);
    break;
    case 55:
      return pgm_read_byte_near(VOICE55 + byteIndex);
    break;
    case 56:
      return pgm_read_byte_near(VOICE56 + byteIndex);
    break;
    case 57:
      return pgm_read_byte_near(VOICE57 + byteIndex);
    break;
    case 58:
      return pgm_read_byte_near(VOICE58 + byteIndex);
    break;
    case 59:
      return pgm_read_byte_near(VOICE59 + byteIndex);
    break;
    case 60:
      return pgm_read_byte_near(VOICE60 + byteIndex);
    break;

    case 61:
      return pgm_read_byte_near(VOICE61 + byteIndex);
    break;
    case 62:
      return pgm_read_byte_near(VOICE62 + byteIndex);
    break;
    case 63:
      return pgm_read_byte_near(VOICE63 + byteIndex);
    break;
    case 64:
      return pgm_read_byte_near(VOICE64 + byteIndex);
    break;
    case 65:
      return pgm_read_byte_near(VOICE65 + byteIndex);
    break;
    case 66:
      return pgm_read_byte_near(VOICE66 + byteIndex);
    break;
    case 67:
      return pgm_read_byte_near(VOICE67 + byteIndex);
    break;
    case 68:
      return pgm_read_byte_near(VOICE68 + byteIndex);
    break;
    case 69:
      return pgm_read_byte_near(VOICE69 + byteIndex);
    break;
    case 70:
      return pgm_read_byte_near(VOICE70 + byteIndex);
    break;
    case 71:
      return pgm_read_byte_near(VOICE71 + byteIndex);
    break;
    case 72:
      return pgm_read_byte_near(VOICE72 + byteIndex);
    break;
    case 73:
      return pgm_read_byte_near(VOICE73 + byteIndex);
    break;
    case 74:
      return pgm_read_byte_near(VOICE74 + byteIndex);
    break;
    case 75:
      return pgm_read_byte_near(VOICE75 + byteIndex);
    break;
    case 76:
      return pgm_read_byte_near(VOICE76 + byteIndex);
    break;
    case 77:
      return pgm_read_byte_near(VOICE77 + byteIndex);
    break;
    case 78:
      return pgm_read_byte_near(VOICE78 + byteIndex);
    break;
    case 79:
      return pgm_read_byte_near(VOICE79 + byteIndex);
    break;
    case 80:
      return pgm_read_byte_near(VOICE80 + byteIndex);
    break;

    case 81:
      return pgm_read_byte_near(VOICE81 + byteIndex);
    break;
    case 82:
      return pgm_read_byte_near(VOICE82 + byteIndex);
    break;
    case 83:
      return pgm_read_byte_near(VOICE83 + byteIndex);
    break;
    case 84:
      return pgm_read_byte_near(VOICE84 + byteIndex);
    break;
    case 85:
      return pgm_read_byte_near(VOICE85 + byteIndex);
    break;
    case 86:
      return pgm_read_byte_near(VOICE86 + byteIndex);
    break;
    case 87:
      return pgm_read_byte_near(VOICE87 + byteIndex);
    break;
    case 88:
      return pgm_read_byte_near(VOICE88 + byteIndex);
    break;
    case 89:
      return pgm_read_byte_near(VOICE89 + byteIndex);
    break;
    case 90:
      return pgm_read_byte_near(VOICE90 + byteIndex);
    break;
    case 91:
      return pgm_read_byte_near(VOICE91 + byteIndex);
    break;
    case 92:
      return pgm_read_byte_near(VOICE92 + byteIndex);
    break;
    case 93:
      return pgm_read_byte_near(VOICE93 + byteIndex);
    break;
    case 94:
      return pgm_read_byte_near(VOICE94 + byteIndex);
    break;
    case 95:
      return pgm_read_byte_near(VOICE95 + byteIndex);
    break;
    case 96:
      return pgm_read_byte_near(VOICE96 + byteIndex);
    break;
    case 97:
      return pgm_read_byte_near(VOICE97 + byteIndex);
    break;
    case 98:
      return pgm_read_byte_near(VOICE98 + byteIndex);
    break;
    case 99:
      return pgm_read_byte_near(VOICE99 + byteIndex);
    break;
    case 100:
      return pgm_read_byte_near(VOICE100 + byteIndex);
    break;

    case 101:
      return pgm_read_byte_near(VOICE101 + byteIndex);
    break;
    case 102:
      return pgm_read_byte_near(VOICE102 + byteIndex);
    break;
    case 103:
      return pgm_read_byte_near(VOICE103 + byteIndex);
    break;
    case 104:
      return pgm_read_byte_near(VOICE104 + byteIndex);
    break;
    case 105:
      return pgm_read_byte_near(VOICE105 + byteIndex);
    break;
    case 106:
      return pgm_read_byte_near(VOICE106 + byteIndex);
    break;
    case 107:
      return pgm_read_byte_near(VOICE107 + byteIndex);
    break;
    case 108:
      return pgm_read_byte_near(VOICE108 + byteIndex);
    break;
    case 109:
      return pgm_read_byte_near(VOICE109 + byteIndex);
    break;
    case 110:
      return pgm_read_byte_near(VOICE110 + byteIndex);
    break;
    case 111:
      return pgm_read_byte_near(VOICE111 + byteIndex);
    break;
    case 112:
      return pgm_read_byte_near(VOICE112 + byteIndex);
    break;
    case 113:
      return pgm_read_byte_near(VOICE113 + byteIndex);
    break;
    case 114:
      return pgm_read_byte_near(VOICE114 + byteIndex);
    break;
    case 115:
      return pgm_read_byte_near(VOICE115 + byteIndex);
    break;
    case 116:
      return pgm_read_byte_near(VOICE116 + byteIndex);
    break;
    case 117:
      return pgm_read_byte_near(VOICE117 + byteIndex);
    break;
    case 118:
      return pgm_read_byte_near(VOICE118 + byteIndex);
    break;
    case 119:
      return pgm_read_byte_near(VOICE119 + byteIndex);
    break;
    case 120:
      return pgm_read_byte_near(VOICE120 + byteIndex);
    break;

    case 121:
      return pgm_read_byte_near(VOICE121 + byteIndex);
    break;
    case 122:
      return pgm_read_byte_near(VOICE122 + byteIndex);
    break;
    case 123:
      return pgm_read_byte_near(VOICE123 + byteIndex);
    break;
    case 124:
      return pgm_read_byte_near(VOICE124 + byteIndex);
    break;
    case 125:
      return pgm_read_byte_near(VOICE125 + byteIndex);
    break;
    case 126:
      return pgm_read_byte_near(VOICE126 + byteIndex);
    break;
    case 127:
      return pgm_read_byte_near(VOICE127 + byteIndex);
    break;
    /*
    case 128:
      return pgm_read_byte_near(VOICE128 + byteIndex);
    break;
    case 129:
      return pgm_read_byte_near(VOICE129 + byteIndex);
    break;
    case 130:
      return pgm_read_byte_near(VOICE130 + byteIndex);
    break;
    case 131:
      return pgm_read_byte_near(VOICE131 + byteIndex);
    break;
    case 132:
      return pgm_read_byte_near(VOICE132 + byteIndex);
    break;
    case 133:
      return pgm_read_byte_near(VOICE133 + byteIndex);
    break;
    case 134:
      return pgm_read_byte_near(VOICE134 + byteIndex);
    break;
    case 135:
      return pgm_read_byte_near(VOICE135 + byteIndex);
    break;
    case 136:
      return pgm_read_byte_near(VOICE136 + byteIndex);
    break;
    case 137:
      return pgm_read_byte_near(VOICE137 + byteIndex);
    break;
    case 138:
      return pgm_read_byte_near(VOICE138 + byteIndex);
    break;
    case 140:
      return pgm_read_byte_near(VOICE140 + byteIndex);
    break;
    case 141:
      return pgm_read_byte_near(VOICE141 + byteIndex);
    break;
    case 142:
      return pgm_read_byte_near(VOICE142 + byteIndex);
    break;
    case 143:
      return pgm_read_byte_near(VOICE143 + byteIndex);
    break;
    case 144:
      return pgm_read_byte_near(VOICE144 + byteIndex);
    break;
    case 145:
      return pgm_read_byte_near(VOICE145 + byteIndex);
    break;
    case 146:
      return pgm_read_byte_near(VOICE146 + byteIndex);
    break;
    case 147:
      return pgm_read_byte_near(VOICE147 + byteIndex);
    break;
    case 148:
      return pgm_read_byte_near(VOICE148 + byteIndex);
    break;
    case 149:
      return pgm_read_byte_near(VOICE149 + byteIndex);
    break;
    case 150:
      return pgm_read_byte_near(VOICE150 + byteIndex);
    break;
    case 151:
      return pgm_read_byte_near(VOICE151 + byteIndex);
    break;
    case 152:
      return pgm_read_byte_near(VOICE152 + byteIndex);
    break;
    case 153:
      return pgm_read_byte_near(VOICE153 + byteIndex);
    break;
    case 154:
      return pgm_read_byte_near(VOICE154 + byteIndex);
    break;
    case 155:
      return pgm_read_byte_near(VOICE155 + byteIndex);
    break;
    case 156:
      return pgm_read_byte_near(VOICE156 + byteIndex);
    break;
    case 157:
      return pgm_read_byte_near(VOICE157 + byteIndex);
    break;
    case 158:
      return pgm_read_byte_near(VOICE158 + byteIndex);
    break;
    case 159:
      return pgm_read_byte_near(VOICE159 + byteIndex);
    break;
    case 160:
      return pgm_read_byte_near(VOICE160 + byteIndex);
    break;
    case 161:
      return pgm_read_byte_near(VOICE161 + byteIndex);
    break;
    case 162:
      return pgm_read_byte_near(VOICE162 + byteIndex);
    break;
    */
  }
}

void setScreen(Screen newScreen){
  currentScreen = newScreen;
  screenActivity->setScreen(&tft, newScreen);
}



bool initSD(){
  bool temp = false;

  voices = 0;
  voices3 = 0;
  tVoices = 0;
  tVoices3 = 0;

  File tempFile;

  vgms = 0;

  sdCard = SD.begin(4);

  if(sdCard){
    temp = true;

    if(!SD.exists("/voices"))
      SD.mkdir("/voices");
    
    if(!SD.exists("/vgm"))
      SD.mkdir("/vgm");
    
    if(!SD.exists("/temp"))
      SD.mkdir("/temp");

    if(!SD.exists("dataA.bin")){
      tempFolder = SD.open("dataA.bin", FILE_WRITE);
      tempFolder.close();
    }

    if(!SD.exists("dataB.bin")){
      tempFolder = SD.open("dataB.bin", FILE_WRITE);
      tempFolder.close();
    }

    if(!SD.exists("dataC.bin")){
      tempFolder = SD.open("dataC.bin", FILE_WRITE);
      tempFolder.close();
    }

    if(!SD.exists("dataD.bin")){
      tempFolder = SD.open("dataD.bin", FILE_WRITE);
      tempFolder.close();
    }

    if(!SD.exists("dataE.bin")){
      tempFolder = SD.open("dataE.bin", FILE_WRITE);
      tempFolder.close();
    }

    if(!SD.exists("init.bin")){
      tempFolder = SD.open("init.bin", FILE_WRITE);
      tempFolder.close();
    }


    voicesFolder = SD.open("/voices");
    vgmFolder = SD.open("/vgm");
    tempFolder = SD.open("/temp");


    String tempName = "";

    while(true){
      File entry = voicesFolder.openNextFile();
      if(!entry)
        break;
      
      if(!entry.isDirectory()){
        tempName = entry.name();

        if(tempName.endsWith(".OP2"))
          voices++;

        if(tempName.endsWith(".OP4"))
          voices3++;

      }

      entry.close();
    }
    
    while(true){
      File entry = tempFolder.openNextFile();
      if(!entry)
        break;
      
      if(!entry.isDirectory()){
        tempName = entry.name();

        if(tempName.endsWith(".OP2"))
          tVoices++;

        if(tempName.endsWith(".OP4"))
          tVoices3++;

      }

      entry.close();
    }

    while(true){
      File entry = vgmFolder.openNextFile();
      if(!entry)
        break;
      
      if(!entry.isDirectory())
        vgms++;    

      entry.close();      
    }

  }
  else{
    Serial.println("NO SD CARD DETECTED!!");
  }

  return temp;
}


void initDisplay(){
  //tft.begin();
  tft.initR(INITR_144GREENTAB);

  //tft.clearScreen();
  tft.setRotation(3);

  tft.fillScreen(WHITE);

  tft.drawBitmap(0, 0, myBitmap1, 128, 128, DARK_GREEN);
  tft.drawBitmap(0, 0, myBitmap2, 128, 128, RED);

  delay(1000);

}

void setLocalVoicesTotals(){
  localVoices = tlVoices;
  localVoicesPages = localVoices / 9;
  int tempDiv = localVoices % 9;
  if(tempDiv != 0)
    localVoicesPages++;


  localVoicesPagesA = localVoices / 4;
  tempDiv = localVoices % 4;
  if(tempDiv != 0)
    localVoicesPagesA++;


  localVoicesB = tl3Voices;
  localVoicesPagesB = localVoicesB / 4;
  tempDiv = localVoicesB % 4;
  if(tempDiv != 0)
    localVoicesPagesB++;
}




void setSDVoicesTotals(){

  sdVoices = (voices + voices3);
  sdVoicesPages = sdVoices / 9;
  int tempDiv = sdVoices % 9;
  if(tempDiv != 0)
    sdVoicesPages++;

  sdVoicesPagesA = sdVoices / 4;
  tempDiv = sdVoices % 4;
  if(tempDiv != 0)
    sdVoicesPagesA++;

  sdVoicesB = (voices + voices3);
  sdVoicesPagesB = sdVoicesB / 4;
  tempDiv = sdVoicesB % 4;
  if(tempDiv != 0)
    sdVoicesPagesB++;


  sdVgms = vgms;
  sdVgmPages = sdVgms / 4;
  tempDiv = sdVgms % 4;
  if(tempDiv != 0)
    sdVgmPages++;


  tempVoices = (tVoices + tVoices3);
  tempVoicesPages = tempVoices / 9;
  tempDiv = tempVoices % 9;
  if(tempDiv != 0)
    tempVoicesPages++;

}


void initAll(){
  
  currentKeyMode = SINGLE;
  previousKeyMode = SINGLE;
  currentInstrument = A;
  ledState = true;

  pinMode(KNOB_A_BTN, INPUT);
  pinMode(KNOB_B_BTN, INPUT);
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_C, INPUT);
  pinMode(BUTTON_D, INPUT);
  pinMode(BUTTON_E, INPUT);
  pinMode(BUTTON_F, INPUT);

  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(LED_E, OUTPUT);
  pinMode(LED_F, OUTPUT);

  PIOB->PIO_SODR = PIO_PB25;      //digitalWrite(LED_A, HIGH);
  PIOC->PIO_SODR = PIO_PC28;      //digitalWrite(LED_B, HIGH);
  PIOC->PIO_SODR = PIO_PC25;      //digitalWrite(LED_C, HIGH);
  PIOC->PIO_SODR = PIO_PC24;      //digitalWrite(LED_D, HIGH);
  PIOC->PIO_SODR = PIO_PC23;      //digitalWrite(LED_E, HIGH);
  PIOD->PIO_SODR = PIO_PD7;       //digitalWrite(LED_F, HIGH);

  Serial.begin(115200);

  MIDI.begin(MIDI_CHANNEL_OMNI);

  screenActivity = new ScreenActivity();

  initDisplay();

  setScreen(MODE_A);

  Serial.println("Starting OP-FM Shield");

  startUpSequence();


  opl3 = new Opl3();

  opl3->init(true);

  initSD();

  pinMode(13, OUTPUT);

}

void incVgmSong(){
  currentSdVgmInPage++;
  currentSdVgm++;

  if(currentSdVgm > sdVgms){

    currentSdVgm = 1;  
    currentSdVgmPage  = 1;

    currentSdVgmInPage = 0;

    getVGMPageList(currentSdVgmPage );

  }
  else{
    if(currentSdVgmInPage > 3){
      currentSdVgmPage ++;
      currentSdVgmInPage = 0;

      getVGMPageList(currentSdVgmPage );
    }
  }//END else
}

void vgmPlayer(){

  bool playVgm = true;

  String tempName = vgmSDNameList[currentSdVgmInPage];

  File selectedFile = SD.open("/vgm/"+tempName);

  int selectedByteA = 0;
  int selectedByteB = 0;
  uint32_t tempDataA = 0;
  float tempCalc = 0;

  bool onSampleWait = false;

  if(vgmPC == 0){
    opl3->reset();
    vgmPC = 80;
  }

  if(selectedFile.seek(vgmPC)){

    nextSong = false;

    while(playVgm){

      tempDataA = 0;

      if(onSampleWait){
        if(!wait4Samples)
          onSampleWait = false;
      }
      else{
        //if(selectedFile.available()){
        selectedByteA = selectedFile.read();
        vgmPC++;

        switch(selectedByteA){
          case 0x5a:
          case 0x5b:
          case 0x5e:          //YMF262 port 0
            selectedByteA = selectedFile.read();
            vgmPC++;

            selectedByteB = selectedFile.read();
            vgmPC++;

            opl3->write(selectedByteA, selectedByteB, false);

          break;
          case 0x5f:          //YMF262 port 1
            selectedByteA = selectedFile.read();
            vgmPC++;

            selectedByteB = selectedFile.read();
            vgmPC++;

            opl3->write(selectedByteA, selectedByteB, true);

          break;
          case 0x61:          //wait n samples
            selectedByteA = selectedFile.read();
            vgmPC++;

            selectedByteB = selectedFile.read();
            vgmPC++;

            tempDataA = selectedByteB & 0xFF;
            tempDataA = tempDataA << 8;

            tempDataA += selectedByteA & 0xFF;

            onSampleWait = true;
            waitSamples(tempDataA);
          break;
          case 0x62:          //wait 735 samples
            onSampleWait = true;
            waitSamples(735);
          break;
          case 0x63:          //wait 882 samples
            onSampleWait = true;
            waitSamples(882);    
          break;
          case 0x66:          //end of sound data
            playVgm = false;
            vgmPC = 0;
          break;
          case 0x70:          //wait 1 samples
            onSampleWait = true;
            waitSamples(1);
          break;
          case 0x71:          //wait 2 samples   
            onSampleWait = true;
            waitSamples(2);
          break;
          case 0x72:          //wait 3 samples
            onSampleWait = true;
            waitSamples(3);
          break;
          case 0x73:          //wait 4 samples
            onSampleWait = true;
            waitSamples(4);
          break;
          case 0x74:          //wait 5 samples
            onSampleWait = true;
            waitSamples(5);
          break;
          case 0x75:          //wait 6 samples
            onSampleWait = true;
            waitSamples(6);
          break;
          case 0x76:          //wait 7 samples
            onSampleWait = true;
            waitSamples(7);
          break;
          case 0x77:          //wait 8 samples
            onSampleWait = true;
            waitSamples(8);
          break;
          case 0x78:          //wait 9 samples
            onSampleWait = true;
            waitSamples(9);
          break;
          case 0x79:          //wait 10 samples
            onSampleWait = true;
            waitSamples(10);
          break;
          case 0x7a:          //wait 11 samples
            onSampleWait = true;
            waitSamples(11);
          break;
          case 0x7b:          //wait 12 samples
            onSampleWait = true;
            waitSamples(12);
          break;
          case 0x7c:          //wait 13 samples
            onSampleWait = true;
            waitSamples(13);
          break;
          case 0x7d:          //wait 14 samples
            onSampleWait = true;
            waitSamples(14);
          break;
          case 0x7e:          //wait 15 samples
            onSampleWait = true;
            waitSamples(15);
          break;
          case 0x7f:          //wait 16 samples
            onSampleWait = true;
            waitSamples(16);
          break;
        }//END switch
        //}

        if(!(PIOA->PIO_PDSR & PIO_PA6) && !internalBtnState){                 //NEXT SONG
          nextSong = true;
          playVgm = false;
          internalBtnState = true;
          vgmPC = 0;
        } 
        else if(PIOA->PIO_PDSR & PIO_PA6)                                
          internalBtnState = false;
          

        if(!(PIOA->PIO_PDSR & PIO_PA3)){                                      //MANUAL STOP     
          playVgm = false;
          vgmPC = 0;
          autoPlayIt = false;
        }
      }

    }//END while

    opl3->reset();

  }//END if(selectedFile.seek(vgmPC))

  selectedFile.close();

  if(autoPlayIt || nextSong)
    incVgmSong();
  else{
    vgmPlay = false;
    vgmPause =true;
    vgmAutoPlay = false;
    vgmGenVoice = false;
  }

}

void generateOP12Voices(){

  bool readVgm = true;

  String tempName = vgmSDNameList[currentSdVgmInPage];

  File selectedFile = SD.open("/vgm/"+tempName);

  int selectedByteA = 0;
  int selectedByteB = 0;
  uint32_t tempDataA = 0;

  unsigned long startVgmTime;
  unsigned long currentVgmTime;

  int tempSize = selectedFile.size();

  if(vgmPC == 0){
    opl3->reset();
    vgmPC = 80;
  }
  
  int voicePC = 0;

  bool is4Opl3 = false;
  bool isAOpl3Voice = false;

  if(selectedFile.seek(vgmPC)){

    screenActivity->updateGenVoiceMessage(&tft, tempSize, vgmPC, voicePC);
    startVgmTime = millis();

    while(readVgm){

      tempDataA = 0;
      is4Opl3 = false;
      isAOpl3Voice = false;

      //Serial.println(vgmPC);

      if(selectedFile.available()){
        selectedByteA = selectedFile.read();
        vgmPC++;

        switch(selectedByteA){
          case 0x5a:
          case 0x5b:
          case 0x5e:          //YMF262 port 0

            if(selectedByteA == 0x5e)
              is4Opl3 = true;

            selectedByteA = selectedFile.read();
            vgmPC++;

            selectedByteB = selectedFile.read();
            vgmPC++;

            opl3->fakeWrite(selectedByteA, selectedByteB, false);

            switch(selectedByteA){
              case 0xB0:

                if(isKOn(selectedByteB) && chEnableA[0]){
                  chEnableA[0] = false;

                  selectedFile.close();

                  if(is4Opl3)
                    isAOpl3Voice = isAOp4Voice(1);


                  if(dontExistTempVoice(1, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 1, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);

        
                }
                  
              break;
              case 0xB1:
                if(isKOn(selectedByteB) && chEnableA[1]){
                  chEnableA[1] = false;

                  selectedFile.close();

                  if(is4Opl3)
                    isAOpl3Voice = isAOp4Voice(2);


                  if(dontExistTempVoice(2, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 2, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB2:
                if(isKOn(selectedByteB) && chEnableA[2]){
                  chEnableA[2] = false;

                  selectedFile.close();

                  if(is4Opl3)
                    isAOpl3Voice = isAOp4Voice(3);


                  if(dontExistTempVoice(3, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 3, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB3:
                if(isKOn(selectedByteB) && chEnableA[3]){
                  chEnableA[3] = false;

                  selectedFile.close();

                  if(dontExistTempVoice(4, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 4, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB4:
                if(isKOn(selectedByteB) && chEnableA[4]){
                  chEnableA[4] = false;

                  selectedFile.close();

                  if(dontExistTempVoice(5, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 5, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB5:
                if(isKOn(selectedByteB) && chEnableA[5]){
                  chEnableA[5] = false;

                  selectedFile.close();

                  if(dontExistTempVoice(6, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 6, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB6:
                if(isKOn(selectedByteB) && chEnableA[6]){
                  chEnableA[6] = false;

                  selectedFile.close();

                  if(dontExistTempVoice(7, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 7, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB7:
                if(isKOn(selectedByteB) && chEnableA[7]){
                  chEnableA[7] = false;

                  selectedFile.close();

                  if(dontExistTempVoice(8, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 8, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB8:
                if(isKOn(selectedByteB) && chEnableA[8]){
                  chEnableA[8] = false;

                  selectedFile.close();

                  if(dontExistTempVoice(9, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 9, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0x20:
                chEnableA[0] = true;
              break;
              case 0x21:
                chEnableA[1] = true;
              break;
              case 0x22:
                chEnableA[2] = true;
              break;
              case 0x28:
                chEnableA[3] = true;
              break;
              case 0x29:
                chEnableA[4] = true;
              break;
              case 0x2A:
                chEnableA[5] = true;
              break;
              case 0x30:
                chEnableA[6] = true;
              break;
              case 0x31:
                chEnableA[7] = true;
              break;
              case 0x32:
                chEnableA[8] = true;
              break;
            }

          break;   
          case 0x5f:          //YMF262 port 1

            is4Opl3 = true;

            selectedByteA = selectedFile.read();
            vgmPC++;

            selectedByteB = selectedFile.read();
            vgmPC++;

            opl3->fakeWrite(selectedByteA, selectedByteB, true);

            switch(selectedByteA){
              case 0xB0:

                if(isKOn(selectedByteB) && chEnableB[0]){
                  chEnableB[0] = false;

                  selectedFile.close();

                  if(is4Opl3)
                    isAOpl3Voice = isAOp4Voice(4);


                  if(dontExistTempVoice(10, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 10, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);

        
                }
                  
              break;
              case 0xB1:
                if(isKOn(selectedByteB) && chEnableB[1]){
                  chEnableB[1] = false;

                  selectedFile.close();

                  if(is4Opl3)
                    isAOpl3Voice = isAOp4Voice(5);


                  if(dontExistTempVoice(11, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 11, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB2:
                if(isKOn(selectedByteB) && chEnableB[2]){
                  chEnableB[2] = false;

                  selectedFile.close();

                  if(is4Opl3)
                    isAOpl3Voice = isAOp4Voice(6);


                  if(dontExistTempVoice(12, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 12, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB3:
                if(isKOn(selectedByteB) && chEnableB[3]){
                  chEnableB[3] = false;

                  selectedFile.close();

                  if(dontExistTempVoice(13, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 13, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB4:
                if(isKOn(selectedByteB) && chEnableB[4]){
                  chEnableB[4] = false;

                  selectedFile.close();

                  if(dontExistTempVoice(14, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 14, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB5:
                if(isKOn(selectedByteB) && chEnableB[5]){
                  chEnableB[5] = false;

                  selectedFile.close();

                  if(dontExistTempVoice(15, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 15, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB6:
                if(isKOn(selectedByteB) && chEnableB[6]){
                  chEnableB[6] = false;

                  selectedFile.close();

                  if(dontExistTempVoice(16, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 16, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB7:
                if(isKOn(selectedByteB) && chEnableB[7]){
                  chEnableB[7] = false;

                  selectedFile.close();

                  if(dontExistTempVoice(17, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 17, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0xB8:
                if(isKOn(selectedByteB) && chEnableB[8]){
                  chEnableB[8] = false;

                  selectedFile.close();

                  if(dontExistTempVoice(18, isAOpl3Voice)){
                    saveTempVoice(vgmNameList[currentSdVgmInPage], voicePC, 18, isAOpl3Voice);
                    voicePC++;
                  }

                  selectedFile = SD.open("/vgm/"+tempName);
                  selectedFile.seek(vgmPC);
                }
              break;
              case 0x20:
                chEnableB[0] = true;
              break;
              case 0x21:
                chEnableB[1] = true;
              break;
              case 0x22:
                chEnableB[2] = true;
              break;
              case 0x28:
                chEnableB[3] = true;
              break;
              case 0x29:
                chEnableB[4] = true;
              break;
              case 0x2A:
                chEnableB[5] = true;
              break;
              case 0x30:
                chEnableB[6] = true;
              break;
              case 0x31:
                chEnableB[7] = true;
              break;
              case 0x32:
                chEnableB[8] = true;
              break;
            }

          break;
          case 0x61:          //wait n samples
            selectedByteA = selectedFile.read();
            vgmPC++;

            selectedByteB = selectedFile.read();
            vgmPC++;

          break;
          case 0x66:          //end of sound data
            readVgm = false;
            vgmPC = 0;
          break;

        }//END switch
      }    

      currentVgmTime = millis() - startVgmTime;

      if(currentVgmTime > 5000){
        startVgmTime = millis();
        screenActivity->updateGenVoiceMessage(&tft, tempSize, vgmPC, voicePC);
      }


    }//END while

    opl3->reset();

  }//END if(selectedFile.seek(vgmPC))

  selectedFile.close();

}

bool isKOn(int tempData){

  bool tempBool = false;

  int temp = tempData & 0x20;

  if(temp == 0x20)
    tempBool = true;

  return tempBool;
}

void clearTEMPFolder(){

  tempFolder.rewindDirectory();

  String tempStr = "";
  bool rmIt = false;

  while(true){
    rmIt = false;
    File entry = tempFolder.openNextFile();
    if(!entry)
      break;

    if(!entry.isDirectory()){
      tempStr = String(entry.name());
      rmIt = true;
    }
    
    entry.close();

    if(rmIt){
      if(SD.remove("/temp/"+tempStr))
        tempFolder.rewindDirectory();
    }

  }

}


void startUpSequence(){
  for(int i = 0; i < 2; i++){
    PIOB->PIO_CODR = PIO_PB25;      //digitalWrite(LED_A, LOW);
    delay(100);
    PIOB->PIO_SODR = PIO_PB25;      //digitalWrite(LED_A, HIGH);
    PIOC->PIO_CODR = PIO_PC28;      //digitalWrite(LED_B, LOW);
    delay(100);
    PIOC->PIO_SODR = PIO_PC28;      //digitalWrite(LED_B, HIGH);
    PIOC->PIO_CODR = PIO_PC25;      //digitalWrite(LED_C, LOW);
    delay(100);
    PIOC->PIO_SODR = PIO_PC25;      //digitalWrite(LED_C, HIGH);
    PIOC->PIO_CODR = PIO_PC24;      //digitalWrite(LED_D, LOW);
    delay(100);
    PIOC->PIO_SODR = PIO_PC24;      //digitalWrite(LED_D, HIGH);
    PIOC->PIO_CODR = PIO_PC23;      //digitalWrite(LED_E, LOW);
    delay(100);
    PIOC->PIO_SODR = PIO_PC23;      //digitalWrite(LED_E, HIGH);
  }
}

void releaseShiftBtn(){
  shiftBtn = false;
  saveAccesFlag = false;
  PIOD->PIO_SODR = PIO_PD7;         //digitalWrite(LED_F, HIGH);
  PIOB->PIO_SODR = PIO_PB25;        //digitalWrite(LED_A, HIGH);
  PIOC->PIO_SODR = PIO_PC28;        //digitalWrite(LED_B, HIGH);
  PIOC->PIO_SODR = PIO_PC25;        //digitalWrite(LED_C, HIGH);
  PIOC->PIO_SODR = PIO_PC24;        //digitalWrite(LED_D, HIGH);
  PIOC->PIO_SODR = PIO_PC23;        //digitalWrite(LED_E, HIGH);
}

bool saveBtnAccess(uint8_t selectedBtn){

  bool tempReturn = false;

  if(sdCard){

    String tempFileName = "";

    switch(selectedBtn){
      case 0:
        tempFileName = "dataA.bin";
      break;
      case 1:
        tempFileName = "dataB.bin";
      break;
      case 2:
        tempFileName = "dataC.bin";
      break;
      case 3:
        tempFileName = "dataD.bin";
      break;
      case 4:
        tempFileName = "dataE.bin";
      break;
    }

    if(SD.remove(tempFileName)){

      File newFile = SD.open(tempFileName, FILE_WRITE);

      if(currentKeyMode == SINGLE){
        newFile.write(0x60);
        delay(10);

        newFile.write(60);
        delay(10);

        if(memState == LOCAL_FOLDER){
          newFile.write(0x4);
          delay(10);
          newFile.write(currentLocalVoice);
          delay(10);
          newFile.write(currentLocalPage);
          delay(10);

          tempReturn = true;
        }//END if(memState == LOCAL_FOLDER)
        else{
          newFile.write(0x8);
          delay(10);
          newFile.print(fileSDNameList[currentSdVoiceInPage]);
          delay(10);

          tempReturn = true;
        }//END else

      }//END if(currentKeyMode)
      else if(currentKeyMode == DUAL){
        newFile.write(0x65);
        delay(10);

        newFile.write(notepoint);
        delay(10);

        if(op4op2Set)
          newFile.write(0x8);
        else
          newFile.write(0x4);

        if(memState == LOCAL_FOLDER){
          newFile.write(0x4);
          delay(10);  
          newFile.write(currentLocalVoiceA);
          delay(10);
          newFile.write(currentLocalPageA);
          delay(10);

          tempReturn = true;
        }
        else{
          newFile.write(0x8);
          delay(10);
          newFile.print(fileSDNameListA[currentSdVoiceInPageA]);
          delay(10);
        }

        for(int i = 0; i < 2; i++){
          newFile.write(0x1);
          delay(10);
        }

        if(localOrSD2){
          newFile.write(0x4);
          delay(10);  
          newFile.write(currentLocalVoiceB);
          delay(10);
          newFile.write(currentLocalPageB);
          delay(10);

          tempReturn = true;
        }
        else{
          newFile.write(0x8);
          delay(10);
          newFile.print(fileSDNameListB[currentSdVoiceInPageB]);
          delay(10);

          tempReturn = true;
        }

      }//END else if()

      newFile.close();
    }//END if(SD.remove(tempFileName))
  }//END if(sdCard)

  return tempReturn;
}

bool loadBtnAccess(uint8_t selectedBtn){

  bool tempReturn = false;
  bool tempReturnB = false;

  if(sdCard){
    String tempFileName = "";

    switch(selectedBtn){
      case 0:
        tempFileName = "dataA.bin";
      break;
      case 1:
        tempFileName = "dataB.bin";
      break;
      case 2:
        tempFileName = "dataC.bin";
      break;
      case 3:
        tempFileName = "dataD.bin";
      break;
      case 4:
        tempFileName = "dataE.bin";
      break;
    }

    File loadFile = SD.open(tempFileName);

    if(loadFile.available()){

      String tempVName = "";

      int tempByte = loadFile.read();

      if(tempByte == 0x60){               //SINGLE MODE

        previousKeyMode = currentKeyMode;
        currentKeyMode = SINGLE;

        currentInstrument = A;

        notepoint = loadFile.read();
        delay(10);

        tempByte = loadFile.read();
        if(tempByte == 0x4){              //LOCAL
          currentLocalVoice = loadFile.read();
          currentLocalPage = loadFile.read();

          memState = LOCAL_FOLDER;

          updateCurrentLocalInPage(currentLocalVoice);

          getLocalVoicePageList(currentLocalPage);
          
          loadLocalVoice(currentLocalVoice);

          tempReturn = true;

        }//END if(tempByte == 0x4)
        else if(tempByte == 0x8){         //SD

          while(loadFile.available()){
            tempByte = loadFile.read();

            if(tempByte != 0)
              tempVName += (char)tempByte;
          }

          if(tempVName.length() > 0){
            if(findSDVoice(tempVName))
              tempReturn = true;
          }

        }//END else if(tempByte == 0x8)

        if(tempReturn){
          cleanSynthNotes();
          cleanSplitNotes();
          currentKeyMode = SINGLE;
          setScreen(MODE_A);
        }
        else
          currentKeyMode = previousKeyMode;

      }//END if(tempByte == 0x60)
      else if(tempByte == 0x65){          //DUAL MODE

        previousKeyMode = currentKeyMode;
        currentKeyMode = DUAL;

        notepoint = loadFile.read();
        delay(10);

        tempByte = loadFile.read();
        if(tempByte == 0x4)          
          op4op2Set = false;
        else if(tempByte == 0x8)
          op4op2Set = true;

        tempByte = loadFile.read();

        currentInstrument = A;

        if(tempByte == 0x4){              //LOCAL
          currentLocalVoiceA = loadFile.read();
          currentLocalPageA = loadFile.read();

          memState = LOCAL_FOLDER;

          updateCurrentLocalInPageA(currentLocalVoiceA);

          getLocalVoicePageListA(currentLocalPageA);
          
          loadLocalVoice(currentLocalVoiceA);

          tempReturn = true;

          tempByte = loadFile.read();

        }//END if(tempByte == 0x4)
        else if(tempByte == 0x8){         //SD

          while(loadFile.available()){
            tempByte = loadFile.read();

            if(tempByte == 0x1)
              break;

            if(tempByte != 0)
              tempVName += (char)tempByte;
          }

          if(tempVName.length() > 0){
            if(findSDVoiceA(tempVName))
              tempReturn = true;
          }

        }//END else if(tempByte == 0x8)


        if(loadFile.available()){
          tempByte =  loadFile.read();
          if(tempByte == 0x1){

            currentInstrument = B;

            tempByte = loadFile.read();
            if(tempByte == 0x4){              //LOCAL

              currentLocalVoiceB = loadFile.read();
              currentLocalPageB = loadFile.read();

              localOrSD2 = true;

              updateCurrentLocalInPageB(currentLocalVoiceB);  

              getLocalVoicePageListB(currentLocalPageB);
          
              loadLocalVoice(currentLocalVoiceB);

              tempReturnB = true;
            }
            else if(tempByte == 0x8){         //SD

              tempVName = "";

              while(loadFile.available()){
                tempByte = loadFile.read();

                if(tempByte != 0)
                  tempVName += (char)tempByte;
              }

              if(tempVName.length() > 0){
                if(findSDVoiceB(tempVName))
                  tempReturnB = true;
              }
            }

          }//END if(tempByte == 0x1)
        }//END if(loadFile.available())  


        if(tempReturn && tempReturnB)
          tempReturn = true;
        else
          tempReturn = false;

        if(tempReturn){
          cleanSynthNotes();
          cleanSplitNotes();
          currentKeyMode = DUAL;
          setScreen(MODE_B);
        }
        else
          currentKeyMode = previousKeyMode;

      }//END if(tempByte == 0x65)

    }//END if(loadFile.available)

    loadFile.close();

  }//END if(sdCard)
  return tempReturn;
}


bool findSDVoice(String vName){
  bool tempFind = false;
  int selectedPage = 0;
  int selectedVoiceSD = 0;
  

  int tempLengthA = vName.length();
  int tempLengthB = 0;

  String tempSName = "";

  setSDVoicesTotals();


  for(int k = 0; k < sdVoicesPages; k++){

    int tempPos = 0;

    int startPoint = (0 + (k * 9));
    int endPoint = (8 + (k * 9));

    int currentIndex = 0;

    voicesFolder.rewindDirectory();
    while(true){
      File entry = voicesFolder.openNextFile();
      if(!entry)
        break;

      if(currentIndex > endPoint)
        break;
    
      if(!entry.isDirectory()){
        if((currentIndex >= startPoint) && (currentIndex <= endPoint)){
          
          tempSName = String(entry.name());
          tempLengthB = tempSName.length();


          if(tempLengthA == tempLengthB){
            bool tempEqual = true;

            for(int l = 0; l < tempLengthA; l++){
              if(vName.charAt(l) != tempSName.charAt(l))
                tempEqual = false;
            }

            if(tempEqual){
              tempFind = true;
              selectedPage = (k+1);
              selectedVoiceSD = tempPos;
            }
          } 
        }

        tempPos++;
      }

      currentIndex++;
    }
  }//END for

  if(tempFind){

    currentSdPage = selectedPage;
    currentSdVoice = (selectedVoiceSD + 1);
    
    memState = SD_FOLDER;

    updateCurrentSDInPage(currentSdVoice);

    getSDVoicePageList(currentSdPage);

    loadTEMPorSDVoice(currentSdVoiceInPage, 0, true);
  }

  return tempFind;
}

bool findSDVoiceA(String vName){
  bool tempFind = false;
  int selectedPage = 0;
  int selectedVoiceSD = 0;

  int tempLengthA = vName.length();
  int tempLengthB = 0;

  String tempSName = "";

  setSDVoicesTotals();


  for(int k = 0; k < sdVoicesPagesA; k++){

    int tempPos = 0;

    int startPoint = (0 + (k * 4));
    int endPoint = (3 + (k * 4));

    int currentIndex = 0;

    voicesFolder.rewindDirectory();
    while(true){
      File entry = voicesFolder.openNextFile();
      if(!entry)
        break;

      if(currentIndex > endPoint)
        break;
    
      if(!entry.isDirectory()){
        if((currentIndex >= startPoint) && (currentIndex <= endPoint)){
          
          tempSName = String(entry.name());
          tempLengthB = tempSName.length();


          if(tempLengthA == tempLengthB){
            bool tempEqual = true;

            for(int l = 0; l < tempLengthA; l++){
              if(vName.charAt(l) != tempSName.charAt(l))
                tempEqual = false;
            }

            if(tempEqual){
              tempFind = true;
              selectedPage = (k+1);
              selectedVoiceSD = tempPos;
            }
          } 
        }

        tempPos++;
      }

      currentIndex++;
    }
  }//END for

  if(tempFind){

    currentSdPageA = selectedPage;
    currentSdVoiceA = (selectedVoiceSD + 1);
    
    memState = SD_FOLDER;

    updateCurrentSDInPageA(currentSdVoiceA);

    getSDVoicePageListA(currentSdPageA);

    loadTEMPorSDVoice(currentSdVoiceInPageA, 1, true);
  }

  return tempFind;
}

bool findSDVoiceB(String vName){
  bool tempFind = false;
  int selectedPage = 0;
  int selectedVoiceSD = 0;

  int tempLengthA = vName.length();
  int tempLengthB = 0;

  String tempSName = "";

  setSDVoicesTotals();


  for(int k = 0; k < sdVoicesPagesB; k++){

    int tempPos = 0;

    int startPoint = (0 + (k * 4));
    int endPoint = (3 + (k * 4));

    int currentIndex = 0;

    voicesFolder.rewindDirectory();
    while(true){
      File entry = voicesFolder.openNextFile();
      if(!entry)
        break;

      if(currentIndex > endPoint)
        break;
    
      if(!entry.isDirectory()){
        if((currentIndex >= startPoint) && (currentIndex <= endPoint)){
          
          tempSName = String(entry.name());
          tempLengthB = tempSName.length();


          if(tempLengthA == tempLengthB){
            bool tempEqual = true;

            for(int l = 0; l < tempLengthA; l++){
              if(vName.charAt(l) != tempSName.charAt(l))
                tempEqual = false;
            }

            if(tempEqual){
              tempFind = true;
              selectedPage = (k+1);
              selectedVoiceSD = tempPos;
            }
          } 
        }

        tempPos++;
      }

      currentIndex++;
    }
  }//END for

  if(tempFind){

    currentSdPageB = selectedPage;
    currentSdVoiceB = (selectedVoiceSD + 1);
    
    localOrSD2 = false;

    updateCurrentSDInPageB(currentSdVoiceB);

    getSDVoicePageListB(currentSdPageB);

    loadTEMPorSDVoice(currentSdVoiceInPageB, 2, true);
  }

  return tempFind;
}


void blinkFLed(){
  unsigned long tempTime = millis() - currentTimeLedF;

  if(tempTime > 400){
    currentTimeLedF = millis();
    if(ledFState){
      PIOD->PIO_SODR = PIO_PD7;     //digitalWrite(LED_F, HIGH);
      ledFState = false;
    }
    else{
      PIOD->PIO_CODR = PIO_PD7;  //digitalWrite(LED_F, LOW);
      ledFState = true;
    }
  }
}

void swapMidiLed(){
  if(ledState){
    ledState = false;
    PIOB->PIO_SODR = PIO_PB27;  //digitalWrite(13, HIGH);
  }
  else{
    ledState = true;
    PIOB->PIO_CODR = PIO_PB27;  //digitalWrite(13, LOW);
  }  
}

void waitSamples(int nSamples){
  samplesCounter = nSamples;
  wait4Samples = true;
}