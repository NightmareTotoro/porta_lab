#include "AccessControl.h"

// Definição das variáveis globais
Plano credenciais[QTD_CRENDENCIAIS];
int qtd_lidas = 0;
SalaIP salas[13];
int currentState;

/**
 * REFACTOR: NOTE: TODO: Alterações feitas nesse arquivo:
 * # Alterada a estrutura DataHora para armazenar cada componente (ano, mês, dia, hora, minuto) separadamente.
 * # Atualizada a função verificarAcesso para construir struct tm diretamente a partir dos componentes separados.
 * # Adicionado um buffer para mensagens de log, detalhando o período permitido e a hora atual.
 * # Ajustado o cálculo do tempo permitido para incluir uma margem de 30 minutos antes e depois do período definido.
 * # Melhorada a legibilidade do código com comentários explicativos.
 */

/**
 * DEVELOP: NOTE: TODO: O que deve ser feito agora nesse arquivo:
 * # Melhor forma de verificar autorização em determinado período
 * ## O ideal é fazer uma validação de horário sempre que uma credencial for apresentada:
 * 1. Receber os dados de entrada:
 *    - Sala onde a pessoa está tentando entrar
 *    - Credencial lida (código do cartão/tag RFID)
 *    - Data e hora atual
 * 2. Consultar a tabela vw_acesso_sala:
 *    - Procurar linhas onde a Credencial corresponde
 *    - Conferir se a Sala é a correta
 *    - Validar se a data/hora atual está entre HorarioInic e HorarioFim
 * 3. Resultado:
 *    - Se existe ao menos um registro válido → Autorizado
 *    - Caso contrário → Negado
 *
 * TODO: INFO:
 * # Boas práticas adicionais
 *    - Credenciais fixas (liberadas sempre) podem ter HorarioInic = 1900-01-01 e HorarioFim = 2999-12-31 → assim o sistema reconhece como sempre válido.
 *    - Índices no banco sobre as colunas Credencial e Sala aceleram a consulta.
 *    - Se várias salas usam o mesmo cartão em horários diferentes, essa estrutura já dá suporte.
 */

/**
 * @brief Função que verifica se uma tag RFID tem acesso permitido.
 *
 * @param tagLida A tag RFID lida pelo sensor.
 * @return true Se o acesso for permitido.
 * @return false Se o acesso for negado.
 */
