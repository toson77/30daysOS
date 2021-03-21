// bootpack.c

extern void io_hlt(void);
extern void io_cli(void);
extern void io_out8(int port, int data);
extern int io_load_eflags(void);
extern void io_store_eflags(int eflags);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram,
              int xsize,
              unsigned char c,
              int x0,
              int y0,
              int x1,
              int y1);
void init_screen(char *vram, int x, int y);

#define COL8_000000     0
#define COL8_FF0000     1
#define COL8_00FF00     2
#define COL8_FFFF00     3
#define COL8_0000FF     4
#define COL8_FF00FF     5
#define COL8_00FFFF     6
#define COL8_FFFFFF     7
#define COL8_C6C6C6     8
#define COL8_840000     9
#define COL8_008400     10
#define COL8_848400     11
#define COL8_000084     12
#define COL8_840084     13
#define COL8_008484     14
#define COL8_848484     15

void HariMain(void)
{
  unsigned char *vram;  // BYTE [...]$BMQHVCO(B
  int xsize, ysize;
  short *binfo_scrnx, *binfo_scrny;
  int *binfo_vram;
  init_palette();
  binfo_scrnx = (short *) 0x0ff4;
  binfo_scrny = (short *) 0x0ff6;
  binfo_vram = (int *) 0x0ff8;
  xsize = *binfo_scrnx;
  ysize = *binfo_scrny;
  vram = (char *) * binfo_vram;

  init_screen(vram, xsize, ysize);
 for(;;){
    io_hlt();
  }
}

void init_palette(void)
{
  static unsigned char table_rgb[16 * 3] = {
    0x00, 0x00, 0x00,   // 000000 : 0 : $B9u(B
    0xff, 0x00, 0x00,   // ff0000 : 1 : $BL@$k$$@V(B
    0x00, 0xff, 0x00,   // 00ff00 : 2 : $BL@$k$$NP(B
    0xff, 0xff, 0x00,   // ffff00 : 3 : $B2+?'(B
    0x00, 0x00, 0xff,   // 0000ff : 4 : $BL@$k$$@D(B
    0xff, 0x00, 0xff,   // ff00ff : 5 : $BL@$k$$;g(B
    0x00, 0xff, 0xff,   // 00ffff : 6 : $BL@$k$$?e?'(B
    0xff, 0xff, 0xff,   // ffffff : 7 : $BGr(B
    0xc6, 0xc6, 0xc6,   // c6c6c6 : 8 : $BL@$k$$3%?'(B
    0x84, 0x00, 0x00,   // 840000 : 9 : $B0E$$@V(B
    0x00, 0x84, 0x00,   // 008400 : 10: $B0E$$NP(B
    0x84, 0x84, 0x00,   // 848400 : 11: $B0E$$2+?'(B
    0x00, 0x00, 0x84,   // 000084 : 12: $B0E$$@D(B
    0x84, 0x00, 0x84,   // 840084 : 13: $B0E$$;g(B
    0x00, 0x84, 0x84,   // 008484 : 14: $B0E$$?e?'(B
    0x84, 0x84, 0x84,   // 848484 : 15: $B0E$$3%?'(B
  };
  set_palette(0, 15, table_rgb);
  return;

    // static char $BL?Na$O!"%G!<%?$K$7$+;H$($J$$$1$I(BDB$BL?NaC4Ev(B
}

void set_palette(int start, int end, unsigned char *rgb)
{
  int i, eflags;
  eflags = io_load_eflags();    // $B3d$j9~$_5v2D%U%i%0$NCM$r5-O?(B
  io_cli();                     // $B5v2D%U%i%0$r(B0$B$K$7$F3d$j9~$_$r6X;_$9$k(B
  io_out8(0x03c8, start);
  for (i = start; i <= end; i++){
    io_out8(0x03c9, rgb[0] / 4);  //R
    io_out8(0x03c9, rgb[1] / 4);  //G
    io_out8(0x03c9, rgb[2] / 4);  //B
    rgb += 3;
  }
  io_store_eflags(eflags);    // $B3d$j9~$_5v2D%U%i%0$r85$K$b$I$9(B
  return;
}

void boxfill8(unsigned char *vram,
              int xsize,
              unsigned char c,
              int x0,
              int y0,
              int x1,
              int y1)
{
  for(int y = y0; y <= y1; y++){
    for(int x = x0; x <= x1; x++){
      vram[y * xsize + x] = c;
    }
  }
}

void init_screen(char *vram, int xsize, int ysize)
{
  boxfill8(vram, xsize, COL8_008484,  0,         0,          xsize -  1, ysize - 29);
  boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 28, xsize -  1, ysize - 28);
  boxfill8(vram, xsize, COL8_FFFFFF,  0,         ysize - 27, xsize -  1, ysize - 27);
  boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 26, xsize -  1, ysize -  1);

  boxfill8(vram, xsize, COL8_FFFFFF,  3,         ysize - 24, 59,         ysize - 24);
  boxfill8(vram, xsize, COL8_FFFFFF,  2,         ysize - 24,  2,         ysize -  4);
  boxfill8(vram, xsize, COL8_848484,  3,         ysize -  4, 59,         ysize -  4);
  boxfill8(vram, xsize, COL8_848484, 59,         ysize - 23, 59,         ysize -  5);
  boxfill8(vram, xsize, COL8_000000,  2,         ysize -  3, 59,         ysize -  3);
  boxfill8(vram, xsize, COL8_000000, 60,         ysize - 24, 60,         ysize -  3);

  boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
  boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
  boxfill8(vram, xsize, COL8_FFFFFF, xsize - 47, ysize -  3, xsize -  4, ysize -  3);
  boxfill8(vram, xsize, COL8_FFFFFF, xsize -  3, ysize - 24, xsize -  3, ysize -  3);
return;
}

