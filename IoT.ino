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

int botao_ultimoEstado = HIGH;
bool luzLigada = false;
bool wifiOk = false;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_RGB, NEO_GRB + NEO_KHZ800);

Temporizador timerMic(1000);       //Captura do microfone - 1 segundo 
Temporizador timerWifi(10000);     //Status do WiFi       - 10 segundos 
Temporizador timerDisplay(2000);   //Tela                 - 2 segundos
Temporizador timerMQTT(5000);      //MQTT                 - 5 segundos

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

void status_Botao() {
  int botao_estadoAtual = digitalRead(BUTTON_PIN);

  // Lógica de Detecção de Borda (Edge Detection)
  // Só entra no IF se o botão mudou de SOLTO para PRESSIONADO
  if (botao_ultimoEstado == LOW && botao_estadoAtual == HIGH) {
    
    statusAceso = !statusAceso; 

    if (statusAceso) {
      acenderLuz();
      client.publish(topic_luz_state, "ON", true); 
    } else {
      apagarLuz();
      client.publish(topic_luz_state, "OFF", true); 
    }

    desenharTela(valorDbAtual, luzLigada, wifiOk); 
  
    Serial.print("O status mudou para: ");
    Serial.println(statusAceso ? "ACESO" : "APAGADO");

    // Pequeno delay para debounce (evita ruído elétrico)
    delay(50); 
  }

  botao_ultimoEstado = botao_estadoAtual;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Mensagem MQTT recebida!");

  if (String(topic) == topic_luz_set) {
    luzLigada = interpretarPayload(payload, length);
    
    if (luzLigada) {
      acenderLuz(); 
    } else {
      apagarLuz();
    }
    
    // Força a atualização do display com os dados atuais
    bool wifiOk = (WiFi.status() == WL_CONNECTED);
    desenharTela(valorDbAtual, luzLigada, wifiOk);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP); 

  setup_display();

  //LED RGB da placa
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //LED
  pinMode(LED_PIN, OUTPUT);

  setup_wifi();

  setup_mqtt();

  setup_mic();
}

void loop() {
  
  if (!client.connected() && timerMQTT.pronto()) {
    reconnect_mqtt();
  }else{
    client.loop();
  }
  
  status_Botao();

  if (timerDisplay.pronto()) {
    desenharTela(valorDbAtual, luzLigada, wifiOk); 
  }  

  if (timerWifi.pronto()) {
     wifiOk = (WiFi.status() == WL_CONNECTED);

    if (!wifiOk) {
        Serial.println("WiFi Desconectado... tentando reconectar em background.");
    }else{
      enviar_decibeis(valorDbAtual);
    }
  } 

  if (timerMic.pronto()) {
     valorDbAtual = lerMic(); 
     desenharTela(valorDbAtual, luzLigada, wifiOk ); 
  }
}