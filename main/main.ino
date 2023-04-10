#include "main.h"

#define NODE_ADDRESS 1
#define RADIO_CS_PIN 18
#define RADIO_DIO0_PIN 26

#define INCLUDE_eTaskGetState 1


RH_RF95 rf95(RADIO_CS_PIN, RADIO_DIO0_PIN);
RHMesh manager(rf95, NODE_ADDRESS);
char addressBook[255][100]; // A 2D array to store node names
SemaphoreHandle_t addressBookMutex;
uint8_t recieveBuffer[RH_MESH_MAX_MESSAGE_LEN];
uint8_t res;
// void LoraApp(void *pvParameters);
TaskHandle_t lora_app_handle;
// void WifiApp(void *pvParameters);
TaskHandle_t wifi_app_handle;
/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void LoraApp(void *pvParameters){
  // Runs in an infinite loop
  Serial.print("LoRa App Loop: Executing on core ");
  Serial.println(xPortGetCoreID());
  Serial.printf("LoRa App Running\n");
  u8g2->clearBuffer();
  u8g2->setFlipMode(0);
  u8g2->setFontMode(1); // Transparent
  u8g2->setDrawColor(1);
  u8g2->setFontDirection(0);
  u8g2->firstPage();
  do {
      u8g2->setFont(u8g2_font_inb19_mf);
      u8g2->drawStr(0, 30, "APP OK");
  } while ( u8g2->nextPage() );
  u8g2->sendBuffer();
  u8g2->setFont(u8g2_font_fur11_tf);
  delay(3000);
  NPACKET Npacket;
  RPACKET Rpacket;
  APACKET Apacket;
  DPACKET Dpacket;
  Serial.println(rf95.available());
  for (;;){
    (void) pvParameters;
    uint8_t len = sizeof(recieveBuffer);
    uint8_t from;

    // if (xSemaphoreTake(addressBookMutex, portMAX_DELAY)){
    //   addressBook[255] = "BROADCAST";
    //   xSemaphoreGive(addressBookMutex);
    // }
    // Serial.printf("%s : %d", addressBook[255], 255);
    if (manager.recvfromAck((uint8_t *)recieveBuffer, &len, &from)){
      // Serial.print("message from node n.");
      // Serial.print(from);
      // Serial.printf(" with name %s ", addressBook[from]);
      // Serial.print(": ");
      // Serial.print((char*)recieveBuffer);
      // Serial.print(" rssi: ");
      // Serial.println(rf95.lastRssi());
      switch(recieveBuffer[0]) {
        case NODE_NAME_FLAG:
          Npacket = unpackNPacket(recieveBuffer);
          if (xSemaphoreTake(addressBookMutex, portMAX_DELAY)){
            if (addressBook[Npacket.nodeID] != Npacket.name){
              strcpy(addressBook[Npacket.nodeID], Npacket.name);
              xSemaphoreGive(addressBookMutex);
              vector<uint8_t> data = generateNPacket(Npacket);
              manager.sendtoWait(data.data(), data.size(), 255);
            }
          }
          break;
        case RESPONSE_FLAG:
          Rpacket = unpackRPacket(recieveBuffer);
          // handleRPacket(Rpacket);
          break;
        case ANNOUNCEMENT_FLAG:
          Apacket = unpackAPacket(recieveBuffer);
          // handleAPacket(Apacket);
          break;
        case DIRECT_FLAG:
          Dpacket = unpackDPacket(recieveBuffer);
          // handleDPacket(Dpacket);
          break;
        default:
          break;
      }
    }
  }
}


void WifiApp(void *pvParameters){
  (void) pvParameters;
  Serial.print("Wifi App Loop: Executing on core ");
  Serial.println(xPortGetCoreID());
  Serial.printf("Wifi App Running\n");
  for (;;){

  }
}


void setup(){
  initBoard();
  // When the power is turned on, a delay is required to allow the lora chip to boot.
  delay(1500);

  if (!manager.init())
    {Serial.println(" init failed");} 
  else
    {Serial.println("Manager init done");}  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36 
  Serial.printf("Max length: %d\n", RH_MESH_MAX_MESSAGE_LEN);  
  rf95.setTxPower(10, false); // with false output is on PA_BOOST, power from 2 to 20 dBm, use this setting for high power demos/real usage
  //rf95.setTxPower(1, true); // true output is on RFO, power from 0 to 15 dBm, use this setting for low power demos ( does not work on lilygo lora32 )
  rf95.setFrequency(868.0);
  rf95.setCADTimeout(500);

  RH_RF95::ModemConfig modem_config = {
    0x82, // Reg 0x1D: BW=250kHz, Coding=4/5, Header=explicit
    0xC4, // Reg 0x1E: Spread=4096chips/symbol (SF12), CRC=enable
    0x08  // Reg 0x26: LowDataRate=On, Agc=Off.  0x0C is LowDataRate=ON, ACG=ON
  };
  rf95.setModemRegisters(&modem_config);
  Serial.println("Lora chip ready");

  // Create mutex's
  addressBookMutex = xSemaphoreCreateMutex();

  if (xSemaphoreTake(addressBookMutex, portMAX_DELAY)){
    strcpy(addressBook[255], "BROADCAST");
    xSemaphoreGive(addressBookMutex);
  }
  
  // Spawn tasks for each method to run on each core
  // ESP32 has 520KB of RAM so I will allocate 100KiB to each
  // task allowing 300KB for other processes such as FreeRTOS

  // Create task (thread) for wifi app and pin to core 0
  xTaskCreatePinnedToCore(
                          WifiApp,
                          "Wifi App",
                          12800,  // Stack size
                          NULL,  // When no parameter is used, simply pass NULL
                          1,  // Priority
                          &wifi_app_handle, // With task handle we will be able to manipulate with this task.
                          0 // Core on which the task will run
  );
  delay(500);
  // Create task (thread) for lora app and pin to core 1
  xTaskCreatePinnedToCore(
                          LoraApp,
                          "Lora App",
                          12800,  // Stack size
                          NULL,  // When no parameter is used, simply pass NULL
                          1,  // Priority
                          &lora_app_handle, // With task handle we will be able to manipulate with this task.
                          1 // Core on which the task will run
  );
  delay(500);
  Serial.print("Main Loop: Executing on core ");
  Serial.println(xPortGetCoreID());
  Serial.println("Setup Complete");
  // vTaskDelete(NULL); // Deletes the setup and loop tasks as these are now not needed
}

void loop(){
  // if(quit){
  //   vTaskDelete(lora_app_handle);
  //   lora_app_handle = NULL;
  //   vTaskDelete(wifi_app_handle);
  //   wifi_app_handle = NULL;
  // }  
}