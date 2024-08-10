import db from "$lib/db/db.server";
import { matchRule, ipAddress, actionValue, ipAddressRelations } from '$lib/db/schema';
import { eq } from 'drizzle-orm';

export async function listAllAddresses() {
    const addresses = await db.query.ipAddress.findMany();

    return addresses;
}

export async function loadAddress(ip: String) {
    const address = await db.query.ipAddress.findFirst({
        where: eq(ipAddress.address, ip),
        with: {
            matchRules: {
                with: {
                    actionValue: true
                }
            }
        }
    });

    return address
}