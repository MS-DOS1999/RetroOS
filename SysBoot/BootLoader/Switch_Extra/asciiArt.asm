
PrintOSName:

pusha

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

popa
ret

OS_NAME_0:
	db '@@@@@@@   @@@@@@@@  @@@@@@@  @@@@@@@    @@@@@@ '

OS_NAME_1:
	db '@@@@@@@@  @@@@@@@@  @@@@@@@  @@@@@@@@  @@@@@@@@'
	
OS_NAME_2:
	db '@@!  @@@  @@!         @@!    @@!  @@@  @@!  @@@'
	
OS_NAME_3:
	db '!@!  @!@  !@!         !@!    !@!  @!@  !@!  @!@'
	
OS_NAME_4:
	db '@!@!!@!   @!!!:!      @!!    @!@!!@!   @!@  !@!'
	
OS_NAME_5:
	db '!!@!@!    !!!!!:      !!!    !!@!@!    !@!  !!!'
	
OS_NAME_6:
	db '!!: :!!   !!:         !!:    !!: :!!   !!:  !!!'
	
OS_NAME_7:
	db ':!:  !:!  :!:         :!:    :!:  !:!  :!:  !:!'
	
OS_NAME_8:
	db '::   :::   :: ::::     ::    ::   :::  ::::: ::'
	
OS_NAME_9:
	db ' :   : :  : :: ::      :      :   : :   : :  : '

OS_STR_0:
	db '               @@@@@@    @@@@@@ '
	
OS_STR_1:
	db '              @@@@@@@@  @@@@@@@ '
	
OS_STR_2:
	db '              @@!  @@@  !@@     '
	
OS_STR_3:
	db '              !@!  @!@  !@!     '
	
OS_STR_4:
	db '              @!@  !@!  !!@@!!  '
	
OS_STR_5:
	db '              !@!  !!!   !!@!!! '
	
OS_STR_6:
	db '              !!:  !!!       !:!'
	
OS_STR_7:
	db '              :!:  !:!      !:! '
	
OS_STR_8:
	db '              ::::: ::  :::: :: '
	
OS_STR_9:
	db '               : :  :   :: : :  '


PROGRAMMER:
	db 'by Julien MAGNIN (a.k.a MSDOS)'
	