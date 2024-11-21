<script lang="ts">
	import AddressTable from './(components)/address-table.svelte';

	import type { PageData } from './$types';

	let { data }: { data: PageData } = $props();
</script>

{#await data.addresses then addresses}
	<AddressTable {addresses}></AddressTable>
{/await}
