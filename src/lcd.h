#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

void flashReceiveScreen();
void confirmScreen();
void setupLCD();
void clearScreen();
void glitchPrint(int x, int y, String text, int loopSize);
