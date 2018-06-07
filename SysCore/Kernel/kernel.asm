[org 0x100000] ;on load le kernel à 1MB
[bits 32]

jmp Kernel

%include "..\Lib\stdio.asm"

welcome:
	db 0x0A, 0x0A, "		I AM THE KERNEL", 0x0A
	db 0x0A, 0x0A, "	MY CREATOR IS THE PROGRAM", 0x0A, 0

Kernel:
	
	;set segment register et la stack
	mov	ax, 0x10
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	esp, 90000h

	mov bh, 0x04
	call ClearScreen
	mov ah, 0x04
	mov ebx, welcome
	call Puts

	cli
	hlt