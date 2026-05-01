#include <Arduino.h>

#include "lcd.h"
#include "message-scroller.h"

MessageScroller::MessageScroller(const char *messsage) {
  m_message = messsage;
  m_lastUpdate = millis();
  m_pauseLength = 500;
  // m_pauseLength = 1500;
  m_step = 0;
  m_line = 0;
  m_loop = false;
  m_linecount = INT8_MAX;
}

void MessageScroller::reset() {
  clearLine(0);
  clearLine(1);
  m_step = 0;
  m_line = 0;
  m_lastUpdate = millis();
  m_loop = false;
}

void MessageScroller::update_line(uint8_t line) {
  int i = 0;
  for (; i < 16; i++) {
    m_linestr[i] = m_message[m_step + i];
    if (m_linestr[i] == '\n') {
      i++;
      break;
    }
    if (m_linestr[i] == '\0') {
      clearLine(line);
      lcd.setCursor(0, line);
      lcd.print(m_linestr);
      if (!m_loop) {
        m_linecount = m_line+1;
      }
      m_loop = true;
      m_step = 0;
      m_line = 0;
      return;
    }
  }

  int prev_i = i;
  while (i > 0 && m_linestr[i - 1] != ' ') {
    i--;
  }

  // if `i` reaches the start without finding a space
  // a word longer than 16 chars was used
  if (i == 0) {
    i = prev_i;
  }

  m_linestr[i] = '\0';
  clearLine(line);
  lcd.setCursor(0, line);
  lcd.print(m_linestr);

  m_step += i;
  m_line++;
}

void MessageScroller::loop() {
  if (millis() - m_lastUpdate <
      m_pauseLength * (m_loop && (m_line == 2) ? 3 : 1)) {
    return;
  }

  if (!m_loop && m_line <= 1) {
    this->update_line(m_line);
  } else if (m_linecount > 2) {
    clearLine(0);
    lcd.setCursor(0, 0);
    lcd.print(m_linestr);
    this->update_line(1);
  }

  m_lastUpdate = millis();
}
