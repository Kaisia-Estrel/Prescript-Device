#include <SoftwareSerial.h>

#include "src/lcd.h"
#include "src/button.h"
#include "src/audio.h"
#include "src/glitchPrint.h"
#include "src/message-scroller.h"

Button button(7);
SoftwareSerial BT(2, 3);  // RX, TX

PrescriptReceivedSFX receivedSFX;

#define MAX_MESSAGE_LENGTH 128
#define MAX_AUTHOR_LENGTH 32

char message[MAX_MESSAGE_LENGTH] = "When lacerating through space itself with a scythe like a certain someone";
char author[MAX_AUTHOR_LENGTH] = "From Hermes:";

GlitchPrint clearPrinter(0, 0, "    _Clear_.    ", 50);
GlitchPrint clear2Printer(0, 1, "                ", 20);

GlitchPrint authorPrinterLine1(0, 0, "From:", 50);
;
GlitchPrint authorPrinter;
MessageScroller messageScroller;
void setup() {
  authorPrinter.begin(0, 1, author, 50);
  messageScroller.begin(message);
  Serial.begin(9600);
  BT.begin(9600);  // default HC-06 baud
  button.setup();
  setupLCD();
}

enum RxState {
  WAIT_SOF,
  WAIT_LEN,
  READ_PAYLOAD,
  WAIT_CHECKSUM
};

int receivePacket(Stream& s, char* buffer, const int max_size) {
  static RxState state = WAIT_SOF;
  static uint8_t index = 0;
  static uint8_t checksum = 0;
  static int length = 0;

  while (s.available()) {

    uint8_t byte = s.read();

    switch (state) {

      case WAIT_SOF:
        if (byte == 0xAA) {
          state = WAIT_LEN;
        }
        break;

      case WAIT_LEN:
        length = byte;
        if (length > max_size) {
          state = WAIT_SOF;
          return -1;
        }
        index = 0;
        checksum = 0;
        state = READ_PAYLOAD;
        break;

      case READ_PAYLOAD:
        buffer[index++] = byte;
        checksum ^= byte;

        if (index >= length)
          state = WAIT_CHECKSUM;
        break;

      case WAIT_CHECKSUM:
        state = WAIT_SOF;

        if (checksum == byte) {
          return length;
        }
        break;
    }
  }
  return -1;
}

enum State {
  START,
  WAIT_FOR_MESSAGE,
  WAIT_FOR_AUTHOR,
  PRESCRIPT_RECEIVED,
  AUTHOR_DISPLAYED,
  PRESCRIPT_DISPLAYED,
  PRESCRIPT_FINISHED,
};

State state = START;
void loop() {
  button.loop();
  if (button.longPressed() && state >= PRESCRIPT_RECEIVED) {
    BT.write((int)1);  //Unsuccessfull return;
    messageScroller.reset();
    authorPrinterLine1.reset();
    authorPrinter.reset();
    state = START;
    tone(BUZZER_PIN, 14000, 100);
    delay(500);
  }

  switch (state) {
    case START:
      closeScreen();
      state = WAIT_FOR_MESSAGE;
      break;
    case WAIT_FOR_MESSAGE:
      {
        int msg_len = receivePacket(BT, message, MAX_MESSAGE_LENGTH);
        if (msg_len == -1) return;
        message[msg_len] = '\0';
        state = WAIT_FOR_AUTHOR;
      }
      break;
    case WAIT_FOR_AUTHOR:
      {
        int msg_len = receivePacket(BT, author, MAX_AUTHOR_LENGTH);
        if (msg_len == -1) return;
        author[msg_len] = '\0';
        state = PRESCRIPT_RECEIVED;
        openScreen();
      }
      break;
    case PRESCRIPT_RECEIVED:
      receivedSFX.loop();
      if (button.clicked()) {
        receivedSFX.restart();
        state = AUTHOR_DISPLAYED;
        tone(BUZZER_PIN, 44000, 100);
        delay(1000);
        confirmScreen();
        delay(500);
        glitchPrint(0, 0, "                ", 4);
        glitchPrint(0, 1, "                ", 2);
        return;
      }
      flashReceiveScreen();
      break;
    case AUTHOR_DISPLAYED:
      {
        authorPrinterLine1.loop();
        authorPrinter.loop();
        if (authorPrinterLine1.finished() && authorPrinter.finished()) {
          delay(2000);
          lcd.clear();
          state = PRESCRIPT_DISPLAYED;
        }
      }
      break;
    case PRESCRIPT_DISPLAYED:
      messageScroller.loop();

      if (button.released()) {
        messageScroller.reset();
        authorPrinterLine1.reset();
        authorPrinter.reset();
        state = PRESCRIPT_FINISHED;
        tone(BUZZER_PIN, 44000, 100);
        delay(500);
      }
      break;
    case PRESCRIPT_FINISHED:
      BT.write((int)0xEE);  //Successfull return;
      clearPrinter.loop();
      clear2Printer.loop();
      if (clearPrinter.finished() && clear2Printer.finished()) {
        clearPrinter.reset();
        clear2Printer.reset();
        delay(5000);
        state = START;
      }
      break;
    default:
      break;
  }
}
