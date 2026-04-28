#pragma once

#define BUZZER_PIN 9

class SFX {
  public:
  SFX();
  virtual void loop() = 0;
  void restart();

  protected:
  // time elapsed since last state change
  unsigned long m_lastUpdate;
  unsigned int m_state;
};


class PrescriptReceivedSFX : public SFX {
  public:
    virtual void loop() override;
};
