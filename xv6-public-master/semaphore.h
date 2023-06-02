#include "param.h"
#include "spinlock.h"

typedef struct Semaphore
{
    struct proc *procs[NPROC];
    unsigned int first_proc;
    unsigned int last_proc;
    unsigned int value;
} Semaphore;