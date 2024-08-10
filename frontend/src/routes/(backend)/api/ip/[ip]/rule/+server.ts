import { json, error } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import db from "$lib/db/db.server";
import { matchRule, actionValue, ipAddress } from '$lib/db/schema';
import { eq } from 'drizzle-orm';

export const POST: RequestHandler = async ({ params, request }) => {
  try {
    const { name, fields, action } = await request.json();
    console.log(fields);

    const address = await db.query.ipAddress.findFirst({
      where: eq(ipAddress.address, params.ip),
    });

    if (!address) {
      return error(404, 'IP address not found');
    }

    const [insertedAction] = await db.insert(actionValue).values({
      type: action.type,
      value: null,
    }).returning({ id: actionValue.id });

    const [insertedMatch] = await db.insert(matchRule).values({
      name: name, // replace with your match rule name
      fields: fields, // JSON field for the match criteria
      action_id: insertedAction.id, // foreign key to action_value
      ip_address_id: 1, // foreign key to ip_address
    }).returning({ id: matchRule.id });

    return json(insertedMatch);
  } catch (error) {
    console.log(error);
    return json({
      status: "error",
      message: "Could not post new match rule."
    })
  }
};

export const GET: RequestHandler = async ({ params }) => {
  try {
    return json({});
  } catch (error) {
    console.log(error);
    return json({
      status: "error",
      message: "Could not edit match rule."
    })
  }
};
/*
export const PUT: RequestHandler = async ({ params, request }) => {
  try {
    
  } catch (error) {
    console.log(error);
    return json({
      status: "error",
      message: "Could not edit match rule."
    })
  }
};

export const DELETE: RequestHandler = async ({ params, request }) => {
  try {
    
  } catch (error) {
    console.log(error);
    return json({
      status: "error",
      message: "Could not edit match rule."
    })
  }
};*/