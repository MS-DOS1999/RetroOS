del *.o
del *.tmp
gcc -ffreestanding -mno-ms-bitfields -c ..\Crt\memset.c -o memset.o
gcc -ffreestanding -mno-ms-bitfields -c ..\Hal\gdt.c -o gdt.o
gcc -ffreestanding -mno-ms-bitfields -c ..\Hal\idt.c -o idt.o
gcc -ffreestanding -mno-ms-bitfields -c ..\Hal\cpu.c -o cpu.o
gcc -ffreestanding -mno-ms-bitfields -c ..\Hal\hal.c -o hal.o
gcc -ffreestanding -mno-ms-bitfields -c vga.c -o vga.o
gcc -masm=intel -ffreestanding -c kernel.c -o kernel.o
ld -T NUL -o kernel.tmp -Ttext 0x100000 kernel.o vga.o hal.o cpu.o idt.o gdt.o memset.o
objcopy -O binary -j .text  kernel.tmp KERNEL.COR
del *.o
del *.tmp
pause