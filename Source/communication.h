#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <WiFi.h>  
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <HTTPClient.h>

const char* mqtt_server = "iotifspcat.ddns.net";
const char* mqtt_user = "iot";
const char* mqtt_pass = "IoT2026#";

//Tópicos
const char* topic_luz_set    = "monitoramento/luz/set";
const char* topic_luz_state  = "monitoramento/luz/status";
const char* topic_som_db     = "monitoramento/som";
const char* topic_som_set    = "monitoramento/som/set";
const char* topic_som_state  = "monitoramento/som/status";

WiFiClient espClient;
PubSubClient client(espClient);
WiFiManager wm;

void setupWifi() {
    wm.setConfigPortalTimeout(180);

    // O autoConnect trava a execução aqui até conectar ou dar timeout
    if (!wm.autoConnect("Config_Decibelimetro_S3")) {
        Serial.println("Falha ao conectar e timeout atingido. Reiniciando...");
        delay(3000);
        ESP.restart();
    }

    Serial.println("WiFi Conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length);

bool interpretarPayload(byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) message += (char)payload[i];

  if (message == "ON") return true;

  if (message == "OFF") return false;

  return false;
}

void reconnectMqtt() {
  if (client.connected()) {
    return;
  }

  Serial.print("Tentando conexão MQTT...");
  
  if (client.connect("ESP32_Luz_Cliente", mqtt_user, mqtt_pass)) {
    Serial.println("Conectado!");
    client.subscribe(topic_luz_set);
    client.subscribe(topic_som_set);
    client.publish(topic_luz_state, "OFF");
  } else {
    Serial.print("Falhou, rc=");
    Serial.print(client.state());
    Serial.println(" - Tentarei novamente no próximo ciclo do timer.");
  }
}

void setupMqtt(){
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void enviar_decibeis(float db) {
    if (client.connected()) {
        String payload = "{\"decibeis\":" + String(db) + "}";                  
  
        client.publish(topic_som_db, payload.c_str()); 

       Serial.println("Envio decibeis MQTT");   
    }
}

void enviarDadosHTTP(float temp, int umid, String desc) {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // Altere para o IP do seu servidor Linux
    http.begin("http://18.224.24.167/salvar.php"); 
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Monta a String de dados igual ao formulário que o PHP espera
    String dadosPost = "temperatura=" + String(temp) + 
                       "&umidade=" + String(umid) + 
                       "&descricao=" + desc;

    int httpResponseCode = http.POST(dadosPost);

    if (httpResponseCode > 0) {
      Serial.print("Resposta do Servidor: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Erro no envio: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}


#endif
