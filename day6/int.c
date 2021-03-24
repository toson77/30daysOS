/* interrupt settings*/

#include "bootpack.h"
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
