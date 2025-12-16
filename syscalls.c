#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/rwsem.h>
#include <linux/uaccess.h>

static DECLARE_RWSEM(rw_lock);

/* Acquire read lock */
SYSCALL_DEFINE0(rw_read_lock)
{
    down_read(&rw_lock);
    printk(KERN_INFO "Reader acquired lock\n");
    return 0;
}

/* Release read lock */
SYSCALL_DEFINE0(rw_read_unlock)
{
    up_read(&rw_lock);
    printk(KERN_INFO "Reader released lock\n");
    return 0;
}

/* Acquire write lock */
SYSCALL_DEFINE0(rw_write_lock)
{
    down_write(&rw_lock);
    printk(KERN_INFO "Writer acquired lock\n");
    return 0;
}

/* Release write lock */
SYSCALL_DEFINE0(rw_write_unlock)
{
    up_write(&rw_lock);
    printk(KERN_INFO "Writer released lock\n");
    return 0;
}