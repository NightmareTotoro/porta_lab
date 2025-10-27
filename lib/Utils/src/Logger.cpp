#include "Logger.h"
#include <FS.h> // Add this include for File class support
#include <LittleFS.h> // Or #include <LittleFS.h> or <SD.h> depending on your storage

// Definição das variáveis globais
String bufferLog[TAM_LOG];
int indiceLog = 0;

// Define FS_SYSTEM as the filesystem you are using
#define FS_SYSTEM LittleFS // Or LittleFS, SD, etc.

/**
 * @brief Função que registra mensagens no log.
 * 
 * @param msg A mensagem a ser registrada.
 */
void Logger::registrarMensagem(const String& msg) {
  Serial.println(msg); // <-- ADICIONADO: Imprime a mensagem no Monitor Serial
  bufferLog[indiceLog] = msg;
  indiceLog = (indiceLog + 1) % TAM_LOG;
}

/**
 * @brief Função que registra o log de acesso.
 * @details Modificar para que cada dia x ele apague da memoria.
 *
 * @param uid A UID do usuário.
 * @param acessoPermitido Indica se o acesso foi permitido.
 */
void Logger::registrarLog(const String &uid, bool acessoPermitido)
{
  File logFile;
  logFile = FS_SYSTEM.open("/log_acesso.csv", "a");
  if (!logFile)
  {
    Logger::registrarMensagem("Erro ao abrir log de acesso.");
    return;
  }

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Logger::registrarMensagem("Erro ao obter hora para log.");
    logFile.close();
    return;
  }

  char timestamp[25];
  strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M:%S", &timeinfo);
  String status = acessoPermitido ? "LIBERADO" : "NEGADO";
  logFile.printf("%s,%s,%s\n", timestamp, uid.c_str(), status.c_str());
  logFile.close();

  Logger::registrarMensagem("Log registrado.");
}

/**
 * @brief Função que registra o log de firmware.
 *
 * @param status O status da atualização do firmware.
 */
void Logger::registrarLogFirmware(const String &status)
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Logger::registrarMensagem("Erro ao obter hora para log de firmware.");
    return;
  }

  File logFile;
  logFile = FS_SYSTEM.open("/log_firmware.csv", "a");
  if (!logFile)
  {
    Logger::registrarMensagem("Erro ao abrir log de firmware.");
    return;
  }

  char timestamp[25];
  strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M:%S", &timeinfo);

  logFile.printf("%s,%s\n", timestamp, status.c_str());
  logFile.close();

  Logger::registrarMensagem("Log de firmware registrado: " + String(status.c_str()));
}