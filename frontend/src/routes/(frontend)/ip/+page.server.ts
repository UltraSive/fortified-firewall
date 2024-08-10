import type { Actions, PageServerLoad } from '../../$types';
import db from "$lib/db/db.server";
import { matchRule, ipAddress, actionValue, ipAddressRelations } from '$lib/db/schema';
import { eq } from 'drizzle-orm';

async function loadAddresses() {
    const addresses = await db.query.ipAddress.findMany();

    return addresses;
}

export const load: PageServerLoad = async ({ params }) => {
	return {
		addresses: loadAddresses(),
	};
};

export const actions = {
	createRule: async ({ event, request }) => {
        console.log("Create rule");

        const data = await request.formData();
		const dst_port = data.get('dst_port');

        console.log("dst_port: ", dst_port);

        return { success: true };
    },
} satisfies Actions;