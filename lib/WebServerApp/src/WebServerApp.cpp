#include "WebServerApp.h"

#if defined(ESP32)
WebServer server(80);
#elif defined(ESP8266)
ESP8266WebServer server(80);
#endif

/**
 * @brief Inicia o servidor web.
 */
void WebServerApp::begin() {
  if (MDNS.begin("esp")) {
    // ok
  }
  server.begin();
}

/**
 * @brief Função que inicia o servidor web.
 */
void WebServerApp::iniciarServidorWeb()
{
  // Tenta iniciar MDNS
  if (MDNS.begin("esp"))
  {
    Logger::registrarMensagem("MDNS iniciado: http://esp.local");
  }
  else
  {
    Logger::registrarMensagem("MDNS falhou, usando IP direto.");
  }

  // Página principal
  server.on("/", HTTP_GET, handleRoot);

  // Upload de credenciais
  server.on("/upload", HTTP_POST, []()
            { server.send(200); }, handleUpload);

  // Página com IP
  server.on("/ip", HTTP_GET, []()
            {
    String html = "<html><body><h2>Endereço IP do ESP</h2>";
    html += "<p>IP atual: <b>" + WiFi.localIP().toString() + "</b></p>";
    html += "<p>Você pode acessar o sistema por esse IP diretamente.</p>";
    html += "</body></html>";
    server.send(200, "text/html", html); });

  // Monitor serial via WiFi
  server.on("/log_serial", HTTP_GET, []()
            {
    String html = "<html><body><h2>Monitor Serial (via WiFi)</h2><pre>";
    for (int i = 0; i < TAM_LOG; i++) {
      int idx = (indiceLog + i) % TAM_LOG;
      if (bufferLog[idx].length() > 0) {
        html += bufferLog[idx] + "\n";
      }
    }
    html += "</pre></body></html>";
    server.send(200, "text/html", html); });

  // Página de envio de firmware OTA
  server.on("/firmware", HTTP_GET, []()
            { server.send(200, "text/html", "<p>Servico OTA pronto. Use o PlatformIO para enviar o firmware.</p>"); });

  // Download de logs
  server.on("/download_log_acesso", HTTP_GET, []()
            {
    File file = FS_SYSTEM.open("/log_acesso.csv", "r");
    if (!file) {
      server.send(404, "text/plain", "Arquivo de log de acesso nao encontrado.");
      return;
    }
    server.streamFile(file, "text/csv");
    file.close(); });

  server.on("/download_log_firmware", HTTP_GET, []()
            {
    File file = FS_SYSTEM.open("/log_firmware.csv", "r");
    if (!file) {
      server.send(404, "text/plain", "Arquivo de log de firmware nao encontrado.");
      return;
    }
    server.streamFile(file, "text/csv");
    file.close(); });

  // Inicia o servidor
  server.begin();
  Logger::registrarMensagem("Servidor HTTP iniciado em: http://" + WiFi.localIP().toString());
}

/**
 * @brief Função que trata a requisição da página principal.
 */
void WebServerApp::handleRoot()
{

  server.send(200, "text/html", R"rawliteral(
      <h2>Upload de Arquivo CSV</h2>
      <form id="uploadForm">
        <input type="file" name="upload" id="fileInput"><br>
        <input type="submit" value="Enviar CSV">
      </form>
      <progress id="progressBar" value="0" max="100" style="width: 100%; display: none;"></progress>
      <p id="status"></p>
  
      <hr>
      <a href='/firmware'><button>Atualizar Firmware OTA</button></a>
      <hr>
      <a href='/download_log_acesso'><button>Baixar Registro CSV de Acesso</button></a><br><br>
      <a href='/download_log_firmware'><button>Baixar Registro de Firmware</button></a>
  
      <script>
        const form = document.getElementById("uploadForm");
        const progressBar = document.getElementById("progressBar");
        const statusText = document.getElementById("status");
  
        form.addEventListener("submit", function(event) {
          event.preventDefault();
  
          const fileInput = document.getElementById("fileInput");
          if (!fileInput.files.length) {
            alert("Selecione um arquivo primeiro!");
            return;
          }
  
          const file = fileInput.files[0];
          const formData = new FormData();
          formData.append("upload", file);
  
          const xhr = new XMLHttpRequest();
          xhr.open("POST", "/upload", true);
  
          xhr.upload.addEventListener("progress", function(e) {
            if (e.lengthComputable) {
              const percent = (e.loaded / e.total) * 100;
              progressBar.style.display = "block";
              progressBar.value = percent;
              statusText.innerText = `Progresso: ${Math.round(percent)}%`;
            }
          });
  
          xhr.onload = function() {
            if (xhr.status === 200) {
              statusText.innerText = "Upload concluido com sucesso!";
            } else {
              statusText.innerText = "Erro no upload.";
            }
          };
  
          xhr.send(formData);
        });
      </script>
    )rawliteral");
}

/**
 * @brief Função que trata o upload de arquivos.
 */
void WebServerApp::handleUpload()
{
  HTTPUpload &upload = server.upload();
  static File fsUploadFile;

  if (upload.status == UPLOAD_FILE_START)
  {
    Logger::registrarMensagem("Recebendo arquivo: " + upload.filename);
    fsUploadFile = LittleFS.open("/vw_acesso_sala", "w");
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (fsUploadFile)
    {
      fsUploadFile.close();
      Logger::registrarMensagem("Upload finalizado. Recarregando credenciais...");
      if (StorageFS::carregarCSV("/vw_acesso_sala"))
      {
        Logger::registrarMensagem("Novo CSV carregado: " + String(qtd_lidas) + " entradas");
        server.send(200, "text/plain", "Upload finalizado. Credenciais atualizadas.");
      }
      else
      {
        Logger::registrarMensagem("Erro ao carregar novo CSV.");
        server.send(500, "text/plain", "Erro ao carregar novo CSV.");
      }
    }
  }
}