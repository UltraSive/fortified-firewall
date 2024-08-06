//go:build ignore

#include "vmlinux.h"
// #include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
/*#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/tcp.h>
#include <linux/udp.h>*/

#define IP_PROTO_TCP 6
#define IP_PROTO_UDP 17
#define IP_PROTO_ICMP 1
#define IP6_PROTO_ICMPV6 58

// Define __constant_htons if not defined
#ifndef __constant_htons
#define __constant_htons(x) __builtin_bswap16(x)
#endif

// Define ETH_P_IP and ETH_P_IPV6 if not defined
#ifndef ETH_P_IP
#define ETH_P_IP 0x0800
#endif

#ifndef ETH_P_IPV6
#define ETH_P_IPV6 0x86DD
#endif

// Default no match action. 0 Block; 1 Allow
#ifndef NO_MATCH
#define NO_MATCH 1
#endif

/*
 * BPF Map Structs
 */
struct ipv4_lpm_trie_key
{
    __u32 prefixlen;
    __u32 ip;
};

struct ipv6_lpm_trie_key
{
    __u32 prefixlen;
    __u8 ip[16];
};

enum action_type
{
    ALLOW,
    BLOCK,
    RATE_LIMIT,
    REDIRECT
};

struct action_value
{
    // enum action_type action;
    __u32 action;
    __u64 last_seen_ns;   // Time of the last received packet in nanoseconds
    __u64 rate_limit_pps; // Rate limit in packets per second
    int xdp_sock;
};

struct firewall_index
{
    __u32 next_match_rule_key;
    __u32 count;
};

enum match_type
{
    DST_IP,
    DST_PORT,
    DST_PORT_RANGE,
    PROTOCOL,
    SRC_IP,
    SRC_PORT,
    SRC_PORT_RANGE,
    ETHERTYPE,
    VLAN_ID
};

// Define the maximum number of match types for a rule
#define MAX_MATCH_FIELDS 5

struct match_rule
{
    enum match_type match_type[MAX_MATCH_FIELDS];
    __u32 match_value[MAX_MATCH_FIELDS];
    struct action_value action;

    __u32 next_match_rule_key;
};

struct
{
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, __u32); // 0
    __type(value, __u64);
    __uint(max_entries, 1);
} pkt_count SEC(".maps");

/**
 * Default No Match Map
 *
 * If the packet is both ethernet and internet protocol and doesnt match other maps
 * what action should we handle.
 */
struct
{
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, __u32); // 0
    __type(value, struct action_value);
    __uint(max_entries, 1);
} no_match_action SEC(".maps");

/**
 * Source Maps
 *
 * Perform an action based on a source IP address and subnet mask only.
 */
/*
struct
{
    __uint(type, BPF_MAP_TYPE_LPM_TRIE);
    __type(key, struct ipv4_lpm_trie_key);
    __type(value, struct action_value);
    __uint(max_entries, 1024);
} ipv4_source_action SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_LPM_TRIE);
    __type(key, struct ipv6_lpm_trie_key);
    __type(value, struct action_value);
    __uint(max_entries, 1024);
} ipv6_source_action SEC(".maps");
*/
/**
 * Destination Maps
 *
 * Perform an action based on a destination IP address and subnet mask only.
 */
/*struct
{
    __uint(type, BPF_MAP_TYPE_LPM_TRIE);
    __type(key, struct ipv4_lpm_trie_key);
    __type(value, struct action_value);
    __uint(max_entries, 1024);
} ipv4_destination_action SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_LPM_TRIE);
    __type(key, struct ipv6_lpm_trie_key);
    __type(value, struct action_value);
    __uint(max_entries, 1024);
} ipv6_destination_action SEC(".maps");
*/

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, __u32);
    __type(value, sizeof(struct firewall_index));
    __uint(max_entries, 1024);
} ipv4_firewall_map SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, __u32);
    __type(value, sizeof(struct match_rule));
    __uint(max_entries, 4096);
} ipv4_match_rule_map SEC(".maps");

/**
 * Sockets Maps
 *
 * This map holds information regarding AF_XDP sockets.
 */
struct
{
    __uint(type, BPF_MAP_TYPE_XSKMAP);
    __uint(max_entries, 64);
    __type(key, __u32);
    __type(value, int);
} xsks_map SEC(".maps");

static __always_inline int handle_action(struct action_value *value, __u64 now_ns)
{
    __u64 time_diff_ns = now_ns - value->last_seen_ns;
    __u64 time_diff_s = time_diff_ns / 1000000000;
    __u64 current_pps = time_diff_s > 0 ? (1000000000 / time_diff_ns) : 0;

    switch (value->action)
    {
    case ALLOW:
        return XDP_PASS;
    case BLOCK:
        return XDP_DROP;
    case RATE_LIMIT:
        if (current_pps > value->rate_limit_pps)
        {
            return XDP_DROP;
        }
        else
        {
            value->last_seen_ns = now_ns;
            return XDP_PASS;
        }
    case REDIRECT:
        return XDP_PASS;
        // return bpf_redirect_map(&xsks_map, value->xdp_sock, 0);
    default:
        return XDP_DROP;
    }
}

