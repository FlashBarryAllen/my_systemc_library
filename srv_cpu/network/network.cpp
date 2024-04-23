#include "network.h"

// Encapsulate an Ethernet, IP, and UDP packet
void EncapsulatePacket(const string& data, uint8_t* dstMac, uint8_t* srcMac, uint32_t sourceIP, uint32_t destinationIP, uint16_t sourcePort, uint16_t destinationPort) {
    // Ethernet header
    EthernetHeader ethernetHeader;
    memcpy(ethernetHeader.dstMac, dstMac, 6);
    memcpy(ethernetHeader.srcMac, srcMac, 6);
    ethernetHeader.etherType = htons(0x0800); // IP

    // IP header
    IPHeader ipHeader;
    ipHeader.versionAndHeaderLength = 0x45; // Version 4, header length 5 words
    ipHeader.typeOfService = 0; // 0 for best effort
    ipHeader.totalLength = htons(sizeof(IPHeader) + sizeof(UDPHeader) + data.length());
    ipHeader.identification = 0; // 0 for now
    ipHeader.flagsAndFragmentOffset = 0; // 0 for now
    ipHeader.timeToLive = 64; // Default TTL
    ipHeader.protocol = 6; // UDP
    ipHeader.checksum = 0; // Will be calculated later
    ipHeader.sourceIP = htonl(sourceIP);
    ipHeader.destinationIP = htonl(destinationIP);

    // Calculate IP checksum
    uint16_t* ipData = (uint16_t*)&ipHeader;
    for (int i = 0; i < sizeof(IPHeader) / 2; i++) {
        ipHeader.checksum += ipData[i];
    }
    ipHeader.checksum = ~ipHeader.checksum;

    // UDP header
    UDPHeader udpHeader;
    udpHeader.sourcePort = htons(sourcePort);
    udpHeader.destinationPort = htons(destinationPort);
    udpHeader.length = htons(sizeof(UDPHeader) + data.length());
    udpHeader.checksum = 0; // Will be calculated later

    // Calculate UDP checksum
    uint8_t* udpData = (uint8_t*)&udpHeader;
    for (int i = 0; i < sizeof(UDPHeader) / 2; i++) {
        udpHeader.checksum += udpData[i];
    }
    uint16_t pseudoHeaderChecksum = htons(0) + htons(6) + htons(udpHeader.length) + htons(sourceIP) + htons(destinationIP);
    udpHeader.checksum += pseudoHeaderChecksum;
    udpHeader.checksum = ~udpHeader.checksum;

    // Copy data into packet
    uint8_t* packet = new uint8_t[sizeof(EthernetHeader) + sizeof(IPHeader) + sizeof(UDPHeader) + data.length()];
    memcpy(packet, &ethernetHeader, sizeof(EthernetHeader));
    memcpy(packet + sizeof(EthernetHeader), &ipHeader, sizeof(IPHeader));
    memcpy(packet + sizeof(EthernetHeader) + sizeof(IPHeader), &udpHeader, sizeof(udpHeader));
}