#include <Arduino.h>

#include "lcd.h"
#include "audio.h"
#include "glitchPrint.h"

int gpAudioState = 0;
unsigned long last_changed = 0;
void glitchprintAudioLoop(int modeAmp) {
  switch (gpAudioState) {
    case 0:
      tone(BUZZER_PIN, 14000, 90 - modeAmp);
      gpAudioState = 1;
      last_changed = millis();
      break;
    case 1:
      if (millis() - last_changed > 80) {
        analogWrite(BUZZER_PIN, 40 + modeAmp / 2.5);
        gpAudioState = 2;
        last_changed = millis();
      }
      break;
    case 2:
      if (millis() - last_changed > 50) {
        tone(BUZZER_PIN, 2000, 30);
        gpAudioState = 3;
        last_changed = millis();
      }
    case 3:
      if (millis() - last_changed > 20) {
        gpAudioState = 0;
        last_changed = 0;
      }
    default:
      gpAudioState = 0;
  }
}

GlitchPrint::GlitchPrint(int x_, int y_, const char* text, int loopSize_)
    : startX(x_), loopSize(loopSize_),
      lastStep(0), x(x_), y(y_), j(0), m_text(text) {
      }

bool GlitchPrint::finished() const {
  return j > loopSize;
}

void GlitchPrint::reset() {
  j = 0;
  x = startX;
}

void GlitchPrint::loop() {
  // Serial.println(m_text);
  if (j > loopSize) return;

  if (j > loopSize * 0.75) {
    glitchprintAudioLoop(-j * 1.5);
  } else {
    glitchprintAudioLoop(j * 1.5);
  }

  unsigned long now = millis();
  if (j >= 1 && now - lastStep < 100) return;
  
  // Serial.println(m_text);

  lcd.setCursor(x, y);
  for (int i = 0; i < 16; i++) {
    if(m_text[i] == '\0') {
      break;
    }
    x++;
    bool isText = random(0, loopSize) - (loopSize - j - 1) >= 0;
    if (isText) {
      lcd.write(m_text[i]);
    } else {
      switch (random(0, 6)) {
        case 0:
          lcd.write('*');
          break;
        case 1:
          lcd.write('%');
          break;
        case 2:
          lcd.write('#');
          break;
        case 3:
          lcd.write(byte(7));
          break;
        case 4:
          lcd.write(byte(random(0, 5)));
          break;
        case 5:
          lcd.write('_');
          break;
      }
    }
  } 

  j++;
  x = startX;

  lastStep = now;
}

