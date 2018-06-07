[bits 16]
[org 0x500]



cli						
xor ax, ax
mov ds, ax
mov es, ax

mov ax, 0x0000
mov ss, ax
mov sp, 0xFFFF
sti


;set 80x25 color text mode
mov ah, 0x00
mov al, 0x03
int 0x10

call PrintOSName

;delay 1s
mov cx, 0x0F
mov dx, 0x4240
mov ah, 0x86
int 0x15

call switch_to_pm


%include "BootLoader\Switch_Extra\asciiArt.asm"
%include "BootLoader\Switch_Extra\gdt.asm"
%include "BootLoader\Switch_Extra\switchToPM.asm"
OUTPUT_RM:
	db "16BIT RM -> load PM"
OUTPUT_PM:
	db "32BIT PM is Loaded", 0