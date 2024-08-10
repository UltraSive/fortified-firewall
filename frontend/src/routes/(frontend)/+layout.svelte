<script>
	import '../../app.css';

	import { page } from '$app/stores';

	import { ModeWatcher } from 'mode-watcher';
	import { Toaster } from '$lib/components/ui/sonner';

	import { setMode } from 'mode-watcher';
	setMode('dark');

	import { ArrowLeft } from 'lucide-svelte';

	import { Separator } from '$lib/components/ui/separator';

	import Nav from '$lib/components/Nav.svelte';

	export let items = [
		{
			title: 'Addresses',
			href: `/`
		},
		{
			title: 'Locations',
			href: `/locations`
		},
		{
			title: 'Nodes',
			href: `/nodes`
		},
	];
</script>

<ModeWatcher />

<Toaster />

<div class="space-y-6 px-2 lg:px-10 pb-16 md:block">
	<div class="space-y-0.5">
		<h2 class="text-4xl font-bold">Fortified Firewall</h2>
		<!--<a
			href="/i/dash/project/{$page.params.project}"
			class="text-muted-foreground hover:underline flex items-center"
		>
			<ArrowLeft class="w-4 h-4" />
			Test
		</a>-->
	</div>
	<Separator class="my-6" />
	<div class="flex flex-col space-y-8 lg:flex-row lg:space-x-12 lg:space-y-0">
		<aside class="lg:w-1/6 overflow-auto">
			<Nav {items} />
		</aside>
		<div class="flex-1">
			<slot />
		</div>
	</div>
</div>
