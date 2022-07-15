/**
 * @file seedFilter.c
 * @author seed 
 * @brief implement a simple packet filter using the netfilter framework and loadable kernel module
 * Goal: block all the packet that are going out to port number 23
 * essentially preventing users from using telnet to connet other machines
 * @version 0.1
 * @date 2022-07-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/inet.h>

/**
 * @brief the callback function
 * 
 * @param priv 
 * @param skb 
 * @param state 
 * @return unsigned int 
 */
unsigned int
telnetFilter(void * priv, struct sk_buff * skb, const struct nf_hook_state * state) {
    struct iphdr * iph;
    struct tcphdr * tcph;

    iph = ip_hdr(skb);
    tcph = (void *)iph + iph->ihl * 4;
    if (iph->protocol == IPPROTO_TCP && tcph->dest == htons(23)) {
        printk(KERN_INFO "Dropping telnet packet to %d.%d.%d.%d\n", 
            ((u_char *)&iph->daddr)[0],
            ((u_char *)&iph->daddr)[1],
            ((u_char *)&iph->daddr)[2],
            ((u_char *)&iph->daddr)[3]);
        return NF_DROP;
    } else {
        return NF_ACCEPT;
    }
}

static struct nf_hook_ops telnetFilterHook;

int
setUpFilter(void) {
    printk(KERN_INFO "Registering a Telnet filter.\n");
    telnetFilterHook.hook = telnetFilter;
    telnetFilterHook.hooknum = NF_INET_POST_ROUTING;
    telnetFilterHook.pf = PF_INET;
    telnetFilterHook.priority = NF_IP_PRI_FIRST;
    /* Register the hook */
    nf_register_net_hook(&init_net, &telnetFilterHook);
    return 0;
}

void
removeFilter(void) {
    printk(KERN_INFO "Telnet filter is being removed.\n");
    nf_unregister_net_hook(&init_net, &telnetFilterHook);
}

module_init(setUpFilter);
module_exit(removeFilter);
MODULE_LICENSE("GPL");