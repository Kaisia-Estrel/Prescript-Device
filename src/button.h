#pragma once

class Button {

  public:
    Button(int pin);

    bool isPressed();
    void setDebounceTime(unsigned long debounceTime);

  private:

    int m_pin;
    unsigned long m_debounceTime = 700;
    unsigned long m_lastPressed = 0;
};
