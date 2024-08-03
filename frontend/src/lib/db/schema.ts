// src/lib/db/schema.ts
import { relations } from 'drizzle-orm';
import { pgTable, pgEnum, serial, varchar, integer, bigint, jsonb, primaryKey, foreignKey } from "drizzle-orm/pg-core";

// Define the enum types
const actionTypeEnum = pgEnum("action_type", ["ALLOW", "BLOCK", "RATE_LIMIT", "REDIRECT"]);
const matchTypeEnum = pgEnum("match_type", ["DST_IP", "DST_PORT", "DST_PORT_RANGE", "PROTOCOL", "SRC_IP", "SRC_PORT", "SRC_PORT_RANGE", "ETHERTYPE", "VLAN_ID"]);

// Define the locations table
export const location = pgTable("location", {
    id: serial("id").primaryKey(),
    name: varchar("name", { length: 255 }),
  });
  
  // Define the nodes table
  export const node = pgTable("node", {
    id: serial("id").primaryKey(),
    name: varchar("name", { length: 255 }),
    location_id: integer("location_id").references(() => location.id),
  });

// Define the action_value table
export const actionValue = pgTable("action_value", {
    id: serial("id").primaryKey(),
    action: varchar("action"),
    rate_limit_pps: integer("rate_limit_pps"),
    xdp_sock: integer("xdp_sock"),
});

// Define the match_rule table
/* export const matchRule = pgTable("match_rule", {
    id: serial("id").primaryKey(),
    match_type: jsonb("match_type"),
    match_value: jsonb("match_value"),
    action_id: integer("action_id").references(() => actionValue.id),
    next_match_rule_key: integer("next_match_rule_key").references(() => matchRule.id),
}); */

// Define the firewall_index table
export const ipAddress = pgTable("ip_address", {
    id: serial("id").primaryKey(),
    address: varchar("address"),
    //initial_match_rule_key: integer("initial_match_rule_key").references(() => matchRule.id),
    count: integer("count"),
});