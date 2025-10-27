#pragma once

#ifndef RFIDREADER_H
#define RFIDREADER_H

#include <Arduino.h>
#include <MFRC522.h>
#include "config.h"
#include <SPI.h>

/**
 * @brief Inicializa o módulo RFID.
 *
 * @return MFRC522
 */
extern MFRC522 rfid;

/**
 * @brief Classe que expõe para o arquivo `main.cpp` esta biblioteca.
 */
class RfidReader
{
public:
  RfidReader(uint8_t ssPin, uint8_t rstPin);
  void begin();
  bool readTag(String &outTag);

private:
  MFRC522 _mfrc = rfid;
};

#endif // RFIDREADER_H