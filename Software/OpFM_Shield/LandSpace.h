#ifndef LANDSPACE_H
#define LANDSPACE_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> 

#define KNOB_A_CLK  47
#define KNOB_A_DT   45
#define KNOB_A_BTN  A2

#define KNOB_B_CLK  51
#define KNOB_B_DT   49
#define KNOB_B_BTN  A3

#define BUTTON_A    A4
#define BUTTON_B    A5
#define BUTTON_C    A6
#define BUTTON_D    A7
#define BUTTON_E    A8
#define BUTTON_F    A9

#define LED_A       2
#define LED_B       3
#define LED_C       5
#define LED_D       6
#define LED_E       7
#define LED_F       11

// Color definitions
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define ORANGE  0xFC80  
#define DARK_GREEN  0x0480
#define WHITE   0xFFFF
//#define DARK_GRAY   0x6B4D
#define DARK_GRAY   0x7BCF
#define DARK_RED    0xA800
#define LIGHT_BLUE  0xC63F

#define __CS 10
#define __DC 9

enum KeyMode{VGM, SINGLE, DUAL};
enum Instrument{A, B};
enum OpMode{OP2, OP4};

enum MemState{LOCAL_FOLDER, SD_FOLDER, TEMP_FOLDER};

enum Screen{IDLE, MODE_A, MODE_B, MODE_C, CHANNEL, OPERATOR, SAVE_INSTRUMENT, NO_SD_CARD, SAVE_MESSAGE, OVERWRITE_VOICE, SAVED_VOICE, EXP_VOICES, SAVED_ACCESS, LOADED_ACCESS};

static void drawVarBar(Adafruit_ST7735 *tft, int value, int preValue, int min, int max, int xStart, int yStart, int presetW, int presetH, uint16_t lColor, uint16_t hColor){

    //if(firstPrint)
    tft->fillRect(xStart, yStart, presetW + 1, presetH + 1, DARK_GRAY);

    int w = ((value-min) * presetW)/(max-min);
    int preW = ((preValue-min) * presetW)/(max-min);
    
    
    tft->drawLine(xStart + preW - 1, yStart, xStart + preW - 1, yStart + presetH, DARK_GRAY);
    tft->drawLine(xStart + preW, yStart, xStart + preW, yStart + presetH, DARK_GRAY);
    tft->drawLine(xStart + preW + 1, yStart, xStart + preW + 1, yStart + presetH, DARK_GRAY);
    
    tft->drawLine(xStart + w - 1, yStart, xStart + w - 1, yStart + presetH, YELLOW);
    tft->drawLine(xStart + w, yStart, xStart + w, yStart + presetH, YELLOW);
    tft->drawLine(xStart + w + 1, yStart, xStart + w + 1, yStart + presetH, YELLOW);
    
    
    tft->drawLine(xStart - 1, yStart, xStart - 1, yStart + presetH, lColor);
    tft->drawLine(xStart - 2, yStart, xStart - 2, yStart + presetH, lColor);
    
    tft->drawLine(xStart + presetW + 1, yStart, xStart + presetW + 1, yStart + presetH, hColor);
    tft->drawLine(xStart + presetW + 2, yStart, xStart + presetW + 2, yStart + presetH, hColor);
    
}

