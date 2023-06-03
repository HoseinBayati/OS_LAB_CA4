#include "types.h"
#include "stat.h"
#include "user.h"
#include "semaphore.h"
#include "fcntl.h"

#define COUNTER_FILE "counter"
#define NUM_READERS 3
#define NUM_WRITERS 2
#define INC_QUANT 200
#define MUTEX 0
#define WRT 1

int read_count = 0;

int counter_init(char *filename, int value)
{
    int fd;

    if ((fd = open(filename, O_CREATE | O_RDWR)) < 0)
    {
        printf(1, "counter_init: error initializing file: %s\n", filename);
        exit();
    }

    printf(fd, "%d\n", value);
    close(fd);

    return 0;
}

int counter_get(char *filename)
{
    int fd, n, value;
    char buffer[32];

    if ((fd = open(filename, O_CREATE | O_RDWR)) < 0)
    {
        printf(1, "counter_get: error opening file: %s\n", filename);
        exit();
    }

    n = read(fd, buffer, 31);
    buffer[n] = '\0';
    value = atoi(buffer);
    close(fd);

    return value;
}

int counter_set(char *filename, int value)
{
    int fd;

    if ((fd = open(filename, O_CREATE | O_RDWR)) < 0)
    {
        printf(1, "counter_set: error opening file: %s\n", filename);
        exit();
    }

    printf(fd, "%d\n", value);
    close(fd);

    return value;
}

void writer(int id)
{

    int file_content;
    sleep(10);

    sem_acquire(WRT);

    printf(1, "WRITER_CRITICAL BEGIN \n");

    file_content = counter_get("counter");
    file_content++;
    printf(1, "%d :: file_content: %d \n", id, file_content);
    counter_set("counter", file_content);

    printf(1, "WRITER_CRITICAL END \n");

    sem_release(WRT);

    exit();
}

void reader(int id)
{
    int counter;

    sem_acquire(MUTEX);
    read_count++;
    if (read_count == 1)
    {
        sem_release(MUTEX);  
        sem_acquire(WRT);    
        sem_acquire(MUTEX);  
    }
    sem_release(MUTEX);

    printf(1, "READER_CRITICAL BEGIN \n");
    counter = counter_get("counter");
    printf(1, "%d :: file_content: %d \n", id, counter);

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
    int pid;

    sem_init(WRT, 1);
    sem_init(MUTEX, 1);
    counter_init(COUNTER_FILE, 0);

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

    int final_content = counter_get(COUNTER_FILE);
    printf(1, "Final content is %d\n", final_content);
    return 0;
}