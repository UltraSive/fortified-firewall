ALTER TABLE "action_value" RENAME COLUMN "xdp_sock" TO "value";--> statement-breakpoint
ALTER TABLE "action_value" DROP COLUMN IF EXISTS "rate_limit_pps";