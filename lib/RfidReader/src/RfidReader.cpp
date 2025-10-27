#include "RfidReader.h"
#include "Logger.h" // Adicionado para usar o sistema de log

// Definição da instância global do RFID
MFRC522 rfid(SS_PIN, RST_PIN);

RfidReader::RfidReader(uint8_t ssPin, uint8_t rstPin) : _mfrc(ssPin, rstPin) {}

void RfidReader::begin() {
  SPI.begin();
  _mfrc.PCD_Init();
}

bool RfidReader::readTag(String& outTag) {
  if (!_mfrc.PICC_IsNewCardPresent() || !_mfrc.PICC_ReadCardSerial()) return false;
  
  outTag = "";
  for (byte i = 0; i < _mfrc.uid.size; i++) {
    if (_mfrc.uid.uidByte[i] < 0x10) outTag += "0";
    outTag += String(_mfrc.uid.uidByte[i], HEX);
  }
  outTag.toUpperCase();

  // --- LOG ADICIONADO AQUI, COMO VOCÊ PEDIU ---
  Logger::registrarMensagem("Tag lida: " + outTag);

  _mfrc.PICC_HaltA();
  _mfrc.PCD_StopCrypto1();
  return true;
}