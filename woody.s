global _woody

_woody:
	push rbp
	mov rax, 0x1040
	call rax
	pop rbp
	ret
