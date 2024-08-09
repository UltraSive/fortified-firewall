import apiClient from "./api";

export const protocols = ['TCP', 'UDP', 'ICMP', 'PUNCH'];

export const actionTypes = ['ALLOW', 'BLOCK', 'RATE_LIMIT', 'REDIRECT'];

export const matchTypes = [
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

export type Match = {
    type: 'PROTOCOL' | 'DST_PORT' | 'SRC_PORT' | 'DST_IP' | 'SRC_IP';
    value: string;
};

export type Action = {
    type: 'ALLOW' | 'BLOCK' | 'RATE_LIMIT' | 'REDIRECT';
    value: Number | undefined;
}

export async function postMatchRule(ip: String, name: String, matches: Match[], action: Action) {
    try {
        const response = await apiClient.post(`/ip/${ip}/rule`, {
            name: name,
            fields: matches,
            action: {
                type: action.type,
                value: action.value ?? null, // Ensure value is either a number or null
            },
        });
        return response.data;
    } catch (err) {
        console.error('Error posting rule:', err);
        throw err;
    }
}

export async function editMatchRule(id: Number, matches: Match[], action: Action) {

}

export async function swapMatchRule(rule1: Number, rule2: Number) {

}

export async function deleteMatchRule(id: Number) {

}