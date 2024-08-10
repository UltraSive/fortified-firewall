import db from "$lib/db/db.server";
import { matchRule, ipAddress, actionValue, ipAddressRelations } from '$lib/db/schema';
import { eq } from 'drizzle-orm';

export async function listAllLocations() {
    const locations = await db.query.location.findMany();

    return locations;
}

export async function loadLocation(id: Number) {
    const foundLocation = await db.query.location.findFirst({
        where: eq(location.id, id),
        with: {
            nodes: true,
            ipAddress: true
        }
    });

    return foundLocation;
}