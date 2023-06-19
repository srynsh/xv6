#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_pgtprint(void) {
  pde_t *pgdir = myproc()->pgdir;
  if (pgdir == 0) {
    return -1;
  }

  for (int i = 0; i < NPDENTRIES; i++) {
    if (!pgdir[i]) {
      continue;
    }

    pte_t *pgtab = (pte_t*)P2V(PTE_ADDR(pgdir[i]));
    for (int j = 0; j < NPTENTRIES; j++) {
      pte_t pte = pgtab[j];
      if (pte & PTE_P && pte & PTE_U) {
        uint va = PGADDR(i, j, 0);
        uint pa = PTE_ADDR(pte);
        int wb = (pte & PTE_W) ? 1 : 0;
        cprintf("Entry number: %d, Virtual address: 0x%p, Physical address: 0x%p, wb is %d\n", i * NPTENTRIES + j, va, pa, wb);
      }
    }
  }

  return 0;
}
