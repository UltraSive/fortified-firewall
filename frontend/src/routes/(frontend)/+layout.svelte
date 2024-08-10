<script>
	import '../../app.css';
	import { ModeWatcher } from 'mode-watcher';
	import { Toaster } from '$lib/components/ui/sonner';

	import { setMode } from 'mode-watcher';
	setMode('dark');
</script>

<ModeWatcher />

<Toaster />

<slot />
