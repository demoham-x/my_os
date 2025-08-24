; /**
;  * @file gdt.s
;  * @brief Assembly routines for GDT management
;  *
;  * This file contains the low-level assembly code required to load the GDT
;  * and properly reload all segment registers for x86 protected mode.
;  */

[GLOBAL gdt_flush]


; Define constants for assembly (matching header file)
;%define GDT_KERNEL_CODE_SELECTOR 0x08
;%define GDT_KERNEL_DATA_SELECTOR 0x10

; /**
;  * @brief Flush GDT and reload all segment registers
;  * @param gdt_ptr Address of GDT pointer structure (on stack at [esp+4])
;  *
;  * This function:
;  * 1. Loads the new GDT using LGDT instruction
;  * 2. Reloads all data segment registers with kernel data selector (0x10)
;  * 3. Performs a far jump to reload the code segment register (0x08)
;  *
;  * The segment selectors used are:
;  * - 0x08: Kernel code segment (GDT entry 1)
;  * - 0x10: Kernel data segment (GDT entry 2)
;  */
gdt_flush:
    mov eax, [esp+4]                ; Load GDT pointer address from stack
    lgdt [eax]                      ; Load the new GDT pointer

    ; Reload all data segment registers with kernel data selector
    mov ax, 0x10 ; 0x10 = kernel data segment
    mov ds, ax                      ; Data segment
    mov es, ax                      ; Extra segment
    mov fs, ax                      ; F segment
    mov gs, ax                      ; G segment
    mov ss, ax                      ; Stack segment

    ; Perform far jump to reload code segment register
    ; This is required because CS cannot be loaded directly
    jmp 0x08:.flush ; 0x08 = kernel code segment

.flush:
    ret                             ; Return to caller with new segments active
