#include "param.h"
#include "spinlock.h"

typedef struct semaphore {
  unsigned int val;
  struct spinlock lock;
  void * procs[NPROC];
  unsigned int next;
  unsigned int end;
} semaphore;