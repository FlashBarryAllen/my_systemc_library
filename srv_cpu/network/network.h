#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <string>
#include <cstring>
#include <netinet/in.h>

using namespace std;

// Ethernet header structure
struct EthernetHeader {
  uint8_t dstMac[6]; // Destination MAC address
  uint8_t srcMac[6]; // Source MAC address
  uint16_t etherType; // Ethernet type (e.g., 0x0800 for IP)
};

// IP header structure
struct IPHeader {
  uint8_t versionAndHeaderLength; // Version and header length
  uint8_t typeOfService; // Type of service
  uint16_t totalLength; // Total length
  uint16_t identification; // Identification
  uint16_t flagsAndFragmentOffset; // Flags and fragment offset
  uint8_t timeToLive; // Time to live
  uint8_t protocol; // Protocol (e.g., 6 for UDP)
  uint16_t checksum; // Header checksum
  uint32_t sourceIP; // Source IP address
  uint32_t destinationIP; // Destination IP address
};

// UDP header structure
struct UDPHeader {
  uint16_t sourcePort; // Source port
  uint16_t destinationPort; // Destination port
  uint16_t length; // Length
  uint16_t checksum; // Header checksum
};

void EncapsulatePacket(const string& data, uint8_t* dstMac, uint8_t* srcMac, uint32_t sourceIP, uint32_t destinationIP, uint16_t sourcePort, uint16_t destinationPort);

#endif