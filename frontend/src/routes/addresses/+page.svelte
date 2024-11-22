<script lang="ts">
	import AddressTable from './(components)/address-table.svelte';
	import NewAddress from './(components)/new-address.svelte';

	import type { PageData } from './$types';

	let { data }: { data: PageData } = $props();
</script>

<div class="space-y-4">
	<div class="flex justify-end">
    {#await data.locations then locations}
		  <NewAddress {locations} />
    {/await}
	</div>
	{#await data.addresses then addresses}
		<AddressTable {addresses}></AddressTable>
	{/await}
</div>
