#ifndef MIC_SENSOR_H
#define MIC_SENSOR_H

#include <math.h>
#include <driver/i2s.h>   

// Definição dos pinos do MIC
#define I2S_WS 4
#define I2S_SD 6
#define I2S_SCK 5
#define I2S_PORT I2S_NUM_0

#define SAMPLE_BLOCK 2048

void setupMic(){
  // Configuração da estrutura I2S
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, // O INMP441 trabalha bem com 32-bit
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1, // Não usado para microfone
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);  
}

float lerMic() {
  // Aumentamos para 1024 amostras (~23ms) para uma leitura mais estável
  const int num_samples = 1024;
  int32_t samples[num_samples];
  size_t bytes_read;
  
  i2s_zero_dma_buffer(I2S_PORT); 
  i2s_read(I2S_PORT, &samples, sizeof(samples), &bytes_read, portMAX_DELAY);

  int samples_count = bytes_read / sizeof(int32_t);
  if (samples_count <= 0) return -100.0;

  // 1. Calcular a média (Offset DC) para centralizar a onda no zero
  long long soma_raw = 0;
  for (int i = 0; i < samples_count; i++) {
    soma_raw += samples[i];
  }
  float offset_dc = (float)soma_raw / samples_count;

  // 2. Calcular o RMS subtraindo o offset
  double sum_sq = 0;
  for (int i = 0; i < samples_count; i++) {
    float pcm_puro = (float)samples[i] - offset_dc; // Remove o ruído DC
    float normalized = pcm_puro / 2147483647.0;     // Normaliza 32 bits
    sum_sq += (normalized * normalized);
  }

  float rms = sqrt(sum_sq / samples_count);
  
  // Evita erro matemático em silêncio absoluto
  if (rms < 0.000001) rms = 0.000001;

  float dbfs = 20.0 * log10(rms);
  
  // 3. CALIBRAÇÃO FINAL: 
  // Se o app marca 60 e o ESP marca 50, aumente este 120 para 130.
  float ajuste_calibracao = 90.0; 
  
  // Exibe os valores
  Serial.print("dBFS: ");
  Serial.print(dbfs);
  Serial.print(" | dB SPL (aprox): ");
  Serial.println(dbfs + ajuste_calibracao);

  float resultado = dbfs + ajuste_calibracao;
  return resultado;   
}

#endif
