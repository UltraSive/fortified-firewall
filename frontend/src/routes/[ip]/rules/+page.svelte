<script lang="ts">
	import { page } from '$app/stores';
	import { enhance } from '$app/forms';

	import * as Dialog from '$lib/components/ui/dialog';
	import { Button, buttonVariants } from '$lib/components/ui/button';

	import { Plus } from 'lucide-svelte';

	import DataTable from './(components)/data-table.svelte';
	import CreateRule from './(components)/create-rule.svelte';

	/** @type {import('./$types').PageData} */
	export let data;
	console.log(data.address.matchRules);
</script>

<!-- <button on:click={() => toast("Hello world")}>Show toast</button>-->

<div class="flex justify-between items-center">
	<h2 class="text-xl font-bold">{$page.params.ip}</h2>
	<Dialog.Root>
		<Dialog.Trigger class={buttonVariants({ variant: 'outline' })}
			>Create Rule <Plus class="ml-2 h-4 w-4" /></Dialog.Trigger
		>
		<Dialog.Content class="sm:max-w-[425px] max-h-[98vh] overflow-y-auto">
			<form method="POST" action="?/createRule" use:enhance>
				<Dialog.Header>
					<Dialog.Title>Create Firewall Rule</Dialog.Title>
					<Dialog.Description>Define your firewall rule below.</Dialog.Description>
				</Dialog.Header>
				<CreateRule form={data.newRuleForm} />
			</form>
		</Dialog.Content>
	</Dialog.Root>
</div>
<DataTable data={data.address.matchRules}/>
