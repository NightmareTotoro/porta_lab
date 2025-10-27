#pragma once

#ifndef WEBSERVERAPP_H
#define WEBSERVERAPP_H

#include <Arduino.h>
#include <Logger.h>
#include <StorageFS.h>
#include <AccessControl.h>

/**
 * @brief Definições de servidor web dependendo da placa conectada.
 */
#if defined(ESP32)
#include <WebServer.h>
#include <ESPmDNS.h>
extern WebServer server;
#elif defined(ESP8266)
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
extern ESP8266WebServer server;
#endif

/**
 * @brief Classe que expõe para o arquivo `main.cpp` esta biblioteca.
 */
class WebServerApp
{
public:
  static void begin();
  static void iniciarServidorWeb();
  static void handleRoot();
  static void handleUpload();
};

#endif // WEBSERVERAPP_H