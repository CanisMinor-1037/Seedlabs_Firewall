/**
 * @file helloFilter.c
 * @author seed 
 * @brief Hooking Functions to Netfilter
 * @version 0.1
 * @date 2022-07-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/socket.h>

/**
 * define two simple functions
 * each printing out a simple message
 */
unsigned int
hello1 (void * priv, struct sk_buff * skb, const struct nf_hook_state * state) {
    printk(KERN_INFO "*** Hello, netfilter 111111!\n");
    return NF_ACCEPT;
}

unsigned int
hello2 (void * priv, struct sk_buff * skb, const struct nf_hook_state * state) {
    printk(KERN_INFO "*** Hello, netfilter 222222!\n");
    return NF_DROP;
}

/**
 * registering hook functions
 */
static struct nf_hook_ops hook1, hook2;

int
registerFilter(void) {
    printk(KERN_INFO "Registering filters.\n");
    hook1.hook = hello1; /* Function Name */
    hook1.hooknum = NF_INET_LOCAL_OUT; /* Hook Number */
    hook1.pf = PF_INET;
    hook1.priority = -100; /* Priority Number */
    nf_register_net_hook(&init_net, &hook1); /* Attach the hook to netfilter */

    hook2.hook = hello2; /* Function Name */
    hook2.hooknum = NF_INET_LOCAL_OUT; /* Hook Number */
    hook2.pf = PF_INET;
    hook2.priority = -180; /* Higher Priority Than hook1 */
    nf_register_net_hook(&init_net, &hook2); /* Attach the hook to netfilter */

    return 0;
}

void
removeFilter(void) {
    printk(KERN_INFO "The filters are being removed.\n");
    nf_unregister_net_hook(&init_net, &hook1);
    nf_unregister_net_hook(&init_net, &hook2);
}

module_init(registerFilter);
module_exit(removeFilter);
MODULE_LICENSE("GPL");