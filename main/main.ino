#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>
#include "boards.h"
#include "addressBook.h"

#define WIFI_CORE 0
#define APP_CORE 1
#define RH_MESH_MAX_MESSAGE_LEN 245


void LoraApp(void *pvParameters);
TaskHandle_t lora_app_handle;
void WifiApp(void *pvParameters);
TaskHandle_t wifi_app_handle;

bool quit = false;

// Singleton instance of the radio driver
RH_RF95 rf95(LLG_CS, LLG_DI0); // slave select pin and interrupt pin, [heltec|ttgo] ESP32 Lora OLED with sx1276/8

// Class to manage message delivery and receipt, using the driver declared above
RHMesh manager(rf95, NODE_ADDRESS);

void setup(){
  initBoard();
  // When the power is turned on, a delay is required to allow the lora chip to boot.
  delay(1500);

  if (!manager.init())
    {Serial.println(" init failed");} 
  else
    {Serial.println(" done");}  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36 
  Serial.printf("Max length: %d\n", RH_MAX_MESSAGE_LEN);  
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
  
  // Spawn tasks for each method to run on each core
  // ESP32 has 520KB of RAM so I will allocate 100KiB to each
  // task allowing 300KB for other processes such as FreeRTOS

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
}

void loop(){
  if(quit){
    vTaskDelete(lora_app_handle);
    lora_app_handle = NULL;
    vTaskDelete(wifi_app_handle);
    wifi_app_handle = NULL;
  }  
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/


void LoraApp(void *pvParameters){
  (void) pvParameters;
  ADDENTRY addrBook[255];
  
  addrBook[255] = (ADDENTRY) ("BROADCAST", 255);

  vTaskDelete(NULL);
}


void WifiApp(void *pvParameters){
  (void) pvParameters;

  vTaskDelete(NULL);
}