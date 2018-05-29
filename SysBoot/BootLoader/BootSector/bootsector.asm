[org 0x7c00] ;Offset, le bootsector doit commencer à cette adresse
[map all map.txt]

;SECTOR 1 :: BOOTLOADER

;init segment
mov ax, 0
mov ds, ax
mov es, ax

;init de la stack
mov bp, 0x9500 ;bp == base pointer
mov sp, bp ;sp == stackpointer

mov bx, 0x7e00 ; l'adresse de départ à laquelle je veux stocker les data des sectors, free space : 0x7e00 -> 0x9fc00
mov dh, 0x03; je veux lire trois secteurs, le 2, 3 et le 4
call disk_loader

;set 80x25 color text mode
mov ah, 0x00
mov al, 0x03
int 0x10

mov bh, 0
mov bp, OS_NAME_0
mov bl, 0x06
mov cx, 47
mov dl, 15
mov dh, 1
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_NAME_1
mov bl, 0x06
mov cx, 47
mov dl, 15
mov dh, 2
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_NAME_2
mov bl, 0x06
mov cx, 47
mov dl, 15
mov dh, 3
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_NAME_3
mov bl, 0x06
mov cx, 47
mov dl, 15
mov dh, 4
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_NAME_4
mov bl, 0x06
mov cx, 47
mov dl, 15
mov dh, 5
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_NAME_5
mov bl, 0x06
mov cx, 47
mov dl, 15
mov dh, 6
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_NAME_6
mov bl, 0x06
mov cx, 47
mov dl, 15
mov dh, 7
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_NAME_7
mov bl, 0x06
mov cx, 47
mov dl, 15
mov dh, 8
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_NAME_8
mov bl, 0x06
mov cx, 47
mov dl, 15
mov dh, 9
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_NAME_9
mov bl, 0x06
mov cx, 47
mov dl, 15
mov dh, 10
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_STR_0
mov bl, 0x04
mov cx, 32
mov dl, 15
mov dh, 12
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_STR_1
mov bl, 0x04
mov cx, 32
mov dl, 15
mov dh, 13
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_STR_2
mov bl, 0x04
mov cx, 32
mov dl, 15
mov dh, 14
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_STR_3
mov bl, 0x04
mov cx, 32
mov dl, 15
mov dh, 15
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_STR_4
mov bl, 0x04
mov cx, 32
mov dl, 15
mov dh, 16
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_STR_5
mov bl, 0x04
mov cx, 32
mov dl, 15
mov dh, 17
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_STR_6
mov bl, 0x04
mov cx, 32
mov dl, 15
mov dh, 18
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_STR_7
mov bl, 0x04
mov cx, 32
mov dl, 15
mov dh, 19
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_STR_8
mov bl, 0x04
mov cx, 32
mov dl, 15
mov dh, 20
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OS_STR_9
mov bl, 0x04
mov cx, 32
mov dl, 15
mov dh, 21
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, PROGRAMMER
mov bl, 0x02
mov cx, 30
mov dl, 79-30
mov dh, 23
mov ah, 0x13
mov al, 1
int 0x10

mov bh, 0
mov bp, OUTPUT_RM
mov bl, 0x03
mov cx, 19
mov dl, 15+32+10
mov dh, 16
mov ah, 0x13
mov al, 1
int 0x10


;delay 1s
mov cx, 0x0F
mov dx, 0x4240
mov ah, 0x86
int 0x15

call switch_to_pm
	
%include "BootLoader\BootSector\diskLoader.asm"

[bits 32]
PM_IS_SET:
	
	call print_OUTPUT_PM
	
	jmp $ ;infinite loop
	
times 510-($-$$) db 0 ; on remplie les bytes non utilisé du sector
;($-$$) == adresse de la dernière instruction actuelle 

dw 0xaa55

;SECTOR 2/3/4 :: asciiArt + switch PM
%include "BootLoader\Switch_Extra\asciiArt.asm"
%include "BootLoader\Switch_Extra\gdt.asm"
%include "BootLoader\Switch_Extra\switchToPM.asm"
OUTPUT_RM:
	db "16BIT RM -> load PM"
OUTPUT_PM:
	db "32BIT PM is Loaded", 0

times 2048-($-$$) db 0

