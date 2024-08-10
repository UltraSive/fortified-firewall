<script lang="ts">
	import Spinner from '$lib/components/Spinner.svelte';

	import DataTable from './(components)/data-table.svelte';

	import type { PageData } from './$types';

	export let data: PageData;
</script>

<div class="flex justify-between items-center">
	<h2 class="text-xl font-bold">Addresses</h2>
</div>
{#await data.addresses}
	<div class="my-4 flex justify-center">
		<Spinner />
	</div>
{:then addresses}
	<DataTable data={addresses} />
{/await}
