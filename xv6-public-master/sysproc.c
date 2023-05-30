#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sem_init(struct semaphore* sem, int value);
int sem_acquire(struct semaphore* sem);
int sem_release(struct semaphore* sem);


int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
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
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


int sys_sem_init(void) {
  int value;
  if (argint(0, &value) < 0)
    return -1;

  struct semaphore* sem;
  if (argptr(1, (void*)&sem, sizeof(*sem)) < 0)
    return -1;

  sem_init(sem, value);

  return 111;
}

int sys_sem_acquire(void) {
  struct semaphore* sem;
  if (argptr(0, (void*)&sem, sizeof(*sem)) < 0)
    return -1;

  sem_acquire(sem);

  return 222;
}

int sys_sem_release(void) {
  struct semaphore* sem;
  if (argptr(0, (void*)&sem, sizeof(*sem)) < 0)
    return -1;

  sem_release(sem);

  return 333;
}

int sys_sem_destroy(void) {

  return 444;
}
