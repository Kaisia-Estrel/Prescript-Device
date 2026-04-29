#include <SoftwareSerial.h>

#include "src/lcd.h"
#include "src/button.h"
#include "src/audio.h"
#include "src/glitchPrint.h"

Button button(7);
SoftwareSerial BT(2, 3);  // RX, TX


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

PrescriptReceivedSFX receivedSFX;
// State state = PRESCRIPT_DISPLAYED;
State state = IDLE;

GlitchPrint messagePrinter(0, 0, "Test Prescript", 30);
GlitchPrint authorPrinter(0, 1, "-Hermes", 20);
GlitchPrint clearPrinter(0, 0, "    _Clear_.    ", 50);
GlitchPrint clear2Printer(0, 1, "                ", 20);

#define MAX_MESSAGE_LENGTH 63
#define MAX_AUTHOR_LENGTH 23
String message = String();
String author = String();

void loop() {
  // BT.print("success");

  switch (state) {
    case IDLE:
      closeScreen();
      message = "";
      author = "";
      while (!BT.available()) delay(10);
      message = BT.readStringUntil('\0');
      author = BT.readStringUntil('\0');
      messagePrinter.setText(message);
      authorPrinter.setText("-" + author);
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
      messagePrinter.loop();
      if (messagePrinter.finished()) {
        authorPrinter.loop();
      }

      if (button.isPressed()) {
        messagePrinter.reset();
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
