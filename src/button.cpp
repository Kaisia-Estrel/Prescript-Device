#include "button.h"

Button::Button(int pin) : m_pin(pin) {}

void Button::setup() {
  pinMode(m_pin, INPUT_PULLUP);

  m_lastRawState = digitalRead(m_pin);
  m_stableState = m_lastRawState;
}

void Button::loop() {
  bool raw = digitalRead(m_pin);

  m_pressedEvent = false;
  m_releasedEvent = false;
  m_clickedEvent = false;
  m_longPressEvent = false;

  if (raw != m_lastRawState) {
    m_lastDebounceTime = millis();
    m_lastRawState = raw;
  }

  if (millis() - m_lastDebounceTime < m_debounceTime) {
    return;
  }

  if (raw != m_stableState) {
    m_stableState = raw;

    if (m_stableState == LOW) {
      // pressed
      m_pressedEvent = true;
      m_pressStartTime = millis();
      m_longPressFired = false;
    } else {
      m_releasedEvent = true;

      if (!m_longPressFired) {
        m_clickedEvent = true;
      }
    }
  }

  if (m_stableState == LOW && !m_longPressFired) {
    if (millis() - m_pressStartTime >= m_longPressTime) {
      m_longPressEvent = true;
      m_longPressFired = true;
    }
  }
}

bool Button::pressed() { return m_pressedEvent; }

bool Button::released() { return m_releasedEvent; }

bool Button::clicked() { return m_clickedEvent; }

bool Button::longPressed() { return m_longPressEvent; }

void Button::setDebounceTime(unsigned long ms) { m_debounceTime = ms; }

void Button::setLongPressTime(unsigned long ms) { m_longPressTime = ms; }
