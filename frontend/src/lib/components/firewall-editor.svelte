<script>
  import * as Table from '$lib/components/ui/table';
	import { Button } from '$lib/components/ui/button';

  import { ArrowUp, ArrowDown } from "lucide-svelte";

  let { rules = $bindable() } = $props()
</script>

<Table.Root>
  <Table.Header>
    <Table.Row>
      <Table.Head class="w-[100px]">Priority</Table.Head>
      <Table.Head>Name</Table.Head>
      <Table.Head>Source IP</Table.Head>
      <Table.Head>Destination IP</Table.Head>
      <Table.Head>Action</Table.Head>
      <Table.Head class="text-right">Adjust Priority</Table.Head>
    </Table.Row>
  </Table.Header>
  <Table.Body>
    {#each rules as rule, i}
      <Table.Row key={rule.id}>
        <Table.Cell class="font-medium">{i + 1}</Table.Cell>
        <Table.Cell>{rule.name}</Table.Cell>
        <Table.Cell>{rule.sourceIp}</Table.Cell>
        <Table.Cell>{rule.destinationIp}</Table.Cell>
        <Table.Cell>{rule.action}</Table.Cell>
        <Table.Cell class="text-right">
          <div class="flex justify-end space-x-2">
            <Button
              variant="outline"
              size="icon"
              disabled={i === 0}
              aria-label={`Move ${rule.name} up`}
            >
              <ArrowUp class="h-4 w-4" />
            </Button>
            <Button
              variant="outline"
              size="icon"
              disabled={i === rules.length - 1}
              aria-label={`Move ${rule.name} down`}
            >
              <ArrowDown class="h-4 w-4" />
            </Button>
          </div>
        </Table.Cell>
      </Table.Row>
    {/each}
  </Table.Body>
</Table.Root>