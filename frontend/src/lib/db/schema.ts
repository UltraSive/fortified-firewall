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
    endpoint: varchar("endpoint"),
    interface: varchar("interface", { length: 255 }),
    location_id: integer("location_id").references(() => location.id),
  });

// Define the action_value table
export const actionValue = pgTable("action_value", {
    id: serial("id").primaryKey(),
    type: varchar("type"),
    value: integer("value"),
    //match_id: integer("match_id").references(() => matchRule.id),
});

// Define the match_rule table
export const matchRule = pgTable("match_rule", {
    id: serial("id").primaryKey(),
    name: varchar("name"),
    fields: jsonb("fields"),
    action_id: integer("action_id").references(() => actionValue.id),
    ip_address_id: integer("ip_address_id").references(() => ipAddress.id),
});

// Define the firewall_index table
export const ipAddress = pgTable("ip_address", {
    id: serial("id").primaryKey(),
    address: varchar("address"),
    count: integer("count"),
    location_id: integer("location_id").references(() => location.id),
});

/**
 * Define Relations
 */
export const locationRelations = relations(location, ({ many }) => ({
    nodes: many(node),
    ipAddresses: many(ipAddress),
}));

export const nodeRelations = relations(node, ({ one }) => ({
    location: one(location, {
        fields: [node.location_id],
        references: [location.id],
    }),
}));

export const ipAddressRelations = relations(ipAddress, ({ one, many }) => ({
    matchRules: many(matchRule),
    location: one(location, {
        fields: [ipAddress.location_id],
        references: [location.id],
    }),
}));

export const matchRuleRelations = relations(matchRule, ({ one }) => ({
    ipAddress: one(ipAddress, {
        fields: [matchRule.ip_address_id],
        references: [ipAddress.id],
    }),
    actionValue: one(actionValue, {
        fields: [matchRule.action_id],
        references: [actionValue.id],
    }),
}));

/*export const actionValueRelations = relations(actionValue, ({ one }) => ({
    matchRules: one(matchRule, {
        fields: [actionValue.match_id],
        references: [matchRule.id],
    }),
}));*/