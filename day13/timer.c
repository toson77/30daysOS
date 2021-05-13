#include "bootpack.h"

#define PIT_CTRL 0x0043
#define PIT_CNT0 0x0040

struct TIMERCTL timerctl;

#define TIMER_FLAGS_ALLOC 1 /* allocate state */
#define TIMER_FLAGS_USING 2 /* timer is running */

void init_pit(void)
{
    int i;
    struct TIMER *t;
    io_out8(PIT_CTRL, 0x34);
    io_out8(PIT_CNT0, 0x9c);
    io_out8(PIT_CNT0, 0x2e);
    timerctl.count = 0;
    for (i = 0; i < MAX_TIMER; i++)
    {
        timerctl.timers0[i].flags = 0; /* not using */
    }
    t = timer_alloc();
    t->timeout = 0xffffffff;
    t->flags = TIMER_FLAGS_USING;
    t->next_timer = 0; /* end of line */
    timerctl.t0 = t;
    timerctl.next_time = 0xffffffff;
    return;
}

struct TIMER *timer_alloc(void)
{
    int i;
    for (i = 0; i < MAX_TIMER; i++)
    {
        if (timerctl.timers0[i].flags == 0)
        {
            timerctl.timers0[i].flags = TIMER_FLAGS_ALLOC;
            return &timerctl.timers0[i];
        }
    }
    return 0; /* can't find*/
}

void timer_free(struct TIMER *timer)
{
    timer->flags = 0; /* don't use*/
    return;
}

void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data)
{
    timer->fifo = fifo;
    timer->data = data;
    return;
}

void timer_settime(struct TIMER *timer, unsigned int timeout)
{
    int e;
    struct TIMER *t, *s;
    timer->timeout = timeout + timerctl.count;
    timer->flags = TIMER_FLAGS_USING;
    e = io_load_eflags();
    io_cli();
    t = timerctl.t0;
    if (timer->timeout <= t->timeout) {
        /* if only one timer in active */
        timerctl.t0 = timer;
        timer->next_timer = t;
        timerctl.next_time = timer->timeout;
        io_store_eflags(e);
        return;
    }
    /* find where to put in */
    for(;;) {
        s = t;
        t = t->next_timer;
        if ( timer->timeout <= t->timeout ) {
            /* if where to put between s and t */
            s->next_timer = timer;
            timer->next_timer = t;
            io_store_eflags(e);
            return;
        }
    }
}




void inthandler20(int *esp)
{
    struct TIMER *timer;
    io_out8(PIC0_OCW2, 0x60); /* recieve IRQ-00 send to PIC */
    timerctl.count++;
    if( timerctl.next_time > timerctl.count) {
        return;
    }
    timer = timerctl.t0;
    for (;;)
    {
        if (timer->timeout > timerctl.count) {
            break;
        }
        /* time out */
        timer->flags = TIMER_FLAGS_ALLOC;
        fifo32_put(timer->fifo, timer->data);
        timer = timer->next_timer;
    }
    /* new slide */
    timerctl.t0 = timer;
    /* timerctl.next setting */
    timerctl.next_time = timerctl.t0->timeout;
    return;
}
