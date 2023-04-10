#ifndef _MAIN_H_
#define _MAIN_H_
#include <stdint.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <vector>
// #include <Arduino_FreeRTOS.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/semphr.h"
// #include <semphr.h>
#include "boards.h"
// #include "globals.hpp"
#include "packet.hpp"
#include <cstring>
// #include "addressBook.h"


#define WIFI_CORE 0
#define APP_CORE 1
#define RH_MESH_MAX_MESSAGE_LEN 245
// #define NODE_ADDRESS 1

// extern RH_RF95 rf95;
// extern RHMesh manager;

// void LoraApp(void *pvParameters);
// TaskHandle_t lora_app_handle;
// void WifiApp(void *pvParameters);
// TaskHandle_t wifi_app_handle;

// bool quit = false;

// char* addressBook[255][100]; // A 2D array to store node names
// SemaphoreHandle_t addressBookMutex;

// Singleton instance of the radio driver
// RH_RF95 rf95(RADIO_CS_PIN, RADIO_DIO0_PIN); // slave select pin and interrupt pin, [heltec|ttgo] ESP32 Lora OLED with sx1276/8

// Class to manage message delivery and receipt, using the driver declared above
// RHMesh manager(rf95, NODE_ADDRESS);

# endif /* _MAIN_H_ */