bool AccessControl::verificarAcesso(const String &tagLida)
{
  // tratamento da tagLida
  String tag = tagLida;
  tag.toUpperCase();

  // estrutura que pega o tempo atual, se ouver erro será mostrado no log
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Logger::registrarMensagem("Erro ao obter hora local");
    return false;
  }
  time_t agora = mktime(&timeinfo);
    char bufTest[30];
    Logger::registrarMensagem(String(strftime(bufTest, sizeof(bufTest), "%d/%m/%Y %H:%M", localtime(&agora)))+ "dias");
      

  // percorre todas as credenciais lidas do CSV, presentes na estrutura credenciais
  for (int i = 0; i < qtd_lidas; i++)
  {
    if (tagLida == credenciais[i].credencial)
    {
      Logger::registrarMensagem(tagLida + "essa foi a tag lida encontrada "+ credenciais[i].credencial);
      int hora_ini_size = credenciais[i].horaIni.ano.length() + credenciais[i].horaIni.mes.length() + credenciais[i].horaIni.dia.length() + credenciais[i].horaIni.hora.length() + credenciais[i].horaIni.minuto.length() + 4;
      // 4 por causa dos separadores

      int hora_fim_size = credenciais[i].horaFim.ano.length() + credenciais[i].horaFim.mes.length() + credenciais[i].horaFim.dia.length() + credenciais[i].horaFim.hora.length() + credenciais[i].horaFim.minuto.length() + 4;
      // 4 por causa dos separadores

      /**
       * TODO:
       * Salas IA1, IA2, IA3, JA1, JA2/3, JA4, JB1, JB2/4, JB3, JB5, JB6, JB8 e FabLab.
       *
       * ip fablab: 172.27.16.75
       */
      salas[0].sala = "FabLab";
      salas[0].ip = "192.168.24.12";
      salas[1].sala = "IA1";
      salas[1].ip = "192.168.24.12";
      salas[2].sala = "IA2";
      salas[2].ip = "172.27.16.77";
      salas[3].sala = "IA3";
      salas[3].ip = "172.27.16.78";
      salas[4].sala = "JA1";
      salas[4].ip = "172.27.16.79";
      salas[5].sala = "JA2/3";
      salas[5].ip = "172.27.16.80";
      salas[6].sala = "JA4";
      salas[6].ip = "172.27.16.81";
      salas[7].sala = "JB1";
      salas[7].ip = "172.27.16.82";
      salas[8].sala = "JB2/4";
      salas[8].ip = "172.27.16.83";
      salas[9].sala = "JB3";
      salas[9].ip = "172.27.16.84";
      salas[10].sala = "JB5";
      salas[10].ip = "172.27.16.85";
      salas[11].sala = "JB6";
      salas[11].ip = "172.27.16.86";
      salas[12].sala = "JB8";
      salas[12].ip = "172.27.16.87";

      String salaAtual = "";

      // Verifica se a sala é a correta
      for (int j = 0; j < 13; j++)
      {
        Logger::registrarMensagem(credenciais[i].sala + " - " + salas[j].sala + " | " + credenciais[i].ipid + " - " + salas[j].ip);
        if ((credenciais[i].sala == salas[j].sala) && (credenciais[i].ipid == salas[j].ip))
        {
          salaAtual = salas[j].sala; // Sala corresponde, continua a verificação
        Logger::registrarMensagem(salaAtual);
        break;
        }
        else
        {
        Logger::registrarMensagem("nao é esta sala");
        }
      }

      // Pega apenas os primeiros 16 caracteres (yyyy-MM-dd HH:mm)
      if (hora_ini_size < 16 || hora_fim_size < 16)
      {
        Logger::registrarMensagem("Erro: Formato de hora incorreto para a credencial " + String(i));
        continue;
      }
      // INFO: DEVELOP: 100%: código antigo que já não é necessário mais, porque a estrutura que guarda o plano da crendencial já guarda separado
      // String ini = credenciais[i].horaIni.substring(0, 16); // yyyy-MM-dd HH:mm
      // String fim = credenciais[i].horaFim.substring(0, 16);

      // Parse horaIni
      int ano_ini = credenciais[i].horaIni.ano.toInt();
      int mes_ini = credenciais[i].horaIni.mes.toInt();
      int dia_ini = credenciais[i].horaIni.dia.toInt();
      int hora_ini = credenciais[i].horaIni.hora.toInt();
      int min_ini = credenciais[i].horaIni.minuto.toInt();

      // Parse horaFim
      int ano_fim = credenciais[i].horaFim.ano.toInt();
      int mes_fim = credenciais[i].horaFim.mes.toInt();
      int dia_fim = credenciais[i].horaFim.dia.toInt();
      int hora_fim = credenciais[i].horaFim.hora.toInt();
      int min_fim = credenciais[i].horaFim.minuto.toInt();

      struct tm tmInicio = {0}, tmFim = {0};
      tmInicio.tm_year = ano_ini - 1900;
      tmInicio.tm_mon = mes_ini - 1;
      tmInicio.tm_mday = dia_ini;
      tmInicio.tm_hour = hora_ini;
      tmInicio.tm_min = min_ini;
      tmInicio.tm_sec = 0;
      tmInicio.tm_isdst = -1;

      tmFim.tm_year = ano_fim - 1900;
      tmFim.tm_mon = mes_fim - 1;
      tmFim.tm_mday = dia_fim;
      tmFim.tm_hour = hora_fim;
      tmFim.tm_min = min_fim;
      tmFim.tm_sec = 59;
      tmFim.tm_isdst = -1;

      time_t tInicio = mktime(&tmInicio) - 1800; // 30 min antes
      time_t tFim = mktime(&tmFim) + 1800;       // 30 min depois

      char bufInicio[30], bufFim[30], bufAgora[30];
      strftime(bufInicio, sizeof(bufInicio), "%d/%m/%Y %H:%M", localtime(&tInicio));
      strftime(bufFim, sizeof(bufFim), "%d/%m/%Y %H:%M", localtime(&tFim));
      strftime(bufAgora, sizeof(bufAgora), "%d/%m/%Y %H:%M", localtime(&agora));

      String msg = salaAtual + " ▧ " + credenciais[i].credencial + " ▧ Agora: " + String(bufAgora) +
                   " | Permitido: de " + String(bufInicio) +
                   " ate " + String(bufFim);

      Logger::registrarMensagem(msg);

      if (agora >= tInicio && agora <= tFim)
      {
        return true;
      }
    }
  }
  return false;
}

/**
 * @brief Função que controla a abertura da porta.
 */
void AccessControl::abertura(String mensagem)
{
  Logger::registrarMensagem(mensagem);
  tone(BUZZER, NOTE_G5); // Bip de sucesso

  // 1. Gira para destravar
  HW::destravar_porta();
  HW::alerta_abertura(LED_VERDE, BUZZER);// Tempo para girar os "860 graus" (ajuste conforme necessário)
  HW::parar_servo();
  
  noTone(BUZZER);

  // 2. Espera 10 segundos
  Logger::registrarMensagem("Porta aberta. Travando em 5 segundos...");
  delay(5000);

  // 3. Gira de volta para travar
  Logger::registrarMensagem("Travando a porta...");
  HW::travar_porta();
  Logger::registrarMensagem("Porta travada.");
}

void AccessControl::acessoNegado(String mensagem)
{
  Logger::registrarMensagem(mensagem);
  HW::alerta(LED_VERMELHO, BUZZER);
}

/**
 * @brief Função que controla a saída da porta.
 */
void AccessControl::saida()
{
  currentState = digitalRead(BTN_SAIDA);
  if (currentState == LOW)
  {
    abertura("Saída autorizada pelo botão");
  }
}