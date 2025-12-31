#ifndef COMMON_H
#define COMMON_H

#include "config.h"
#include "rgb_display.h"
#include "light_sensor.h"

#include <WiFi.h>
#include <PubSubClient.h>
#include <ESPNtpClient.h>



#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <ESP32-HUB75-VirtualMatrixPanel_T.hpp>

// ---------------- Panel configuration ----------------
#define PANEL_RES_X        80
#define PANEL_RES_Y        40
#define VDISP_NUM_ROWS     2
#define VDISP_NUM_COLS     1
#define PANEL_CHAIN_TYPE   CHAIN_BOTTOM_RIGHT_UP
#define PANEL_SCAN_TYPE    FOUR_SCAN_40PX_HIGH

using MyScanTypeMapping = ScanTypeMapping<PANEL_SCAN_TYPE>;

// ---------------- Extern declaration ----------------
extern VirtualMatrixPanel_T<PANEL_CHAIN_TYPE, MyScanTypeMapping>* virtualDisp;

#ifdef MQTT_USE_SSL
#include <HTTPClient.h>
extern WiFiClientSecure wifiClient;
#else
extern WiFiClient wifiClient;
#endif

extern int status;

// Initialize MQTT client
extern PubSubClient client;

//Time of last status update
extern unsigned long lastStatusSend;

//Time of last sensor events
extern unsigned long lastSensorRead;
extern unsigned long lastLightRead;
extern unsigned long lastLedBlink;

//Log message persistence
//Is a log message currently displayed?
extern bool logMessageActive;
//When was the message shown?
extern unsigned long messageDisplayMillis;

// NTP
extern bool wifiFirstConnected;

extern bool syncEventTriggered; // True if a time event has been triggered

//RGB display
extern MatrixPanel_I2S_DMA *dma_display;

//Current time and date
extern struct tm timeinfo;

//Flags to trigger display section updates
extern bool clockStartingUp;
extern bool newSensorData;
extern bool sensorDead;

//The actual sensor data
extern float sensorTemp;
extern int sensorHumi;

//Just a heartbeat for the watchdog...
extern bool heartBeat;

//Light sensor
extern Adafruit_TSL2591 tsl;

//Weather data
extern uint8_t forecast5Days[5];
extern int8_t minTempToday;
extern int8_t maxTempToday;

#endif

