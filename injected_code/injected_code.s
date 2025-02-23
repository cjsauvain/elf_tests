bits 64

section .woody
    global _woody

_woody:
	xor 	rax, rax
	mov		rdi, 1
	lea		rsi, [rel msg]
	mov		rdx, len
	mov		rax, 1
	syscall
	xor		rax, rax
	xor		rdi, rdi
	xor		rsi, rsi
	xor		rdx, rdx
	mov		r8, 0x401720
	jmp		r8

section .data
	msg	db "....WOODY....", 0xa
	len	equ $-msg
