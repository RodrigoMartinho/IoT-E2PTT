#include <Adafruit_NeoPixel.h>
#include "common.h"
#include "display.h"
#include "mic_sensor.h"
#include "communication.h"

// --- Pinos ---
#define LED_RGB 48
#define LED_PIN 17
#define NUM_LEDS 1
#define BUTTON_PIN 7
#define BUTTONMIC_PIN 37

int botaoUltimoEstado = HIGH;
int botaoMicUltimoEstado = HIGH;

bool luzLigada = false;
bool micLigado = false;
bool wifiOk = false;
float valorDbAtual = 0.0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_RGB, NEO_GRB + NEO_KHZ800);

Temporizador timerMic(1000);       //Captura do microfone - 1 segundo 
Temporizador timerWifi(10000);     //Status do WiFi       - 10 segundos 
Temporizador timerDisplay(500);    //Tela                 - 1/2 segundo
Temporizador timerMQTT(1000);      //MQTT                 - 1 segundo

void acenderLuz(){
  strip.setPixelColor(0, strip.Color(255, 0, 0)); // Vermelho
  strip.show();

  digitalWrite(LED_PIN, LOW);

  luzLigada = true;
}

void apagarLuz(){
  strip.setPixelColor(0, strip.Color(0, 0, 0)); 
  strip.show();

  digitalWrite(LED_PIN, HIGH);

  luzLigada = false;
}

void statusBotao() {
  int botaoEstadoAtual = digitalRead(BUTTON_PIN);

  // Lógica de Detecção de Borda (Edge Detection)
  // Só entra no IF se o botão mudou de SOLTO para PRESSIONADO
  if (botaoUltimoEstado == LOW && botaoEstadoAtual == HIGH) {
    
    luzLigada = !luzLigada; 

    if (luzLigada) {
      acenderLuz();
      client.publish(topic_luz_state, "ON", true); 
    } else {
      apagarLuz();
      client.publish(topic_luz_state, "OFF", true); 
    }

    desenharTela(valorDbAtual, luzLigada, wifiOk, micLigado); 
  
    Serial.println("Luz: " + String(luzLigada ? "ACESA" : "APAGADA"));

    // Pequeno delay para debounce (evita ruído elétrico)
    delay(50); 
  }

  botaoUltimoEstado = botaoEstadoAtual;
}

void statusBotaoMic() {
  int botaoMicEstadoAtual = digitalRead(BUTTONMIC_PIN);

  if (botaoMicUltimoEstado == LOW && botaoMicEstadoAtual == HIGH) {
    micLigado = !micLigado; 

    client.publish(topic_som_state, (micLigado ? "ON" : "OFF"), true); 

    desenharTela(valorDbAtual, luzLigada, wifiOk, micLigado); 

    Serial.println("Microfone: " + String(micLigado ? "LIGADO" : "DESLIGADO"));
  }

  botaoMicUltimoEstado = botaoMicEstadoAtual;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Mensagem MQTT recebida! " + String(topic));

  if (String(topic) == topic_luz_set) {
    luzLigada = interpretarPayload(payload, length);
    
    if (luzLigada) {
      acenderLuz(); 
    } else {
      apagarLuz();
    }
    
    // Força a atualização do display com os dados atuais
    // bool wifiOk = (WiFi.status() == WL_CONNECTED);
    // desenharTela(valorDbAtual, luzLigada, wifiOk, micLigado);
  }else  if (String(topic) == topic_som_set) {
    micLigado = interpretarPayload(payload, length);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  pinMode(BUTTONMIC_PIN, INPUT_PULLUP); 
  //LED
  pinMode(LED_PIN, OUTPUT);

  setupDisplay();

  //LED RGB da placa
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  setupWifi();

  setupMqtt();

  setupMic();
}

void loop() {
  
  if (timerDisplay.pronto()) {
    desenharTela(valorDbAtual, luzLigada, wifiOk, micLigado); 
  }

  if (!client.connected() && timerMQTT.pronto()) {
    reconnectMqtt();
  }else{
    client.loop();
  }

  statusBotao();
  
  statusBotaoMic();

  if (timerWifi.pronto()) {
     wifiOk = (WiFi.status() == WL_CONNECTED);

    if (!wifiOk) {
        Serial.println("WiFi Desconectado... tentando reconectar em background.");
    }
  } 

  if (micLigado && timerMic.pronto()) {
     valorDbAtual = lerMic();      
  }

  if (micLigado && timerMQTT.pronto() ){
      enviar_decibeis(valorDbAtual);
  }

}