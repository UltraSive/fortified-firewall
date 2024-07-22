//go:build ignore

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/tcp.h>
#include <linux/udp.h> 

#define IP_PROTO_TCP 6
#define IP_PROTO_UDP 17
#define IP_PROTO_ICMP 1
#define IP6_PROTO_ICMPV6 58

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY); 
    __type(key, __u32);
    __type(value, __u64);
    __uint(max_entries, 1);
} pkt_count SEC(".maps"); 

struct ipv4_flowspec_key {
    __u32 src_ip;
    __u32 dst_ip;
    __u16 src_port;
    __u16 dst_port;
    __u8  protocol;
};

struct ipv6_flowspec_key {
    __u8 src_ip[16];
    __u8 dst_ip[16];
    __u16 src_port;
    __u16 dst_port;
    __u8  protocol;
};

enum action_type {
    ALLOW,
    BLOCK,
    RATE_LIMIT
};

struct flowspec_value {
    enum action_type action;
    __u64 last_seen_ns;  // Time of the last received packet in nanoseconds
    __u64 rate_limit_pps;  // Rate limit in packets per second
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, struct ipv4_flowspec_key);
    __type(value, struct flowspec_value);
    __uint(max_entries, 1024);
} ipv4_firewall SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, struct ipv6_flowspec_key);
    __type(value, struct flowspec_value);
    __uint(max_entries, 1024);
} ipv6_firewall SEC(".maps");

static __always_inline int handle_action(struct flowspec_value *value, __u64 now_ns) {
    __u64 time_diff_ns = now_ns - value->last_seen_ns;
    __u64 time_diff_s = time_diff_ns / 1000000000;
    __u64 current_pps = time_diff_s > 0 ? (1000000000 / time_diff_ns) : 0;

    switch (value->action) {
        case ALLOW:
            return XDP_PASS;
        case BLOCK:
            return XDP_DROP;
        case RATE_LIMIT:
            if (current_pps > value->rate_limit_pps) {
                return XDP_DROP;
            } else {
                value->last_seen_ns = now_ns;
                return XDP_PASS;
            }
    }
    return XDP_DROP;
}

static __always_inline int process_ipv4(struct xdp_md *ctx, struct ethhdr *eth, __u64 now_ns) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct iphdr *ip = data + sizeof(*eth);
    if ((void *)(ip + 1) > data_end)
        return XDP_PASS;

    struct ipv4_flowspec_key key = {};
    key.src_ip = ip->saddr;
    key.dst_ip = ip->daddr;
    key.protocol = ip->protocol;

    if (ip->protocol == IP_PROTO_TCP) {
        struct tcphdr *tcp = data + sizeof(*eth) + sizeof(*ip);
        if ((void *)(tcp + 1) > data_end)
            return XDP_PASS;

        key.src_port = tcp->source;
        key.dst_port = tcp->dest;
    } else if (ip->protocol == IP_PROTO_UDP) {
        struct udphdr *udp = data + sizeof(*eth) + sizeof(*ip);
        if ((void *)(udp + 1) > data_end)
            return XDP_PASS;

        key.src_port = udp->source;
        key.dst_port = udp->dest;
    } else {
        return XDP_PASS;
    }

    struct flowspec_value *value = bpf_map_lookup_elem(&ipv4_firewall, &key);
    if (value) {
        return handle_action(value, now_ns);
    }
    return XDP_DROP;
}

static __always_inline int process_ipv6(struct xdp_md *ctx, struct ethhdr *eth, __u64 now_ns) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ipv6hdr *ip6 = data + sizeof(*eth);
    if ((void *)(ip6 + 1) > data_end)
        return XDP_PASS;

    struct ipv6_flowspec_key key = {};
    __builtin_memcpy(key.src_ip, ip6->saddr.s6_addr, 16);
    __builtin_memcpy(key.dst_ip, ip6->daddr.s6_addr, 16);
    key.protocol = ip6->nexthdr;

    if (ip6->nexthdr == IP_PROTO_TCP) {
        struct tcphdr *tcp = data + sizeof(*eth) + sizeof(*ip6);
        if ((void *)(tcp + 1) > data_end)
            return XDP_PASS;

        key.src_port = tcp->source;
        key.dst_port = tcp->dest;
    } else if (ip6->nexthdr == IP_PROTO_UDP) {
        struct udphdr *udp = data + sizeof(*eth) + sizeof(*ip6);
        if ((void *)(udp + 1) > data_end)
            return XDP_PASS;

        key.src_port = udp->source;
        key.dst_port = udp->dest;
    } else {
        return XDP_PASS;
    }

    struct flowspec_value *value = bpf_map_lookup_elem(&ipv6_firewall, &key);
    if (value) {
        return handle_action(value, now_ns);
    }
    return XDP_DROP;
}

SEC("xdp")
int firewall(struct xdp_md *ctx) {
    /* START COUNT PACKET */
    __u32 key    = 0; 
    __u64 *count = bpf_map_lookup_elem(&pkt_count, &key); 
    if (count) { 
        __sync_fetch_and_add(count, 1); 
    }
    /* END COUNT PACKET */

    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ethhdr *eth = data;
    if ((void *)(eth + 1) > data_end)
        return XDP_PASS;

    __u64 now_ns = bpf_ktime_get_ns();

    if (eth->h_proto == __constant_htons(ETH_P_IP)) {
        return process_ipv4(ctx, eth, now_ns);
    } else if (eth->h_proto == __constant_htons(ETH_P_IPV6)) {
        return process_ipv6(ctx, eth, now_ns);
    }

    return XDP_PASS;
}

char __license[] SEC("license") = "Dual MIT/GPL";