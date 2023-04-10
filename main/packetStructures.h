#ifndef _PACKETSTRUCTURES_H_
#define _PACKETSTRUCTURES_H_

#include <stdint.h>

enum messageType{
    NODE_NAME_FLAG = 0b00000000,
    RESPONSE_FLAG = 0b00000001,
    ANNOUNCEMENT_FLAG = 0b00000010,
    DIRECT_FLAG = 0b00000011,
    GROUP_FLAG = 0b00000100,
};

struct nodeNamePacket{
  /* Message Type Identifier */
  uint8_t messageType = NODE_NAME_FLAG;
  /* Source ID*/
  uint8_t sourceID;
  /* node ID */
  uint8_t nodeID;
  /* node name */
  char name[100] = {0};
};
typedef struct nodeNamePacket NPACKET;

struct responseReceiptPacket{
  /* Message Type Identifier */
  uint8_t messageType = RESPONSE_FLAG;
  /* Source ID */
  uint8_t sourceID;
  /* Destination ID */
  uint8_t destinationID;
  /* Message ID */
  uint8_t messageID;
  /* Number of packets = first 4 bits - Packet ID = last 4 bits */
  uint8_t numberOfPackets_packetID;
  /* length of payload */
  uint8_t payloadLength;
  char checksum[238] = {0};
};
typedef struct responseReceiptPacket RPACKET;

struct announcementPacket{
  /* Message Type Identifier */
  uint8_t messageType = ANNOUNCEMENT_FLAG;
  /* Source ID */
  uint8_t sourceID;
  /* Message ID */
  uint8_t messageID;
  /* Number of packets = first 4 bits - Packet ID = last 4 bits */
  uint8_t numberOfPackets_packetID;
  /* length of payload */
  uint8_t payloadLength;
  char message[239] = {0};
};
typedef struct announcementPacket APACKET;

struct directMessagePacket{
  /* Message Type Identifier */
  uint8_t messageType = DIRECT_FLAG;
  /* Source ID */
  uint8_t sourceID;
  /* Destination ID */
  uint8_t destinationID;
  /* Message ID */
  uint8_t messageID;
  /* Number of packets = first 4 bits - Packet ID = last 4 bits */
  uint8_t numberOfPackets_packetID;
  /* length of payload */
  uint8_t payloadLength;
  char message[238] = {0};
};
typedef struct directMessagePacket DPACKET;
# endif /* _PACKETSTRUCTURES_H_ */