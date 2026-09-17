#ifndef SMART_LAMP_CONFIG_H
#define SMART_LAMP_CONFIG_H

#include <Arduino.h>

namespace Config {
constexpr uint8_t LIGHT_SENSOR_PIN = A0;
constexpr uint8_t TEMPERATURE_SENSOR_PIN = A1;
constexpr uint8_t SOUND_SENSOR_PIN = A2;
constexpr uint8_t PROXIMITY_SENSOR_PIN = 8;

constexpr uint8_t RED_LED_PIN = 9;
constexpr uint8_t GREEN_LED_PIN = 10;
constexpr uint8_t BLUE_LED_PIN = 11;

constexpr bool PROXIMITY_ACTIVE_LOW = true;

constexpr int MIN_LIGHT_READING_TO_ENABLE = 9;
constexpr int SOUND_THRESHOLD = 150;
constexpr int TEMPERATURE_MIN_RAW = 50;
constexpr int TEMPERATURE_MAX_RAW = 70;

constexpr unsigned long SENSOR_INTERVAL_MS = 20;
constexpr unsigned long CLAP_COOLDOWN_MS = 500;
constexpr unsigned long DIAGNOSTIC_INTERVAL_MS = 500;

constexpr unsigned long SERIAL_BAUD_RATE = 9600;
}  // namespace Config

#endif

