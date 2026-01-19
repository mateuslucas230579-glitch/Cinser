BITS 32

; ---------------------------------------------------------------------------
; Tervia Cinser OS - Interrupt/IRQ stubs
; ---------------------------------------------------------------------------
; Builds a regs_t-compatible stack frame and calls:
;   - isr_handler(regs_t*) for CPU exceptions (0-31)
;   - irq_dispatch(regs_t*) for PIC IRQs (mapped to 0x20-0x2F)
;
; Stack layout matches include/isr.h regs_t:
;   gs, fs, es, ds, edi, esi, ebp, esp, ebx, edx, ecx, eax, int_no, err_code,
;   eip, cs, eflags, useresp, ss
; ---------------------------------------------------------------------------

GLOBAL isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
GLOBAL isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
GLOBAL isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
GLOBAL isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31

GLOBAL irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7
GLOBAL irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15

EXTERN isr_handler
EXTERN irq_dispatch

%macro ISR_NOERR 1
isr%1:
    push dword 0          ; err_code
    push dword %1         ; int_no
    jmp isr_common
%endmacro

%macro ISR_ERR 1
isr%1:
    push dword %1         ; int_no (CPU already pushed err_code)
    jmp isr_common
%endmacro

%macro IRQ_STUB 1
irq%1:
    push dword 0          ; err_code
    push dword (0x20 + %1); int_no
    jmp irq_common
%endmacro

; Exceptions without error code
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7

; Exceptions with error code
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15

ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_ERR   30
ISR_NOERR 31

; Hardware IRQs
IRQ_STUB 0
IRQ_STUB 1
IRQ_STUB 2
IRQ_STUB 3
IRQ_STUB 4
IRQ_STUB 5
IRQ_STUB 6
IRQ_STUB 7
IRQ_STUB 8
IRQ_STUB 9
IRQ_STUB 10
IRQ_STUB 11
IRQ_STUB 12
IRQ_STUB 13
IRQ_STUB 14
IRQ_STUB 15

; ---------------- Common stubs ----------------

isr_common:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call isr_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8            ; pop int_no + err_code
    iret

irq_common:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_dispatch
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8            ; pop int_no + err_code
    iret
