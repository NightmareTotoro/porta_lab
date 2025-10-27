#include "HW.h"

Servo servo;

// Constantes para o servo de rotação contínua
const int SERVO_PARADO = 90;
const int SERVO_VELOCIDADE_ABRIR = 180; // Gira em um sentido
const int SERVO_VELOCIDADE_FECHAR = 0;  // Gira no outro sentido

void HW::setup() {
    servo.attach(SERVO_PIN);
    HW::travar_porta(); // Garante que a porta comece travada
}

void HW::travar_porta() {
    servo.attach(SERVO_PIN);
    servo.write(SERVO_VELOCIDADE_FECHAR);
    delay(1500); // Tempo para garantir que a trava engatou (ajuste se necessário)
    servo.write(SERVO_PARADO);
    servo.detach();
}

void HW::destravar_porta() {
    servo.attach(SERVO_PIN);
    servo.write(SERVO_VELOCIDADE_ABRIR);
}

void HW::parar_servo() {
    servo.write(SERVO_PARADO);
    servo.detach();
}

/**
 * @brief Função que controla o alerta.
 */
void HW::alerta(uint8_t ledPin, uint8_t buzzerPin) {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, NOTE_F5);
  delay(500);
  tone(buzzerPin, NOTE_E5);
  delay(500);
  noTone(buzzerPin);
  delay(2000);
  digitalWrite(ledPin, LOW);
}
void HW::alerta_abertura(uint8_t ledPin, uint8_t buzzerPin) {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, NOTE_F5);
  delay(300);
  tone(buzzerPin, NOTE_E5);
  delay(500);
  tone(buzzerPin, NOTE_G5);
  delay(800);
  noTone(buzzerPin);
  delay(900);
  digitalWrite(ledPin, LOW);
}