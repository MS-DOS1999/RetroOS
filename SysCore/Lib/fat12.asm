[bits 16]

%define ROOT_OFFSET 0x2E00
%define FAT_SEG 0x2C0
%define ROOT_SEG 0x2E0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;		FAT12 organisation
;
;		;Boot Sector;	;Extra Reserved Sectors;	;File Allocation Table 1;	;File Allocation Table 2;	;Root Directory (FAT12/FAT16 Only);		;Data Region containng files and directories.;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	ROOT DIRECTORY TABLE
;
;	Bytes 0-7 : DOS File name (Padded with spaces)
;	Bytes 8-10 : DOS File extension (Padded with spaces)
;	Bytes 11 : File attributes. This is a bit pattern:
;	Bit 0 : Read Only
;	Bit 1 : Hidden
;	Bit 2 : System
;	Bit 3 : Volume Label
;	Bit 4 : This is a subdirectory
;	Bit 5 : Archive
;	Bit 6 : Device (Internal use)
;	Bit 6 : Unused
;	Bytes 12 : Unused
;	Bytes 13 : Create time in ms
;	Bytes 14-15 : Created time, using the following format:
;	Bit 0-4 : Seconds (0-29)
;	Bit 5-10 : Minutes (0-59)
;	Bit 11-15 : Hours (0-23)
;	Bytes 16-17 : Created year in the following format:
;	Bit 0-4 : Year (0=1980; 127=2107
;	Bit 5-8 : Month (1=January; 12=December)
;	Bit 9-15 : Hours (0-23)
;	Bytes 18-19 : Last access date (Uses same format as above)
;	Bytes 20-21 : EA Index (Used in OS/2 and NT, dont worry about it)
;	Bytes 22-23 : Last Modified time (See byte 14-15 for format)
;	Bytes 24-25 : Last modified date (See bytes 16-17 for format)
;	Bytes 26-27 : First Cluster
;	Bytes 28-32 : File Size
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	fileSector et le secteur où le stockage des fichiers commence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;		LoadRoot()
;		;charge la Root Directory Table
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LoadRoot:
	pusha
	push es ;on save le segment register pour le remettre après avoir load

	;on calcul le nombre de sector que prend le root directory et on la met dans cx
	xor cx, cx
	xor dx, dx
	mov ax, 32
	mul word [_RootEntries]
	div word [_BytesPerSector]
	xchg ax, cx

	;on calcul le sector où commence le root directory et on le met sans ax
	mov al, [_NumberOfFATs]
	mul word [_SectorsPerFAT]
	add ax, [_ReservedSectors]
	mov [fileSector], ax
	add [fileSector], cx

	;on a toute les infos pour lire le root directory, lets READ !
	push ROOT_SEG
	pop es
	mov bx, 0x0
	call ReadSectors
	pop es
	popa
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;		LoadFAT(es:di)
;		;charge les FAT Table
;		;es:di == Root Directory Table
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LoadFAT:
	pusha
	push es ;on save le segment register pour le remettre après avoir load

	;on calcul le nombre de sector que prend le FAT et on la met dans cx
	xor ax, ax
	mov al, [_NumberOfFATs]
	mul word [_SectorsPerFAT]
	mov cx, ax

	;on calcul le sector où commence le FAT et on le met dans ax
	mov ax, [_ReservedSectors]

	;on a toute les infos pour lire le FAT, lets READ !
	push FAT_SEG
	pop es
	xor bx, bx
	call ReadSectors
	pop es
	popa
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;		ax FindFile(ds:si)
;		;chercher un fichier dans la Root Table
;		;ds:si == nom du fichier
;		;ax == index du fichier dans directory table, renvoie -1 si il y a une erreur
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

FindFile:
	push cx 
	push dx
	push bx ;on save les registers sensible issou
	mov bx, si ;on copy le nom du fichier pour plus tard

	;on va parcourir tout les fichiers, pour chercher si le notre existe
	mov cx, [_RootEntries] ;on va boucler sur chaque fichier
	mov di, ROOT_OFFSET ;premier fichier pour commencer la boucle
	cld

.l00p:
	push cx
	mov cx, 11
	mov si, bx
	push di
	rep cmpsb

	pop di
	je found
	pop cx
	add di, 32
	loop .l00p

notFound:
	pop bx
	pop dx
	pop cx
	mov ax, -1
	ret

found:
	pop ax
	pop bx
	pop dx
	pop cx
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;		ax cx LoadFile(es:si, bx:bp)
;		;carger un fichier en memoire
;		;es:si == nom du fichier
;		;bx:bp == zone memoire où on veut stocker le fichier
;		;ax == 0 si ça a marché, renvoie -1 si il y a une erreur
;		;cx == nombre de sector chargé
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LoadFile:
	xor ecx, ecx
	push ecx

findFile:
	push bx
	push bp ;on save le buffer memoire pour plus tard

	call FindFile ; si contient le nom de notre fichier

	cmp ax, -1 
	jne loadImageInit ; si le fichier est trouvé alors on charge les datas !
	;sinon on renvoie une erreur
	pop bp
	pop bx
	pop ecx
	mov ax, -1
	ret

loadImageInit:
	sub edi, ROOT_OFFSET ;on ramene a une valeur absolue
	sub eax, ROOT_OFFSET

	;on va demarrer le cluster

	push ROOT_SEG
	pop es
	mov dx, [es:di + 0x001A] ;on stock le pointer de la file entry dans dx, on rajoute un offset de 26 pour tomber sur le numero du premier cluster
	mov [cluster], dx ;on save le premier cluster
	pop bx
	pop es ;on save la zone memoire
	push bx
	push es ;on on la remet sur le haut de la stack pour plus tard
	call LoadFAT ;on load le FAT

	;maintenant que le FAT est chargé, on va lire dans le file sector
loadImage:
	mov ax, [cluster] ;on set tout les paramètres de la fonction readSector
	pop es
	pop bx
	call CHS2LBA
	xor cx, cx
	mov cl, [_SectorPerCluster]
	call ReadSectors ;et on CALLLLLLLLL

	;ça y est le chargement de notre fichier commence
	pop ecx
	inc ecx
	push ecx ;on incrémente le nombre de sector chargé

	push bx
	push es

	mov ax, FAT_SEG
	mov es, ax
	xor bx, bx ;on pointe vers le FAT_SEG, on va check si on est sur la fin du fichier ou si il faut charger d'autres cluster

	;si le cluster est impair on prend le 12 bits de poid fort, sinon on prend les 12bits low
	
	;test pour savoir si le cluster est pair ou impair
	;-------------------------------------------------
	mov ax, [cluster]
	mov cx, ax ;on va save deux fois le cluster pour faire deux calcul
	mov dx, ax
	shr dx, 1 ;on divise par 2, 1 fois
	add cx, dx

	mov bx, 0
	add bx, cx
	mov dx, [es:bx]
	test ax, 0x0001
	jnz oddCluster
	jmp evenCluster
	;-------------------------------------------------
evenCluster:
	and dx, 0000111111111111b ;low 12 bits
	jmp done
oddCluster:
	shr dx, 0x0004 ;high 12 bits
	jmp done

done:
	mov [cluster], dx ;on save le nouveau cluster
	cmp dx, 0x0FF0 ;si dex est inférieur à la fin de fichier, on go sur le prochain cluster
	jb loadImage ;if dx < 0x0FF0

	;else, fin de la funcion
	pop es
	pop bx
	pop ecx
	xor ax, ax ;ax == 0, la fonction a bien était executée
	ret

