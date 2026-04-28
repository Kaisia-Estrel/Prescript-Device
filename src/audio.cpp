#include <Arduino.h>

#include "audio.h"


SFX::SFX() {
  this->m_lastUpdate = 0;
  this->m_state = 0;
}

void SFX::restart() {
  analogWrite(BUZZER_PIN, 0);
  this->m_lastUpdate = 0;
  this->m_state = 0;
}

#define playVal(val, startTime) \
  if (timeElapsed >= 26) { \
    analogWrite(BUZZER_PIN, val); \
    this->m_lastUpdate = millis(); \
    this->m_state++; \
  }


void PrescriptReceivedSFX::loop() {
  unsigned long timeElapsed = millis() - this->m_lastUpdate;
  switch (m_state) {
    case 0:
      analogWrite(BUZZER_PIN, 150);
      this->m_lastUpdate = millis();
      this->m_state = 1;
      break;
    case 1:
      if (timeElapsed >= 26) {
        analogWrite(BUZZER_PIN, 0);
        this->m_state = 2;
        this->m_lastUpdate = millis();
      }
      break;
    case 2:
      if (timeElapsed >= 100) {
        analogWrite(BUZZER_PIN, 150);
        this->m_state = 3;
        this->m_lastUpdate = millis();
      }
      break;
    case 3:
      if (timeElapsed >= 26) {
        analogWrite(BUZZER_PIN, 0);
        this->m_state = 4;
        this->m_lastUpdate = millis();
      }
      break;
    case 4:
      if (timeElapsed >= 100) {
        analogWrite(BUZZER_PIN, 150);
        this->m_state = 5;
        this->m_lastUpdate = millis();
      }
      break;
    case 5:
      if (timeElapsed >= 26) {
        analogWrite(BUZZER_PIN, 0);
        this->m_state = 6;
        this->m_lastUpdate = millis();
      }
      break;
    case 6:
      if (timeElapsed >= 10000) {
        this->m_state = 0;
        this->m_lastUpdate = millis();
      }
      break;
  }
}
