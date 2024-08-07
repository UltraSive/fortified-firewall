<script lang="ts">
	import DataTable from './(components)/data-table.svelte';

	import type { PageData } from './$types';

	export let data: PageData;
</script>

<div class="flex justify-between items-center">
	<h2 class="text-xl font-bold">Addresses</h2>
</div>
<DataTable data={data.address} />
