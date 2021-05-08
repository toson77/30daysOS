#include "bootpack.h"
#define EFLAGS_AC_BIT     0x00040000
#define CR0_CACHE_DISABLE 0x60000000

unsigned int memtest(unsigned int start, unsigned int end)
{
  char flg486 = 0;
  unsigned int eflg, cr0, i;
  /*confirm 386 after 486 */
  eflg = io_load_eflags();
  eflg |= EFLAGS_AC_BIT; /* AC-bit = 1 */
  io_store_eflags(eflg);
  eflg = io_load_eflags();
  if((eflg & EFLAGS_AC_BIT) != 0){ /* if 386, did AC=1 but return AC=0  */
    flg486 = 1;
  }
  eflg &= ~EFLAGS_AC_BIT; /* AC-bit = 0 */
  io_store_eflags(eflg);

  if(flg486 != 0){
    cr0 = load_cr0();
    cr0 |= CR0_CACHE_DISABLE; /* cache disable */
    store_cr0(cr0);
  }

  i = memtest_sub(start, end);

  if(flg486 != 0){
    cr0 = load_cr0();
    cr0 &= ~CR0_CACHE_DISABLE; /* cache enable */
    store_cr0(cr0);
  }
  return i;
}

void memman_init(struct MEMMAN *man)
{
  man->frees = 0;     /* free info count */
  man->maxfrees = 0;  /* frees max fee */
  man->lostsize = 0;
  man->losts = 0;     /* lost count */
  return;
}

unsigned int memman_total(struct MEMMAN *man)
  /*report free size sum */
{
  unsigned int i, t = 0;
  for(i = 0; i < man->frees; i++){
    t += man->free[i].size;
  }
  return t;
}

unsigned int memman_alloc(struct MEMMAN *man, unsigned int size)
  /* catch */
{
  unsigned int i, a;
  for (i = 0; i < man->frees; i++){
    if(man->free[i].size >= size){
      a = man->free[i].addr;
      man->free[i].addr += size;
      man->free[i].size -= size;
      if(man->free[i].size == 0){
        man->frees--;
        for(; i < man->frees; i++){
          man->free[i] = man->free[i+1];
        }
      }
      return a;
    }
  }
  return 0;
}

int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size)
/* free */
{
  int i, j;
  for(i = 0; i< man->frees; i++){
    if(man->free[i].addr > addr){
      break;
    }
  }
  /* ree[i-1].addr < addr < free[i].addr */
  if( i > 0){
    if(man->free[i-1].addr + man->free[i-1].size == addr){
      man->free[i-1].size += size;
      if(i < man->frees){
        if(addr + size == man->free[i].addr){
          man->free[i-1].size += man->free[i].size;
          man->frees--;
          for(; i < man->frees; i++){
            man->free[i] = man->free[i+1];
          }
        }
      }
      return 0;
    }
  }
  if(i < man->frees){
    if(addr + size == man->free[i].addr){
      man->free[i].addr = addr;
      man->free[i].size += size;
      return 0;
    }
  }
  if(man->frees < MEMMAN_FREES){
    for(j = man->frees; j > i; j--){
      man->free[j] = man->free[j-1];
    }
    man->frees++;
    if(man->maxfrees < man->frees){
      man->maxfrees = man->frees;
    }
    man->free[i].addr = addr;
    man->free[i].size = size;
    return 0;
  }
  man->losts++;
  man->lostsize += size;
  return -1;
}


