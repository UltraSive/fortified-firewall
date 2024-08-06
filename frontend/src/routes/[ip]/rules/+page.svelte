<script lang="ts">
	import DataTable from './(components)/data-table.svelte';
	import CreateRule from './(components)/create-rule-dialog.svelte';

	/** @type {import('./$types').PageData} */
	export let data;
</script>

<!-- <button on:click={() => toast("Hello world")}>Show toast</button>-->

<div class="flex justify-end">
	<CreateRule form={data.newRuleForm} />
</div>
<DataTable />
