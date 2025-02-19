bits 64

section .text
    global _start

_start:
	xor rax, rax
	mov rax, 0x401650
	jmp rax
