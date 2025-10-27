#include "rfid.h"
#include <SPI.h>

RFID::RFID(byte SS_PIN , byte RST_PIN) {
    this->ss_pin = ss_pin;
    this->rst_pin = rst_pin;
    this->mfrc522 = new MFRC522(ss_pin, rst_pin);
}

void RFID::init() {
    SPI.begin();
    mfrc522->PCD_Init();
    Serial.println("Leitor RFID inicializado.");
}

void RFID::read_card() {
    if (mfrc522->PICC_IsNewCardPresent() && mfrc522->PICC_ReadCardSerial()) {
        Serial.print("UID da tag: ");
        String uid = "";
        for (byte i = 0; i < mfrc522->uid.size; i++) {
            uid += (mfrc522->uid.uidByte[i] < 0x10 ? " 0" : " ");
            uid += String(mfrc522->uid.uidByte[i], HEX);
        }
        uid.toUpperCase();
        Serial.println(uid);
        mfrc522->PICC_HaltA();
    }
}