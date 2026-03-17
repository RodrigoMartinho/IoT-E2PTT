#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>      //OLED
#include <Adafruit_SSD1306.h>  //OLED
#include "imagens.h"

// ===== Configuração da Tela OLED =====
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  //Ligação da Tela GND -> GND | VDD -> 3V3 | SCK -> D22 | SDA -> D21 

// Pinos I2C para a Tela
#define I2C_SDA 8
#define I2C_SCL 9

void setupDisplay(){
  Wire.begin(I2C_SDA, I2C_SCL);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }   
}

void mostrarTela(String Titulo, String valor) {
  display.clearDisplay();           // Limpa o buffer do display
  display.setTextSize(1);           // Tamanho do título
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.println(Titulo); 

  display.setTextSize(3);           // Aumenta o tamanho para o valor
  display.setCursor(10, 20);
  display.print( valor );           // Imprime o valor da variável

  display.display();                // Atualiza o display com as informações
}

void desenharTela(float decibel, bool luzLigada, bool wifiConectado, bool micLigado) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (wifiConectado) {
    display.drawBitmap(112, 0, bitmap_wifi, 16, 16, SSD1306_WHITE);
  } else {
    display.drawBitmap(112, 0, bitmap_wifi_off, 16, 16, SSD1306_WHITE);
  }
  
  display.setTextSize(1);
  display.setCursor(0, 4);
  display.print(wifiConectado ? "SISTEMA ONLINE" : "SISTEMA OFFLINE");

  if (luzLigada) {
    display.drawBitmap(0, 20, bitmap_luz, 40, 40, SSD1306_WHITE);
  } else {
    display.drawBitmap(0, 20, bitmap_luz_off, 40, 40, SSD1306_WHITE);
  }

  if (micLigado) {
    display.setTextSize(2);
    display.setCursor(50, 35); 
    display.print((int)decibel);
    
    display.setTextSize(1);
    display.print("dB");

    display.drawBitmap(96, 24, bitmap_mic_on, 32, 32, SSD1306_WHITE);
  }else{
    display.drawBitmap(96, 24, bitmap_mic_off, 32, 32, SSD1306_WHITE);    
  }
  
  
  display.display();
}

#endif
