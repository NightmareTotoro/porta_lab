#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <config.h>

String STASSID = "";
String STAPSK = "";
String STASIP = "";

// Função para ler variáveis do arquivo esp.env
bool carregarEnv(const char* path) {
  File file = LittleFS.open(path, "r");
  if (!file) return false;
  while (file.available()) {
    String linha = file.readStringUntil('\n');
    linha.trim();
    if (linha.length() == 0 || linha.startsWith("#")) continue;
    int idx = linha.indexOf('=');
    if (idx == -1) continue;
    String key = linha.substring(0, idx);
    String value = linha.substring(idx + 1);
    key.trim(); value.trim();
    if (key == "STASSID") STASSID = value;
    else if (key == "STAPSK") STAPSK = value;
    else if (key == "STASIP") STASIP = value;
  }
  file.close();
  return true;
}
