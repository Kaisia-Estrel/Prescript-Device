#pragma once

class GlitchPrint {
public:
  GlitchPrint(int x_, int y_, String text_, int loopSize_);
  bool finished() const;
  void loop();
  void reset();
  void setText(String text_);

private:
  int startX;
  int x;
  int y;
  String text;
  int loopSize;
  unsigned long lastStep;
  int j;
  // int i;
};
