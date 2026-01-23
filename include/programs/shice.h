#pragma once
#ifdef __cplusplus
extern "C" {
#endif

// Inicializa e roda o shell Shice (console/TTY).
// shice_run() não retorna.
void shice_init(void);
void shice_run(void);

#ifdef __cplusplus
}
#endif
