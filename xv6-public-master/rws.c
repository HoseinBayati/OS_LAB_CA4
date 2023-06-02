#include "types.h"
#include "stat.h"
#include "user.h"
#include "semaphore.h"
#include "fcntl.h"

#define NUM_READERS 3
#define NUM_WRITERS 2
#define BUF_SIZE 512

#define WRT 2
#define MUTEX 1

int read_count = 0;
int file_content = 0;

void writer(int id)
{
    do
    {
        sem_acquire(WRT);

        printf(1, "WRITER_CRITICAL BEGIN \n");

        file_content++;
        printf(1, "%d :: file_content: %d\n", id, file_content);

        printf(1, "WRITER_CRITICAL END \n");

        sem_release(WRT);
        sleep(1);
    } while (1);
}

void reader(int id)
{
    do
    {
        sem_acquire(MUTEX);
        read_count++;
        if (read_count == 1)
        {
            sem_acquire(WRT);
        }
        sem_release(MUTEX);

        printf(1, "READER_CRITICAL BEGIN \n");

        printf(1, "%d :: file_content: %d\n", id, file_content);

        printf(1, "READER_CRITICAL END \n");

        sem_acquire(MUTEX);
        read_count--;
        if (read_count == 0)
        {
            sem_release(WRT);
        }
        sem_release(MUTEX);
        sleep(1);
    } while (1);
}

int main(void)
{
    sem_init(WRT, 1);
    sem_init(MUTEX, 1);

    int pid;

    for (int i = 0; i < NUM_WRITERS; i++)
    {
        if ((pid = fork()) == 0)
        {
            writer(i);
        }
    }

    for (int i = 0; i < NUM_READERS; i++)
    {
        if ((pid = fork()) == 0)
        {
            reader(i);
        }
    }

    printf(1, "%d :: file_content: %d\n", pid, file_content);

    exit();
    return 0;
}