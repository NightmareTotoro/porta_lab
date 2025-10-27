
#ifndef ACCESSCONTROL_H
#define ACCESSCONTROL_H

#include <Arduino.h>
#include "config.h"
#include "pitches.h"
#include <vector>
#include <WString.h>
#include <Logger.h>
#include <HW.h>

/**
 * @brief Define a quantidade máxima de crendenciais aceita dentro da placa ESP32 ou ESP8266.
 */
#define QTD_CRENDENCIAIS 30

/**
 * @brief Estrutura que representa uma sala e seu respectivo endereço Ip.
 */
struct SalaIP {
  String sala;
  String ip;
};
extern SalaIP salas[13]; // Defina aqui o identificador da sala atual

/**
 * @brief Estrutura que representa uma data e hora. 
 * 
 */
struct DataHora
{
  String ano;
  String mes;
  String dia;
  String hora;
  String minuto;
};

/**
 * @brief Estrutura que representa um plano de acesso.
 */
struct Plano
{
  String ipid;
  String sala;
  DataHora horaIni; // Formato: yyyy-MM-dd HH:mm
  DataHora horaFim; // Formato: yyyy-MM-dd HH:mm
  String credencial;
};

/**
 * @brief Estado atual do sistema, utilizado para controle de fluxo e lógica do programa.
 */
extern int currentState;

/**
 * @brief Array que armazena as credenciais de acesso.
 */
extern Plano credenciais[QTD_CRENDENCIAIS];

/**
 * @brief Contador de credenciais lidas a partir do arquivo CSV.
 */
extern int qtd_lidas;

/**
 * @brief Classe que expõe para o arquivo `main.cpp` esta biblioteca.
 */
class AccessControl
{
public:
  static bool verificarAcesso(const String &tagLida);
  static void abertura(String mensagem);
  static void acessoNegado(String mensagem);
  static void saida();
};

#endif // ACCESSCONTROL_H