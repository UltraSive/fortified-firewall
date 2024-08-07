<script lang="ts">
	import { enhance } from '$app/forms';

	import * as Form from '$lib/components/ui/form';
	import { ruleSchema, type RuleSchema } from '../(data)/schema';
	import type { SuperValidated } from 'sveltekit-superforms';

	export let form: SuperValidated<RuleSchema>;

	import * as Select from '$lib/components/ui/select';
	import { Button, buttonVariants } from '$lib/components/ui/button';
	import { Switch } from '$lib/components/ui/switch';
	import { Label } from '$lib/components/ui/label';
	import { Input } from '$lib/components/ui/input';
	import { Separator } from '$lib/components/ui/separator';

	import { Plus, Minus } from 'lucide-svelte';

	const protocols = ['TCP', 'UDP', 'ICMP', 'PUNCH'];

	const actions = ['ALLOW', 'BLOCK', 'RATE_LIMIT', 'REDIRECT'];
	$: selectedAction = actions[0];

	const matchTypes = [
		'DST_IP',
		'DST_PORT',
		'DST_PORT_RANGE',
		'PROTOCOL',
		'SRC_IP',
		'SRC_PORT',
		'SRC_PORT_RANGE',
		'ETHERTYPE',
		'VLAN_ID'
	];

	let matches = [
		{
			type: 'PROTOCOL',
			value: 'TCP',
		},
		{
			type: 'DST_PORT',
			value: '80',
		}
	];

	$: console.log(matches);

	function changeType(type, i) {
		matches[i].type = type;
	}

	function changeValue(value, i) {
		matches[i].value = value;
	}
</script>

<div class="space-y-4">
	<div>
		<Label for="name">Name</Label>
		<Input id="name" value="HTTP" />
	</div>
	<Separator />
	<div class="flex justify-between items-center">
		<h3 class="font-semibold text-lg">Matches</h3>
		<Button
			variant="outline"
			on:click={() => {
				console.log('Click');
				matches = [
					...matches,
					{
						type: 'DST_PORT',
						value: '80'
					}
				];
				console.log(matches);
			}}>Add Match<Plus class="ml-2 h-4 w-4" /></Button
		>
	</div>
	{#each matches as match, i}
		<div>
			<Label>Match #{i}</Label>
			<div class="space-y-1">
				<div class="flex space-x-2 items-center">
					<Select.Root selected={{value: match.type, label: match.type, selected: true}}>
						<Select.Trigger>
							<Select.Value/>
						</Select.Trigger>
						<Select.Content>
							{#each matchTypes as type}
								<Select.Item value={type} on:click={() => changeType(type, i)}>{type}</Select.Item>
							{/each}
						</Select.Content>
					</Select.Root>
					<Button
						variant="destructive"
						on:click={() => {
							if (matches.length > 1) {
								matches = matches.filter((_, index) => index !== i);
							}
						}}><Minus class="h-4 w-4" /></Button
					>
				</div>

				{#if match.type === 'DST_PORT'}
					<Input bind:value={matches[i].value} />
				{:else if match.type === 'PROTOCOL'}
					<Select.Root>
						<Select.Trigger>
							<Select.Value />
						</Select.Trigger>
						<Select.Content>
							{#each protocols as protocol}
								<Select.Item value={protocol} on:click={() => changeValue(protocol, i)}>{protocol}</Select.Item>
							{/each}
						</Select.Content>
					</Select.Root>
				{/if}
			</div>
		</div>
	{/each}
	<Separator />
	<div class="flex items-center space-x-2">
		<Label for="allow" class="text-right">Allow</Label>
		<Switch id="allow" checked />
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
<div class="flex justify-end mt-4">
	<Button type="submit">Create Rule</Button>
</div>
<!-- <Form.Button>Create Rule</Form.Button> -->
<!-- </Form.Root> -->
