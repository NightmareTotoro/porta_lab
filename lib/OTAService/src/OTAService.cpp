#include "OTAService.h"

void OTAService::begin() {
  // TODO: mover callbacks e begin() do main.cpp
  ArduinoOTA.begin();
}

/**
 * @brief Função que inicia o processo de atualização OTA.
 */
void OTAService::iniciarOTA() {
  ArduinoOTA.setHostname("esp-rfid");
  ArduinoOTA.setPassword("123456");

  ArduinoOTA.onStart([]() {
    Logger::registrarMensagem("Iniciando OTA...");
  });

  ArduinoOTA.onEnd([]() {
    Logger::registrarMensagem("OTA concluída.");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    int porcentagem = (progress * 100) / total;
    Logger::registrarMensagem("Progresso: " + String(porcentagem) + "%");
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Logger::registrarMensagem("Erro OTA [" + String(error) + "]");
    if (error == OTA_AUTH_ERROR) Logger::registrarMensagem("Erro de autenticação");
    else if (error == OTA_BEGIN_ERROR) Logger::registrarMensagem("Erro no início");
    else if (error == OTA_CONNECT_ERROR) Logger::registrarMensagem("Erro de conexão");
    else if (error == OTA_RECEIVE_ERROR) Logger::registrarMensagem("Erro de recebimento");
    else if (error == OTA_END_ERROR) Logger::registrarMensagem("Erro ao finalizar");
  });

  ArduinoOTA.begin();
  Logger::registrarMensagem("OTA pronta.");
}