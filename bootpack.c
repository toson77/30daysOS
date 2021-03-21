// bootpack.c

extern void io_hlt(void);
extern void io_cli(void);
extern void io_out8(int port, int data);
extern int io_load_eflags(void);
extern void io_store_eflags(int eflags);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);

void HariMain(void)
{
  char *p;  // BYTE [...]$BMQHVCO(B
  p = (char *) 0xa0000;

  for (int i=0; i <= 0xffff; i++){
    p[i] = i & 0x0f;
  }

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
    io_out8(0x03c9, rgb[0] / 4);
    io_out8(0x03c9, rgb[1] / 4);
    io_out8(0x03c9, rgb[2] / 4);
    rgb += 3;
  }
  io_store_eflags(eflags);    // $B3d$j9~$_5v2D%U%i%0$r85$K$b$I$9(B
  return;
}
