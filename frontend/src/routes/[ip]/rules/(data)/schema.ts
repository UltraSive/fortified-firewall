import { z } from "zod";

export const ruleSchema = z.object({
    source: z.string().min(8).max(18).default("0.0.0.0/0"),
    source_asn: z.number().optional(),
    destination: z.string(),
    protocol: z.enum(["tcp", "udp", "icmp"]).default("tcp"),
    dst_port: z.string().optional(),
    src_port: z.string().optional(),
    rate_limiter_id: z.string().uuid().optional(),
    whitelist: z.boolean(),
    priority: z.boolean(),
    comment: z.string(),
});

export type RuleSchema = typeof ruleSchema;