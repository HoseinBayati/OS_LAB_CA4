// semaphore.h

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "param.h"
#include "spinlock.h"

#define NUMSEMAPHORE 3

typedef struct semaphore
{
    int val;
    struct spinlock lock;
    struct proc *procs[NPROC];
    int next;
    int end;
} semaphore;

int sem_init(uint id, int v);
int sem_acquire(uint id);
int sem_release(uint id);

#endif  // SEMAPHORE_H
