<script lang="ts">
	import '../app.css';
	import { Toaster } from "$lib/components/ui/sonner";
	import * as Sidebar from '$lib/components/ui/sidebar';
	import AppSidebar from '$lib/components/app-sidebar.svelte';

	let { children } = $props();
</script>

<Toaster />

<Sidebar.Provider>
	<AppSidebar />
	<main>
		<div class="absolute">
			<Sidebar.Trigger class="translate-x-2 translate-y-2" />
		</div>

		<div class="mt-4 container">
			{@render children?.()}
		</div>
	</main>
</Sidebar.Provider>
