[bits 32]

%define VRAM 0xB8000

_CurrentX db 0
_CurrentY db 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;		Putc(bl, bh)
;		Print un char à la position actuelle
;		bl == char à print, bh == attribute
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


Putc:

	pusha
	mov edi, VRAM

	;GetCursor
	;-----------------------------------------------------------
		xor eax, eax ;clear eax

		;formule pour get la pos : x + y * taille ecran en byte

		mov ecx, 80*2 ;80 == largeur ecran, *2 car chaque char à une taille de 2byte en 32bit
		mov al, [_CurrentY]
		mul ecx ;le parametre de mul est al, c'est le multiplicateur, ça donne donc ecx * CurrentY, le resultat est retourné dans eax
		push eax ;on save eax

		mov al, [_CurrentX]
		mov cl, 2
		mul cl
		pop ecx
		add eax, ecx
		;notre formule est fini

		xor ecx, ecx
		add edi, eax
	;-----------------------------------------------------------

	cmp bl, 0x0A ; if bl == '\n'
	je .newLine

	;else
	mov dl, bl
	mov dh, bh
	mov [edi], dx
	inc byte [_CurrentX]
	cmp byte [_CurrentX], 80 ; fin de la ligne ?
	je .newLine
	jmp .done

.newLine:
	mov byte [_CurrentX], 0
	inc byte [_CurrentY]

.done:
	popa
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;		Puts(ebx, ah)
;		Print une string à la position actuelle
;		ebx == pointeur vers la string, ah == attribute
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Puts:

	pusha
	push ebx ;on save la string dans edi
	pop edi

.l00p:
	mov bl, [edi]
	cmp bl, 0
	je .done
	mov bh, ah
	call Putc

	inc edi
	jmp .l00p

.done:
	mov bl, [_CurrentX]
	mov bh, [_CurrentY]

	call MovCursor

	popa
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;		MovCursor(bl, bh)
;		Bouge le curseur avec les paramètres
;		bl == xPos, bh == yPos
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MovCursor:

	pusha

	;calcul de la nouvelle pos dans la memoire
	xor eax, eax
	mov ecx, 80
	mov al, bh
	mul ecx
	add al, bl
	mov ebx, eax

	;set low byte index vga register
	mov al, 0x0f
	mov dx, 0x03D4
	out dx, al ;on copie la value dans le I/O Port voulu

	mov al, bl
	mov dx, 0x03D5
	out dx, al

	;clear eax
	xor eax, eax

	;set high byte index vga register
	mov al, 0x0e
	mov dx, 0x03D4
	out dx, al

	mov al, bh
	mov dx, 0x03D5
	out dx, al

	popa
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;		ClearScreen(bh)
;		Efface tout le contenu de l'écran
;		bh == attribute du fond
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ClearScreen:

	pusha

	cld ;set direction flag à 0
	mov edi, VRAM
	mov cx, 2000 ; 80*25 = 2000, on obtient donc le nombre de byte que prend l'ecran avec ce calcul
	mov ah, bh
	mov al, ' '
	rep stosw ;met le contenu de al et ah dans la memoire edi, repète cette opération le nombre de fois indiqué dans cx, et incrémente la mémoire à chaque répétition

	mov byte [_CurrentX], 0
	mov byte [_CurrentY], 0

	popa
	ret












