#include <Arduino.h>

#include "button.h"


Button::Button(int pin) {
  pinMode(pin, INPUT_PULLUP);
  this->m_pin = pin;
}

bool Button::isPressed() {
  if (millis() - m_lastPressed < m_debounceTime) {
    return false;
  } 

  if (digitalRead(m_pin) == LOW) {
    this->m_lastPressed = millis();
    return true;
  } else {
    return false;
  }
}

void Button::setDebounceTime(unsigned long time) {
  this->m_debounceTime = time;
}
