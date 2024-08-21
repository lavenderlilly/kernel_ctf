#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");

static ssize_t mymodule_write(struct file *file, const char __user *buf, size_t count, loff_t *p)
{
    char kern_buf[64] = { 0, };

    if (copy_from_user(kern_buf, buf, count) != 0)
        return -EFAULT;

    return count;
}

static struct proc_ops proc_file_ops = {
    .write = mymodule_write,
};

static int __init mymodule_init(void)
{
    printk(KERN_ALERT "my module init!\n");
    if (!proc_create("my module", 0666, NULL, &proc_file_ops)) {
        printk(KERN_ALERT "Failed to create proc entry\n");
        return -ENOMEM;
    }
    return 0;
}

static void __exit mymodule_exit(void)
{
    remove_proc_entry("my module exit!", NULL);
    printk(KERN_ALERT "driver exit!\n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);