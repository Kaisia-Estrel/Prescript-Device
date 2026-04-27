// #include <SoftwareSerial.h>
#include <ezButton.h>
// #include <LiquidCrystal_I2C.h>

#include "src/lcd.h"

ezButton button(7);
// SoftwareSerial BT(2, 3);  // RX, TX


void setup() {
  Serial.begin(9600);
  button.setDebounceTime(50);
  // BT.begin(9600);  // default HC-06 baud
  //

  setupLCD();
}

String message = String();

enum State {
  IDLE,
  PRESCRIPT_RECEIVED,
  PRESCRIPT_DISPLAYED,
  PRESCRIPT_FINISHED,
};

State state = PRESCRIPT_RECEIVED;

void loop() {
  button.loop();
  bool buttonPressed = button.isPressed();
  switch (state) {
    case PRESCRIPT_RECEIVED:
      if (buttonPressed) {
        state = PRESCRIPT_DISPLAYED;
        confirmScreen();
        glitchPrint(0, 0, "Prescript Text", 20);
        glitchPrint(1, 1, "-Sender", 20);
        button.clearState();
        return;
      }
      flashReceiveScreen();
      break;
    case PRESCRIPT_DISPLAYED:
      if (buttonPressed) {
        Serial.println(button.isPressed());
      }
      // if (button.isPressed()) {
      //   state = PRESCRIPT_FINISHED;
      //   clearScreen();
      //   glitchPrint(4, 0, "_Clear._", 50);
      // }
      break;
    case PRESCRIPT_FINISHED:
        state = PRESCRIPT_DISPLAYED;
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
