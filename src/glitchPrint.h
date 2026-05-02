#pragma once

class GlitchPrint {
public:
  GlitchPrint();
  GlitchPrint(int x_, int y_, const char* text, int loopSize_);
  void begin(int x_, int y_, const char* text, int loopSize_);
  bool finished() const;
  void setX(int x);
  void setY(int y);
  void loop();
  void reset();

private:
  int startX;
  int x;
  int y;
  const char* m_text;
  int loopSize;
  unsigned long lastStep;
  int j;
};
