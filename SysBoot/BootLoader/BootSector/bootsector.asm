[bits 16]
[org 0x0]

start: jmp main


;SECTOR 1 :: BOOTLOADER

TIMES 03h-$+start DB 0

;Bios parameter block

_OEM:
	db "RETRO OS"
_BytesPerSector:
	dw 512
_SectorPerCluster:
	db 1
_ReservedSectors:
	dw 1
_NumberOfFATs:
	db 2
_RootEntries:
	dw 224
_TotalSectors:
	dw 2880
_Media:
	db 0xF0
_SectorsPerFAT:
	dw 9
_SectorsPerTrack:
	dw 18
_HeadsPerCylinder:
	dw 2
_HiddenSectors:
	dd 0
_TotalSectorsBig:
	dd 0

;Floppy info 
_DriveNumber:
	db 0
_Unused:
	db 0
_ExtBootSignature:
	db 0x29
_SerialNumber:
	dd 0x27051999
_VolumeLabel:
	dd "OS FLOPPY  "
_FileSystem:
	db "FAT12   "

absoluteSector:
	db 0
absoluteHead:
	db 0
absoluteTrack:
	db 0

fileSector:
	dw 0
cluster:
	dw 0

Print:
			lodsb				; load next byte from string from SI to AL
			or	al, al			; Does AL=0?
			jz	PrintDone		; Yep, null terminator found-bail out
			mov	ah, 0eh			; Nope-Print the character
			int	10h
			jmp	Print			; Repeat until null terminator found
	PrintDone:
			ret				; we are done, so return


;--------------------temp floppy16

CHS2LBA:
	sub ax, 2
	xor cx,cx
	mov cl, [_SectorPerCluster]
	mul cx
	add ax, [fileSector]
	ret

LBA2CHS:
	xor dx, dx ;on utilise dx:ax
	div word [_SectorsPerTrack]
	inc dl
	mov [absoluteSector], dl
	xor dx, dx
	div word [_HeadsPerCylinder]
	mov [absoluteHead], dl
	mov [absoluteTrack], al
	ret

ReadSectors:
	mov di, 5 ;nombre d'essaie pour lire le secteur en cas d'erreur
sectorl00p:
	push ax
	push bx
	push cx
	call LBA2CHS ;on converti le secteur de depart en CHS

	;read sector into memory
	mov ah, 0x02
	mov al, 0x01
	mov ch, [absoluteTrack]
	mov cl, [absoluteSector]
	mov dh, [absoluteHead]
	mov dl, [_DriveNumber]
	int 0x13

	;si le carry == 0 alors la lecture s'est bien passé
	jnc success
	;sinon
	xor ax, ax
	int 0x13
	dec di
	pop cx
	pop bx
	pop ax
	;si di != 0
	jnz sectorl00p
	;sinon on stop le proco
	cli
	hlt

success:
	pop cx
	pop bx
	pop ax
	add bx, [_BytesPerSector] ; on incremente la memoire pour pointer vers la nouvelle zone pour le nouveau sector
	inc ax
	loop ReadSectors
	ret
;----------------------------------

main:

	cli						
	mov ax, 0x07C0 ;on ajust les segments register
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ax, 0x0000
	mov ss, ax
	mov sp, 0xFFFF
	sti

	;----------------------LoadRoot à 200, juste après le bootsector
	xor cx, cx
	xor dx, dx
	mov ax, 32
	mul word [_RootEntries]
	div word [_BytesPerSector]
	xchg ax, cx

	mov al, [_NumberOfFATs]
	mul word [_SectorsPerFAT]
	add ax, [_ReservedSectors]
	mov [fileSector], ax
	add [fileSector], cx

	mov bx, 0x0200
	call ReadSectors

	;----------------------On cherche BOOTK.COR
	mov cx, [_RootEntries] ;on va boucler sur chaque fichier
	mov di, 0x0200 ;premier fichier pour commencer la boucle

.l00p:
	push cx
	mov cx, 11
	mov si, BootKernelName
	push di
	rep cmpsb

	pop di
	je found
	pop cx
	add di, 32
	loop .l00p

notFound:
	cli
	hlt

found:
	;--------------------On load le FAT

	;on save le cluster du BootKernel
	mov dx, [di + 0x001A]
	mov word [cluster], dx

	;on calcul la taille du Fat et on le met dans cx
	xor ax, ax
	mov al, [_NumberOfFATs]
	mul word [_SectorsPerFAT]
	mov cx, ax

	;on met la location du FAT dans ax
	mov ax, [_ReservedSectors]

	;on met le FAT dans la memoire (7C00:0200)
	mov bx, 0x0200
	call ReadSectors

	;--------------------On charge BOOTK.COR dans la memoire(0050:0000)
	mov ax, 0x0050
	mov es, ax
	mov bx, 0x0000
	push bx

LoadBootK:
	mov ax, [cluster]
	pop bx
	call CHS2LBA
	xor cx, cx
	mov cl, [_SectorPerCluster]
	call ReadSectors
	push bx

	mov ax, [cluster]
	mov cx, ax
	mov dx, ax
	shr dx, 0x0001
	add cx, dx
	mov bx, 0x0200
	add bx, cx
	mov dx, word [bx]
	test ax, 0x0001
	jnz odd

even:
	and dx, 0000111111111111b
	jmp done

odd:
	shr dx, 0x0004

done:
	mov word [cluster], dx
	cmp dx, 0x0FF0
	jb LoadBootK

	push word 0x0050
	push word 0x0000
	retf
	

BootKernelName:
	db "BOOTK   COR"
	
times 510-($-$$) db 0 ; on remplie les bytes non utilisé du sector
;($-$$) == adresse de la dernière instruction actuelle 

dw 0xaa55

