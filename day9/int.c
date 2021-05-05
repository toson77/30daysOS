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

void inthandler27(int *esp)
/* PIC0からの不完全割り込み対策 */
/* Athlon64X2機などではチップセットの都合によりPICの初期化時にこの割り込みが1度だけおこる */
/* この割り込み処理関数は、その割り込みに対して何もしないでやり過ごす */
/* なぜ何もしなくていいの？
	→  この割り込みはPIC初期化時の電気的なノイズによって発生したものなので、
		まじめに何か処理してやる必要がない。									*/
{
	io_out8(PIC0_OCW2, 0x67); /* IRQ-07受付完了をPICに通知(7-1参照) */
	return;
}
