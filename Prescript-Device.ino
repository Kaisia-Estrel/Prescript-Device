
#include "src/lcd.h"
#include "src/button.h"

#include <SoftwareSerial.h>

Button button(7);
// SoftwareSerial BT(2, 3);  // RX, TX


void setup() {
  Serial.begin(9600);
  Serial.println(sizeof(unsigned long));
  Serial.println(sizeof(uint64_t));
  // BT.begin(9600);  // default HC-06 baud

  setupLCD();
}

String message = String();

enum State {
  IDLE,
  PRESCRIPT_RECEIVED,
  PRESCRIPT_DISPLAYED,
  PRESCRIPT_FINISHED,
};

State state = PRESCRIPT_FINISHED;
void loop() {
  switch (state) {
    case IDLE:
      closeScreen();
      if (button.isPressed()) {
        openScreen();
        state = PRESCRIPT_RECEIVED;
      }
      break;
    case PRESCRIPT_RECEIVED:
      if (button.isPressed()) {
        state = PRESCRIPT_DISPLAYED;
        confirmScreen();
        glitchPrint(0, 0, "Prescript Text", 20);
        glitchPrint(1, 1, "-Sender", 20);
        // button.clearState();
        return;
      }
      flashReceiveScreen();
      break;
    case PRESCRIPT_DISPLAYED:
      if (button.isPressed()) {
        state = PRESCRIPT_FINISHED;
        clearScreen();
        glitchPrint(4, 0, "_Clear._", 50);
        delay(2000);
      }
      break;
    case PRESCRIPT_FINISHED:
      state = IDLE;
      break;
    default:
      break;
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
  // }
}
