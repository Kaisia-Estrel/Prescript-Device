#pragma once

class MessageScroller {
public:
  MessageScroller(const char* messsage);
  void setPauseLength(int scrollLength);
  // void setText(const String& message);
  void loop();
private:
  unsigned long m_lastUpdate;
  unsigned long m_pauseLength;
  int m_step;
  const char* m_message;
};
