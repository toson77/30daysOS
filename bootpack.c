extern void io_hlt(void);
//extern void write_mem8(int addr, int data);

void HariMain(void)
{
	char *p;
	int i;
	for( i = 0x000a0000; i <= 0x000affff; i++){
	//	write_mem8(i, i & 0x0f);
	//	write_mem8(i, 13);
		p = (char *) i;
		*p = i & 0x0f;
	}

	for(;;){
		io_hlt();
	}
}

