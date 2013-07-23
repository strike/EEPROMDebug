/*
  EEPROMDebug.h - EEPROM Debug library
  Copyright (c) 2013 LarkIT.  All right reserved.
  Author: Zubarev Alexander aka Strike <actionstrike@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef EEPROMDEBUG_h
#define EEPROMDEBUG_h

#include <Arduino.h>

class EEPROMDebug
{
  public:
    EEPROMDebug(uint8_t startPos_, uint8_t maxPos_, uint8_t endMarker_);

    inline uint8_t getStartPos(){ return startPos; };
    inline uint8_t getMaxPos(){ return maxPos; };
    inline uint8_t getEndMarker(){ return endMarker; };
    inline uint8_t getPos(){ return pos; }

    uint8_t getNumElements();

    void Write(uint8_t i);

    int Read(uint8_t i);
    int ReadLast();
    int ReadPrev(uint8_t i);
    int ReadPrevAfterLastRead();

    void PrintAll();

  private:
    uint8_t startPos, maxPos, pos;
    uint8_t endMarker;
    uint8_t lastRead;

    void searchEndMarker();
    int getPrevPos(uint8_t i);
};


#endif

