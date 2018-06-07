dd if=SysBoot\bootloader.bin of=\\.\a:
del a:\BOOTK.COR
del a:\KERNEL.COR
copy SysBoot\BOOTK.COR a:
copy SysCore\Kernel\KERNEL.COR a:
pause