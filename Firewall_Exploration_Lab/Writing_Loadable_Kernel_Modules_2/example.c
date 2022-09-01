/**
 * @file example.c
 * @author seed 
 * @brief 学习编写可加载内核模块
 * @version 0.1
 * @date 2022-09-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

int kmodule_init(void) {
    printk(KERN_INFO "Initializing Kernel Module\n");
    return 0;
}

void kmodule_exit(void) {
    printk(KERN_INFO "Removing Kernel Module\n");
}

module_init(kmodule_init);
module_exit(kmodule_exit);
MODULE_LICENSE("GPL");