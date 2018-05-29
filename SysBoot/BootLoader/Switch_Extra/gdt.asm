;gdt = GLOBAL TABLE DESCRIPTOR

;NULL descriptor / Offset:  0 - 0x0 byte
gdt_null:
	dd 0x0
	dd 0x0
	
;Kernel Code / Offset 8 - 0x8 bytes
gdt_kernel_code:
	dw 0xFFFF 	 ;limit low
	dw 0x0 		 ;base low
	db 0x0		 ;base middle
	db 10011010b ;acces ring 0 car le kernel doit avoir accès à tout - bit set to code
	db 11001111b ;flags
	db 0x0 		 ;base high
	
;Kernel Data  / Offset 16 - 0x10 bytes
gdt_kernel_data:
	dw 0xFFFF 	 ;limit low
	dw 0x0 		 ;base low
	db 0x0		 ;base middle
	db 10010010b ;acces ring 0 car le kernel doit avoir accès à tout - bit set to data
	db 11001111b ;flags
	db 0x0 		 ;base high
	
;User Code / Offset 24 - 0x18 bytes
gdt_user_code:
	dw 0xFFFF 	 ;limit low
	dw 0x0 		 ;base low
	db 0x0		 ;base middle
	db 11111010b ;acces ring 3 car le user ne peut pas avoir acces au hardware + kernel + driver - bit set to code
	db 11001111b ;flags
	db 0x0 		 ;base high
	
;User Data / Offset 32 - 0x20 bytes
gdt_user_data:
	dw 0xFFFF 	 ;limit low
	dw 0x0 		 ;base low
	db 0x0		 ;base middle
	db 11110010b ;acces ring 3 car le user ne peut pas avoir acces au hardware + kernel + driver - bit set to data
	db 11001111b ;flags
	db 0x0 		 ;base high
	
gdt_end:

;GDT descriptor
gdt_descriptor:
	dw gdt_end - gdt_null - 1 ; gdt size, toujours la taille - 1
	dd gdt_null ; debut de la gdt
	
KERNEL_CODE_SEG equ gdt_kernel_code - gdt_null
KERNEL_DATA_SEG equ gdt_kernel_data - gdt_null
USER_CODE_SEG	equ gdt_user_code - gdt_null
USER_DATA_SEG	equ gdt_user_data - gdt_null