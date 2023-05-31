#include "param.h"
#include "spinlock.h"

typedef struct Semaphore
{
    unsigned int index; // for debug
    struct proc *procs[NPROC];
    unsigned int first;
    unsigned int last;
    unsigned int capacity;
    unsigned int value;
} Semaphore;