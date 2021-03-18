default :
	make img

ipl.bin : ipl.asm Makefile
	nasm ipl.asm -o ipl.bin -l ipl.lst

haribote.sys : haribote.asm Makefile
	nasm haribote.asm -o haribote.sys -l haribote.lst

haribote.img : ipl.bin Makefile haribote.sys
	mformat -f 1440 -C -B ipl.bin -i haribote.img ::
	mcopy haribote.sys -i haribote.img ::

asm :
	make -r ipl.bin

img :
	make -r haribote.img

run :
	make img
	qemu-system-i386 -fda helloos.img

clean :
	@rm *.img *.bin
