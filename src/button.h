#pragma once

#include <Arduino.h>

class Button {
public:
  Button(int pin);

  void setup();
  void loop();

  bool pressed();
  bool released();
  bool clicked();
  bool longPressed();

  void setDebounceTime(unsigned long ms);
  void setLongPressTime(unsigned long ms);

private:
  int m_pin;

  unsigned long m_debounceTime = 50;
  unsigned long m_longPressTime = 800;

  unsigned long m_lastDebounceTime = 0;
  unsigned long m_pressStartTime = 0;

  bool m_lastRawState = HIGH;
  bool m_stableState = HIGH;

  bool m_pressedEvent = false;
  bool m_releasedEvent = false;
  bool m_clickedEvent = false;
  bool m_longPressEvent = false;

  bool m_longPressFired = false;
};
