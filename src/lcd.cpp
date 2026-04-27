#include "lcd.h"

static byte idx_0[8] = {
  0b00000,
  0b00100,
  0b00011,
  0b00011,
  0b00111,
  0b00010,
  0b00111,
  0b10110
};

static byte idx_1[8] = {
  0b00100,
  0b01110,
  0b11011,
  0b00100,
  0b01100,
  0b01100,
  0b01100,
  0b11000
};

static byte idx_2[8] = {
  0b00000,
  0b00100,
  0b11000,
  0b11000,
  0b11100,
  0b01000,
  0b11000,
  0b01101
};

static byte idx_3[8] = {
  0b10000,
  0b10000,
  0b10000,
  0b11000,
  0b01100,
  0b00110,
  0b00011,
  0b00001
};

static byte idx_4[8] = {
  0b11111,
  0b11101,
  0b11101,
  0b01100,
  0b01110,
  0b00000,
  0b11111,
  0b11111
};

static byte idx_5[8] = {
  0b11001,
  0b10001,
  0b00001,
  0b00011,
  0b00110,
  0b01100,
  0b11000,
  0b10000
};

static byte block_bytes[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
static byte block = byte(6);

static byte halftone_bytes[8] = {
  0b10101,
  0b01010,
  0b10101,
  0b01010,
  0b10101,
  0b01010,
  0b10101,
  0b01010
};
static byte halftone = byte(7);

static LiquidCrystal_I2C lcd(0x3F, 16, 2);

static bool displayed = false;
void flashReceiveScreen() {
  unsigned long time = millis() % 2000;
  if (!displayed && time < 1500) {
    displayed = true;
    lcd.setCursor(2,0);
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.write(byte(2));
    lcd.setCursor(2, 1);
    lcd.write(byte(3));
    lcd.write(byte(4));
    lcd.write(byte(5));
    lcd.setCursor(6, 0);
    lcd.print("Prescript");
    lcd.setCursor(6, 1);
    lcd.print("Received");
  } else if (displayed && time >= 1500) {
    displayed = false;
    lcd.clear();
  }
}

// lcd is 17 characters wide
void confirmScreen() {
  lcd.setCursor(7, 0); 
  lcd.write(block);
  lcd.setCursor(8, 0); 
  lcd.write(block);
  lcd.setCursor(7, 1); 
  lcd.write(block);
  lcd.setCursor(8, 1); 
  lcd.write(block);
  delay(200);

  for (int i = 1; i <= 3; i++) {
    lcd.setCursor(8 + i, 0); 
    lcd.write(block);
    lcd.setCursor(8 - i - 1, 0); 
    lcd.write(block);
    lcd.setCursor(8 + i, 1); 
    lcd.write(block);
    lcd.setCursor(8 - i - 1, 1); 
    lcd.write(block);
    delay(200);
  }

  lcd.setCursor(8 - 5, 0); 
  lcd.write(block);
  lcd.setCursor(8 - 5, 1); 
  lcd.write(block);
  delay(200);
  for (int i = 1; i <= 4; i++) {
    lcd.setCursor(8 + i + 3, 0); 
    lcd.write(halftone);
    lcd.setCursor(8 - i - 5, 0); 
    lcd.write(halftone);
    lcd.setCursor(8 + i + 3, 1); 
    lcd.write(halftone);
    lcd.setCursor(8 - i - 5, 1); 
    lcd.write(halftone);
    delay(200);
  }

  lcd.setCursor(6, 0); 
  lcd.write(byte(0));
  delay(200);
  lcd.write(byte(1));
  delay(200);
  lcd.write(byte(2));
  delay(200);
  lcd.setCursor(6, 1); 
  lcd.write(byte(3));
  delay(200);
  lcd.write(byte(4));
  delay(200);
  lcd.write(byte(5));
  delay(200);
}

void setupLCD() {
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, idx_0);
  lcd.createChar(1, idx_1);
  lcd.createChar(2, idx_2);
  lcd.createChar(3, idx_3);
  lcd.createChar(4, idx_4);
  lcd.createChar(5, idx_5);
  lcd.createChar(6, block_bytes);
  lcd.createChar(7, halftone_bytes);
  lcd.setCursor(0, 0);
}

void glitchPrint(int x, int y, String text, int loopSize) {
  for (int j = 0; j < loopSize; j++) {
    if (j >= 1) {
      delay(100);
    }
    lcd.setCursor(x, y);
    for (int i = 0; i < min(16, text.length()); i++) {
      bool isText = random(0, loopSize) - (loopSize - j - 1) >= 0;

      switch (random(0,5) + isText*5) {
        case 0:
          lcd.write(byte(6));
          break;
        case 1:
          lcd.write('%');
          break;
        case 2:
          lcd.write('#');
          break;
        case 3:
          lcd.write(byte(7));
          break;
        case 4:
          lcd.write(byte(random(0,5)));
          break;
        default:
          if (text.length() > 16 && i == 15) {
            lcd.write('-');
          } else {
            lcd.write(text[i]);
          }
          break;
      }
      if (j == 0) {
        delay(10);
      }
    }
  }
}

void clearScreen() {
  lcd.clear();
}
