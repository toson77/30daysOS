#include "bootpack.h"
struct FIFO8 mousefifo;
void inthandler2c(int *esp)
/* PS/2マウスからの割り込み */
{
  unsigned char data;
  io_out8(PIC1_OCW2, 0x64);
  io_out8(PIC0_OCW2, 0x62);
  data = io_in8(PORT_KEYDAT);
  fifo8_put(&mousefifo, data);
  return;
}

#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4
void enable_mouse(struct MOUSE_DEC *mdec)
{
  wait_KBC_sendready();
  io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
  wait_KBC_sendready();
  io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
  /* if success, send 0xfa*/
  /* wait 0xfa */
  mdec->phase = 0;
  return;
}

int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
{
  if (mdec->phase == 0)
  {
    /*wait mouse 0xfa*/
    if (dat == 0xfa)
    {
      mdec->phase = 1;
    }
    return 0;
  }
  if (mdec->phase == 1)
  {
    /*wait mouse 1rd byte point*/
    if ((dat & 0xc8) == 0x08)
    {
      /* correct 1rd byte */
      mdec->buf[0] = dat;
      mdec->phase = 2;
    }
    return 0;
  }
  if (mdec->phase == 2)
  {
    /*wait mouse 2rd byte point */
    mdec->buf[1] = dat;
    mdec->phase = 3;
    return 0;
  }
  if (mdec->phase == 3)
  {
    /*wait mouse 3rd byte point */
    mdec->buf[2] = dat;
    mdec->phase = 1;
    mdec->btn = mdec->buf[0] & 0x07;
    mdec->x = mdec->buf[1];
    mdec->y = mdec->buf[2];
    if ((mdec->buf[0] & 0x10) != 0)
    {
      mdec->x |= 0xffffff00;
    }
    if ((mdec->buf[0] & 0x20) != 0)
    {
      mdec->y |= 0xffffff00;
    }
    /* On the mouse, the sign in the y direction is opposite to the screen */
    mdec->y = -mdec->y;
    return 1;
  }
  return -1;
}
