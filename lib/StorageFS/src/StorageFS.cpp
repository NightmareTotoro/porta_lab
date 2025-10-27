#include "StorageFS.h"

/**
 * REFACTOR: NOTE: TODO: Alterações feitas nesse arquivo:
 * # Atualizada a função carregarCSV para utilizar a estrutura DataHora com componentes separados.
 * # Melhorada a legibilidade do código com comentários explicativos.
 */

/**
 * @brief Inicia o sistema de arquivos.
 * 
 * @return true 
 * @return false 
 */
bool StorageFS::begin() {
  return LittleFS.begin();
}

/**
 * @brief Função que carrega um arquivo CSV.
 *
 * @param nomeArquivo O nome do arquivo.
 * @return true
 * @return false
 */
bool StorageFS::carregarCSV(const char *nomeArquivo)
{
  File file = LittleFS.open(nomeArquivo, "r");
  if (!file)
    return false;

  bool primeiraLinha = true;
  qtd_lidas = 0;

  while (file.available() && qtd_lidas < QTD_CRENDENCIAIS)
  {
    String linha = file.readStringUntil('\n');
    linha.trim();
    if (linha.length() == 0)
      continue;
    if (primeiraLinha)
    {
      primeiraLinha = false;
      continue;
    }

    // 172.27.16.75;FabLab;2025-01-01 11:50;2025-01-01 15:30;8646D2B0
    int idx1 = linha.indexOf(';'),
      idx2 = linha.indexOf(';', idx1 + 1),
      idx3 = linha.indexOf(';', idx2 + 1),
      idx4 = linha.indexOf(';', idx3 + 1);
    if (idx1 == -1 || idx2 == -1 || idx3 == -1 || idx4 == -1)
      continue;

    // ipid
    credenciais[qtd_lidas].ipid = linha.substring(0, idx1);
    // sala
    credenciais[qtd_lidas].sala = linha.substring(idx1 + 1, idx2);
    // horaIni
    credenciais[qtd_lidas].horaIni.ano = linha.substring(idx2 + 1, idx2 + 5).c_str();
    credenciais[qtd_lidas].horaIni.mes = linha.substring(idx2 + 6, idx2 + 8).c_str();
    credenciais[qtd_lidas].horaIni.dia = linha.substring(idx2 + 9, idx2 + 11).c_str();
    credenciais[qtd_lidas].horaIni.hora = linha.substring(idx2 + 12, idx2 + 14).c_str();
    credenciais[qtd_lidas].horaIni.minuto = linha.substring(idx2 + 15, idx2 + 17).c_str();
    // horaFim
    credenciais[qtd_lidas].horaFim.ano = linha.substring(idx3 + 1, idx3 + 5).c_str();
    credenciais[qtd_lidas].horaFim.mes = linha.substring(idx3 + 6, idx3 + 8).c_str();
    credenciais[qtd_lidas].horaFim.dia = linha.substring(idx3 + 9, idx3 + 11).c_str();
    credenciais[qtd_lidas].horaFim.hora = linha.substring(idx3 + 12, idx3 + 14).c_str();
    credenciais[qtd_lidas].horaFim.minuto = linha.substring(idx3 + 15, idx3 + 17).c_str();
    // credencial
    credenciais[qtd_lidas].credencial = linha.substring(idx4 + 1);
    credenciais[qtd_lidas].credencial.toUpperCase();

    qtd_lidas++;
  }

  file.close();
  return true;
}