// 1.内核空间的代码更偏向于系统管理，而用户空间中的代码更偏重
// 业务逻辑实现
// 2.进程只有从用户空间切换到内核空间才可以使用系统的硬件资源，切换的方式有三种：系
// 统调用，软中断，硬中断

// kernel/include/linux/uaccess.h
// 函数原型：
//         unsigned long copy_to_user_inatomic(void __user *to, const void *from, unsigned long n);
// 函数作用：
//         把内核空间的数据复制到用户空间。
// 参数含义：
//         *to 是用户空间的指针
//         *from 是内核空间的指针
//         n 是从内核空间向用户空间拷贝的字节数

// 函数原型：
//         unsigned long copy_from_user(void *to, const void __user *from, unsigned long n)
// 函数作用：
//         把用户空间的数据复制到内核空间。
// 参数含义：
//         *to 是内核空间的指针
//         *from 是用户空间的指针
//         n 是从用户空间向内核空间拷贝的字节数

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

static dev_t dev_num;
static int major = 0;
static int minor = 0;

struct cdev cdev_test;

struct class *cls;
struct device *dev;

static int cdev_open(struct inode *inode, struct file *file) //打开设备文件时调用
{
    printk("cdev_open\n");
    return 0;
}

static ssize_t cdev_read(struct file *file, char __user *buf, size_t size, loff_t *off) //读取设备文件时调用
{
    char kbuf[128] = "copy_from_user success\n";
    if (copy_to_user(buf, kbuf, size) != 0) {
        printk("copy_to_user failed\n");
        return -1;
    }
    return size;
}

static ssize_t cdev_write(struct file *file, const char __user *buf, size_t size, loff_t *off) //写入设备文件时调用
{
    char kbuf[128] = {0};
    if (copy_from_user(kbuf, buf, size) != 0) {
        printk("copy_from_user failed\n");
        return -1;
    }
    return size;
}

static int cdev_release(struct inode *inode, struct file *file) //关闭设备文件时调用
{
    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE, 
    .open = cdev_open,
    .read = cdev_read,
    .write = cdev_write,
    .release = cdev_release,
};


static int __init chr_fops_init(void) //驱动入口函数
{
    int ret;
    ret = alloc_chrdev_region(&dev_num, 0, 1, "chr_fops");
    if (ret < 0) {
        printk("alloc_chrdev_region failed\n");
        return -1;
    }

    major = MAJOR(dev_num);
    minor = MINOR(dev_num);

    cdev_init(&cdev_test, &fops);
    cdev.owner = THIS_MODULE;
    cdev_add(&cdev_test, dev_num, 1);

    cls = class_create(THIS_MODULE, "chr_fops");
    dev = device_create(cls, NULL, dev_num, NULL, "chr_fops");
    return 0;
}

static void __exit chr_fops_exit(void) //驱动出口函数
{
    device_destroy(cls, dev_num);
    class_destroy(cls);
    cdev_del(&cdev_test);
    unregister_chrdev_region(dev_num, 1);
}

module_init(chr_fops_init); //注册入口函数
module_exit(chr_fops_exit); //注册出口函数
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("biu");