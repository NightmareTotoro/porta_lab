#include <Arduino.h>
#include <LittleFS.h>
#include "config.h"
#include "RfidReader.h"
#include "StorageFS.h"
#include "AccessControl.h"
#include "HW.h"
#include "Logger.h"
#include "WiFiService.h"
#include "WebServerApp.h"
#include "OTAService.h"

// --- Constantes para o NTP (Network Time Protocol) ---
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -3 * 3600; // GMT-3 (Fuso horário de Brasília)
const int   daylightOffset_sec = 0;    // Horário de verão (desativado no Brasil)

// --- Variáveis Globais para WiFi ---
String STASSID = "";
String STAPSK = "";
String STASIP = "";

RfidReader rfidReader(SS_PIN, RST_PIN);

// --- Função para carregar credenciais WiFi do LittleFS ---
void carregarCredenciaisWiFi() {
  File file = LittleFS.open("/esp.env", "r");
  if (!file) {
    Logger::registrarMensagem("Arquivo esp.env nao encontrado.");
    return;
  }
  while (file.available()) {
    String linha = file.readStringUntil('\n');
    linha.trim();
    if (linha.startsWith("STASSID=")) STASSID = linha.substring(8);
    if (linha.startsWith("STAPSK=")) STAPSK = linha.substring(7);
    if (linha.startsWith("STASIP=")) STASIP = linha.substring(7);
  }
  file.close();
  Logger::registrarMensagem("Credenciais WiFi carregadas do esp.env.");
}


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- Sistema de Controle de Acesso ---");

  // 1. INICIALIZAÇÃO CRÍTICA DO HARDWARE
  Logger::registrarMensagem("Inicializando hardware...");
  HW::setup(); // Isso já anexa o servo e TRAVA a porta.
  pinMode(BTN_SAIDA, INPUT_PULLUP);
  Logger::registrarMensagem("Hardware OK. Porta travada.");

  // 2. INICIALIZAÇÃO DO SISTEMA DE ARQUIVOS
  if (!StorageFS::begin()) {
    Logger::registrarMensagem("ERRO CRITICO: Falha ao montar o sistema de arquivos!");
    while(true) delay(1000); // Trava o sistema se o FS falhar
  }
  Logger::registrarMensagem("Sistema de arquivos OK.");

  // 3. CARREGAR CREDENCIAIS RFID
  if (StorageFS::carregarCSV("/vw_acesso_sala")) {
    Logger::registrarMensagem("Credenciais RFID carregadas: " + String(qtd_lidas) + " registros.");
  } else {
    Logger::registrarMensagem("AVISO: Nao foi possivel carregar o arquivo de credenciais RFID.");
  }

  // 4. INICIALIZAR LEITOR RFID
  rfidReader.begin();
  Logger::registrarMensagem("Leitor RFID OK.");

  // 5. TENTAR CONECTAR AO WIFI (Não bloqueante)
  carregarCredenciaisWiFi();
  WiFiService::connect();
  if (WiFi.status() == WL_CONNECTED) {
    // Configura o NTP para obter a hora da internet
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    Logger::registrarMensagem("Cliente NTP configurado.");
    
    WebServerApp::iniciarServidorWeb();
    OTAService::iniciarOTA(); // <-- ADICIONADO AQUI
  }

  Logger::registrarMensagem("--- Sistema Pronto ---");
}

void loop() {
  // Manter o servidor web e o OTA funcionando
  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
    ArduinoOTA.handle(); // <-- ADICIONADO AQUI
  }

  // Verificar leitura de tag RFID
  String tagLida;
  if (rfidReader.readTag(tagLida)) {
    Logger::registrarMensagem("Credenciais RFID carregadas: " + tagLida + " registros.");
    if (AccessControl::verificarAcesso(tagLida)) {
        AccessControl::abertura("Acesso Liberado para tag " + tagLida);
    } else {
      AccessControl::acessoNegado("Acesso Negado para tag " + tagLida);
    }
  }

  // Verificar botão de saída
  AccessControl::saida();
  
  delay(50); // Pequeno delay para estabilidade
}