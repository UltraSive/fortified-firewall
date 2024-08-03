CREATE TABLE IF NOT EXISTS "action_value" (
	"id" serial PRIMARY KEY NOT NULL,
	"action" "action_type",
	"rate_limit_pps" integer,
	"xdp_sock" integer
);
--> statement-breakpoint
CREATE TABLE IF NOT EXISTS "ip_address" (
	"id" serial PRIMARY KEY NOT NULL,
	"address" varchar,
	"count" integer
);
--> statement-breakpoint
CREATE TABLE IF NOT EXISTS "location" (
	"id" serial PRIMARY KEY NOT NULL,
	"name" varchar(255)
);
--> statement-breakpoint
CREATE TABLE IF NOT EXISTS "node" (
	"id" serial PRIMARY KEY NOT NULL,
	"name" varchar(255),
	"location_id" integer
);
--> statement-breakpoint
DO $$ BEGIN
 ALTER TABLE "node" ADD CONSTRAINT "node_location_id_location_id_fk" FOREIGN KEY ("location_id") REFERENCES "public"."location"("id") ON DELETE no action ON UPDATE no action;
EXCEPTION
 WHEN duplicate_object THEN null;
END $$;
