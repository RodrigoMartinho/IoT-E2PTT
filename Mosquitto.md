
# Instalação e Configuração do Mosquitto

```console
  sudo apt install mosquitto mosquitto-clients
```
# Liberar a porta 1883

Caso não tenha o nano instalado
```console
  sudo apt install nano
```
Abrir o arquivo de configurações do mosquitto
```console
  sudo nano /etc/mosquitto/mosquitto.conf
```
  Inserir as seguintes linhas (sem segurança):  \
    listener 1883  \
    allow_anonymous true \
  
  aperte CTRL + X para salvar

```console
  sudo systemctl enable mosquitto
  sudo systemctl start mosquitto
```
  
# Liberar a porta 1883 no Firewall do Google

# Segurança no MQTT
```console
  sudo mosquitto_passwd -c /etc/mosquitto/passwd <usuario>

  sudo nano /etc/mosquitto/mosquitto.conf
```

  Inserir as seguintes linhas:
    listener 1883 0.0.0.0
    allow_anonymous false
    password_file /etc/mosquitto/passwd
```console
  sudo systemctl restart mosquitto (reiniciar o mosquitto)

  sudo chown mosquitto:mosquitto /etc/mosquitto/passwd
  sudo chmod 0640 /etc/mosquitto/passwd
```
