/*
  ePaper.h
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Modified by Loovee
  www.seeedstudio.com
  2013-7-2

  Copyright 2013 Pervasive Displays, Inc.
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at:

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing,
  software distributed under the License is distributed on an
  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
  express or implied.  See the License for the specific language
  governing permissions and limitations under the License.

  This program is to illustrate the display operation as described in
  the datasheets.  The code is in a simple linear fashion and all the
  delays are set to maximum, but the SPI clock is set lower than its
  limit.  Therfore the display sequence will be much slower than
  normal and all of the individual display stages be clearly visible.
*/

#include <SPI.h>
#include <Arduino.h>

#include "GT20L16_drive.h"

/*********************************************************************************************************
** Function name:           begin
** Descriptions:            init IO
*********************************************************************************************************/
void GT20L16_drive::begin(int pinSelect)
{
    pinCS = pinSelect;
    pinMode(pinCS, OUTPUT);
    digitalWrite(pinCS, HIGH);
}

/*********************************************************************************************************
** Function name:           getMatrixUnicode
** Descriptions:            get matrix, unicode
*********************************************************************************************************/
int GT20L16_drive::getMatrixUnicode(unsigned int uniCode, unsigned char *matrix)
{
    unsigned char i;
    unsigned char tempdata;
    unsigned long Add=0;
    
    Add=getAddrFromUnicode(uniCode);
    
    delayMicroseconds(10);
    
    GT_Select();
    SPI.transfer(0x03);
    SPI.transfer(Add>>16);
    SPI.transfer(Add>>8);
    SPI.transfer(Add);
    
    
    SPI.setBitOrder(LSBFIRST);
    for(i=0;i<32;i++)
   {
      
      tempdata=SPI.transfer(0x00);
      matrix[i]=(255-tempdata);   /*save dot matrix data in matrixdata[i]*/
      delay(10);
    }
    SPI.setBitOrder(MSBFIRST);
    GT_UnSelect();   
}

/*********************************************************************************************************
** Function name:           GT_Select
** Descriptions:            chip select
*********************************************************************************************************/
void GT20L16_drive::GT_Select()
{
    digitalWrite(pinCS, LOW);
}

/*********************************************************************************************************
** Function name:           GT_UnSelect
** Descriptions:            chip unselect
*********************************************************************************************************/
void GT20L16_drive::GT_UnSelect()
{
    digitalWrite(pinCS, HIGH);
}

/*********************************************************************************************************
** Function name:           getAddrFromUnicode
** Descriptions:            get .. address 
*********************************************************************************************************/
unsigned long GT20L16_drive::getAddrFromUnicode(unsigned int uniCode)
{

    unsigned long  ZFAdd,HZAdd;
    unsigned char  MSB,LSB;
    unsigned long ChineseTab;
    unsigned int data;
    unsigned long  Add_Chinese;
    MSB = uniCode>>8;
    LSB = uniCode;
    ZFAdd=36224;
    HZAdd=93452;
    ChineseTab=87436;
    
    if(MSB>=0xA1&&MSB<=0xA5)  //char area
    {
      if(MSB==0xA1&&LSB>=0xA1&&LSB<=0xBF)
        Add_Chinese=32*(LSB-0xA1)+ZFAdd;
      else if(MSB==0xA3&&LSB>=0xA1&&LSB<=0xFE)
        Add_Chinese=32*(31+LSB-0xA1)+ZFAdd;
      else if(MSB==0xA4&&LSB>=0xA1&&LSB<=0xF3)
        Add_Chinese=32*(125+LSB-0xA1)+ZFAdd;
      else if(MSB==0xA5&&LSB>=0xA1&&LSB<=0xF6)
        Add_Chinese=32*(208+LSB-0xA1)+ZFAdd;
      else
        Add_Chinese=ZFAdd;
    }
    else if((MSB>=0xB0&&MSB<=0xD7)&&(LSB>=0xA1&&LSB<=0xFE)) //chinese 5720
    
    { 
        Add_Chinese=(MSB-176)*94+(LSB-161)+1;
        Add_Chinese=Add_Chinese*32 +HZAdd;
        
    }
    else if((MSB>=0xD8&&MSB<=0xF7)&&(LSB>=0xA1&&LSB<=0xFE)) //chinese 5720~6763
    {
       Add_Chinese=(MSB-0xD8)*94+(LSB-0xA1);
       Add_Chinese=Add_Chinese*2+ChineseTab;
       data=GTRead(Add_Chinese);
       Add_Chinese=32*data+HZAdd;
    }
    return Add_Chinese;
}


/*********************************************************************************************************
** Function name:           GTRead
** Descriptions:            GTRead
*********************************************************************************************************/
unsigned long GT20L16_drive::GTRead(unsigned long Address)
{    
    unsigned char i;
    unsigned char buffer[2]={0};
    unsigned int data;
    delayMicroseconds(10);
    GT_Select();
    SPI.transfer(0x03);
    SPI.transfer(Address>>16);
    SPI.transfer(Address>>8);
    SPI.transfer(Address);
    for(i=0;i<2;i++)
    {
      buffer[i]=SPI.transfer(0x00);
    }
    GT_UnSelect();
    data=buffer[0]*256+buffer[1];
    delay(1000);
    return data;
}

GT20L16_drive GT20L16;

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/