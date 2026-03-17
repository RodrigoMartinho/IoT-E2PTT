#ifndef COMMON_H
#define COMMON_H

struct Temporizador {
  unsigned long tempoAnterior = 0;
  unsigned long intervalo;

  // Construtor: define o intervalo em milissegundos
  Temporizador(unsigned long ms) { intervalo = ms; }

  bool pronto() {
    unsigned long agora = millis();
    if (agora - tempoAnterior >= intervalo) {
      tempoAnterior = agora;
      return true;
    }
    return false;
  }
};

#endif
