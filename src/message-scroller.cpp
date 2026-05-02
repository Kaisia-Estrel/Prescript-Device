#include <Arduino.h>

#include "lcd.h"
#include "message-scroller.h"

MessageScroller::MessageScroller() {

}

void MessageScroller::begin(const char *messsage)  {
  m_glitchPrinter.begin(0, 0, m_linestr, 15); 
  m_pauseLength = 1500; 
  m_message = messsage; 
  m_lastUpdate = 0; 
  m_step = 0; 
  m_line = 0; 
  m_loop = false; 
  m_linecount = UINT8_MAX;
}

void MessageScroller::reset() {
  m_step = 0;
  m_line = 0;
  m_linecount = UINT8_MAX;
  m_lastUpdate = millis();
  m_loop = false;
  m_glitchPrinter.reset();
}

void MessageScroller::update_line(uint8_t line) {

  if (!m_loop) {
    m_glitchPrinter.reset();
  }

  m_glitchPrinter.setY(line);
  clearLine(line);
  int i = 0;
  for (; i < 16; i++) {
    m_linestr[i] = m_message[m_step + i];
    if (m_linestr[i] == '\n') {
      i++;
      break;
    }
    if (m_linestr[i] == '\0') {
      if (m_loop) {
        clearLine(line);
        lcd.setCursor(0, line);
        lcd.print(m_linestr);
      } 
      if (!m_loop) {
        m_linecount = m_line + 1;
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
  m_step += i;
  m_line++;

  if (m_loop) {
    clearLine(line);
    lcd.setCursor(0, line);
    lcd.print(m_linestr);
  } 
}

void MessageScroller::loop() {

  // to make sure it runs only after the first line has been processed
  // and to remember to print the last line
  if ((m_step != 0 || m_loop) && !m_glitchPrinter.finished()) {
    m_glitchPrinter.loop();
    return;
  }

  // Doesnt run any further if the line only has 2 or less lines
  // On the first loop, delay is handled by `GlitchPrint`
  bool triple_delay = m_loop && (m_line == 0 || m_line == 2);
  unsigned long curPLength = m_pauseLength * (triple_delay ? 2 : 1);
  if (m_linecount <= 2 || (m_loop && millis() - m_lastUpdate < curPLength)) {
    return;
  }

  if (m_loop && m_line == 0) {
    this->update_line(0);
    this->update_line(1);
  } else if (m_line <= 1) {
    this->update_line(m_line);
  } else if (m_linecount > 2) {
    clearLine(0);
    lcd.setCursor(0, 0);
    lcd.print(m_linestr);

    this->update_line(1);
  }

  m_lastUpdate = millis();
}
