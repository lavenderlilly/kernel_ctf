#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");

static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *p)
{
    char kern_buf[64] = { 0, };

    if (copy_from_user(kern_buf, buf, count) != 0)
        return -EFAULT;

    return count;
}

static struct proc_ops proc_file_ops = {
    .proc_write = my_write,
};

static int __init my_init(void)
{
    printk(KERN_ALERT "my module init!\n");
    if (!proc_create("my", 0666, NULL, &proc_file_ops)) {
        printk(KERN_ALERT "Failed to create proc entry\n");
        return -ENOMEM;
    }
    return 0;
}

static void __exit my_exit(void)
{
    remove_proc_entry("my module exit!", NULL);
    printk(KERN_ALERT "driver exit!\n");
}

module_init(my_init);
module_exit(my_exit);
