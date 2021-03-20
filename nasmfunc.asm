; nasmfunc.asm
; TAB=4

section .text
		GLOBAL	io_hlt
		GLOBAL 	write_mem8

io_hlt:		; void io_hlt(void);
		HLT
		RLT

write_mem8:		;void write_mem8(int addr, int data);
		MOV 	ECX, [ESP+4] 	; ECX <- addr
		MOV 	AL, [ESP+8] 	; AL <- data
		MOV 	[ECX], AL
		RET

; vim:ft=nasm

