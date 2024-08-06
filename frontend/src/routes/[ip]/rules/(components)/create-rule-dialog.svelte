<script lang="ts">
	import { enhance } from '$app/forms';

	import * as Form from '$lib/components/ui/form';
	import { ruleSchema, type RuleSchema } from '../(data)/schema';
	import type { SuperValidated } from 'sveltekit-superforms';

	export let form: SuperValidated<RuleSchema>;

	import * as Dialog from '$lib/components/ui/dialog';
	import * as Select from '$lib/components/ui/select';
	import { Button, buttonVariants } from '$lib/components/ui/button';
	import { Switch } from '$lib/components/ui/switch';
	import { Label } from '$lib/components/ui/label';
	import { Input } from '$lib/components/ui/input';
	import { Plus } from 'lucide-svelte';

	const protocols = ['tcp', 'udp', 'icmp', 'port punch'];
	$: selectedProtocol = protocols[0];
</script>

<Dialog.Root>
	<Dialog.Trigger class={buttonVariants({ variant: 'outline' })}
		>Create Rule <Plus class="ml-2 h-4 w-4" /></Dialog.Trigger
	>
	<Dialog.Content class="sm:max-w-[425px]">
		<form method="POST" action="?/createRule" use:enhance>
			<Dialog.Header>
				<Dialog.Title>Create Firewall Rule</Dialog.Title>
				<Dialog.Description>Define your firewall rule below.</Dialog.Description>
			</Dialog.Header>

			<div class="grid gap-4 py-4">
				<div class="grid grid-cols-4 items-center gap-4">
					<Label class="text-right">Comment</Label>
					<Input id="comment" placeholder="SSH" class="col-span-3" />
				</div>
				<div class="grid grid-cols-4 items-center gap-4">
					<Label class="text-right">Source</Label>
					<Input id="source" value="0.0.0.0/0" class="col-span-3" />
				</div>
				<div class="grid grid-cols-4 items-center gap-4">
					<Label class="text-right">Protocol</Label>
					<Select.Root>
						<Select.Trigger class="col-span-3">
							<Select.Value placeholder={selectedProtocol} />
						</Select.Trigger>
						<Select.Content>
							{#each protocols as protocol}
								<Select.Item value={protocol}>{protocol}</Select.Item>
							{/each}
						</Select.Content>
					</Select.Root>
				</div>
				<div class="grid grid-cols-4 items-center gap-4">
					<Label class="text-right">Src Port</Label>
					<Input id="src_port" value="null" class="" />
					<Label class="text-right">Dst Port</Label>
					<Input id="dst_port" placeholder="22" class="" />
				</div>
				<div class="grid grid-cols-4 items-center gap-4">
					<Label for="whitelist" class="text-right">Allow</Label>
					<Switch id="whitelist" checked />
				</div>
			</div>

			<!-- 
		<Form.Root method="POST" action="?/createRule" {form} schema={ruleSchema} let:config>
			<Form.Field {config} name="comment">
				<Form.Item>
					<Form.Label>Comment</Form.Label>
					<Form.Input />
					<Form.Description>What is this rule.</Form.Description>
					<Form.Validation />
				</Form.Item>
			</Form.Field>

			<Form.Field {config} name="source">
				<Form.Item>
					<Form.Label>Source Address</Form.Label>
					<Form.Input placeholder="0.0.0.0/0" />
					<Form.Description>Port traffic can come in from.</Form.Description>
					<Form.Validation />
				</Form.Item>
			</Form.Field>

			<Form.Field {config} name="protocol">
				<Form.Item>
					<Form.Label>Protocol Address</Form.Label>
					<Form.Select>
						<Form.SelectTrigger placeholder="Select a protocol..." />
						<Form.SelectContent>
							<Form.SelectItem value="tcp">TCP</Form.SelectItem>
							<Form.SelectItem value="udp">UDP</Form.SelectItem>
							<Form.SelectItem value="icmp">ICMP</Form.SelectItem>
						</Form.SelectContent>
					</Form.Select>
					<Form.Description>Application network protocol used.</Form.Description>
					<Form.Validation />
				</Form.Item>
			</Form.Field>

			<div class="flex justify-between space-x-2">
				<Form.Field {config} name="src_port">
					<Form.Item>
						<Form.Label>Source Port</Form.Label>
						<Form.Input type="number" placeholder="null" />
						<Form.Description>Port traffic can come in from.</Form.Description>
						<Form.Validation />
					</Form.Item>
				</Form.Field>

				<Form.Field {config} name="dst_port">
					<Form.Item>
						<Form.Label>Destination Port</Form.Label>
						<Form.Input type="number" placeholder="80" />
						<Form.Description>Port traffic lands on.</Form.Description>
						<Form.Validation />
					</Form.Item>
				</Form.Field>
			</div>

			<Form.Field {config} name="whitelist">
				<Form.Item>
					<div class="flex items-center">
						<Form.Label>Allow</Form.Label>
						<Form.Switch class="ml-4" checked />
					</div>
					<Form.Description>Allow or block traffic with this criteria.</Form.Description>
					<Form.Validation />
				</Form.Item>
			</Form.Field>

			<Form.Button>Create Rule</Form.Button>
		 -->
			<Dialog.Footer>
				<Button type="submit">Create Rule</Button>
				<!-- <Form.Button>Create Rule</Form.Button> -->
			</Dialog.Footer>
			<!-- </Form.Root> -->
		</form>
	</Dialog.Content>
</Dialog.Root>