static __always_inline int process_ipv4(struct xdp_md *ctx, struct ethhdr *eth, __u64 now_ns)
{
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct iphdr *ip = data + sizeof(*eth);
    if ((void *)(ip + 1) > data_end)
        return XDP_PASS;

    __u16 src_port;
    __u16 dst_port;

    // Convert the destination IP address to host byte order
    __u32 dest_ip = __builtin_bswap32(ip->daddr);

    // Lookup the first rule index using the destination IP
    struct firewall_index *firewall_index_ptr = bpf_map_lookup_elem(&ipv4_firewall_map, &dest_ip);
    if (!firewall_index_ptr)
    {
        bpf_printk("No rule found for dest_ip: %d.%d.%d.%d\n",
                   (ip->daddr >> 24) & 0xFF,
                   (ip->daddr >> 16) & 0xFF,
                   (ip->daddr >> 8) & 0xFF,
                   ip->daddr & 0xFF);
        if (NO_MATCH == 0)
        {
            return XDP_DROP;
        }
        else
        {
            return XDP_PASS;
        }
    }

    __u32 next_match_rule_key;

    if (firewall_index_ptr != NULL)
    {
        next_match_rule_key = firewall_index_ptr->next_match_rule_key;
    }

    bpf_printk("Initial Rule key: %u\n", next_match_rule_key);

    if (ip->protocol == IP_PROTO_TCP)
    {
        struct tcphdr *tcp = data + sizeof(*eth) + sizeof(*ip);
        if ((void *)(tcp + 1) > data_end)
            return XDP_PASS;

        src_port = tcp->source;
        dst_port = tcp->dest;
    }
    else if (ip->protocol == IP_PROTO_UDP)
    {
        struct udphdr *udp = data + sizeof(*eth) + sizeof(*ip);
        if ((void *)(udp + 1) > data_end)
            return XDP_PASS;

        src_port = udp->source;
        dst_port = udp->dest;
    }
    else
    {
        return XDP_PASS;
    }

    while (next_match_rule_key)
    {
        bpf_printk("While Loop: \n");

        struct firewall_index *match_rule = bpf_map_lookup_elem(&ipv4_match_rule_map, &next_match_rule_key);
        if (!match_rule)
        {
            bpf_printk("Rule not found for key: %u\n", next_match_rule_key);
            break;
        }
        
        bool match = true;
        for (int i = 0; i < MAX_MATCH_FIELDS; i++)
        {
            switch (rule->match_type[i])
            {
                case DST_IP:
                    if (dest_ip != rule->match_value[i])
                        match = false;
                    break;
                case SRC_IP:
                    if (__builtin_bswap32(ip->saddr) != rule->match_value[i])
                        match = false;
                    break;
                case DST_PORT:
                    if (dst_port != rule->match_value[i])
                        match = false;
                    break;
                case SRC_PORT:
                    if (src_port != rule->match_value[i])
                        match = false;
                    break;
                // Add other cases as needed
                default:
                    break;
            }
            if (!match)
                break;
        }

        if (match)
        {
            // Perform the action specified in the rule
            switch (rule->action.action)
            {
                case ALLOW:
                    return XDP_PASS;
                case BLOCK:
                    return XDP_DROP;
                case RATE_LIMIT:
                    // Implement rate limiting logic
                    break;
                case REDIRECT:
                    // Implement redirect logic
                    break;
                default:
                    return XDP_PASS;
            }
        }

        next_match_rule_key = rule->next_match_rule_key;
    }

    if (NO_MATCH == 0)
    {
        return XDP_DROP;
    }
    else
    {
        return XDP_PASS;
    }
}

/*
static __always_inline int process_ipv6(struct xdp_md *ctx, struct ethhdr *eth, __u64 now_ns)
{
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ipv6hdr *ip6 = data + sizeof(*eth);
    if ((void *)(ip6 + 1) > data_end)
        return XDP_PASS;

    struct ipv6_dst_punch_key key = {};
    //__builtin_memcpy(key.src_ip, ip6->saddr.s6_addr, 16);
    __builtin_memcpy(key.dst_ip, &ip6->daddr, 16);
    key.protocol = ip6->nexthdr;

    if (ip6->nexthdr == IP_PROTO_TCP)
    {
        struct tcphdr *tcp = data + sizeof(*eth) + sizeof(*ip6);
        if ((void *)(tcp + 1) > data_end)
            return XDP_PASS;

        // key.src_port = tcp->source;
        key.dst_port = tcp->dest;
    }
    else if (ip6->nexthdr == IP_PROTO_UDP)
    {
        struct udphdr *udp = data + sizeof(*eth) + sizeof(*ip6);
        if ((void *)(udp + 1) > data_end)
            return XDP_PASS;

        // key.src_port = udp->source;
        key.dst_port = udp->dest;
    }
    else
    {
        return XDP_PASS;
    }

    struct action_value *value = bpf_map_lookup_elem(&ipv6_dst_punch_action, &key);
    if (value)
    {
        return handle_action(value, now_ns);
    }
    return XDP_DROP;
}*/

SEC("xdp")
int firewall(struct xdp_md *ctx)
{
    /* START COUNT PACKET */
    __u32 key = 0;
    __u64 *count = bpf_map_lookup_elem(&pkt_count, &key);
    if (count)
    {
        __sync_fetch_and_add(count, 1);
    }
    /* END COUNT PACKET */

    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ethhdr *eth = data;
    if ((void *)(eth + 1) > data_end)
        return XDP_PASS;

    __u64 now_ns = bpf_ktime_get_ns();

    if (eth->h_proto == __constant_htons(ETH_P_IP))
    {
        return process_ipv4(ctx, eth, now_ns);
    }
    else if (eth->h_proto == __constant_htons(ETH_P_IPV6))
    {
        return process_ipv6(ctx, eth, now_ns);
    }

    /*struct action_value *value = bpf_map_lookup_elem(&no_match_action, 0);
    if (value)
    {
        return handle_action(value, now_ns);
    }*/
    return XDP_PASS;
}

char __license[] SEC("license") = "Dual MIT/GPL";