extern void io_hlt(void);
//extern void write_mem8(int addr, int data);

void HariMain(void)
{
	char *p;
	p = (char *) 0x000a0000;
	for( int i = 0; i <= 0xffff; i++){
	//	write_mem8(i, i & 0x0f);
	//	write_mem8(i, 13);
		p[i] = i & 0x0f;
	}

	for(;;){
		io_hlt();
	}
}

