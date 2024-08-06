CREATE TABLE IF NOT EXISTS "match_rule" (
	"id" serial PRIMARY KEY NOT NULL,
	"match_type" jsonb,
	"match_value" jsonb,
	"action_id" integer,
	"ip_address_id" integer
);
--> statement-breakpoint
ALTER TABLE "action_value" ALTER COLUMN "action" SET DATA TYPE varchar;--> statement-breakpoint
ALTER TABLE "node" ADD COLUMN "endpoint" varchar;--> statement-breakpoint
DO $$ BEGIN
 ALTER TABLE "match_rule" ADD CONSTRAINT "match_rule_action_id_action_value_id_fk" FOREIGN KEY ("action_id") REFERENCES "public"."action_value"("id") ON DELETE no action ON UPDATE no action;
EXCEPTION
 WHEN duplicate_object THEN null;
END $$;
--> statement-breakpoint
DO $$ BEGIN
 ALTER TABLE "match_rule" ADD CONSTRAINT "match_rule_ip_address_id_ip_address_id_fk" FOREIGN KEY ("ip_address_id") REFERENCES "public"."ip_address"("id") ON DELETE no action ON UPDATE no action;
EXCEPTION
 WHEN duplicate_object THEN null;
END $$;
