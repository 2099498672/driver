// 1.节省主设备号:杂项设备的主设备号固定为 10，而字符设备不管是动态分配还是静态分
// 配设备号，都会消耗一个主设备号，进而造成了主设备号浪费。当系统中注册了多个 misc 设
// 备驱动时，只需使用子设备号进行区分即可。
// 2.使用简单：当使用普通的字符设备驱动时，如果开发人员需要导出操作接口给用户空间，
// 就需要注册对应的字符驱动，并创建字符设备 class 从而自动在/dev 下生成设备节点，而 misc
// 驱动只需要将基本信息通过结构体传递给相应处理函数即可。

// struct miscdevice {
//     int minor;          /* 子设备号 需要用户填写*/
//     const char *name;   /* 设备名 需要用户填写*/
//     const struct file_operations *fops;/* 设备操作集 需要用户填写*/
//     struct list_head list;
//     struct device *parent;
//     struct device *this_device;
//     const struct attribute_group **groups;
//     const char *nodename;
//     umode_t mode;
// };

// 杂项设备的注册：
// 函数原型：
//         int misc_register(struct miscdevice *misc)
// 函数作用：
//         基于misc_class构造一个设备，将miscdevice结构挂载到misc_list列表上，并初始化与linux
//     设备模型相关的结构。进而起到杂项设备注册的作用。
// 参数含义：
//         misc: 杂项设备的结构体指针
// 函数返回值：
//         申请成功返回 0，申请失败返回负数

// 杂项设备的卸载：
// 函数原型：
//         int misc_deregister(struct miscdevice *misc)
// 函数作用：
//         从 mist_list 中删除 miscdevice，进而起到杂项设备卸载的作用。
// 参数含义：
//         misc: 杂项设备的结构体指针
// 函数返回值：
//         卸载成功返回 0，申请失败返回负数

#include <linux/init.h>         // 初始化头文件
#include <linux/module.h>       // 支持动态添加，卸载头文件
#include <linux/miscdevice.h>   // 杂项设备头文件
#include <linux/fs.h>           // 注册设备节点的头文件

struct file_operations misc_fops
{
    .owner = THIS_MODULE,           // 将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模
};


struct miscdevice misc_dev{
    .minor = MISC_DYNAMIC_MINOR,    // 动态分配次设备号
    .name = "misc_dev",             // 设备名
    .fops = &misc_fops,             // 设备操作集
}
static int __init misc_init(void) {
    int ret;
    ret = misc_register(&misc_dev);
    if (ret < 0) {
        printk("misc_register failed!\n");
        return -1;
    }
    printk("misc_register success!\n");
    return 0;
}

static void __exit misc_exit(void) {
    misc_deregister(&misc_dev);
    printk("misc_deregister success!\n");
}

module_init(misc_init);
module_exit(misc_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("biu");