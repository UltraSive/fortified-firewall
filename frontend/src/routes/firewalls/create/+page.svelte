<script>
	import { Button } from '$lib/components/ui/button';

	import Editor from '$lib/components/firewall-editor.svelte';

	let rules = $state([
		{
			id: 1,
			name: 'Allow Internal Traffic',
			sourceIp: '192.168.1.0/24',
			destinationIp: '192.168.1.0/24',
			action: 'Allow'
		},
		{ id: 2, name: 'Block External SSH', sourceIp: 'Any', destinationIp: 'Any', action: 'Deny' },
		{
			id: 3,
			name: 'Allow HTTP/HTTPS',
			sourceIp: 'Any',
			destinationIp: '10.0.0.1',
			action: 'Allow'
		},
		{ id: 4, name: 'Deny All', sourceIp: 'Any', destinationIp: 'Any', action: 'Deny' }
	]);
</script>

<h1 class="my-4 text-3xl font-bold">Create Rule</h1>
<Editor bind:rules></Editor>
