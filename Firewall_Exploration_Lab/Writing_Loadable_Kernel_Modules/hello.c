/**
 * @file hello.c
 * @author seed 
 * @brief example code showing how to write a loadable kernel module
 * @version 0.1
 * @date 2022-07-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <linux/module.h>
#include <linux/kernel.h>

/**
 * @brief function to be executed while
 * the kernel module is being inserted
 * @return int 
 */
int initialization(void) {
    printk(KERN_INFO "Hello World!\n"); /* The kernel use printk() to print to the kernel log buffer */
    return 0;
}

/**
 * @brief function to be executed while
 * the kernel module is being removed from the kernel
 */
void cleanup(void) {
    printk(KERN_INFO "Bye-bye World!\n");
}

module_init(initialization); /* entry points for setup */
module_exit(cleanup); /* entry points for cleanup */

MODULE_LICENSE("GPL"); /* declares the license for this kernel module */