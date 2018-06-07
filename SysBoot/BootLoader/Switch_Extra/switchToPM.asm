; kernel name (11 bytes)
KernelName:
	db "KERNEL  COR"

; taille du kernel en bytes
KernelSize:
	db 0

%include "..\SysCore\Lib\floppy16.asm"
%include "..\SysCore\Lib\fat12.asm"

;okok on va passer en protected mode ! ce qui nous permettre d'avoir des zones memoire protégé
;de créer des memoires virtuelles, d'avoir le mode 32bit et pas mal d'autres chose, mais on aura plus acces aux interrupts du bios

;pour switcher du mode real au mode protéger on doit faire :
; 1 -> desactiver les interrupts
; 2 -> chargé la GDT descriptor
; 3 -> set le mode 32bit
; 4 -> faire un far jump en utilisant le segment de code du Kernel
; 5 -> une fois en protected mode, il faut qu'on init le segment avec les bons paramètres, ceux dans gdt_kernel_data
; 6 -> on oublie pas d'init la stack

[bits 16]
switch_to_pm:

	;on charge le root directory et le kernel avant de switch
	;--------------------------------------------------------
	
	cli ; 1
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ax, 0x0
	mov ss, ax
	mov sp, 0xFFFF

	lgdt [gdt_descriptor] ; 2

	;---------------A20
	mov	ax, 0x2401
	int	0x15
	;---------------

	sti

	call LoadRoot

	mov ebx, 0
	mov bp, 0x3000
	mov si, KernelName
	call LoadFile
	mov [KernelSize], ecx
	cmp ax, 0
	je switch
	cli
	hlt

	;--------------------------------------------------------

	switch:
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax ; 3
	jmp KERNEL_CODE_SEG:init_seg ; 4

[bits 32]	
init_seg:
	mov ax, KERNEL_DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax ; 5
	
	mov ebp, 0x90000
	mov esp, ebp ; 6
	
	call print_OUTPUT_PM

CopyKernel:
	mov eax, [KernelSize]
	movzx ebx, word [_BytesPerSector]
	mul ebx
	mov ebx, 4
	div ebx
	cld
	mov esi, 0x3000
	mov edi, 0x100000
	mov ecx, eax
	rep movsd
	
	jmp KERNEL_CODE_SEG:0x100000
	
print_OUTPUT_PM:
	pusha
	
	;32bit PM print
	mov edx, 0xB8000 ;memoire video
	add edx, 2 * (18 * 80 + (15+32+10)) ;on selectionne l'emplacement du premier char
	;formule 0xb8000 + 2 * (  row * 80 + col )
	mov ebx, OUTPUT_PM ;string
	
printPM_loop:
	mov al, [ebx] ;string pointer
	mov ah, 0x03 ;color
	
	cmp al, 0
	je printPM_end
	
	mov [edx], ax ;on stock le char et sa couleur dans l'empalcement video
	add ebx, 1 ;on incremente le pointeur
	add edx, 2 ;on incremente la memoire video
	
	jmp printPM_loop ;on boucle

printPM_end:
	popa
	ret
	