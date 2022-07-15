/**
 * @file seedFilter.c
 * @author seed 
 * @brief implement a simple packet filter using netfilter
 * Goal: block all the DNS query going to the DNS Server 8.8.8.8
 * Step:
 *  1. implement a filtering function
 *  2. hook the function to one of the netfilter hooks
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
#include <linux/udp.h>
#include <linux/inet.h>

/**
 * @brief 
 * blocks any UDP packet if its destination IP is 8.8.8.8 and the destination port is 53.
 * The rule is hardcoded inside this hook function
 * @param priv 
 * @param skb the pointer to the actual packet
 * @param state 
 * @return unsigned int 
 */
unsigned int
blockUDP(void * priv, struct sk_buff * skb, const struct nf_hook_state * state) {
    struct iphdr * iph; /* IP header */
    struct udphdr * udph; /* UDP header */
    u32 ip_addr;
    char ip[16] = "8.8.8.8";

    /* Convert the IPv4 address from dotted decimal to a 32-bit number */
    /* string "1.2.3.4" to 32-bit binary 0x01020304 */
    in4_pton(ip, -1, (u8 *)&ip_addr, '\0', NULL);

    iph = ip_hdr(skb); /* Get the IP header of the packet */
    if (iph->protocol == IPPROTO_UDP) {
        udph = udp_hdr(skb); /* Get the UDP header of the packet */
        /* compare the destination IP address and the port number with the values in our specified rule */
        if (iph->daddr == ip_addr && ntohs(udph->dest) == 53) {
            printk(KERN_DEBUG "****Dropping %pI4 (UDP), port %d\n", &(iph->daddr), ntohs(udph->source));
            return NF_DROP; /* Drop the packet */
        }
    }
    return NF_ACCEPT; /* Let the packet continur its journal */
}

static struct nf_hook_ops udpFilterHook;

int
setUpFilter(void) {
    printk(KERN_INFO "Registering a UDP filter.\n");
    udpFilterHook.hook = blockUDP;
    udpFilterHook.hooknum = NF_INET_POST_ROUTING;
    udpFilterHook.pf = PF_INET;
    udpFilterHook.priority = NF_IP_PRI_FIRST;
    /* Register the hook */
    nf_register_net_hook(&init_net, &udpFilterHook);
    return 0;
}

void
removeFilter(void) {
    printk(KERN_INFO "UDP Filter is being Removed.\n");
    nf_unregister_net_hook(&init_net, &udpFilterHook);
}

module_init(setUpFilter);
module_exit(removeFilter);
MODULE_LICENSE("GPL");