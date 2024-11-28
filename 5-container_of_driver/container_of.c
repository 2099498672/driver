//     container_of 在 Linux 内核中是一个常用的宏，用于从包含在某个结构中的指针获得结构本
// 身的指针，通俗地讲就是通过结构体变量中某个成员的首地址进而获得整个结构体变量的首地
// 址。那么可以使用这个函数获取不同设备的地址，来对不同的设备进行操作，从而一个驱动可
// 以兼容不同的设备。


// 函数原型：
//         container_of(ptr,type,member)
// 函数作用：
//         通过结构体变量中某个成员的首地址获取到整个结构体变量的首地址。
// 参数含义：
//         ptr 是结构体变量中某个成员的地址。
//         type 是结构体的类型
//         member 是该结构体变量的具体名字

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

struct device_test
{
    dev_t dev_num; 
    int major; 
    int minor; 
    struct cdev cdev_test; 
    struct class *class; 
    struct device *device; 
    char kbuf[32]; 
};

struct device_test dev1; 
struct device_test dev2; 

struct int chr_open(struct inode *inode, struct file *filp)
{
    file->private_data = container_of(inode->i_cdev, struct device_test, cdev_test);
    return 0;
}

struct int chr_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t chr_read(struct file *filp, char __user *buf, size_t cnt, loff_t *off)
{
    struct device_test *dev = (struct device_test *)filp->private_data;
    if (dev->major = dev1.major) {
        if (copy_to_user(buf, dev->kbuf, cnt)) {
            return -EFAULT;
        }
        return cnt;
    } else if ()
}

static ssize_t chr_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *off)
{

}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = chr_open,
    .release = chr_release,
    .read = chr_read,
    .write = chr_write,
};

static int chr_fops_init(void)
{
    int ret;
    ret = alloc_chrdev_region(&dev1.dev_num, 0, 1, "chr_dev1");
    if (ret < 0) {
        printk("alloc_chrdev_region failed\n");
        return -1;
    }
    ret = alloc_chrdev_region(&dev2.dev_num, 0, 1, "chr_dev2");
    if (ret < 0) {
        printk("alloc_chrdev_region failed\n");
        return -1;
    }

    dev1.major = MAJOR(dev1.dev_num);
    dev1.minor = MINOR(dev1.dev_num);
    cdev_init(&dev1.cdev_test, &fops);
    cdev_add(&dev1.cdev_test, dev1.dev_num, 1);
    dev1.cdev_test.owner = THIS_MODULE;
    dev1.class = class_create(THIS_MODULE, "chr_class1");
    dev1.device = device_create(dev1.class, NULL, dev1.dev_num, NULL, "chr_dev1");

    dev2.major = MAJOR(dev2.dev_num);
    dev2.minor = MINOR(dev2.dev_num);
    cdev_init(&dev2.cdev_test, &fops);
    cdev_add(&dev2.cdev_test, dev2.dev_num, 1);
    dev2.cdev_test.owner = THIS_MODULE;
    dev2.class = class_create(THIS_MODULE, "chr_class2");
    dev2.device = device_create(dev2.class, NULL, dev2.dev_num, NULL, "chr_dev2");

    return 0;
}
static void chr_fops_exit(void)
{

}

module_init(chr_fops_init);
module_exit(chr_fops_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("biu");