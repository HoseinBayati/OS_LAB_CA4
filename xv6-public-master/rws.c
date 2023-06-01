#include <pthread.h>
#include "types.h"
#include "stat.h"
#include "user.h"
#include "semaphore.h"
#include "fcntl.h"

#define NUM_READERS 3
#define NUM_WRITERS 2
#define BUF_SIZE 512

int read_count = 0;

void writer()
{
    while (1)
    {
        sem_acquire(3); // Acquire resource_mutex
        // Write operation

        int fd = open("output.txt", O_WRONLY | O_CREATE);
        if (fd < 0)
        {
            printf(1, "Error opening the file for writing\n");
            exit();
        }

        char *message = "Hello, xv6!";
        int bytesWritten = write(fd, message, strlen(message));
        if (bytesWritten < 0)
        {
            printf(1, "Error writing to the file\n");
            close(fd);
            exit();
        }

        printf(1, "Write successful. Wrote %d bytes to the file.\n", bytesWritten);

        // Close the file
        close(fd);

        sem_release(3); // Release resource_mutex

        sem_release(0); // Release rw_mutex
        sleep(1);
    }
}

void reader()
{
    while (1)
    {
        sem_acquire(1); // Acquire mutex
        read_count++;
        if (read_count == 1)
        {
            sem_acquire(0); // Acquire rw_mutex
        }
        sem_release(1); // Release mutex

        sem_acquire(3); // Acquire resource_mutex
        // Read operation
        int fd = open("readers_writers_shared_file.txt", O_RDONLY);

        if (fd < 0)
        {
            printf(1, "Error opening the file for reading\n");
            exit();
        }

        char buffer[BUF_SIZE];
        int bytesRead = read(fd, buffer, sizeof(buffer));
        if (bytesRead < 0)
        {
            printf(1, "Error reading from the file\n");
            close(fd);
            exit();
        }

        printf(1, "File contents:\n");
        write(1, buffer, bytesRead); // Write to stdout
        close(fd);

        sem_release(3); // Release resource_mutex

        sem_acquire(1); // Acquire mutex
        read_count--;
        if (read_count == 0)
        {
            sem_release(0); // Release rw_mutex
        }
        sem_release(1); // Release mutex

        sleep(1);
    }
}

int main()
{
    // Initialize semaphores and variables
    sem_init(0, 1); // rw_mutex
    sem_init(1, 1); // mutex
    sem_init(3, 1); // resource_mutex
    read_count = 0;
    printf(1, "rws main\n");

    // Create reader processes
    for (int i = 0; i < NUM_READERS; i++)
    {
        printf(1, "readers in::  %d\n", i);
        // Create a new process for each reader
        int pid = fork();
        if (pid == 0)
        {
            reader();
        }
        printf(1, "readers out::  %d\n", i);
    }

    // Create writer processes
    for (int i = 0; i < NUM_WRITERS; i++)
    {
        // Create a new process for each writer
        printf(1, "writers in::  %d\n", i);

        int pid = fork();
        if (pid == 0)
        {
            writer();
        }
        printf(1, "writers out::  %d\n", i);
    }

    exit();
    return 0;
}