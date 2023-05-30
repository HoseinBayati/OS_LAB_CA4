#include "types.h"
#include "user.h"
#include "proc.h"

int main()
{
    struct semaphore sem;

    // Create a semaphore with an initial value of 1
    sem_init(&sem, 1);

    // Fork child processes
    printf(1, "TESTART\n");

    int pid = fork();
    printf(1, "id: %d\n", pid);

    if (pid < 0)
    {
        printf(1, "Fork failed\n");
        exit();
    }
    else if (pid == 0)
    {
        // Child process
        printf(1, "Child process\n");

        sem_acquire(&sem);
        printf(1, "Child process: Entered critical section\n");
        sleep(500);
        printf(1, "Child process: Exiting critical section\n");
        sem_release(&sem);
        exit();
    }
    else
    {
        // Parent process
        printf(1, "Parent process\n");

        sem_acquire(&sem);
        printf(1, "Parent process: Entered critical section\n");
        sleep(500);
        printf(1, "Parent process: Exiting critical section\n");
        sem_release(&sem);
        wait();
    }

    // Clean up the semaphore
    // sem_destroy(&sem);

    exit();
}
