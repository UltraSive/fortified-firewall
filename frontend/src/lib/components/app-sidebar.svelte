<script lang="ts">
  import { Network, BrickWall, EthernetPort, Settings } from "lucide-svelte";
  import * as Sidebar from "$lib/components/ui/sidebar/index.js";
  
  // Menu items.
  const items = [
   {
    title: "Addresses",
    url: "/addresses",
    icon: Network,
   },
   {
    title: "Firewalls",
    url: "/firewalls",
    icon: BrickWall,
   },
   {
    title: "Nodes",
    url: "/nodes",
    icon: EthernetPort,
   },
   {
    title: "Settings",
    url: "/settings",
    icon: Settings,
   },
  ];
 </script>
  
 <Sidebar.Root>
  <Sidebar.Content>
   <Sidebar.Group>
    <Sidebar.GroupLabel>Firewall</Sidebar.GroupLabel>
    <Sidebar.GroupContent>
     <Sidebar.Menu>
      {#each items as item (item.title)}
       <Sidebar.MenuItem>
        <Sidebar.MenuButton>
         {#snippet child({ props })}
          <a href={item.url} {...props}>
           <item.icon />
           <span>{item.title}</span>
          </a>
         {/snippet}
        </Sidebar.MenuButton>
       </Sidebar.MenuItem>
      {/each}
     </Sidebar.Menu>
    </Sidebar.GroupContent>
   </Sidebar.Group>
  </Sidebar.Content>
 </Sidebar.Root>