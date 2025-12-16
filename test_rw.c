#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <string.h>

#define SYS_RW_READ_LOCK 471
#define SYS_RW_READ_UNLOCK 472
#define SYS_RW_WRITE_LOCK 473
#define SYS_RW_WRITE_UNLOCK 474

int main()
{
    long ret;

    // Reader acquires lock
    printf("Reader trying to acquire lock\n");
    ret = syscall(SYS_RW_READ_LOCK);
    if (ret == -1)
    {
        printf("Error acquiring read lock: %s\n", strerror(errno));
        return 1;
    }
    printf("Reader acquired lock\n");

    sleep(2); // Simulate reading

    // Reader releases lock
    ret = syscall(SYS_RW_READ_UNLOCK);
    if (ret == -1)
    {
        printf("Error releasing read lock: %s\n", strerror(errno));
        return 1;
    }
    printf("Reader released lock\n");

    // Writer acquires lock
    printf("Writer trying to acquire lock\n");
    ret = syscall(SYS_RW_WRITE_LOCK);
    if (ret == -1)
    {
        printf("Error acquiring write lock: %s\n", strerror(errno));
        return 1;
    }
    printf("Writer acquired lock\n");

    sleep(2); // Simulate writing

    // Writer releases lock
    ret = syscall(SYS_RW_WRITE_UNLOCK);
    if (ret == -1)
    {
        printf("Error releasing write lock: %s\n", strerror(errno));
        return 1;
    }
    printf("Writer released lock\n");

    return 0;
}