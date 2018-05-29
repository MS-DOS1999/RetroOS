disk_loader:
	pusha
	
	mov ah, 0x02 ; mode de lecture 0x02 == read
	mov al, dh ; nombre de secteur à lire,, on stock ce nombre dans dh avant de call disk_loader
	mov cl, 0x02 ;le premier secteur à lire
	
	mov ch, 0x00 ; cylindre 0
	mov dh, 0x00 ; tête 0
	
	;le pointer de ces data sera es:bx
	
	int 0x13
	
	popa
	ret
