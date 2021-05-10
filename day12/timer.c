#include "bootpack.h"
struct TIMERCTL timerctl;

#define PIT_CTRL        0x0043
#define PIT_CNT0        0x0040

void init_pit(void)
{
    io_out8(PIT_CTRL, 0x34);
    io_out8(PIT_CNT0, 0x9c); /*specify 11932=0x9c2e so 100Hz */
    io_out8(PIT_CNT0, 0x2e);
    timerctl.count = 0;
    timerctl.timeout = 0;
    return;
}

void inthandler20(int *esp)
{
    io_out8(PIC0_OCW2, 0x60); /* if IRQ-00 recieve , send to PIC */
    timerctl.count++;
    if (timerctl.timeout > 0){
        timerctl.timeout--;
        if(timerctl.timeout == 0){
            fifo8_put(timerctl.fifo, timerctl.data);
        }
    }
    return;
}

void settimer(unsigned int timeout, struct FIFO8 *fifo, unsigned char data)
{
    int eflags;
    eflags = io_load_eflags();
    io_cli();
    timerctl.timeout = timeout;
    timerctl.fifo = fifo;
    timerctl.data = data;
    io_store_eflags(eflags);
    return;
}
