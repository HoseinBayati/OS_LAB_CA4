#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "semaphore.h"

#define NUMSEMAPHORE 5

struct Semaphore semaphore[NUMSEMAPHORE];

void sem_init(uint i, uint v)
{
    semaphore[i].index = i;
    semaphore[i].value = 0;
    semaphore[i].capacity = v;
    for (int j = 0; j < NPROC; j++)
        semaphore[i].procs[j] = 0;
    semaphore[i].first = semaphore[i].last = 0;
    return;
}

void sem_acquire(uint i)
{
    xchg(&semaphore[i].value, semaphore[i].value + 1);
    if (semaphore[i].value > semaphore[i].capacity)
    {
        semaphore[i].procs[semaphore[i].last] = myproc();
        semaphore[i].last = (semaphore[i].last + 1) % NPROC;
        sleepcurrent();
    }
}

void sem_release(uint i)
{
    xchg(&semaphore[i].value, semaphore[i].value - 1);
    if(semaphore[i].procs[semaphore[i].first])
    {
        wakeupprocess(semaphore[i].procs[semaphore[i].first]);
        semaphore[i].procs[semaphore[i].first] = 0;
        semaphore[i].first = (semaphore[i].first + 1) % NPROC;
    }
}