static void drawAlgorithm(Adafruit_ST7735 *tft, int xStart, int yStart, int algorithm){

    tft->fillRoundRect(xStart, yStart, 122, 76, 2, WHITE);   //90, 76
    tft->drawRoundRect(xStart, yStart, 122, 76, 2, BLACK);

    switch(algorithm){
        case 0:                                                 //FM
            tft->drawLine(xStart + 8, yStart + 11, xStart + 61, yStart + 11, BLACK);
            tft->drawLine(xStart + 8, yStart + 12, xStart + 61, yStart + 12, BLACK);
            tft->drawLine(xStart + 8, yStart + 13, xStart + 8, yStart + 38, BLACK);
            tft->drawLine(xStart + 9, yStart + 13, xStart + 9, yStart + 38, BLACK);
            tft->drawLine(xStart + 10, yStart + 37, xStart + 117, yStart + 37, BLACK);
            tft->drawLine(xStart + 10, yStart + 38, xStart + 117, yStart + 38, BLACK);
            tft->drawLine(xStart + 60, yStart + 13, xStart + 60, yStart + 36, BLACK);
            tft->drawLine(xStart + 61, yStart + 13, xStart + 61, yStart + 36, BLACK);
            tft->drawLine(xStart + 117, yStart + 37, xStart + 113, yStart + 33, BLACK);
            tft->drawLine(xStart + 116, yStart + 37, xStart + 112, yStart + 33, BLACK);
            tft->drawLine(xStart + 117, yStart + 38, xStart + 113, yStart + 42, BLACK);
            tft->drawLine(xStart + 116, yStart + 38, xStart + 112, yStart + 42, BLACK);

            tft->fillRect(xStart + 18, yStart + 25, 35, 26, LIGHT_BLUE);
            tft->drawRect(xStart + 18, yStart + 25, 35, 26, BLUE);

            tft->setCursor(xStart + 18 + 15, yStart + 25 + 10);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("1");

            tft->fillRect(xStart + 69, yStart + 25, 35, 26, LIGHT_BLUE);
            tft->drawRect(xStart + 69, yStart + 25, 35, 26, BLUE);

            tft->setCursor(xStart + 69 + 15, yStart + 25 + 10);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("2");

        break;
        case 1:                                                 //AM

            tft->drawLine(xStart + 34, yStart + 5, xStart + 87, yStart + 5, BLACK);
            tft->drawLine(xStart + 34, yStart + 6, xStart + 87, yStart + 6, BLACK);
            tft->drawLine(xStart + 34, yStart + 7, xStart + 34, yStart + 25, BLACK);
            tft->drawLine(xStart + 35, yStart + 7, xStart + 35, yStart + 25, BLACK);
            tft->drawLine(xStart + 36, yStart + 24, xStart + 85, yStart + 24, BLACK);
            tft->drawLine(xStart + 36, yStart + 25, xStart + 85, yStart + 25, BLACK);
            tft->drawLine(xStart + 86, yStart + 7, xStart + 86, yStart + 56, BLACK);
            tft->drawLine(xStart + 87, yStart + 7, xStart + 87, yStart + 56, BLACK);
            tft->drawLine(xStart + 76, yStart + 55, xStart + 85, yStart + 55, BLACK);
            tft->drawLine(xStart + 76, yStart + 56, xStart + 85, yStart + 56, BLACK);
            tft->drawLine(xStart + 88, yStart + 39, xStart + 113, yStart + 39, BLACK);
            tft->drawLine(xStart + 88, yStart + 40, xStart + 113, yStart + 40, BLACK);
            tft->drawLine(xStart + 109, yStart + 35, xStart + 113, yStart + 39, BLACK);
            tft->drawLine(xStart + 108, yStart + 35, xStart + 112, yStart + 39, BLACK);
            tft->drawLine(xStart + 109, yStart + 44, xStart + 113, yStart + 40, BLACK);
            tft->drawLine(xStart + 108, yStart + 44, xStart + 112, yStart + 40, BLACK);

            tft->fillRect(xStart + 44, yStart + 12, 35, 26, LIGHT_BLUE);
            tft->drawRect(xStart + 44, yStart + 12, 35, 26, BLUE);

            tft->setCursor(xStart + 44 + 15, yStart + 12 + 10);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("1");

            tft->fillRect(xStart + 44, yStart + 43, 35, 26, LIGHT_BLUE);
            tft->drawRect(xStart + 44, yStart + 43, 35, 26, BLUE);

            tft->setCursor(xStart + 44 + 15, yStart + 43 + 10);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("2");


        break;
        case 2:                                                 //FM-FM

            tft->drawLine(xStart + 5, yStart + 25, xStart + 34, yStart + 25, BLACK);
            tft->drawLine(xStart + 5, yStart + 26, xStart + 34, yStart + 26, BLACK);
            tft->drawLine(xStart + 5, yStart + 27, xStart + 5, yStart + 38, BLACK);
            tft->drawLine(xStart + 6, yStart + 27, xStart + 6, yStart + 38, BLACK);
            tft->drawLine(xStart + 7, yStart + 37, xStart + 118, yStart + 37, BLACK);
            tft->drawLine(xStart + 7, yStart + 38, xStart + 118, yStart + 38, BLACK);
            tft->drawLine(xStart + 33, yStart + 27, xStart + 33, yStart + 36, BLACK);
            tft->drawLine(xStart + 34, yStart + 27, xStart + 34, yStart + 36, BLACK);
            tft->drawLine(xStart + 118, yStart + 37, xStart + 116, yStart + 35, BLACK);
            tft->drawLine(xStart + 117, yStart + 37, xStart + 115, yStart + 35, BLACK);
            tft->drawLine(xStart + 118, yStart + 38, xStart + 116, yStart + 40, BLACK);
            tft->drawLine(xStart + 117, yStart + 38, xStart + 115, yStart + 40, BLACK);

            tft->fillRect(xStart + 11, yStart + 31, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 11, yStart + 31, 18, 14, BLUE);

            tft->setCursor(xStart + 11 + 7, yStart + 31 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("1");

            tft->fillRect(xStart + 38, yStart + 31, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 38, yStart + 31, 18, 14, BLUE);

            tft->setCursor(xStart + 38 + 7, yStart + 31 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("2");

            tft->fillRect(xStart + 65, yStart + 31, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 65, yStart + 31, 18, 14, BLUE);

            tft->setCursor(xStart + 65 + 7, yStart + 31 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("3");

            tft->fillRect(xStart + 92, yStart + 31, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 92, yStart + 31, 18, 14, BLUE);

            tft->setCursor(xStart + 92 + 7, yStart + 31 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("4");


        break;
        case 3:                                                 //AM-FM

            tft->drawLine(xStart + 22, yStart + 10, xStart + 51, yStart + 10, BLACK);
            tft->drawLine(xStart + 22, yStart + 11, xStart + 51, yStart + 11, BLACK);
            tft->drawLine(xStart + 22, yStart + 12, xStart + 22, yStart + 23, BLACK);
            tft->drawLine(xStart + 23, yStart + 12, xStart + 23, yStart + 23, BLACK);
            tft->drawLine(xStart + 24, yStart + 22, xStart + 105, yStart + 22, BLACK);
            tft->drawLine(xStart + 24, yStart + 23, xStart + 105, yStart + 23, BLACK);
            tft->drawLine(xStart + 50, yStart + 12, xStart + 50, yStart + 21, BLACK);
            tft->drawLine(xStart + 51, yStart + 12, xStart + 51, yStart + 21, BLACK);
            tft->drawLine(xStart + 104, yStart + 24, xStart + 104, yStart + 56, BLACK);
            tft->drawLine(xStart + 105, yStart + 24, xStart + 105, yStart + 56, BLACK);
            tft->drawLine(xStart + 45, yStart + 55, xStart + 103, yStart + 55, BLACK);
            tft->drawLine(xStart + 45, yStart + 56, xStart + 103, yStart + 56, BLACK);
            tft->drawLine(xStart + 106, yStart + 37, xStart + 118, yStart + 37, BLACK);
            tft->drawLine(xStart + 106, yStart + 38, xStart + 118, yStart + 38, BLACK);
            tft->drawLine(xStart + 118, yStart + 37, xStart + 116, yStart + 35, BLACK);
            tft->drawLine(xStart + 117, yStart + 37, xStart + 115, yStart + 35, BLACK);
            tft->drawLine(xStart + 118, yStart + 38, xStart + 116, yStart + 40, BLACK);
            tft->drawLine(xStart + 117, yStart + 38, xStart + 115, yStart + 40, BLACK);

            tft->fillRect(xStart + 28, yStart + 16, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 28, yStart + 16, 18, 14, BLUE);

            tft->setCursor(xStart + 28 + 7, yStart + 16 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("1");

            tft->fillRect(xStart + 28, yStart + 49, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 28, yStart + 49, 18, 14, BLUE);

            tft->setCursor(xStart + 28 + 7, yStart + 49 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("2");

            tft->fillRect(xStart + 55, yStart + 49, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 55, yStart + 49, 18, 14, BLUE);

            tft->setCursor(xStart + 55 + 7, yStart + 49 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("3");

            tft->fillRect(xStart + 82, yStart + 49, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 82, yStart + 49, 18, 14, BLUE);

            tft->setCursor(xStart + 82 + 7, yStart + 49 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("4");


        break;
        case 4:                                                 //FM-AM

            tft->drawLine(xStart + 24, yStart + 7, xStart + 52, yStart + 7, BLACK);
            tft->drawLine(xStart + 24, yStart + 8, xStart + 52, yStart + 8, BLACK);
            tft->drawLine(xStart + 24, yStart + 9, xStart + 24, yStart + 20, BLACK);
            tft->drawLine(xStart + 25, yStart + 9, xStart + 25, yStart + 20, BLACK);
            tft->drawLine(xStart + 26, yStart + 19, xStart + 104, yStart + 19, BLACK);
            tft->drawLine(xStart + 26, yStart + 20, xStart + 104, yStart + 20, BLACK);
            tft->drawLine(xStart + 51, yStart + 9, xStart + 51, yStart + 18, BLACK);
            tft->drawLine(xStart + 52, yStart + 9, xStart + 52, yStart + 18, BLACK);
            tft->drawLine(xStart + 103, yStart + 21, xStart + 103, yStart + 53, BLACK);
            tft->drawLine(xStart + 104, yStart + 21, xStart + 104, yStart + 53, BLACK);
            tft->drawLine(xStart + 47, yStart + 52, xStart + 102, yStart + 52, BLACK);
            tft->drawLine(xStart + 47, yStart + 53, xStart + 102, yStart + 53, BLACK);
            tft->drawLine(xStart + 105, yStart + 37, xStart + 118, yStart + 37, BLACK);
            tft->drawLine(xStart + 105, yStart + 38, xStart + 118, yStart + 38, BLACK);
            tft->drawLine(xStart + 118, yStart + 37, xStart + 116, yStart + 35, BLACK);
            tft->drawLine(xStart + 117, yStart + 37, xStart + 115, yStart + 35, BLACK);
            tft->drawLine(xStart + 118, yStart + 38, xStart + 116, yStart + 40, BLACK);
            tft->drawLine(xStart + 117, yStart + 38, xStart + 115, yStart + 40, BLACK);

            tft->fillRect(xStart + 30, yStart + 13, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 30, yStart + 13, 18, 14, BLUE);

            tft->setCursor(xStart + 30 + 7, yStart + 13 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("1");

            tft->fillRect(xStart + 74, yStart + 13, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 74, yStart + 13, 18, 14, BLUE);

            tft->setCursor(xStart + 74 + 7, yStart + 13 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("2");

            tft->fillRect(xStart + 30, yStart + 46, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 30, yStart + 46, 18, 14, BLUE);

            tft->setCursor(xStart + 30 + 7, yStart + 46 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("3");

            tft->fillRect(xStart + 74, yStart + 46, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 74, yStart + 46, 18, 14, BLUE);

            tft->setCursor(xStart + 74 + 7, yStart + 46 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("4");

        break;
        case 5:                                                 //AM-AM 

            tft->drawLine(xStart + 22, yStart + 5, xStart + 50, yStart + 5, BLACK);
            tft->drawLine(xStart + 22, yStart + 6, xStart + 50, yStart + 6, BLACK);
            tft->drawLine(xStart + 22, yStart + 7, xStart + 22, yStart + 18, BLACK);
            tft->drawLine(xStart + 23, yStart + 7, xStart + 23, yStart + 18, BLACK);
            tft->drawLine(xStart + 24, yStart + 17, xStart + 105, yStart + 17, BLACK);
            tft->drawLine(xStart + 24, yStart + 18, xStart + 105, yStart + 18, BLACK);
            tft->drawLine(xStart + 49, yStart + 7, xStart + 49, yStart + 16, BLACK);
            tft->drawLine(xStart + 50, yStart + 7, xStart + 50, yStart + 16, BLACK);
            tft->drawLine(xStart + 104, yStart + 19, xStart + 104, yStart + 57, BLACK);
            tft->drawLine(xStart + 105, yStart + 19, xStart + 105, yStart + 57, BLACK);
            tft->drawLine(xStart + 103, yStart + 56, xStart + 45, yStart + 56, BLACK);
            tft->drawLine(xStart + 103, yStart + 57, xStart + 45, yStart + 57, BLACK);
            tft->drawLine(xStart + 45, yStart + 37, xStart + 118, yStart + 37, BLACK);
            tft->drawLine(xStart + 45, yStart + 38, xStart + 118, yStart + 38, BLACK);
            tft->drawLine(xStart + 118, yStart + 37, xStart + 116, yStart + 35, BLACK);
            tft->drawLine(xStart + 117, yStart + 37, xStart + 115, yStart + 35, BLACK);
            tft->drawLine(xStart + 118, yStart + 38, xStart + 116, yStart + 40, BLACK);
            tft->drawLine(xStart + 117, yStart + 38, xStart + 115, yStart + 40, BLACK);

            tft->fillRect(xStart + 28, yStart + 11, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 28, yStart + 11, 18, 14, BLUE);

            tft->setCursor(xStart + 28 + 7, yStart + 11 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("1");

            tft->fillRect(xStart + 28, yStart + 31, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 28, yStart + 31, 18, 14, BLUE);

            tft->setCursor(xStart + 28 + 7, yStart + 31 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("2");

            tft->fillRect(xStart + 66, yStart + 31, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 66, yStart + 31, 18, 14, BLUE);

            tft->setCursor(xStart + 66 + 7, yStart + 31 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("3");

            tft->fillRect(xStart + 28, yStart + 50, 18, 14, LIGHT_BLUE);
            tft->drawRect(xStart + 28, yStart + 50, 18, 14, BLUE);

            tft->setCursor(xStart + 28 + 7, yStart + 50 + 4);
            tft->setTextColor(BLACK, LIGHT_BLUE);
            tft->print("4");

        break;        
    }

}

#endif
