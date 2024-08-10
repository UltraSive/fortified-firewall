import db from "$lib/db/db.server";
import { matchRule, ipAddress, actionValue, ipAddressRelations } from '$lib/db/schema';
import { eq } from 'drizzle-orm';

export async function getAllAddresses() {
    const addresses = await db.query.ipAddress.findMany();

    return addresses;
}

export 