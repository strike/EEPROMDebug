/*
  EEPROMDebug.cpp - EEPROM Debug library
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

int EEPROMDebug::Read(uint8_t i){
  if (startPos + i > maxPos){
    return -1;
  }
  lastRead = i;
  return EEPROM.read(startPos + i);
}

int EEPROMDebug::ReadLast(){
  return Read(pos);
}

int EEPROMDebug::ReadPrevAfterLastRead(){
  return Read( getPrevPos(lastRead) );
}

int EEPROMDebug::ReadPrev(uint8_t i){
  return Read( getPrevPos(i) );
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
  Serial.println("EEPROM:");
  for (int i = 0; i < getNumElements(); i++){
    Serial.print(i);
    Serial.print(":\t");
    Serial.println(Read(i));
  }
}

