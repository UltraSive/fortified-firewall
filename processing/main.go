package main

import (
	"encoding/binary"
	"fmt"
	"log"
	"net"
	"os"
	"os/signal"
	"time"
	"unsafe"

	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/rlimit"
)

// Define the Go equivalents of the eBPF structures
type MatchType uint32

const (
	DST_IP MatchType = iota
	DST_PORT
	DST_PORT_RANGE
	PROTOCOL
	SRC_IP
	SRC_PORT
	SRC_PORT_RANGE
	ETHERTYPE
	VLAN_ID
)

type ActionType uint32

const (
	ALLOW ActionType = iota
	BLOCK
	RATE_LIMIT
	REDIRECT
)

type ActionValue struct {
	Action       ActionType
	LastSeenNs   uint64
	RateLimitPps uint64
	XdpSock      int32
}

type MatchField struct {
	Type  MatchType
	Value uint32
}

type MatchRule struct {
	Field   [5]MatchField
	Action  ActionValue
	NextKey uint32
}

// Convert IPv4 address in dotted-decimal format to uint32
func ipToUint32(ipStr string) (uint32, error) {
	ip := net.ParseIP(ipStr)
	if ip == nil {
		return 0, fmt.Errorf("invalid IP address: %s", ipStr)
	}

	// Ensure the IP address is an IPv4 address
	ip = ip.To4()
	if ip == nil {
		return 0, fmt.Errorf("not an IPv4 address: %s", ipStr)
	}

	return binary.BigEndian.Uint32(ip), nil
}

func main() {
	// Remove resource limits for kernels <5.11.
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatal("Removing memlock:", err)
	}

	// Load the compiled eBPF ELF and load it into the kernel.
	var objs firewallObjects
	if err := loadFirewallObjects(&objs, nil); err != nil {
		log.Fatal("Loading eBPF objects:", err)
	}
	defer objs.Close()

	ifname := "enp0s3" // Change this to an interface on your machine.
	iface, err := net.InterfaceByName(ifname)
	if err != nil {
		log.Fatalf("Getting interface %s: %s", ifname, err)
	}

	// Attach count_packets to the network interface.
	link, err := link.AttachXDP(link.XDPOptions{
		Program:   objs.Firewall,
		Interface: iface.Index,
	})
	if err != nil {
		log.Fatal("Attaching XDP:", err)
	}
	defer link.Close()

	loadFirewallMap(objs)
	loadMatchRuleMap(objs)

	log.Printf("Counting incoming packets on %s..", ifname)

	// Periodically fetch the packet counter from PktCount,
	// exit the program when interrupted.
	tick := time.Tick(time.Second)
	stop := make(chan os.Signal, 5)
	signal.Notify(stop, os.Interrupt)
	for {
		select {
		case <-tick:
			var count uint64
			err := objs.PktCount.Lookup(uint32(0), &count)
			if err != nil {
				log.Fatal("Map lookup:", err)
			}
			log.Printf("Received %d packets", count)
		case <-stop:
			log.Print("Received signal, exiting..")
			return
		}
	}
}

func loadFirewallMap(objs firewallObjects) {
	// Access the map from counterMaps
	firewallMap := objs.Ipv4FirewallMap

	// Example: Write a value to the map
	keyStr := "10.0.2.15"          // Example IP address
	key, err := ipToUint32(keyStr) // Example key
	if err != nil {
		log.Fatalf("failed to convert IP to uint32: %v", err)
	}
	value := uint32(123) // Example value

	if err := firewallMap.Update(unsafe.Pointer(&key), unsafe.Pointer(&value), 0); err != nil {
		log.Fatalf("failed to update firewall map: %v", err)
	}

	log.Println("Successfully updated the firewall map")

	// Example: Read a value from the map
	var readValue uint32
	if err := firewallMap.Lookup(unsafe.Pointer(&key), unsafe.Pointer(&readValue)); err != nil {
		log.Fatalf("failed to lookup map: %v", err)
	}

	log.Printf("Read value: %d\n", readValue)
}

func loadMatchRuleMap(objs firewallObjects) {
	// Access the map from counterMaps
	matchRuleMap := objs.Ipv4MatchRuleMap

	// Example: Write a value to the map
	key := uint32(123)  // Example key
	value := MatchRule{ // Define the MatchRule value
		Field: [5]MatchField{
			0: { // Initialize the first element
				Type:  DST_PORT,
				Value: 22,
			},
			// The remaining elements are initialized with zero values
		},
		Action: ActionValue{
			Action:       BLOCK,
			LastSeenNs:   0,
			RateLimitPps: 0,
			XdpSock:      0,
		},
		NextKey: 0, // No next rule
	}

	if err := matchRuleMap.Update(unsafe.Pointer(&key), unsafe.Pointer(&value), 0); err != nil {
		log.Fatalf("failed to update match rule map: %v", err)
	}

	log.Println("Successfully updated the match rule map")

	// Example: Read a value from the map
	var readValue uint32
	if err := matchRuleMap.Lookup(unsafe.Pointer(&key), unsafe.Pointer(&readValue)); err != nil {
		log.Fatalf("failed to lookup map: %v", err)
	}

	log.Printf("Read value: %d\n", readValue)
}
