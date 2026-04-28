#pragma once

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void flashReceiveScreen();
void confirmScreen();
void setupLCD();
void clearScreen();
void glitchPrint(int x, int y, String text, int loopSize);
void openScreen();
void closeScreen();
void clearLine(int line);
