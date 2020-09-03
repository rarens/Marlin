/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#if defined(__IMXRT1062__)
#include <Arduino.h>
#include "HAL.h"
#include <SPI.h>
#include <pins_arduino.h>
#include "../../core/macros.h"
#include "spi_pins.h"
#include "Sd2PinMap.h"
#include "SdInfo.h"

static SPISettings settings;



 
 #define USE_SPI_LIB
 void spiBegin(){
    spiInit(6);
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
 }
 void spiInit(uint8_t spiRate) {
  switch (spiRate) {

    // the top 2 speeds are set to 24 MHz, for the SD library defaults
    case 0:  settings = SPISettings(25200000, MSBFIRST, SPI_MODE0); break;

    case 1:  settings = SPISettings(24000000, MSBFIRST, SPI_MODE0); break;
    case 2:  settings = SPISettings(8000000, MSBFIRST, SPI_MODE0); break;
    case 3:  settings = SPISettings(4000000, MSBFIRST, SPI_MODE0); break;
    case 4:  settings = SPISettings(3000000, MSBFIRST, SPI_MODE0); break;
    case 5:  settings = SPISettings(2000000, MSBFIRST, SPI_MODE0); break;
    default: settings = SPISettings(400000, MSBFIRST, SPI_MODE0);
  }
  SPI.begin();
} 

  void spiSend(uint8_t b) {
	SPI.transfer(b);
 }
 
   uint8_t spiRec(void) {
	return SPI.transfer(0xff);
 }
  
  void spiRecIgnore(size_t len) {
	for (size_t i=0; i < len; i++) 
		SPI.transfer(0xff);
 }
 
//------------------------------------------------------------------------------



void spiSendBlock(uint8_t token, const uint8_t* buf) {
  SPI.beginTransaction(settings);
  SPDR = token;
  for (uint16_t i = 0; i < 512; i += 2) {
    while (!TEST(SPSR, SPIF)) { /* nada */ };
    SPDR = buf[i];
    while (!TEST(SPSR, SPIF)) { /* nada */ };
    SPDR = buf[i + 1];
  }
  while (!TEST(SPSR, SPIF)) { /* nada */ };
  SPI.endTransaction();
}

// Begin SPI transaction, set clock, bit order, data mode
void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode) {
  settings = SPISettings(spiClock, bitOrder, dataMode);
  SPI.beginTransaction(settings);
}
void spiRead(uint8_t* buf, uint16_t nbyte) {
  SPI.beginTransaction(settings);
  SPI.transfer(buf, nbyte);
  SPI.endTransaction();
}
#endif