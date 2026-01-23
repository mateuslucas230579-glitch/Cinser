#ifndef SPLASH_H
#define SPLASH_H

#include <stdint.h>

// Mostra a splash ASCII centralizada (em vermelho) por 'seconds' segundos.
// Retorna após o tempo, sem iniciar o shell.
void splash_show(uint32_t seconds);

#endif
