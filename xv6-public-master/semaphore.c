// semaphore.c

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "semaphore.h"

struct semaphore sems[NUMSEMAPHORE];

int sem_init(uint id, int v)
{
    if (id >= NUMSEMAPHORE)
    {
        return -1;  // Invalid semaphore ID
    }

    initlock(&sems[id].lock, "semaphore");

    acquire(&sems[id].lock);
    sems[id].val = v;
    sems[id].next = 0;
    sems[id].end = 0;
    for (int i = 0; i < NPROC; i++)
        sems[id].procs[i] = 0;
    release(&sems[id].lock);

    return 0;  // Semaphore initialized successfully
}

int sem_acquire(uint id)
{
    if (id >= NUMSEMAPHORE)
    {
        return -1;  // Invalid semaphore ID
    }

    acquire(&sems[id].lock);

    sems[id].val--;

    if (sems[id].val < 0)
    {
        int next = sems[id].end;
        sems[id].end = (sems[id].end + 1) % NPROC;

        if (sems[id].end == sems[id].next)
        {
            release(&sems[id].lock);
            return -2;  // Semaphore full (circular queue is full)
        }

        sems[id].procs[next] = myproc();
        sleep(sems[id].procs[next], &sems[id].lock);
    }

    release(&sems[id].lock);
    return 0;  // Semaphore acquired successfully
}

int sem_release(uint id)
{
    if (id >= NUMSEMAPHORE)
    {
        return -1;  // Invalid semaphore ID
    }

    acquire(&sems[id].lock);

    sems[id].val++;

    if (sems[id].val <= 0)
    {
        int next = sems[id].next;
        while (sems[id].procs[next] == 0)
        {
            next = (next + 1) % NPROC;
            if (next == sems[id].next)
            {
                release(&sems[id].lock);
                return -2;  // No processes waiting on the semaphore
            }
        }

        wakeup(sems[id].procs[next]);
        sems[id].procs[next] = 0;
        sems[id].next = next;
    }

    release(&sems[id].lock);
    return 0;  // Semaphore released successfully
}
