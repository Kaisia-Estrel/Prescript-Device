#include <Arduino.h>

#include "lcd.h"

#include "message-scroller.h"


MessageScroller::MessageScroller(const char* message) {
  this->m_message = message;
  this->m_lastUpdate = millis();
  this->m_pauseLength = 600;
  this->m_step = 0;
}

void MessageScroller::setPauseLength(int scrollLength) {
  this->m_pauseLength = scrollLength;
}

void MessageScroller::loop() {
  unsigned long timeElapsed = millis() - m_lastUpdate;

  if (timeElapsed >= m_pauseLength * (m_step == 1 ? 3 : 1)) {
    lcd.setCursor(0, 0);
    int loopPoint = -1;
    for (int i = 0; i < 16; i++) {
      char c = m_message[m_step + i];
      if (c == '\0') {
        loopPoint = i;
        break;
      }
      lcd.write(c);
    }

    for (int i = 0; i < 16 - loopPoint; i++) {
      lcd.write(m_message[i]);
    }

    this->m_step++;
    if (m_message[m_step] == '\0') {
      this->m_step = 0;
    }
    this->m_lastUpdate = millis();
  }
}
