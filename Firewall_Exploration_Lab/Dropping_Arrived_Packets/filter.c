/**
 * @file filter.c
 * @author seed
 * @brief Dropping Arrived Packets
 * @version 0.1
 * @date 2022-09-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/skbuff.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

static struct nf_hook_ops nfho;

unsigned int
hook_func (
    void * priv,
    struct sk_buff* skb,
    const struct nf_hook_state * state
) {
    return NF_DROP;
}

static int 
kexec_test_init(void) {
    printk(KERN_INFO "kexec test start\n");
    nfho.hook = hook_func;
    nfho.hooknum = NF_INET_LOCAL_OUT;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;

    nf_register_net_hook(&init_net, &nfho);
    return 0;
}

static void
kexec_test_exit(void) {
    printk(KERN_INFO "kexec test end\n");
    nf_unregister_net_hook(&init_net, &nfho);
}

module_init(kexec_test_init);
module_exit(kexec_test_exit);
MODULE_LICENSE("GPL");