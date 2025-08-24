global loader                 ; the entry symbol for ELF

MAGIC_NUMBER equ 0x1BADB002   ; define the magic number constant
FLAGS        equ 0x0          ; multiboot flags
CHECKSUM     equ -MAGIC_NUMBER ; calculate the checksum
                             ; (magic number + checksum + flags should be equal to 0)

;section .note.GNU-stack noalloc noexec nowrite progbits ; special section for stack
;section .note.ABI-tag noalloc noexec nowrite progbits ; special section for ABI tag
;section .note multiboot noalloc noexec nowrite progbits ; multiboots

section .text                ; start of the text section
align 4                     ; the code must be 4 byte aligned
    dd MAGIC_NUMBER         ; write the magic number to the machine code
    dd FLAGS                ; the flags
    dd CHECKSUM             ; and the checksum

loader:                     ; the loader label (defined as entry point in the linker script)
    mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to the end (top) of the stack

;    extern sum_of_three      ; declare external function

;    push dword 3            ; arg3
;    push dword 2            ; arg2
;    push dword 1            ; arg1
;    call sum_of_three       ; call the function, result in eax
;    extern main              ; declare external funct main
;    call main                ; call main function
    jmp.sum

.sum:
    extern sum_of_three      ; declare external function

    push dword 3            ; arg3
    push dword 2            ; arg2
    push dword 1            ; arg1
    call sum_of_three       ; call the function, result in eax
    jmp.main

.main:
    extern main              ; declare external funct main
    call main                ; call main function

.loop:
    jmp .loop              ; loop forever


KERNEL_STACK_SIZE equ 4096   ; size of stack in bytes

section .bss
align 4                     ; align at 4 bytes
kernel_stack:               ; label points to beginning of memory
    resb KERNEL_STACK_SIZE  ; reserve stack for the kernel


