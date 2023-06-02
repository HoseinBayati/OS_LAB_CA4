#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "semaphore.h"

#define NUMSEMAPHORE 3

struct Semaphore sems[NUMSEMAPHORE];

void sem_init(uint i, uint v)
{
    sems[i].value = v;
    for (int j = 0; j < NPROC; j++)
        sems[i].procs[j] = 0;
    sems[i].first_proc = sems[i].last_proc = 0;
    return;
}

void sem_acquire(uint i)
{
    sems[i].value--;

    if (sems[i].value < 0)
    {
        sems[i].procs[sems[i].last_proc] = myproc();
        sems[i].last_proc = (sems[i].last_proc + 1) % NPROC;
        sleepcurrent();
    }
}

void sem_release(uint i)
{
    sems[i].value++;

    if (sems[i].procs[sems[i].first_proc])
    {
        wakeupprocess(sems[i].procs[sems[i].first_proc]);
        sems[i].procs[sems[i].first_proc] = 0;
        sems[i].first_proc = (sems[i].first_proc + 1) % NPROC;
    }
}