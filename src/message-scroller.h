#pragma once

#include <stdint.h>

#include "glitchPrint.h"

class MessageScroller {
public:
  MessageScroller();
  void begin(const char* message);
  void loop();
  void reset();

private:
  void update_line(uint8_t line);

  unsigned long m_lastUpdate;
  unsigned long m_pauseLength;
  unsigned int m_step;
  uint8_t m_line;
  char m_linestr[17];
  bool m_loop;
  uint8_t m_linecount;
  GlitchPrint m_glitchPrinter;

  const char *m_message;
};
