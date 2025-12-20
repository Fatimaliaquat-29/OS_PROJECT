#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define SYS_RW_READ_LOCK 471
#define SYS_RW_READ_UNLOCK 472
#define SYS_RW_WRITE_LOCK 473
#define SYS_RW_WRITE_UNLOCK 474

#define NUM_USERS 6 // Total processes (readers + writers)

void reader_process(int id)
{
    long ret;

    printf("[Reader %d] Trying to acquire read lock\n", id);
    ret = syscall(SYS_RW_READ_LOCK);
    if (ret == -1)
    {
        printf("[Reader %d] Error: %s\n", id, strerror(errno));
        exit(1);
    }

    printf("[Reader %d] Acquired read lock\n", id);
    sleep(2);

    ret = syscall(SYS_RW_READ_UNLOCK);
    if (ret == -1)
    {
        printf("[Reader %d] Error unlocking: %s\n", id, strerror(errno));
        exit(1);
    }

    printf("[Reader %d] Released read lock\n", id);
    exit(0);
}

void writer_process(int id)
{
    long ret;

    printf("[Writer %d] Trying to acquire write lock\n", id);
    ret = syscall(SYS_RW_WRITE_LOCK);
    if (ret == -1)
    {
        printf("[Writer %d] Error: %s\n", id, strerror(errno));
        exit(1);
    }

    printf("[Writer %d] Acquired write lock\n", id);
    sleep(3);

    ret = syscall(SYS_RW_WRITE_UNLOCK);
    if (ret == -1)
    {
        printf("[Writer %d] Error unlocking: %s\n", id, strerror(errno));
        exit(1);
    }

    printf("[Writer %d] Released write lock\n", id);
    exit(0);
}

int main()
{
    pid_t pid;

    for (int i = 0; i < NUM_USERS; i++)
    {
        pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0)
        {
            // Child process
            if (i % 2 == 0)
                reader_process(i);
            else
                writer_process(i);
        }
    }

    // Parent waits for all children
    for (int i = 0; i < NUM_USERS; i++)
        wait(NULL);

    printf("All users finished execution\n");
    return 0;
}
