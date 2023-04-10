#ifndef _PACKET_H_
#define _PACKET_H_
#include "packetStructures.h"   
#include <array>
#include <stdint.h>
#include <string.h>
#include <vector>
#include <stdint.h>
#include <RHMesh.h>
#include <RH_RF95.h>
// #include <semphr.h>
// #include "globals.hpp"

#define MAX_PACKET_SIZE 245
#define NPACKET_HEADER_SIZE 3
#define RPACKET_HEADER_SIZE 6
#define APACKET_HEADER_SIZE 5
#define DPACKET_HEADER_SIZE 6

using namespace std;

// Create data stream from a packet
vector<uint8_t> generateNPacket(NPACKET packet);
vector<uint8_t> generateRPacket(RPACKET packet);
vector<uint8_t> generateAPacket(APACKET packet);
vector<uint8_t> generateDPacket(DPACKET packet);

// Create a packet from a data stream
NPACKET unpackNPacket(uint8_t inData[]);
RPACKET unpackRPacket(uint8_t inData[]);
APACKET unpackAPacket(uint8_t inData[]);
DPACKET unpackDPacket(uint8_t inData[]);

// // Handle a packet
// void handleNPacket(NPACKET packet, RHMesh* manager);
// void handleRPacket(RPACKET packet);
// void handleAPacket(APACKET packet);
// void handleDPacket(DPACKET packet);

# endif /* _PACKET_H_ */