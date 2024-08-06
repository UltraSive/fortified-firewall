ALTER TABLE "ip_address" ADD COLUMN "location_id" integer;--> statement-breakpoint
DO $$ BEGIN
 ALTER TABLE "ip_address" ADD CONSTRAINT "ip_address_location_id_location_id_fk" FOREIGN KEY ("location_id") REFERENCES "public"."location"("id") ON DELETE no action ON UPDATE no action;
EXCEPTION
 WHEN duplicate_object THEN null;
END $$;
