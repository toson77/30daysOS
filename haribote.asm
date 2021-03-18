; haribote-os
; TAB=4
; BOOT_INFO
CYLS	EQU		0x0ff0		; set boot sector
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2		; bit color info
SCRNX	EQU		0x0ff4		; screen x size
SCRNY	EQU		0x0ff6		; screen y size
VRAM	EQU		0x0ff8		; point of start gui buf
		ORG		0xc200		; 0xc200 <- 0x8000 + 0x4200
		MOV		AL, 0x13	; VGA graphics
		MOV		AH, 0x00
		INT		0x10
		MOV		BYTE [VMODE], 8		; set screenmode
		MOV		WORD [SCRNX], 320	; set screen x size
		MOV		WORD [SCRNY], 200	; set screen y size
		MOV		DWORD [VRAM], 0x000a000
; BIOS teach LED state on keyboard
		MOV		AH, 0x02
		INT		0x16		; keyboard BIOS
		MOV		[LEDS], AL

fin:
		HLT
		JMP		fin

; vim:ft=nasm
