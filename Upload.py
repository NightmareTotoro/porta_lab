import pyodbc
import pandas as pd
import requests
from datetime import datetime
import schedule
import time
import os

# CONFIGURAÇÕES
SERVIDOR = "octans"
BANCO = "APP_INFRA"
USUARIO = "UsrControleAcesso"
SENHA = "Usr@acesso"
VIEW_SQL = "SELECT * FROM vw_acesso_sala"
CAMINHO_CSV = "data/vw_acesso_sala"
URL_ESP32 = "http://172.27.16.75/upload"  # IP do ESP32


# Conecta e exporta a view para CSV
def exportar_csv():
  try:
    print(f"[{datetime.now()}] Conectando ao SQL Server...")
    conn = pyodbc.connect(
      f"DRIVER={{SQL Server}};SERVER={SERVIDOR};DATABASE={BANCO};UID={USUARIO};PWD={SENHA}"
    )
    df = pd.read_sql(VIEW_SQL, conn)
    df.to_csv(CAMINHO_CSV, index=False, sep=";")
    conn.close()
    print(f"[{datetime.now()}] CSV gerado com sucesso: {CAMINHO_CSV}")
    return True
  except Exception as e:
    print(f"[ERRO] Falha ao exportar CSV: {e}")
    return False


# Envia o CSV para o ESP32
def enviar_para_esp32():
  try:
    with open(CAMINHO_CSV, "rb") as f:
      print(f"[{datetime.now()}] Enviando CSV para ESP32...")
      response = requests.post(URL_ESP32, files={"upload": f})
      if response.status_code == 200:
        print(f"[{datetime.now()}] Arquivo enviado com sucesso!")
      else:
        print(
          f"[ERRO] Falha ao enviar arquivo: {response.status_code} - {response.text}"
        )
  except Exception as e:
    print(f"[ERRO] Erro no envio: {e}")


if __name__ == "__main__":
  exportar_csv()
  enviar_para_esp32()
