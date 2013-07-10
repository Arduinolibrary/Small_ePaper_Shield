/*-------------------------------------------------------------------------------------------
  demo of ePaper Shield for drawing, such as draw a line, draw a circle...
  
  loovee
  2013-7-10
  
  note: if you use an Arduin UNO, Seeeduino 3.0 (any board that use Atmega 328P or 32U4) 
        you should insert an SD card if you want to use this demo
        if you use a Arduino Mega(any board that use Atmega1280 or Atmega2560), you neen't
        insert an SD card for this demo
-------------------------------------------------------------------------------------------*/ 

#include <ePaper.h>
#include <SPI.h>
#include <SD.h>
#include "GT20L16_drive.h"
#include <Streaming.h>

#define SCREEN_SIZE 270                     // choose screen size here: 144, 200, 270

#if (SCREEN_SIZE == 144)
#define EPD_SIZE    EPD_1_44

#elif (SCREEN_SIZE == 200)
#define EPD_SIZE    EPD_2_0

#elif (SCREEN_SIZE == 270)
#define EPD_SIZE    EPD_2_7

#else
#error "Unknown EPB size: Change the #define SCREEN_SIZE to a supported value"
#endif

void setup()
{

    Serial.begin(38400);
    Serial.println("hello world");
    EPAPER.begin(EPD_SIZE);                             // setup epaper, size 
    EPAPER.setDirection(DIRNORMAL);                     // set display direction
    
    cout << "1" << endl;
    eSD.begin(EPD_SIZE); 
    cout << "2" << endl;
    
    GT20L16.begin();                                    // init GT20L16, a font chip
    
    EPAPER.drawRectangle(10, 10, 100, 80);
    EPAPER.fillCircle(50, 50, 30);
    EPAPER.fillRectangle(50, 65, 50, 20);
    
    EPAPER.drawCircle(150, 50, 25);
    EPAPER.drawCircle(150, 50, 20);
    EPAPER.drawCircle(150, 50, 15);
    EPAPER.drawCircle(150, 50, 10);
    EPAPER.fillCircle(150, 50, 5);
    
    EPAPER.drawHorizontalLine(120, 50, 60);
    EPAPER.drawVerticalLine(150, 20, 60);
    
    cout << "2" << endl;
    EPAPER.display();                                   // display, you can use this function once

}

void loop()
{
    // add code here
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/