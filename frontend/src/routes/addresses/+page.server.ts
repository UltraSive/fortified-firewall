import type { PageServerLoad, Actions } from './$types';
import { ListAddress } from '$lib/server/helpers/addresses';

export const load: PageServerLoad = async ({ url }) => {
  const limit = url.searchParams.get('limit') ? Number(url.searchParams.get('limit')) : 20;
  const offset = url.searchParams.get('offset') ? Number(url.searchParams.get('offset')) : 0;

	return {
		addresses: await ListAddress(limit, offset)
	};
};

export const actions = {
	new: async (event) => {
		// TODO log the user in
	}
} satisfies Actions;