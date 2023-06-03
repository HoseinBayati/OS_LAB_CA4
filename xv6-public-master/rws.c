#include "types.h"
#include "stat.h"
#include "user.h"
#include "semaphore.h"
#include "fcntl.h"

#define NUM_READERS 3
#define NUM_WRITERS 2

#define WRT 1
#define MUTEX 0

int read_count = 0;

int file_content = 0;

void writer(int id)
{
    sem_acquire(WRT);

    printf(1, "WRITER_CRITICAL BEGIN \n");

    file_content++;
    printf(1, "%d :: file_content: %d \n", id, file_content);

    printf(1, "WRITER_CRITICAL END \n");

    sem_release(WRT);
    exit();
}

void reader(int id)
{
    sem_acquire(MUTEX);
    read_count++;
    if (read_count == 1)
    {
        sem_acquire(WRT);
    }
    sem_release(MUTEX);

    printf(1, "READER_CRITICAL BEGIN \n");

    printf(1, "%d :: file_content: %d \n", id, file_content);

    printf(1, "READER_CRITICAL END \n");

    sem_acquire(MUTEX);
    read_count--;
    if (read_count == 0)
    {
        sem_release(WRT);
    }
    sem_release(MUTEX);

    exit();
}

int main(void)
{
    sem_init(WRT, 1);
    sem_init(MUTEX, 1);

    int pid;

    for (int i = 0; i < NUM_READERS; i++)
    {
        if ((pid = fork()) == 0)
        {
            reader(i);
        }
    }

    for (int i = 0; i < NUM_WRITERS; i++)
    {
        if ((pid = fork()) == 0)
        {
            writer(i);
        }
    }

    for (int i = 0; i < NUM_READERS + NUM_WRITERS; i++)
    {
        wait();
    }

    printf(1, "%d :: file_content: %d\n", getpid(), file_content);

    return 0;
}