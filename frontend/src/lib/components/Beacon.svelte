<script>
    export let text;
    export let live;
    let colorClass; 
    
    if (live) { 
        colorClass = `animate-pulse bg-red-500 h-3 w-3 rounded-full`;
    } else {
        colorClass = `animate-pulse bg-yellow-500 h-3 w-3 rounded-full`;
    }
</script>

<div class="p-1 inline-flex items-center space-x-2">
    <div class={colorClass}></div>
    <span class="">{text}</span>
</div>

<style>
    @keyframes pulse {
        0%, 100% { opacity: 1; }
        50% { opacity: .5; }
    }
    .animate-pulse {
        animation: pulse 1s cubic-bezier(0, 0, .2, 1) infinite;
    }
</style>
