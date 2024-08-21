#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");

static int __init hello_init(void)
{
    printk("hello world.\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk("Goodbye, cruel world.\n");
}

module_init(hello_init);
module_exit(hello_exit);
