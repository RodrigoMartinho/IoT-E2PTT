# IoT-E2PTT
Trabalho da matéria Plataformas de prototipação para IoT

# Materiais utilizados:

- ESP32 S3
- Protoboard
- Cabos
- Display OLED 2 cores (SSD1306)
- Microfone omnidirecional (INMP441)
- Led vermelho
- 2 Push button
- Resistor 220 Ohms

# Sistema Operacional e Softwares Utilizados

 - Servidor Linux 24.04 hospedado no GCP (Google Cloud Platform).
 - Banco de Dados MySQL
 - Mosquitto (MQTT)
 - NodeRED (criação dos fluxos, para comunicação via MQTT e salvar os dados em MySQL).
 - ArduinoIDE para codificar o ESP32

# Como o projeto funciona

A ideia desse projeto é que o sistema vai se conectar com WiFi (utilizando biblioteca wifimanager para não deixar senhas de wifi salvas no código).
Ao ligar o dispositivo, ele inicia em modo Access Point, efetua a conexão na rede que ele gera (Config_Decibelimetro_S3) e ai, seleciona uma rede
e insere a senha dessa rede.

Após conectado ao Wifi, o sistema inicializa o visor com as informações de Wifi ligado/desligado, led ligado/desligado e microfone ligado/desligado.
Ao apertar um botão ao lado da Luz, ele acende o LED e envia via MQTT a informação que a luz está acesa. O dashboard vai ficar com a luz acesa, e 
se clicar no botão do dashboard, ele apaga a luz do dispositivo.

Ao apertar um botão ao lado do microfone, ele começa a "escutar" e a cada 5 segundos, ele envia a medição via MQTT.

Na parte de codificação em ArduinoIDE, separei o código em vários arquivos, para melhor compreensão:
 - display.h (codificação do display OLED)
 - imagens.h (codificação das imagens que aparecem no visor)
 - mic_sensor.h (codificação do sensor de microfone)
 - communication.h (codificação Wifi, MQTT e WiFiManager)
 - common.h (codificação do temporizador)
 - IoT.ino (código principal do projeto)

   # Acesso ao dashboard

   http://iotifspcat.ddns.net:1880/dashboard
   
