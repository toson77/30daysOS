/* interrupt settings*/

#include "bootpack.h"
#include <stdio.h>
void init_pic(void)
{
	io_out8(PIC0_IMR, 0xff); /* disable all interrupt */
	io_out8(PIC1_IMR, 0xff);
	io_out8(PIC0_ICW1, 0x11); /* edge trigger mode */
	io_out8(PIC0_ICW2, 0x20); /* INT20-27 receive IRQ0-7 */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1 connect IRQ2 */
	io_out8(PIC0_ICW4, 0x01); /* non buff mode */

	io_out8(PIC1_ICW1, 0x11); /* edge trigger mode */
	io_out8(PIC1_ICW2, 0x28); /* INT28-2f receive IRQ8-15 */
	io_out8(PIC1_ICW3, 2); /*PIC1 connect IRQ2 */
	io_out8(PIC1_ICW4, 0x01); /* non buffer mode */

	io_out8(PIC0_IMR, 0xfb); /* 11111011 all diconect without PIC1 */
	io_out8(PIC1_IMR, 0xff); /* 11111111 don't receive all interrupt*/
	return;
}

#define PORT_KEYDAT 	0x0060
struct FIFO8 keyfifo;/* ps/2 keyboard interrupt */
void inthandler21(int *esp)
{
	unsigned char data;
	io_out8(PIC0_OCW2, 0x61);
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&keyfifo, data);
	return;
}

struct FIFO8 mousefifo;
void inthandler2c(int *esp)
/* PS/2$B%^%&%9$+$i$N3d$j9~$_(B */
{
	unsigned char data;
	io_out8(PIC1_OCW2, 0x64);
	io_out8(PIC0_OCW2, 0x62);
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&mousefifo, data);
	return;
}

void inthandler27(int *esp)
/* PIC0$B$+$i$NIT40A43d$j9~$_BP:v(B */
/* Athlon64X2$B5!$J$I$G$O%A%C%W%;%C%H$NET9g$K$h$j(BPIC$B$N=i4|2=;~$K$3$N3d$j9~$_$,(B1$BEY$@$1$*$3$k(B */
/* $B$3$N3d$j9~$_=hM}4X?t$O!"$=$N3d$j9~$_$KBP$7$F2?$b$7$J$$$G$d$j2a$4$9(B */
/* $B$J$<2?$b$7$J$/$F$$$$$N!)(B
	$B"*(B  $B$3$N3d$j9~$_$O(BPIC$B=i4|2=;~$NEE5$E*$J%N%$%:$K$h$C$FH/@8$7$?$b$N$J$N$G!"(B
		$B$^$8$a$K2?$+=hM}$7$F$d$kI,MW$,$J$$!#(B									*/
{
	io_out8(PIC0_OCW2, 0x67); /* IRQ-07$B<uIU40N;$r(BPIC$B$KDLCN(B(7-1$B;2>H(B) */
	return;
}
