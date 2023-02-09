# ESP32 Fan Speed Controller

Reads temperatures from a pair of DS18B20 sensors, compares temperature difference, and adjusts speed of PWM fan based on temperature difference.

Publishes temperature and other parameters to ESP-NOW 

Allows for limited control of fan parameters via commands sent via ESP-NOW.

All ESP-NOW communication is to and from broadcast addresses

Uses ArduinoJson and OneWireNG libraries.

## Output 
 Output is JSON formatted ESP-NOW packets:

### Data Packet:
{"D":"ESP-Fanspeed","Temperature A":63.2,"Temperature B":32,"Reads":52,"Errors":0}

### Status Packet
{"D":"ESP-Fanspeed","T":" 0d 0h10m","R":748,"W":1272,"H":221348}

## Input

### Command Packet

Format:
{"D" : "ESP-Fanspeed", "CMD" : "SET", "Param" : "Relay", "Value" : "1" }

Valid values for 'Param' are "Relay", "Override", "Start Temp", "Max Temp", "Low Speed" - corresponding to equivalient values in settings.h