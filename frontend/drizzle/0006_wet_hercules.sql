ALTER TABLE "match_rule" ADD COLUMN "fields" jsonb;--> statement-breakpoint
ALTER TABLE "match_rule" DROP COLUMN IF EXISTS "match_type";--> statement-breakpoint
ALTER TABLE "match_rule" DROP COLUMN IF EXISTS "match_value";