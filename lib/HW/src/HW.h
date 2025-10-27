#pragma once

#ifndef HW_H
#define HW_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "config.h"
#include "pitches.h"

extern Servo servo;

/**
 * @brief Classe que expõe para o arquivo `main.cpp` esta biblioteca.
 */
class HW
{
public:
  static void setup();
  static void travar_porta();
  static void destravar_porta();
  static void parar_servo();
  static void alerta(uint8_t ledPin, uint8_t buzzerPin);
  static void alerta_abertura(uint8_t ledPin, uint8_t buzzerPin);
};

#endif // HW_H