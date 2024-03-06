#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <espnow.h>

#include "fan.h"
#include "ds18b20.h"
#include "settings.h"

PWMFan fan;

sensorPair sensors(SENSORPIN_A, SENSORPIN_B, SENSORTYPE);

void dumpFanStatus();  // Dump Sensor status to terminal window
void uptime();
void readSensors(void*);

TaskHandle_t xreadSensorsHandle = NULL;

void setup() {
  Serial.begin(115200);
  Serial.println("This device is Woke!");

  // Setup status LED and Relay
  pinMode(LED_OUT, OUTPUT);
  digitalWrite(LED_OUT, LOW);

  // Start esp-now
  WiFi.mode(WIFI_STA); 
  if (!initEspNow()) {
    return;
  };

  // Initialize fan and temperature sensors
  fan.begin();
  sensors.begin();

  digitalWrite(RELAY_OUT, HIGH);

  uptime();

  // Background task, Temperature Read
  xTaskCreate(readSensors, "Read Sensors", 2500, NULL, 6, &xreadSensorsHandle);
}

void loop() {

  char receiveBuffer[ESP_BUFFER_SIZE];
  StaticJsonDocument<ESP_BUFFER_SIZE + 32> doc;

  if (xQueueReceive(recv_from_EspNow_queue, receiveBuffer, portMAX_DELAY) == pdTRUE) {
    // See if we recieved a JSON doc for this device
    DeserializationError err = deserializeJson(doc, receiveBuffer);
    if (err) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(err.c_str());
    } else {
      // Valid JSON doc, now test if relevent to us
      if (doc.containsKey("D"))
        if (doc["D"] == DEVICE_NAME && doc.containsKey("CMD") && doc.containsKey("Param")) {
          digitalWrite(LED_OUT, HIGH);
          Serial.print(doc["Param"].as<String>() + ": ");
          if (doc["CMD"] == "SET") {
            if(doc["Param"] == "Relay"){
              fan.relayOut = doc["Value"].as<int>();
              digitalWrite(RELAY_OUT, fan.relayOut);
            } else if (doc["Param"] == "Override") {
              fan.override = doc["Value"].as<int>();
            } else if (doc["Param"] == "Start Temp") {
              fan.startTemp = doc["Value"].as<int>();
            } else if (doc["Param"] == "Max Temp") {
              // Serial.println(doc["Value"].as<String>());
              fan.maxTemp = doc["Value"].as<int>();
            } else if (doc["Param"] == "Low Speed") {
              // Serial.println(doc["Value"].as<String>());
              fan.lowSpeed = doc["Value"].as<int>();
            }
            fan.setFanSpeed((int)sensors.temperatureA, (int)sensors.temperatureB);
            dumpFanStatus();
          }
        }
    }
    for (size_t _i = 0; _i < ESP_BUFFER_SIZE; _i++) {
      receiveBuffer[_i] = 0;
    }
  }
  vTaskDelay(100 / portTICK_PERIOD_MS);
  digitalWrite(LED_OUT, LOW);
}

// Read both temp sensors
// Set fan speed
void readSensors(void* parameter) {
  StaticJsonDocument<ESP_BUFFER_SIZE> doc;
  for (;;) {  // infinite loop
    vTaskDelay(HEARTBEAT * 2 / portTICK_PERIOD_MS);
    digitalWrite(LED_OUT, HIGH);

    if (sensors.readFahrenheit()) {
      fan.setFanSpeed((int)sensors.temperatureA, (int)sensors.temperatureB);
      doc.clear();
      doc["D"] = DEVICE_NAME;
      doc["Temperature A"] = (double)((int)(sensors.temperatureA * 10 + .5))/10.0 ;
      doc["Temperature B"] = (double)((int)(sensors.temperatureB * 10 + .5))/10.0 ;
      doc["Reads"] = sensors.readCount;
      doc["Errors"] = sensors.errorCount;
      espNowSend((JsonDocument&)doc);
    }
    else {
      // 
    }
    dumpFanStatus();
    digitalWrite(LED_OUT, LOW);
    uptime();
  }
}

// Output sensor status to ESP-NOW
void dumpFanStatus() {
  StaticJsonDocument<ESP_BUFFER_SIZE> doc;
  doc.clear();
  doc["D"] = DEVICE_NAME;
  doc["Fan Speed"] = fan.fanSpeed;
  doc["Duty Cycle"] = fan.dutyCycle;
  doc["Start Temp"] = fan.startTemp;
  doc["Max Temp"] = fan.maxTemp;
  doc["Low Speed"] = fan.lowSpeed;
  doc["Override"] = fan.override;
  doc["Relay"] = fan.relayOut;
  
  espNowSend((JsonDocument&)doc);
}
