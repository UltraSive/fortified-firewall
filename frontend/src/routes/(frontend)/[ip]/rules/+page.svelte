<script lang="ts">
	import { page } from '$app/stores';
	import { enhance } from '$app/forms';

	import * as Dialog from '$lib/components/ui/dialog';
	import { Button, buttonVariants } from '$lib/components/ui/button';

	import { Plus, ArrowLeft } from 'lucide-svelte';

	import Spinner from "$lib/components/Spinner.svelte";

	import DataTable from './(components)/data-table.svelte';
	import CreateRule from './(components)/create-rule.svelte';

	let createRuleDialog = false;

	import type { PageData } from './$types';
	export let data: PageData;
</script>

<!-- <button on:click={() => toast("Hello world")}>Show toast</button>-->

<div class="flex justify-between items-center">
	<div>
		<h2 class="text-xl font-bold">{$page.params.ip}</h2>
		<a href="/" class="flex items-center text-muted-foreground hover:underline"
			><ArrowLeft class="w-4 h-4" />Addresses</a
		>
	</div>
	<Dialog.Root bind:open={createRuleDialog}>
		<Dialog.Trigger class={buttonVariants({ variant: 'outline' })}
			>Create Rule <Plus class="ml-2 h-4 w-4" /></Dialog.Trigger
		>
		<Dialog.Content class="sm:max-w-[425px] max-h-[98vh] overflow-y-auto">
			<form method="POST" action="?/createRule" use:enhance>
				<Dialog.Header>
					<Dialog.Title>Create Firewall Rule</Dialog.Title>
					<Dialog.Description>Define your firewall rule below.</Dialog.Description>
				</Dialog.Header>
				<CreateRule bind:open={createRuleDialog} />
			</form>
		</Dialog.Content>
	</Dialog.Root>
</div>
{#await data.address}
	<div class="my-4 flex justify-center">
		<Spinner />
	</div>
{:then address}
	<DataTable data={address.matchRules} />
{/await}
