[bits 16]

struc	MemoryMapRegion
	.baseAddress resq 1
	.length 	 resq 1
	.type		 resd 1
	.acpi_3_0	 resd 1
endstruc

;--------------------------------------

;BIOS INT 0x15 Function 0xE801 - Get Memory Size For > 64 MB Configurations
;Return
;CF = Clear if successful
;EAX = Extended memory between 1MB and 16MB in KB
;EBX = Extended memory above 16MB, in 64KB blocks
;ECX = Configured memory 1MB to 16MB in KB
;EDX = Configured memory above 16MB in 64JB blocks

;--------------------------------------
GetMemSize:
	push ecx
	push edx
	xor	ecx, ecx
	xor	edx, edx
	mov	ax, 0xe801
	int	0x15
	test cx, cx
	je use_ax
	mov	ax, cx
	mov	bx, dx

use_ax:
	pop	edx
	pop	ecx
	ret

;--------------------------------------

;BIOS INT 0x15 Function 0xE820 - Get Memory Map
;Input
;EAX = 0x0000E820
;EBX = continuation value or 0 to start at beginning of map
;ECX = size of buffer for result (Must be >= 20 bytes)
;EDX = 0x534D4150h ('SMAP')
;ES:DI = Buffer for result

;Return

;CF = clear if successful
;EAX = 0x534D4150h ('SMAP')
;EBX = offset of next entry to copy from or 0 if done
;ECX = actual length returned in bytes
;ES:DI = buffer filled
;If error, AH containes error code

;--------------------------------------
GetMemMap:
	pushad

	xor ebx, ebx	;on set ebx à 0
	xor bp, bp
	mov edx, 0x534D4150
	mov eax, 0xE820
	mov ecx, 24 ;taille de notre struct

	int 0x15 ;on get la première region de la map
	jmp startRegion

nextRegion:
	mov edx, 0x534D4150
	mov eax, 0xE820
	mov ecx, 24

	int 0x15
startRegion:
	test ecx, ecx ;si le bit returné est egal à 0 alors on skip la region
	je skipRegion

	mov ecx, [es:di + MemoryMapRegion.length] ;get length (low dword)
	test ecx, ecx
	jne short goodRegion
	mov ecx, [es:di + MemoryMapRegion.length + 4] ;get length (high dword)
	test ecx, ecx
	je short skipRegion

goodRegion:
	inc bp ;on incremente le compteur de region
	add di, 24 ;on fait pointer di vers la prochaine region

skipRegion:
	cmp	ebx, 0 ;si ebx = 0 alors le mapping est fait sinon on continu
	jne nextRegion
	jmp mappingDone

mappingDone:
	popad
	ret
	