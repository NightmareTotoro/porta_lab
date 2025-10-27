#include "WiFiService.h"

extern String STASSID ;
extern String STAPSK ;
extern String STASIP;

void WiFiService::connect() {
    if (STASSID.length() == 0 || STASSID == "<id>") {
        Logger::registrarMensagem("WiFi: Credenciais invalidas ou nao configuradas em esp.env.");
        return;
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin("Kami", "Kamisamaback"); 

    Logger::registrarMensagem("Conectando ao WiFi: " + STASSID);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Logger::registrarMensagem("WiFi Conectado!");
        Logger::registrarMensagem("Endereco IP: " + WiFi.localIP().toString());
    } else {
        Serial.println();
        Logger::registrarMensagem("Falha ao conectar no WiFi.");
    }
}