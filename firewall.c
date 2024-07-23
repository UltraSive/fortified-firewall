//go:build ignore
#include "vmlinux.h"
//#include <linux/bpf.h>
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

struct ipv4_pair_key
{
    __u32 src_ip;
    __u32 dst_ip;
};

struct ipv6_pair_key
{
    __u8 src_ip[16];
    __u8 dst_ip[16];
};

struct ipv4_dst_punch_key
{
    //__u32 src_ip;
    __u32 dst_ip;
    //__u16 src_port;
    __u16 dst_port;
    __u8 protocol;
};

struct ipv6_dst_punch_key
{
    //__u8 src_ip[16];
    __u8 dst_ip[16];
    //__u16 src_port;
    __u16 dst_port;
    __u8 protocol;
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
    //enum action_type action;
    __u32 action;
    __u64 last_seen_ns;   // Time of the last received packet in nanoseconds
    __u64 rate_limit_pps; // Rate limit in packets per second
    int xdp_sock;
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
/**
 * Address Pair Maps
 *
 * Perform an action based on a source IP address and destination IP address
 * with subnet mask covering all bits. /32 for IPv4 and /128 for IPv6.
 */
struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, struct ipv4_pair_key);
    __type(value, struct action_value);
    __uint(max_entries, 1024);
} ipv4_pair_action SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, struct ipv6_pair_key);
    __type(value, struct action_value);
    __uint(max_entries, 1024);
} ipv6_pair_action SEC(".maps");

/**
 * Punch Maps
 *
 * This performs an action based on destination criteria.
 */
struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, struct ipv4_dst_punch_key);
    __type(value, struct action_value);
    __uint(max_entries, 1024);
} ipv4_dst_punch_action SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, struct ipv6_dst_punch_key);
    __type(value, struct action_value);
    __uint(max_entries, 1024);
} ipv6_dst_punch_action SEC(".maps");

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
        return XDP_DROP;
        //return bpf_redirect_map(&xsks_map, value->xdp_sock, 0);
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

    struct ipv4_dst_punch_key key = {};
    // key.src_ip = ip->saddr;
    key.dst_ip = ip->daddr;
    key.protocol = ip->protocol;

    if (ip->protocol == IP_PROTO_TCP)
    {
        struct tcphdr *tcp = data + sizeof(*eth) + sizeof(*ip);
        if ((void *)(tcp + 1) > data_end)
            return XDP_PASS;

        // key.src_port = tcp->source;
        key.dst_port = tcp->dest;
    }
    else if (ip->protocol == IP_PROTO_UDP)
    {
        struct udphdr *udp = data + sizeof(*eth) + sizeof(*ip);
        if ((void *)(udp + 1) > data_end)
            return XDP_PASS;

        // key.src_port = udp->source;
        key.dst_port = udp->dest;
    }
    else
    {
        return XDP_PASS;
    }

    struct action_value *value = bpf_map_lookup_elem(&ipv4_dst_punch_action, &key);
    if (value)
    {
        return handle_action(value, now_ns);
    }
    return XDP_DROP;
}

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
}

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