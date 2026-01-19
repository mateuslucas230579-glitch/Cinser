/****************************************************************************
 * Projeto: Tervia Cinser OS
 * Arquivo: irq.h
 * Descricao: Camada de IRQ (registro de handlers e dispatcher)
 * Copyright (C) 2026 Tervia Corporation.
 *
 * Este programa e um software livre: voce pode redistribui-lo e/ou
 * modifica-lo sob os termos da Licenca Publica Geral GNU como publicada
 * pela Free Software Foundation, bem como a versao 3 da Licenca.
 *
 * Este programa e distribuido na esperanca de que possa ser util,
 * mas SEM NENHUMA GARANTIA; sem uma garantia implicita de ADEQUACAO
 * a qualquer MERCADO ou APLICACAO EM PARTICULAR. Veja a
 * Licenca Publica Geral GNU para mais detalhes.
 ****************************************************************************/

#pragma once
#include <stdint.h>
#include "isr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*irq_handler_t)(regs_t *r);

void irq_init(void);
void irq_install_handler(uint8_t irq, irq_handler_t handler);
void irq_uninstall_handler(uint8_t irq);

// Chamado pelos stubs de IRQ (int 0x20-0x2F)
void irq_dispatch(regs_t *r);

#ifdef __cplusplus
}
#endif
