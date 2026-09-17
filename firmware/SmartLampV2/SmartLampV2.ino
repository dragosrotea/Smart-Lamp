#include "Config.h"

struct SensorReadings {
  int light;
  int temperature;
  int sound;
  bool userIsNear;
};

bool lampEnabled = true;
bool soundWasAboveThreshold = false;

unsigned long lastSensorReadAt = 0;
unsigned long lastClapAt = 0;
unsigned long lastDiagnosticAt = 0;

void setup() {
  pinMode(Config::PROXIMITY_SENSOR_PIN, INPUT);
  pinMode(Config::RED_LED_PIN, OUTPUT);
  pinMode(Config::GREEN_LED_PIN, OUTPUT);
  pinMode(Config::BLUE_LED_PIN, OUTPUT);

  Serial.begin(Config::SERIAL_BAUD_RATE);
  setColor(0, 0, 0);
}

void loop() {
  const unsigned long now = millis();

  if (now - lastSensorReadAt < Config::SENSOR_INTERVAL_MS) {
    return;
  }

  lastSensorReadAt = now;
  const SensorReadings readings = readSensors();

  updateClapControl(readings.sound, now);
  updateLamp(readings);
  printDiagnostics(readings, now);
}

SensorReadings readSensors() {
  const int proximitySignal = digitalRead(Config::PROXIMITY_SENSOR_PIN);
  const bool userIsNear = Config::PROXIMITY_ACTIVE_LOW
    ? proximitySignal == LOW
    : proximitySignal == HIGH;

  return {
    analogRead(Config::LIGHT_SENSOR_PIN),
    analogRead(Config::TEMPERATURE_SENSOR_PIN),
    analogRead(Config::SOUND_SENSOR_PIN),
    userIsNear
  };
}

void updateClapControl(int soundReading, unsigned long now) {
  const bool soundIsAboveThreshold = soundReading >= Config::SOUND_THRESHOLD;
  const bool risingSoundEdge = soundIsAboveThreshold && !soundWasAboveThreshold;
  const bool cooldownExpired = now - lastClapAt >= Config::CLAP_COOLDOWN_MS;

  if (risingSoundEdge && cooldownExpired) {
    lampEnabled = !lampEnabled;
    lastClapAt = now;
  }

  soundWasAboveThreshold = soundIsAboveThreshold;
}

void updateLamp(const SensorReadings& readings) {
  const bool lightConditionMet =
    readings.light >= Config::MIN_LIGHT_READING_TO_ENABLE;

  if (!lampEnabled || !readings.userIsNear || !lightConditionMet) {
    setColor(0, 0, 0);
    return;
  }

  setTemperatureColor(readings.temperature);
}

void setTemperatureColor(int temperatureReading) {
  const int boundedTemperature = constrain(
    temperatureReading,
    Config::TEMPERATURE_MIN_RAW,
    Config::TEMPERATURE_MAX_RAW
  );

  const int red = map(
    boundedTemperature,
    Config::TEMPERATURE_MIN_RAW,
    Config::TEMPERATURE_MAX_RAW,
    100,
    240
  );
  const int green = map(
    boundedTemperature,
    Config::TEMPERATURE_MIN_RAW,
    Config::TEMPERATURE_MAX_RAW,
    100,
    107
  );
  const int blue = map(
    boundedTemperature,
    Config::TEMPERATURE_MIN_RAW,
    Config::TEMPERATURE_MAX_RAW,
    255,
    10
  );

  setColor(red, green, blue);
}

void setColor(int red, int green, int blue) {
  analogWrite(Config::RED_LED_PIN, red);
  analogWrite(Config::GREEN_LED_PIN, green);
  analogWrite(Config::BLUE_LED_PIN, blue);
}

void printDiagnostics(const SensorReadings& readings, unsigned long now) {
  if (now - lastDiagnosticAt < Config::DIAGNOSTIC_INTERVAL_MS) {
    return;
  }

  lastDiagnosticAt = now;

  Serial.print(F("light="));
  Serial.print(readings.light);
  Serial.print(F(" temperature="));
  Serial.print(readings.temperature);
  Serial.print(F(" sound="));
  Serial.print(readings.sound);
  Serial.print(F(" near="));
  Serial.print(readings.userIsNear ? F("yes") : F("no"));
  Serial.print(F(" enabled="));
  Serial.println(lampEnabled ? F("yes") : F("no"));
}
