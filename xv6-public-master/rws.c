#include "types.h"
#include "stat.h"
#include "user.h"
#include "semaphore.h"
#include "fcntl.h"

#define NUM_READERS 3
#define NUM_WRITERS 2
#define BUF_SIZE 512

#define WRITE 2
#define MUTEX 1

int read_count = 0;

int file_content = 0;

void writer()
{
    do
    {
        sem_acquire(WRITE);

        printf(1, "WRITER_CRITICAL BEGIN \n");
        // char buff[1024];
        // int fd;
        // fd = open("/rwss.txt", O_CREATE | O_RDWR);
        // strcpy(buff, "\nwriting to file!");
        // int bytesWritten = write(fd, buff, strlen(buff));
        // if (bytesWritten < 0)
        // {
        //     printf(1, "Error writing to the file\n");
        //     close(fd);
        //     exit();
        // }
        // printf(1, "Write successful. Wrote %d bytes to the file.\n", bytesWritten);

        // close(fd);
        file_content++;
        printf(1, "WRITER_CRITICAL END \n");

        sem_release(WRITE);
        sleep(10);
    } while (1);
}

void reader()
{
    do
    {
        sem_acquire(MUTEX);
        read_count++;
        if (read_count == 1)
        {
            sem_acquire(WRITE);
        }
        sem_release(MUTEX);

        printf(1, "READER_CRITICAL BEGIN \n");

        // char buff[1024];
        // memset(buff, 0, 1024);

        // int fd = open("/rwss.txt", O_RDONLY);
        // if (fd < 0)
        // {
        //     printf(1, "Error opening the file for reading\n");
        //     exit();
        // }

        // int bytesRead = read(fd, buff, 1024);
        // if (bytesRead < 0)
        // {
        //     printf(1, "Error reading from the file\n");
        //     close(fd);
        //     exit();
        // }
        // printf(1, "file content: %s\n", buff);
        // close(fd);
        printf(1, "file_content: %d", file_content);

        printf(1, "READER_CRITICAL END \n");

        sem_acquire(MUTEX);
        read_count--;
        if (read_count == 0)
        {
            sem_release(WRITE);
        }
        sem_release(MUTEX);
        sleep(10);
    } while (1);
}

int main()
{
    sem_init(WRITE, 1);
    sem_init(MUTEX, 1);
    read_count = 0;
    printf(1, "rws main\n");

    for (int i = 0; i < NUM_READERS; i++)
    {
        printf(1, "readers in::  %d\n", i);
        int pid = fork();
        if (pid == 0)
        {
            reader();
            printf(1, "readers out::  %d\n", i);
        }
    }

    for (int i = 0; i < NUM_WRITERS; i++)
    {
        printf(1, "writers in::  %d\n", i);

        int pid = fork();
        if (pid == 0)
        {
            writer();
            printf(1, "writers out::  %d\n", i);
        }
    }

    exit();
    return 0;
}