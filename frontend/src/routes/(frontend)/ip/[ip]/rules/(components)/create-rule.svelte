<script lang="ts">
	import { page } from '$app/stores';

	import type { Match, Action } from '$lib/rules';
	import { protocols, actionTypes, matchTypes } from '$lib/rules';
	import { postMatchRule } from '$lib/rules';

	import { toast } from 'svelte-sonner';

	import * as Select from '$lib/components/ui/select';
	import { Button, buttonVariants } from '$lib/components/ui/button';
	import { Switch } from '$lib/components/ui/switch';
	import { Label } from '$lib/components/ui/label';
	import { Input } from '$lib/components/ui/input';
	import { Separator } from '$lib/components/ui/separator';

	import { Plus, Trash2 } from 'lucide-svelte';

	export let open;

	let name = 'Rule Name';

	let matches: Match[] = [
		{
			type: 'PROTOCOL',
			value: 'TCP'
		},
		{
			type: 'DST_PORT',
			value: '80'
		}
	];

	let action: Action = {
		type: 'ALLOW',
		value: undefined
	};

	$: console.log(action);

	function changeType(type, i) {
		matches[i].type = type;
	}

	function changeValue(value, i) {
		matches[i].value = value;
	}

	function changeAction(type) {
		console.log(action);
		action.type = type;
	}
</script>

<div class="space-y-4">
	<div>
		<Label for="name">Name</Label>
		<Input id="name" bind:value={name} />
	</div>
	<Separator />
	<div class="flex justify-between items-center">
		<h3 class="font-semibold text-lg">Matches</h3>
		<Button
			variant="outline"
			on:click={() => {
				console.log('Click');
				matches = [
					...matches,
					{
						type: 'DST_PORT',
						value: '80'
					}
				];
				console.log(matches);
			}}>Add Match<Plus class="ml-2 h-4 w-4" /></Button
		>
	</div>
	{#each matches as match, i}
		<div>
			<Label>Match #{i}</Label>
			<div class="space-y-1">
				<div class="flex space-x-2 items-center">
					<Select.Root selected={{ value: match.type, label: match.type }}>
						<Select.Trigger>
							<Select.Value />
						</Select.Trigger>
						<Select.Content>
							{#each matchTypes as type}
								<Select.Item value={type} on:click={() => changeType(type, i)}>{type}</Select.Item>
							{/each}
						</Select.Content>
					</Select.Root>
					<Button
						variant="destructive"
						on:click={() => {
							if (matches.length > 1) {
								matches = matches.filter((_, index) => index !== i);
							}
						}}><Trash2 class="h-4 w-4" /></Button
					>
				</div>

				{#if match.type === 'DST_PORT'}
					<Input bind:value={matches[i].value} />
				{:else if match.type === 'PROTOCOL'}
					<Select.Root>
						<Select.Trigger>
							<Select.Value />
						</Select.Trigger>
						<Select.Content>
							{#each protocols as protocol}
								<Select.Item value={protocol} on:click={() => changeValue(protocol, i)}
									>{protocol}</Select.Item
								>
							{/each}
						</Select.Content>
					</Select.Root>
				{/if}
			</div>
		</div>
	{/each}
	<Separator />
	<div>
		<Label>Action</Label>

		<div class="space-y-1">
			<Select.Root selected={{ value: action.type, label: action.type }}>
				<Select.Trigger>
					<Select.Value />
				</Select.Trigger>
				<Select.Content>
					{#each actionTypes as type}
						<Select.Item value={type} on:click={() => changeAction(type)}>{type}</Select.Item>
					{/each}
				</Select.Content>
			</Select.Root>
			{#if action.type === 'ALLOW' || action.type === 'BLOCK'}{:else}
				<Input type="number" bind:value={action.value} />
			{/if}
		</div>
	</div>
</div>
<div class="flex justify-end mt-4">
	<Button
		on:click={async () => {
			try {
				const rule = await postMatchRule($page.params.ip, name, matches, action);
				console.log(rule);
				toast.success('Created rule.');
				open = false;
				document.location.reload();
			} catch (error) {
				toast.success('Error creating rule.');
			}
		}}>Create Rule</Button
	>
</div>
