#pragma once

#ifndef OTASERVICE_H
#define OTASERVICE_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <Logger.h>

/**
 * @brief Classe que expõe para o arquivo `main.cpp` esta biblioteca.
 */
class OTAService
{
public:
  static void begin();
  static void iniciarOTA();
};

#endif // OTASERVICE_H