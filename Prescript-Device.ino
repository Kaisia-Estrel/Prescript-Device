#include <SoftwareSerial.h>

#include "src/lcd.h"
#include "src/button.h"
#include "src/audio.h"
#include "src/glitchPrint.h"

Button button(7);
SoftwareSerial BT(2, 3);  // RX, TX


void setup() {
  Serial.begin(9600);
  BT.begin(38400);  // default HC-06 baud
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
GlitchPrint authorPrinter(2, 1, "-Hermes", 20);
GlitchPrint clearPrinter(0, 0, "    _Clear_.    ", 50);
GlitchPrint clear2Printer(0, 1, "                ", 20);

#define MAX_MESSAGE_LENGTH 63
#define MAX_AUTHOR_LENGTH 23
String message = String();
String author = String();

void loop() {

  Serial.println("FOO");

  switch (state) {
    case IDLE:
      closeScreen();
      message = "";
      author = "";
      // Serial.print(BT.read());



      // while (!BT.available()) { delay(10); }
      // Serial.print(BT.peek());
      // delay(100);
      // while (BT.peek() != '\0') {
      // message += BT.read();
      // }
      // message = BT.readStringUntil('\0');
      // author = BT.readStringUntil('\0');

      // Serial.println(message);
      // Serial.println(author);
      //
      // messagePrinter.setText(message);
      // authorPrinter.setText(author);
      // state = PRESCRIPT_RECEIVED;
      // openScreen();
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


// button.loop();
// if (button.isPressed()) {
//   BT.println("Button Press\n");
// }
//
// while (BT.available()) {
//   if (message.length() >= 28) {
//     continue;
//   }
//   int c = BT.read();
//   if (c == '\0') {
//     Serial.println("Message: \"" + message + '"');
//     Serial.println("Length: " + String(message.length()));
//     message = "";
//   }
//   message += (char)c;
