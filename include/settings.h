#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

//ESP-NOW configuration
#define ESPNOW_QUEUE_SIZE 10
#define ESP_BUFFER_SIZE 200

#define HEARTBEAT 5000L         // Sensor and WiFi loop delay (ms)
#define DEVICE_NAME "ESP-Fanspeed"

// Default PWM parameters
// Linear fan speed. I.E 75F to 90F will result in fan speed from 40% to 100% of max RPM
#define FAN_START_TEMP         65   // Temp at which to turn on fan at low speed
#define FAN_MAX_TEMP           90   // Temp above which fan will run at 100%
#define FAN_TEMP_HIGH_LIMIT   120   // Max acceptable value for fan.maxTemp
#define FAN_TEMP_LOW_LIMIT     40   // Min acceptable value for fan.startTemp
#define FAN_MAX_SPEED         100   // Fan max speed (percent, 0-100)
#define FAN_LOW_SPEED          20   // PWM pulse width at minimum fan speed
                                    //   - must be less than PWM_MAX_DUTY_CYCLE)

// Fixed PWM parameters
#define PWM_PIN                 3   // GPIO pin 3
#define PWM_CHANNEL             0   // 
#define PWM_RESOLUTION          8   // 8-bit resolution, 255 possible values
#define PWM_MAX_DUTY_CYCLE    255   // Maximum value when PWM is fully 'ON'. Sets PWM range 8-bits = 255 possible values.
#define PWM_FREQUENCY       20000   // How often PWM cycles on & off 
                                    //   - Set high enough that you don't hear fan pulses

// Fan control pins
#define RELAY_OUT              10
#define LED_OUT                 7

// Sensor type, pins and config
// ---- DS18B20 Setion
static const uint8_t DS18B20{18};      /** DS18B20 type sensor */
#define SENSORTYPE DS18B20
#define DS18B20SENSOR
#define SENSORPIN_A  5
#define SENSORPIN_B  SENSORPIN_A      // Dummy def for compatibility with DHT version
// ---- End DS Section

#define TEMP_FAHRENHEIT true          // Set to True for Fahrenheit, false for Celsius

#endif //SETTINS_H