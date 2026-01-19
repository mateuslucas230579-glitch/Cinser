/****************************************************************************
 * Projeto: Tervia Cinser OS
 * Arquivo: kernel.c
 * Descrição: Núcleo do sistema operacional / Gerenciamento de processos.
 * * Copyright (C) 2026 Tervia Corporation.
 *
 * Este programa é um software livre: você pode redistribuí-lo e/ou 
 * modificá-lo sob os termos da Licença Pública Geral GNU como publicada 
 * pela Free Software Foundation, bem como a versão 3 da Licença.
 *
 * Este programa é distribuído na esperança de que possa ser útil, 
 * mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO 
 * a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a 
 * Licença Pública Geral GNU para mais detalhes.
 ****************************************************************************/

#include <stdint.h>
#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "irq.h"
#include "io.h"

#define MULTIBOOT_MAGIC 0x2BADB002u

static void print_hex32(uint32_t v) {
    const char *hex = "0123456789ABCDEF";
    vga_write("0x");
    for (int i = 7; i >= 0; i--) {
        uint8_t n = (v >> (i * 4)) & 0xF;
        vga_putc(hex[n]);
    }
}

static volatile uint32_t g_ticks = 0;

static void timer_irq(regs_t *r) {
    (void)r;
    g_ticks++;
    // imprime um ponto a cada ~18 ticks (aprox 1 segundo no PIT padrao)
    if ((g_ticks % 18u) == 0u) {
        vga_putc('.');
    }
}

static void keyboard_irq(regs_t *r) {
    (void)r;
    uint8_t sc = inb(0x60);

    // Ignora key release (bit 7)
    if (sc & 0x80) return;

    vga_write(" [KBD ");
    print_hex32((uint32_t)sc);
    vga_write("] ");
}

void kernel_main(uint32_t magic, uint32_t mb_info) {
    // 1) VGA primeiro: se qualquer coisa travar, voce ainda ve o log
    vga_init();
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    vga_write("Tervia Cinser\n");
    vga_write("A i386 Operating System\n\n");

    vga_write("Multiboot magic: ");
    if (magic == MULTIBOOT_MAGIC) {
        vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
        vga_write("OK\n");
    } else {
        vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
        vga_write("INVALID (");
        print_hex32(magic);
        vga_write(")\n");
    }

    vga_set_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    vga_write("Multiboot info ptr: ");
    print_hex32(mb_info);
    vga_write("\n\n");

    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga_write("[1] IDT... ");
    idt_init();
    vga_write("OK\n");

    vga_write("[2] PIC... ");
    pic_init();
    vga_write("OK\n");

    vga_write("[3] IRQ layer... ");
    irq_init();
    vga_write("OK\n");

    // Instala handlers basicos
    irq_install_handler(0, timer_irq);     // IRQ0 = PIT
    irq_install_handler(1, keyboard_irq);  // IRQ1 = PS/2

    // Libera so o necessario
    pic_unmask_irq(0);
    pic_unmask_irq(1);

    vga_write("[4] STI (enable interrupts)... ");
    __asm__ volatile("sti");
    vga_write("OK\n\n");

    vga_write("Cinser Kernel OK! ");
    vga_write("(dots = timer, KBD shows scancode)\n\n");

    for (;;) {
        __asm__ volatile("hlt");
    }
}
