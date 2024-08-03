// Code generated by bpf2go; DO NOT EDIT.
//go:build mips || mips64 || ppc64 || s390x

package main

import (
	"bytes"
	_ "embed"
	"fmt"
	"io"

	"github.com/cilium/ebpf"
)

type firewallActionValue struct {
	Action       uint32
	_            [4]byte
	LastSeenNs   uint64
	RateLimitPps uint64
	XdpSock      int32
	_            [4]byte
}

type firewallIpv4DstPunchKey struct {
	DstIp    uint32
	DstPort  uint16
	Protocol uint8
	_        [1]byte
}

type firewallIpv4PairKey struct {
	SrcIp uint32
	DstIp uint32
}

type firewallIpv6DstPunchKey struct {
	DstIp    [16]uint8
	DstPort  uint16
	Protocol uint8
	_        [1]byte
}

type firewallIpv6PairKey struct {
	SrcIp [16]uint8
	DstIp [16]uint8
}

// loadFirewall returns the embedded CollectionSpec for firewall.
func loadFirewall() (*ebpf.CollectionSpec, error) {
	reader := bytes.NewReader(_FirewallBytes)
	spec, err := ebpf.LoadCollectionSpecFromReader(reader)
	if err != nil {
		return nil, fmt.Errorf("can't load firewall: %w", err)
	}

	return spec, err
}

// loadFirewallObjects loads firewall and converts it into a struct.
//
// The following types are suitable as obj argument:
//
//	*firewallObjects
//	*firewallPrograms
//	*firewallMaps
//
// See ebpf.CollectionSpec.LoadAndAssign documentation for details.
func loadFirewallObjects(obj interface{}, opts *ebpf.CollectionOptions) error {
	spec, err := loadFirewall()
	if err != nil {
		return err
	}

	return spec.LoadAndAssign(obj, opts)
}

// firewallSpecs contains maps and programs before they are loaded into the kernel.
//
// It can be passed ebpf.CollectionSpec.Assign.
type firewallSpecs struct {
	firewallProgramSpecs
	firewallMapSpecs
}

// firewallSpecs contains programs before they are loaded into the kernel.
//
// It can be passed ebpf.CollectionSpec.Assign.
type firewallProgramSpecs struct {
	Firewall *ebpf.ProgramSpec `ebpf:"firewall"`
}

// firewallMapSpecs contains maps before they are loaded into the kernel.
//
// It can be passed ebpf.CollectionSpec.Assign.
type firewallMapSpecs struct {
	Ipv4DstPunchAction *ebpf.MapSpec `ebpf:"ipv4_dst_punch_action"`
	Ipv4FirewallMap    *ebpf.MapSpec `ebpf:"ipv4_firewall_map"`
	Ipv4MatchRuleMap   *ebpf.MapSpec `ebpf:"ipv4_match_rule_map"`
	Ipv4PairAction     *ebpf.MapSpec `ebpf:"ipv4_pair_action"`
	Ipv6DstPunchAction *ebpf.MapSpec `ebpf:"ipv6_dst_punch_action"`
	Ipv6PairAction     *ebpf.MapSpec `ebpf:"ipv6_pair_action"`
	NoMatchAction      *ebpf.MapSpec `ebpf:"no_match_action"`
	PktCount           *ebpf.MapSpec `ebpf:"pkt_count"`
	XsksMap            *ebpf.MapSpec `ebpf:"xsks_map"`
}

// firewallObjects contains all objects after they have been loaded into the kernel.
//
// It can be passed to loadFirewallObjects or ebpf.CollectionSpec.LoadAndAssign.
type firewallObjects struct {
	firewallPrograms
	firewallMaps
}

func (o *firewallObjects) Close() error {
	return _FirewallClose(
		&o.firewallPrograms,
		&o.firewallMaps,
	)
}

// firewallMaps contains all maps after they have been loaded into the kernel.
//
// It can be passed to loadFirewallObjects or ebpf.CollectionSpec.LoadAndAssign.
type firewallMaps struct {
	Ipv4DstPunchAction *ebpf.Map `ebpf:"ipv4_dst_punch_action"`
	Ipv4FirewallMap    *ebpf.Map `ebpf:"ipv4_firewall_map"`
	Ipv4MatchRuleMap   *ebpf.Map `ebpf:"ipv4_match_rule_map"`
	Ipv4PairAction     *ebpf.Map `ebpf:"ipv4_pair_action"`
	Ipv6DstPunchAction *ebpf.Map `ebpf:"ipv6_dst_punch_action"`
	Ipv6PairAction     *ebpf.Map `ebpf:"ipv6_pair_action"`
	NoMatchAction      *ebpf.Map `ebpf:"no_match_action"`
	PktCount           *ebpf.Map `ebpf:"pkt_count"`
	XsksMap            *ebpf.Map `ebpf:"xsks_map"`
}

func (m *firewallMaps) Close() error {
	return _FirewallClose(
		m.Ipv4DstPunchAction,
		m.Ipv4FirewallMap,
		m.Ipv4MatchRuleMap,
		m.Ipv4PairAction,
		m.Ipv6DstPunchAction,
		m.Ipv6PairAction,
		m.NoMatchAction,
		m.PktCount,
		m.XsksMap,
	)
}

// firewallPrograms contains all programs after they have been loaded into the kernel.
//
// It can be passed to loadFirewallObjects or ebpf.CollectionSpec.LoadAndAssign.
type firewallPrograms struct {
	Firewall *ebpf.Program `ebpf:"firewall"`
}

func (p *firewallPrograms) Close() error {
	return _FirewallClose(
		p.Firewall,
	)
}

func _FirewallClose(closers ...io.Closer) error {
	for _, closer := range closers {
		if err := closer.Close(); err != nil {
			return err
		}
	}
	return nil
}

// Do not access this directly.
//
//go:embed firewall_bpfeb.o
var _FirewallBytes []byte
