#include "packet.hpp"

vector<uint8_t> generateNPacket(NPACKET packet){
    // return variable
    vector<uint8_t> dataPacket;
    // write packet flag to the array
    dataPacket.push_back(packet.messageType);
    // write source id to the array
    dataPacket.push_back(packet.sourceID);
    dataPacket.push_back(packet.nodeID);
    // wtite data to the array
    for (int i=0; i<MAX_PACKET_SIZE - NPACKET_HEADER_SIZE - 1; i++){
        dataPacket.push_back(packet.name[i]);
        if (&packet.name[i] == "\0"){
          dataPacket.push_back(' \0');
          break;
        }
    };
    // uint8_t buf[] = dataPacket;
    return dataPacket;
};

vector<uint8_t> generateRPacket(RPACKET packet){
    // return variable
    vector<uint8_t> dataPacket;
    // write packet flag to the array
    dataPacket.push_back(packet.messageType);
    // write source id to the array
    dataPacket.push_back(packet.sourceID);
    // write destination id to the array
    dataPacket.push_back(packet.destinationID);
    // write message id to the array
    dataPacket.push_back(packet.messageID);
    // write packet info to the array
    dataPacket.push_back(packet.numberOfPackets_packetID);
    // wtite checksum data to the array
    dataPacket.push_back(packet.payloadLength);
    for (int i=0; i<MAX_PACKET_SIZE - RPACKET_HEADER_SIZE - 1; i++){
        dataPacket.push_back(packet.checksum[i]);
        if (&packet.checksum[i] == "\0"){
          dataPacket.push_back(' \0');
          break;
        }
    };
    return dataPacket;
};

vector<uint8_t> generateAPacket(APACKET packet){
    // return variable
    vector<uint8_t> dataPacket;
    // write packet flag to the array
    dataPacket.push_back(packet.messageType);
    // write source id to the array
    dataPacket.push_back(packet.sourceID);
    // write message id to the array
    dataPacket.push_back(packet.messageID);
    // write packet info to the array
    dataPacket.push_back(packet.numberOfPackets_packetID);
    // wtite checksum data to the array
    dataPacket.push_back(packet.payloadLength);
    for (int i=0; i<MAX_PACKET_SIZE - APACKET_HEADER_SIZE - 1; i++){
        dataPacket.push_back(packet.message[i]);
        if (&packet.message[i] == "\0"){
          dataPacket.push_back(' \0');
          break;
        }
    };
    return dataPacket;
};

vector<uint8_t> generateDPacket(DPACKET packet){
    // return variable
    vector<uint8_t> dataPacket;
    // write packet flag to the array
    dataPacket.push_back(packet.messageType);
    // write source id to the array
    dataPacket.push_back(packet.sourceID);
    // write destination id to the array
    dataPacket.push_back(packet.destinationID);
    // write message id to the array
    dataPacket.push_back(packet.messageID);
    // write packet info to the array
    dataPacket.push_back(packet.numberOfPackets_packetID);
    dataPacket.push_back(packet.payloadLength);
    // wtite data to the array
    for (int i=0; i<MAX_PACKET_SIZE - DPACKET_HEADER_SIZE - 1; i++){
        dataPacket.push_back(packet.message[i]);
        if (&packet.message[i] == "\0"){
          dataPacket.push_back(' \0');
          break;
        }
    };
    return dataPacket;
};

NPACKET unpackNPacket(uint8_t inData[]){
  uint8_t messageType = inData[0];
  // return variable
  NPACKET packet;
  // write packet flag to the packet
  packet.messageType = messageType;
  // write source id to the packet
  packet.sourceID = inData[1];
  packet.nodeID = inData[2];
  // wtite data to the packet
  for (int i=0; i<((sizeof(inData) / sizeof(inData[0])) - NPACKET_HEADER_SIZE); i++){
      packet.name[i] = inData[i + NPACKET_HEADER_SIZE];
  };
  return packet;
}

RPACKET unpackRPacket(uint8_t inData[]){
  uint8_t messageType = inData[0];
  // return variable
  RPACKET packet;
  // write packet flag to the packet
  packet.messageType = messageType;
  // write source id to the packet
  packet.sourceID = inData[1];
  // write destination id to the packet
  packet.destinationID = inData[2];
  // write message id to the packet
  packet.messageID = inData[3];
  // write packet info to the packet
  packet.numberOfPackets_packetID = inData[4];
  packet.payloadLength = inData[5];
  // wtite data to the packet
  for (int i=0; i<((sizeof(inData) / sizeof(inData[0])) - RPACKET_HEADER_SIZE); i++){
      packet.checksum[i] = inData[i + RPACKET_HEADER_SIZE];
  };
  return packet;
}

APACKET unpackAPacket(uint8_t inData[]){
  uint8_t messageType = inData[0];
  // return variable
  APACKET packet;
  // write packet flag to the packet
  packet.messageType = messageType;
  // write source id to the packet
  packet.sourceID = inData[1];
  // write message id to the packet
  packet.messageID = inData[2];
  // write packet info to the packet
  packet.numberOfPackets_packetID = inData[3];
  packet.payloadLength = inData[4];
  // wtite data to the packet
  for (int i=0; i<((sizeof(inData) / sizeof(inData[0])) - APACKET_HEADER_SIZE); i++){
      packet.message[i] = inData[i + APACKET_HEADER_SIZE];
  };
  return packet;
}

DPACKET unpackDPacket(uint8_t inData[]){
  DPACKET testPacket;
  // String testStr = "This is a test string for the packet.";
  // uint8_t messageTest[240];
  // strcpy( (char*) messageTest, testStr.c_str());
  testPacket.sourceID = inData[1];
  testPacket.destinationID = inData[2];
  testPacket.messageID = inData[3];
  testPacket.numberOfPackets_packetID = inData[4];
  testPacket.payloadLength = inData[5];
  for (int i=0; i<((sizeof(inData) / sizeof(inData[0])) - DPACKET_HEADER_SIZE); i++){
        testPacket.message[i] = inData[i + DPACKET_HEADER_SIZE];
  };
  return testPacket;

  // // wtite data to the packet
  // for (int i=0; i<((sizeof(inData)/sizeof(inData[0]))-9); i++){
  //     packet.message[i] = inData[i+9];
  // };
  // return packet;
}

// void handleNPacket(NPACKET packet, RHMesh* manager){
//   // if (xSemaphoreTake(addressBookMutex, portMAX_DELAY)){
//   //   if (addressBook[packet.nodeID] != packet.name){
//   //     strcpy(addressBook[packet.nodeID], packet.name);
//   //     xSemaphoreGive(addressBookMutex);
//   //     vector<uint8_t> data = generateNPacket(packet);
//   //     manager.sendtoWait(data.data(), data.size(), 255);
//   //   }
//   // }
// }

// void handleRPacket(RPACKET packet){

// }

// void handleAPacket(APACKET packet){
  
// }

// void handleDPacket(DPACKET packet){

// }
