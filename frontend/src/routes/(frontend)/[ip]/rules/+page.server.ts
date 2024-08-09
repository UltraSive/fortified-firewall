import type { Actions, PageServerLoad } from './$types';
import db from "$lib/db/db.server";
import { matchRule, ipAddress, actionValue, ipAddressRelations } from '$lib/db/schema';
import { eq } from 'drizzle-orm';

export const load: PageServerLoad = async ({ params }) => {
    /*const rules = await db.select({
        id: matchRule.id,
        name: matchRule.name,
        fields: matchRule.fields,
        action: actionValue.action,
        rateLimit: actionValue.rate_limit_pps,
        xdpSock: actionValue.xdp_sock,
        ipAddress: ipAddress.address,
      })
      .from(matchRule)
      .innerJoin(ipAddress, eq(matchRule.ip_address_id, ipAddress.id))
      .innerJoin(actionValue, eq(matchRule.action_id, actionValue.id))
      .where(eq(ipAddress.address, params.ip));*/

    const address = await db.query.ipAddress.findFirst({
        where: eq(ipAddress.address, params.ip),
        with: {
            matchRules: {
                with: {
                    actionValue: true
                }
            }
        }
    })

	return {
		address
	};
};

/*
export const actions = {
	createRule: async ({ event, request }) => {
        console.log("Create rule");

        const data = await request.formData();
		const dst_port = data.get('dst_port');

        console.log("dst_port: ", dst_port);

        return { success: true };
    },
} satisfies Actions;
 */