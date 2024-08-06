ALTER TABLE "action_value" ADD COLUMN "match_id" integer;--> statement-breakpoint
DO $$ BEGIN
 ALTER TABLE "action_value" ADD CONSTRAINT "action_value_match_id_match_rule_id_fk" FOREIGN KEY ("match_id") REFERENCES "public"."match_rule"("id") ON DELETE no action ON UPDATE no action;
EXCEPTION
 WHEN duplicate_object THEN null;
END $$;
