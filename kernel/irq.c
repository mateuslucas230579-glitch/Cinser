/****************************************************************************
 * Projeto: Tervia Cinser OS
 * Arquivo: irq.c
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


#include <stdint.h>
#include "irq.h"
#include "pic.h"

#define IRQ_COUNT 16

static irq_handler_t irq_handlers[IRQ_COUNT];

void irq_init(void) {
    for (int i = 0; i < IRQ_COUNT; i++) irq_handlers[i] = (irq_handler_t)0;
}

void irq_install_handler(uint8_t irq, irq_handler_t handler) {
    if (irq < IRQ_COUNT) irq_handlers[irq] = handler;
}

void irq_uninstall_handler(uint8_t irq) {
    if (irq < IRQ_COUNT) irq_handlers[irq] = (irq_handler_t)0;
}

void irq_dispatch(regs_t *r) {
    // IRQs foram remapeadas para 0x20-0x2F
    uint32_t int_no = r->int_no;
    if (int_no < PIC1_OFFSET || int_no > (PIC1_OFFSET + 15)) {
        return;
    }

    uint8_t irq = (uint8_t)(int_no - PIC1_OFFSET);

    // Chama handler se existir
    if (irq < IRQ_COUNT && irq_handlers[irq]) {
        irq_handlers[irq](r);
    }

    // Sempre manda EOI, senao o PIC pode travar as proximas IRQs
    pic_send_eoi(irq);
}
