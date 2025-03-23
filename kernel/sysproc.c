#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

#define MAXBIT 4096
uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
	myproc()->uksharedpg->pid = myproc()->pid;
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
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

uint64
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


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
		uint64 stvm;
		int pagenum;
		uint64 bitmask;
		
		if(argaddr(0, &stvm) < 0 || argint(1, &pagenum) < 0 || argaddr(2, &bitmask) < 0) {
			return -1;	
		}

		// using char tmpbitmask[MAXBIT] will make a kernel trap
		uint64 tmpbitmask;
	  uint64	vm;
		pte_t* pte;
		int bit;

		tmpbitmask = 0;
		for (bit = 0, vm = stvm; vm < stvm + pagenum * PGSIZE; bit += 1, vm += PGSIZE) {
			if ((pte = walk(myproc()->pagetable, vm, 0)) == 0) {
				return -1;
			}
			
			// previous error: *pte | PTE_A
			if (*pte & PTE_A) {
				tmpbitmask |= 1 << bit;
				*pte &= ~PTE_A;	
			}
		}
		// question: why use copyout instead of using bitmask directly
		if (copyout(myproc()->pagetable, bitmask, (char*)&tmpbitmask, pagenum) < 0) {
			return -1;
		}
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

