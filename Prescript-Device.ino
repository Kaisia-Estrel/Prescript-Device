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

char message[MAX_MESSAGE_LENGTH] = "abcdefghijklmnopqrstuv";
char author[MAX_AUTHOR_LENGTH] = "From Hermes:";

MessageScroller messageScroller = MessageScroller(message);
GlitchPrint authorPrinter = GlitchPrint(0, 0, author, 80);
GlitchPrint clearPrinter(0, 0, "    _Clear_.    ", 50);
GlitchPrint clear2Printer(0, 1, "                ", 20);

void setup() {
  Serial.begin(9600);
  BT.begin(9600);  // default HC-06 baud
  button.setup();
  setupLCD();
}

enum State {
  IDLE,
  PRESCRIPT_RECEIVED,
  PRESCRIPT_DISPLAYED,
  PRESCRIPT_FINISHED,
};



State state = PRESCRIPT_DISPLAYED;
// State state = IDLE;
void loop() {
  switch (state) {
    case IDLE:
      closeScreen();
      while (!BT.available()) delay(10);
      // message = BT.readStringUntil('\0');
      // author = BT.readStringUntil('\0');
      // messagePrinter.setText(message);
      // authorPrinter.setText("-" + author);
      // messageScroller.setText(message);
      state = PRESCRIPT_RECEIVED;
      openScreen();
      break;
    case PRESCRIPT_RECEIVED:
      receivedSFX.loop();
      if (button.isPressed()) {
        receivedSFX.restart();
        state = PRESCRIPT_DISPLAYED;
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
    case PRESCRIPT_DISPLAYED:
      // authorPrinter.loop();
      messageScroller.loop();
      // messagePrinter.loop();
      // if (messagePrinter.finished()) {
      //   messageScroller->loop();
      //   authorPrinter.loop();
      // }

      if (button.isPressed()) {
        // messagePrinter.reset();
        authorPrinter.reset();
        state = PRESCRIPT_FINISHED;
        tone(BUZZER_PIN, 44000, 100);
        delay(500);
      }
      break;
    case PRESCRIPT_FINISHED:
      clearPrinter.loop();
      clear2Printer.loop();
      if (clearPrinter.finished() && clear2Printer.finished()) {
        if (button.isPressed()) {
          BT.write((int)1);
        } else {
          BT.write((int)0);
        }
        clearPrinter.reset();
        clear2Printer.reset();
        delay(5000);
        state = IDLE;
      }
      break;
    default:
      break;
  }
}
