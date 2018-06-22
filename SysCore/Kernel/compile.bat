del *.o
del *.tmp
gcc -ffreestanding -c vga.c -o vga.o
gcc -masm=intel -ffreestanding -c kernel.c -o kernel.o
ld -T NUL -o kernel.tmp -Ttext 0x100000 kernel.o vga.o
objcopy -O binary -j .text  kernel.tmp KERNEL.COR
del *.o
del *.tmp
pause