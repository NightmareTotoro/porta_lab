#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

/**
 * @brief Tamanho do buffer de log para registrar eventos e mensagens do sistema.
 */
#define TAM_LOG 20

/**
 * @brief Buffer de log para registrar eventos e mensagens do sistema.
 */
extern String bufferLog[TAM_LOG];

/**
 * @brief Indice de log do registro de eventos.
 */
extern int indiceLog;

/**
 * @brief Classe que expõe para o arquivo `main.cpp` esta biblioteca.
 */
class Logger
{
public:
  static void registrarMensagem(const String &msg);
  static void registrarLog(const String &uid, bool acessoPermitido);
  static void registrarLogFirmware(const String &status);
};

#endif // LOGGER_H