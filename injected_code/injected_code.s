bits 64

section .text
    global _woody

_woody:
	xor rax, rax
	mov	rdi, 1
	mov	rsi, msg
	mov	rdx, len
	mov	rax, 1
	syscall
	xor	rax, rax
	mov rax, 0x401650
	jmp rax

section .data
	msg	db "...WOODY...\n"
	len	equ $-msg
