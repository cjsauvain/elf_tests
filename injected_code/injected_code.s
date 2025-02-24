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
	xor		rdx, rdx
	mov		r8, 0xFFEEDDCCBBAA998A ; _start address
	mov		r9, 0x8B77665544332211 ; _woody address
	sub		r8, r9
	call	_rocket_launcher	; lea rip, [_rocket_launcher]
								; push rip
_rocket_launcher:
	pop r9						; r9 = rip
	sub r9, diff
	add r9, r8
	jmp	    r9
	
section .data
	msg	db "....WOODY....", 0xa
	len	equ $-msg
	diff equ _rocket_launcher-_woody

section .init_array
    dq _woody
