#include <Arduino.h>

#include "button.h"


Button::Button(int pin) {
  this->m_pin = pin;
}

void Button::setup() {
  pinMode(m_pin, INPUT_PULLUP);
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
