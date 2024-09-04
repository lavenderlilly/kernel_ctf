#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>

 #if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
 #define HAVE_PROC_OPS
 #endif

#define PROCFS_NAME "prob"
#define MAX_LENGTH 64

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HSPACE CTF Challenge Kernel Module");

static struct proc_dir_entry *prob_proc;
static char procfs_buffer[MAX_LENGTH];
static size_t procfs_buffer_size = 0;

/* write */
static ssize_t prob_write(struct file *file, const char __user *ubuf, size_t count, loff_t *off)
{
    ssize_t ret;

    printk(KERN_INFO "prob: called prob_write\n");

    ret = copy_from_user(procfs_buffer, ubuf, count);
    
    if (ret != 0) {
        printk(KERN_INFO "prob: error copying data from userspace\n");
        return -EFAULT;
    }

    procfs_buffer_size = count;
    printk(KERN_INFO "prob: buf is %s\n", procfs_buffer);

    return count;
}

/* read */
static ssize_t prob_read(struct file *file, char __user *ubuf, size_t count, loff_t *off)
{
    char buf[MAX_LENGTH];
    size_t len;

    printk(KERN_INFO "prob: called prob_read\n");

    memset(buf, 0, sizeof(buf));
    strcpy(buf, "Welcome to the HSPACE CTF challenge. Best of luck!\n");

    if (copy_to_user(ubuf, buf, len)) {
        printk(KERN_ERR "prob: error copying data to userspace\n");
        return -EFAULT;
    }

    return len;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops prob_fops = {
    .proc_read = prob_read,
    .proc_write = prob_write,
};
#else
static const struct file_operations prob_fops = {
    .read = prob_read,
    .write = prob_write,
};
#endif

static int __init prob_init(void)
{
    printk(KERN_INFO "prob: loading module\n");
    prob_proc = proc_create(PROCFS_NAME, 0666, NULL, &prob_fops);
    
    if (!prob_proc) {
        printk(KERN_ERR "prob: could not create /proc/%s\n", PROCFS_NAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "prob: created /proc/%s entry\n", PROCFS_NAME);
    
    return 0;
}

static void __exit prob_exit(void)
{
    proc_remove(prob_proc);
    printk(KERN_INFO "prob: unloading module\n");
}

module_init(prob_init);
module_exit(prob_exit);
