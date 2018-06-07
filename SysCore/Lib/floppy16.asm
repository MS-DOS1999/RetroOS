[bits 16]

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


;CHS = Cylinder/Head/Sector
;LBA = Linear Block Adressing

;ici nous allons avoir besoins de deux fonctions de conversion entre
;le LBA et le CHS
;par exemple quand on va chercher un fichier, ça nous reverra le numero du sector sous la forme LBA
;mais pour charger les données depuis la floppy en mémoire nous avons besoins du CHS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;		CHS2LBA(ax)
;		ax == cluster to convert
;		formule : (cluster - 2) * SectorPerCluster
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CHS2LBA:
	sub ax, 2
	xor cx,cx
	mov cl, [_SectorPerCluster]
	mul cx
	add ax, [fileSector]
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;		LBA2CHS(ax)
;		ax == LBA to convert
;		formule :
;		absolute sector = (LBA % SectorsPerTrack) + 1
;		absolute head = (LBA / SectorsPerTrack) % HeadsPerCylinder
;		absolute track = LBA / (SectorsPerTrack * HeadsPerCylinder)
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;		ReadSectors(cx, ax, es:bx)
;		cx == nombre de sector à lire
;		ax == sector de debut
;		es:bx == adresse memoire où seront stockée les données
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ReadSectors:
	mov di, 5 ;nombre d'essaie pour lire le secteur en cas d'erreur
l00p:
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
	jnz l00p
	;sinon on reboot l'ordi
	int 0x18

success:
	pop cx
	pop bx
	pop ax
	add bx, [_BytesPerSector] ; on incremente la memoire pour pointer vers la nouvelle zone pour le nouveau sector
	inc ax
	loop ReadSectors
	ret
