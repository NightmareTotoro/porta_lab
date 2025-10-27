#pragma once
#include <Arduino.h>
// Configurações globais do projeto.

#ifndef CONFIG_H
#define CONFIG_H

// --- Pinos para RFID-RC522 ---
// A biblioteca MFRC522 usa o barramento SPI padrão do ESP32 (VSPI).
// Certifique-se de que sua fiação corresponda a estes pinos:
// MISO -> GPIO 19
// MOSI -> GPIO 23
// SCK  -> GPIO 18
#define SS_PIN       5   // SDA / CS pin
#define RST_PIN      22  // RST pin

// --- Pinos de Hardware ---
#define RELAY_PIN    23
#define BUZZER       15
#define LED_VERMELHO 4
#define LED_VERDE    16 
#define BTN_SAIDA    12
#define SERVO_PIN    13

#endif // CONFIG_H