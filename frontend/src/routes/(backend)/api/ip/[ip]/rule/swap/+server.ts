import { json, error } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import db from "$lib/db/db.server";
import { matchRule, actionValue, ipAddress } from '$lib/db/schema';
import { eq } from 'drizzle-orm';

export const GET: RequestHandler = async ({ params, request }) => {
    try {

    } catch (error) {
        console.log(error);
        return json({
            status: "error",
            message: "Could not edit match rule."
        })
    }
};
