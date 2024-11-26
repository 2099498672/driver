// 函数原型：
//         register_chrdev_region(dev_t from, unsigned count, const char *name)
// 函数作用：
//         静态申请设备号，对指定好的设备号进行申请。
// 参数含义：
//         from: 自定义的 dev_t 类型设备号
//         count: 申请设备的数量
//         name: 申请的设备名称
// 函数返回值：
//         申请成功返回 0，申请失败返回负数

// 函数原型：
//         alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,const char *name)
// 函数作用：
//         动态申请设备号，内核会自动分配一个未使用的设备号，相较于静态申请设备号，动态申
//     请会避免注册设备号相同引发冲突的问题。
// 参数含义：
//         dev *: 会将申请完成的设备号保存在 dev 变量中
//         baseminor: 次设备号可申请的最小值
//         count: 申请设备的数量
//         name: 申请的设备名称
// 函数返回值：
//         申请成功返回 0，申请失败返回负数

// 内核源码/include/linux/kdev_t.h”中提供了设备号相关的宏定义
// dev_t 是一个无符号的 32 位整形类型,其中高 12 位表示主设备号，低 20 位表示次设备号

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
static dev_t dev_num;  // 定义 dev_t 类型（32 位大小）的变量 dev_num,用来存放设备号
struct cdev cdev_test; // 定义 cdev 结构体类型的变量 cdev_test
struct file_operations cdev_test_ops
{
    .owner = THIS_MODULE // 将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
}; // 定义 file_operations 结构体类型的变量 cdev_test_ops
static int __init module_cdev_init(void) // 驱动入口函数
{
    int ret;                                                  // 定义 int 类型变量 ret，进行函数返回值判断
    int major, minor;                                         // 定义 int 类型的主设备号 major 和次设备号 minor
    ret = alloc_chrdev_region(&dev_num, 0, 1, "chrdev_name"); // 自动获取设备号，设备名 chrdev_name
    if (ret < 0)
    {
        printk("alloc_chrdev_region is error\n");
    }
    printk("alloc_register_region is ok\n");
    major = MAJOR(dev_num); // 使用 MAJOR()函数获取主设备号
    minor = MINOR(dev_num); // 使用 MINOR()函数获取次设备号
    printk("major is %d\n", major);
    printk("minor is %d\n", minor);
    cdev_init(&cdev_test, &cdev_test_ops); // 使用 cdev_init()函数初始化 cdev_test 结构体，并链接到cdev_test_ops 结构体
    cdev_test.owner = THIS_MODULE; // 将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
    ret = cdev_add(&cdev_test, dev_num, 1); // 使用 cdev_add()函数进行字符设备的添加
    if (ret < 0)
    {
        printk("cdev_add is error\n");
    }
    printk("cdev_add is ok\n");
    return 0;
}
static void __exit module_cdev_exit(void) // 驱动出口函数
{
    cdev_del(&cdev_test);                 // 使用 cdev_del()函数进行字符设备的删除
    unregister_chrdev_region(dev_num, 1); // 释放字符驱动设备号
    printk("module exit \n");
}
module_init(module_cdev_init); // 注册入口函数
module_exit(module_cdev_exit); // 注册出口函数
MODULE_LICENSE("GPL v2");      // 同意 GPL 开源协议
MODULE_AUTHOR("topeet");       // 作者信息