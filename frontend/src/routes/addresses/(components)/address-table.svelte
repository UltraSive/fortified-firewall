<script lang="ts">
	import * as Table from '$lib/components/ui/table';
	import { Button } from '$lib/components/ui/button';

	import { Settings2 } from 'lucide-svelte';

	let { addresses } = $props();
</script>

<div class="rounded border">
	<Table.Root>
		<Table.Header>
			<Table.Row>
				<Table.Head class="w-[100px]">Address</Table.Head>
				<Table.Head>Version</Table.Head>
				<Table.Head>Location</Table.Head>
				<Table.Head>Default</Table.Head>
				<Table.Head>Firewall</Table.Head>
				<Table.Head class="text-right">Actions</Table.Head>
			</Table.Row>
		</Table.Header>
		<Table.Body>
			{#each addresses as address}
				<Table.Row>
					<Table.Cell class="font-medium">{address.id}</Table.Cell>
					<Table.Cell>{address.version}</Table.Cell>
					<Table.Cell>{address.location.id}</Table.Cell>
					<Table.Cell>{address.default}</Table.Cell>
					<Table.Cell
						>{#if address.firewall}{address.firewall.name}{:else}N/A{/if}</Table.Cell
					>
					<Table.Cell class="text-right"
						><Button href="addresses/{address.id}" variant="ghost" size="sm"><Settings2 class="h-4 w-4"></Settings2></Button
						></Table.Cell
					>
				</Table.Row>
			{/each}
		</Table.Body>
	</Table.Root>
</div>
