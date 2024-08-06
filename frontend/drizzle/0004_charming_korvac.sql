ALTER TABLE "action_value" DROP CONSTRAINT "action_value_match_id_match_rule_id_fk";
--> statement-breakpoint
ALTER TABLE "action_value" DROP COLUMN IF EXISTS "match_id";