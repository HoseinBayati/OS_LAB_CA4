#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "semaphore.h"

#define NUMSEMAPHORE 3

struct semaphore sems[NUMSEMAPHORE];

void sem_init(uint id, uint v)
{
    initlock(&sems[id].lock, (char *)&sems[id]);
    acquire(&sems[id].lock);

    sems[id].val = v;
    sems[id].next = sems[id].end = 0;

    for (int i = 0; i < NPROC; i++)
        sems[id].procs[i] = 0;

    release(&sems[id].lock);
}

void sem_acquire(uint id)
{
    acquire(&sems[id].lock);

    sems[id].val--;

    if (sems[id].val < 0)
    {
        sems[id].procs[sems[id].end] = myproc();
        sems[id].end = (sems[id].end + 1) % NPROC;
        sleepcurrent();
    }

    release(&sems[id].lock);
}

void sem_release(uint id)
{
    acquire(&sems[id].lock);

    sems[id].val++;

    if (sems[id].val <= 0 && sems[id].procs[sems[id].next] != 0)
    {
        wakeupprocess(sems[id].procs[sems[id].next]);
        sems[id].procs[sems[id].next] = 0;
        sems[id].next = (sems[id].next + 1) % NPROC;
    }

    release(&sems[id].lock);
}