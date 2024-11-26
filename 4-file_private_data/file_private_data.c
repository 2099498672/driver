//     1.Linux 中并没有明确规定要使用文件私有数据，但是在 linux 驱动源码中，广泛使用了文
// 件私有数据，这是 Linux 驱动遵循的“潜规则”，实际上也体现了 Linux 面向对象的思想。struct
// file 结构体中专门为用户留了一个域用于定义私有数据。结构体内容如下所示： 
//     2.文件私有数据就是将私有数据
// private_data 指向设备结构体。通过它可以将私有数据一路从 open 函数带到 read, write 函数层
// 层传入。一般是在 open 的时候赋值，read、write 时使用。
// struct file
// {
//     union
//     {
//         struct llist_node fu_llist;
//         struct rcu_head fu_rcuhead;
//     } f_u;
//     struct path f_path;
//     struct inode *f_inode; /* cached value */
//     const struct file_operations *f_op;
//     /*
//      * Protects f_ep_links, f_flags. * Must not be taken from IRQ context. */
//     spinlock_t f_lock;
//     enum rw_hint f_write_hint;
//     atomic_long_t f_count;
//     unsigned int f_flags;
//     fmode_t f_mode;
//     struct mutex f_pos_lock;
//     loff_t f_pos;
//     struct fown_struct f_owner;
//     const struct cred *f_cred;
//     struct file_ra_state f_ra;
//     u64 f_version;
// #ifdef CONFIG_SECURITY
//     void *f_security;
// #endif
//     /* needed for tty driver, and maybe others */
//     void *private_data; // 私有数据
// #ifdef CONFIG_EPOLL
//     /* Used by fs/eventpoll.c to link all the hooks to this file */
//     struct list_headf_ep_links;
//     struct list_headf_tfile_llink;
// #endif /* #ifdef CONFIG_EPOLL */
//     struct address_space *f_mapping;
//     errseq_t f_wb_err;
// }

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

struct dev_data {
    dev_t dev_num;
    int major;
    int minor;
    struct cdev cdev_test;
    struct class *cls;
    struct device *dev;
    char *kbuf[128];
};

struct dev_data dev;

static int cdev_open(struct inode *inode, struct file *file)
{
    file->private_data = &dev;
    return 0;
}

static int cdev_release(struct inode *inode, struct file *file)
{
    printk("release\n");
    return 0;
}

static size_t cdev_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    struct dev_data *dev = (struct dev_data *)file->private_data;
    if (copy_to_user(buf, dev->kbuf, len) != 0) {
        return -1;
    }
    return len;
}

static ssize_t cdev_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    struct dev_data *dev = (struct dev_data *)file->private_data;
    if (copy_from_user(dev->kbuf, buf, len) != 0) {
        return -1;
    }
    return len;
}


struct file_operations cdev_fops = {
    .owner =    THIS_MODULE,
    .open = cdev_open,
    .release = cdev_release,
    .read = cdev_read,
    .write = cdev_write,
};
static int __init file_private_data_init(void)
{
    int ret;
    ret = alloc_chrdev_region(&dev.dev_num, 0, 1, "file_private_data");
    if (ret < 0) {
        printk("alloc_chrdev_region failed\n");
        return -1;
    }

    dev.major = MAJOR(dev.dev_num);
    dev.minor = MINOR(dev.dev_num);

    cdev_init(&dev.cdev_test, cdev_fops);
    dev.cdev_test.owner = THIS_MODULE;

    cdev_add(&dev.cdev_test, dev.dev_num, 1);

    dev.cls = class_create(THIS_MODULE, "file_private_data");
    dev.dev = device_create(dev.cls, NULL, dev.dev_num, NULL, "file_private_data");
    return 0;
}

static void __exit file_private_data_exit(void)
{
    device_destroy(dev.cls, dev.dev_num);
    class_destroy(dev.cls);
    cdev_del(&dev.cdev_test);
    unregister_chrdev_region(dev.dev_num, 1);
}


module_init(file_private_data_init);
module_exit(file_private_data_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("biu");
