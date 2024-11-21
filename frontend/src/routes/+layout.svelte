<script lang="ts">
	import '../app.css';

	import * as Sidebar from '$lib/components/ui/sidebar/index.js';
	import AppSidebar from '$lib/components/app-sidebar.svelte';

	let { children } = $props();
</script>

<Sidebar.Provider>
	<AppSidebar />
	<main>
		<div class="absolute">
			<Sidebar.Trigger class="translate-x-2 translate-y-2" />
		</div>
		<div class="container">
			{@render children?.()}
		</div>
	</main>
</Sidebar.Provider>
