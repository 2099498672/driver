//     1.在 Linux 内核中使用 atomic_t 和 atomic64_t 结构体分别来完成 32 位系统和 64 位系统的
// 整形数据原子操作，两个结构体定义在“内核源码/include/linux/types.h”文件中

// typedef struct {
//     int counter;
// } atomic_t;

// #ifdef CONFIG_64BIT
// typedef struct {
//     long counter;
// } atomic64_t;
// #endif

// ATOMIC_INIT(int i)                          定义原子变量的时候对其初始化，赋值为 i
// int atomic_read(atomic_t *v)                读取 v 的值，并且返回。
// void atomic_set(atomic_t *v, int i)         向原子变量 v 写入 i 值。
// void atomic_add(int i, atomic_t *v)         原子变量 v 加上 i 值。
// void atomic_sub(int i, atomic_t *v)         原子变量 v 减去 i 值。
// void atomic_inc(atomic_t *v)                原子变量 v 加 1
// void atomic_dec(atomic_t *v)                原子变量 v 减 1
// int atomic_dec_return(atomic_t *v)          原子变量 v 减 1，并返回 v 的值。
// int atomic_inc_return(atomic_t *v)          原子变量 v 加 1，并返回 v 的值。
// int atomic_sub_and_test(int i, atomic_t *v) 原子变量 v 减 i，如果结果为 0 就返回真，否则返回假
// int atomic_dec_and_test(atomic_t *v)        原子变量 v 减 1，如果结果为 0 就返回真，否则返回假
// int atomic_inc_and_test(atomic_t *v)        原子变量 v 加 1，如果结果为 0 就返回真，否则返回假
// int atomic_add_negative(int i, atomic_t *v) 原子变量 v 加 i，如果结果为负就返回真，否则返回假

//     2.和原子整形变量不同，原子位操作没有 atomic_t 的数据结
// 构，原子位操作是直接对内存进行操作，原子位操作相关 API 函数如下

// void set_bit(int nr, void *p)               将 p 地址的第 nr 位置 1。
// void clear_bit(int nr,void *p)              将 p 地址的第 nr 位清零。
// void change_bit(int nr, void *p)            将 p 地址的第 nr 位进行翻转。
// int test_bit(int nr, void *p)               获取 p 地址的第 nr 位的值。
// int test_and_set_bit(int nr, void *p)       将 p 地址的第 nr 位置 1，并且返回 nr 位原来的值。
// int test_and_clear_bit(int nr, void *p)     将 p 地址的第 nr 位清零，并且返回 nr 位原来的值。
// int test_and_change_bit(int nr, void *p)    将 p 地址的第 nr 位翻转，并且返回 nr 位原来的值。

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/atomic.h>
#include <linux/errno.h>

static atomic64_t v = ATOMIC_INIT(1); 

struct chrdev_test {
    dev_t dev_num;
    int maior;
    int minor;
    struct cdev cdev_test;
    struct class *cls;
    struct device *dev;
}