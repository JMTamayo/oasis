#include "hcsr04.h"

namespace peripherals {

const unsigned int Hcsr04::getEchoPin() const { return this->echoPin; }

const unsigned int Hcsr04::getTriggerPin() const { return this->triggerPin; }

float Hcsr04::getSensorHeight() const { return this->sensorHeight; }

float Hcsr04::getMaxLevelHeight() const { return this->maxLevelHeight; }

float Hcsr04::getSignalDelayMicroseconds() const {
  return this->signalDelayMicroseconds;
}

float Hcsr04::getSoundSpeedCmPerMicrosecond() const {
  return this->soundSpeedCmPerMicrosecond;
}

Hcsr04::Hcsr04(const unsigned int echoPin, const unsigned int triggerPin,
               float sensorHeight, float maxLevelHeight,
               float signalDelayMicroseconds, float soundSpeedCmPerMicrosecond)
    : echoPin(echoPin), triggerPin(triggerPin), sensorHeight(sensorHeight),
      maxLevelHeight(maxLevelHeight),
      signalDelayMicroseconds(signalDelayMicroseconds),
      soundSpeedCmPerMicrosecond(soundSpeedCmPerMicrosecond) {
  pinMode(this->getTriggerPin(), OUTPUT);
  pinMode(this->getEchoPin(), INPUT);
  digitalWrite(this->getTriggerPin(), LOW);
}

Hcsr04::~Hcsr04() {}

float Hcsr04::Read() {
  digitalWrite(this->getTriggerPin(), HIGH);
  delayMicroseconds(this->getSignalDelayMicroseconds());
  digitalWrite(this->getTriggerPin(), LOW);

  long duration = pulseIn(this->getEchoPin(), HIGH);

  float distance =
      (float)(duration * this->getSoundSpeedCmPerMicrosecond()) / 2.0f;

  float height = constrain(this->getSensorHeight() - distance, 0.0f,
                           this->getMaxLevelHeight());
  float level =
      constrain((height / this->getMaxLevelHeight()) * 100.0f, 0.0f, 100.0f);

  return level;
}

} // namespace peripherals