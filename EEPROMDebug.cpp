/*
  EEPROMDebug.cpp - EEPROM Debug library
  Copyright (c) 2013 LarkIT (www.larkit.ru).  All right reserved.
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

/******************************************************************************
 * Includes
 ******************************************************************************/

#include "EEPROMDebug.h"
#include <EEPROM.h>
#include <Arduino.h>

/******************************************************************************
 * Definitions
 ******************************************************************************/

uint8_t startPos, maxPos, pos;
uint8_t endMarker;
uint8_t lastRead;

/******************************************************************************
 * Constructors
 ******************************************************************************/

EEPROMDebug::EEPROMDebug(uint8_t startPos_, uint8_t maxPos_, uint8_t endMarker_){
  startPos = startPos_;
  maxPos = maxPos_;
  endMarker = endMarker_;

  searchEndMarker();
}

void EEPROMDebug::searchEndMarker(){
  uint8_t c;
  for (int i = 0; i < getNumElements(); i++){
    c = EEPROM.read(startPos + i);
    if (c == endMarker){
      pos = i;
      break;
    }
  }
}

/******************************************************************************
 * User API
 ******************************************************************************/

void EEPROMDebug::Write(uint8_t i){
  EEPROM.write(startPos + pos, i);
  pos++;
  if (pos > getNumElements()){
    pos = 0;
  }
  EEPROM.write(startPos + pos, endMarker);
}

void EEPROMDebug::Write(uint8_t n, char *date){
  for (int i = 0; i < 18; i = i + 3){
    Write(atoi(date+i));
  }
  Write(n);
}

int EEPROMDebug::Read(uint8_t i){
  if (startPos + i > maxPos){
    return -1;
  }
  lastRead = i;
  return EEPROM.read(startPos + i);
}

int EEPROMDebug::Read(uint8_t i, char *date){
  int n;
  int r = Read(i);
  char c[3];
  for (int i = 16; i > 0; i -= 3){
    itoa( n = ReadPrevAfterLastRead(), c, 10);
    if (n < 10){
      date[i-1] = '0';
      date[i] = c[0];
    } else {
      date[i-1] = c[0];
      date[i] = c[1];
    }
    if (i > 9){
      date[i+1] = ':';
    } else {
      date[i+1] = '/';
    }
  }

  date[8] = ',';
  date[17] = 0;

  return r;
}

int EEPROMDebug::ReadLast(){
  return Read(getPrevPos(pos));
}

int EEPROMDebug::ReadLast(char *date){
  return Read(getPrevPos(pos), date);
}

int EEPROMDebug::ReadPrevAfterLastRead(){
  return Read( getPrevPos(lastRead) );
}

int EEPROMDebug::ReadPrevAfterLastRead(char *date){
  return Read( getPrevPos(lastRead), date);
}

int EEPROMDebug::ReadPrev(uint8_t i){
  return Read( getPrevPos(i) );
}

int EEPROMDebug::ReadPrev(uint8_t n, char *date){
  return Read( getPrevPos(n), date);
}

int EEPROMDebug::getPrevPos(uint8_t i){
  if (i >= getNumElements()){
    return -1;
  }

  i--;
  if (i == 255) {
    i = getNumElements()-1;
  }
  return i;
}

uint8_t EEPROMDebug::getNumElements(){
  return (maxPos - startPos + 1);
}

void EEPROMDebug::PrintAll(){
  Serial.println(F("EEPROM:"));
  for (int i = 0; i < getNumElements(); i++){
    Serial.print(i);
    Serial.print(F(":\t"));
    Serial.println(Read(i));
  }
}

