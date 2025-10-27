#pragma once

#ifndef STORAGEFS_H
#define STORAGEFS_H

#include <Arduino.h>
#include <LittleFS.h>
#include "AccessControl.h"

#define FS_SYSTEM LittleFS

/**
 * @brief Definições do projeto dependendo da placa conectada.
 */
#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
#include <ESPmDNS.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>
#include <Updater.h>
#include <ESP8266mDNS.h>
#ifndef UPDATE_SIZE_UNKNOWN
#define UPDATE_SIZE_UNKNOWN 0xFFFFFFFF
#endif
#endif

/**
 * @brief Classe que expõe para o arquivo `main.cpp` esta biblioteca.
 */
class StorageFS
{
public:
  static bool begin();
  static bool carregarCSV(const char *nomeArquivo = "/acesso.csv");
};

#endif // STORAGEFS_H