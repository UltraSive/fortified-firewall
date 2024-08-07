<script>
	import '../app.css';
	import { ModeWatcher } from 'mode-watcher';

	import { setMode } from 'mode-watcher';
	setMode('dark');
</script>

<ModeWatcher />

<div class="my-4 container">
	<slot />
</div>
