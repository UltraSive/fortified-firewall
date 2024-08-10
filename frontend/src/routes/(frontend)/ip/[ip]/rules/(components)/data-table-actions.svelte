<script lang="ts">
	import * as DropdownMenu from '$lib/components/ui/dropdown-menu';
	import { Button } from '$lib/components/ui/button';
	import { MoreHorizontal, ArrowDown, ArrowUp } from 'lucide-svelte';
	export let id: string;
</script>

<DropdownMenu.Root>
	<DropdownMenu.Trigger asChild let:builder>
		<Button variant="ghost" builders={[builder]} size="icon" class="relative w-8 h-8 p-0">
			<span class="sr-only">Open menu</span>
			<MoreHorizontal class="w-4 h-4" />
		</Button>
	</DropdownMenu.Trigger>
	<DropdownMenu.Content>
		<DropdownMenu.Group>
			<DropdownMenu.Label>Actions</DropdownMenu.Label>
			<!--<DropdownMenu.Item on:click={() => navigator.clipboard.writeText(id)}>
				Copy ID
			</DropdownMenu.Item>-->
			<DropdownMenu.Item on:click={() => navigator.clipboard.writeText(id)}>
				<ArrowUp class="mr-1 w-4 h-4" />Move Up
			</DropdownMenu.Item>
			<DropdownMenu.Item on:click={() => navigator.clipboard.writeText(id)}>
				<ArrowDown class="mr-1 w-4 h-4" />Move Down
			</DropdownMenu.Item>
		</DropdownMenu.Group>
		<DropdownMenu.Separator />
		<DropdownMenu.Item>Edit</DropdownMenu.Item>
		<!--<DropdownMenu.Item>Delete</DropdownMenu.Item>-->
	</DropdownMenu.Content>
</DropdownMenu.Root>
