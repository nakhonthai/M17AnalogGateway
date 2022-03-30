#include "LMS.h"
#include <SPI.h>
#include "analogShield.h"

LMSClass LMS;
#define REDLEDPin 3
void setup() {
  // put your setup code here, to run once:
  pinMode(REDLEDPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(REDLEDPin, HIGH);
  LMS.pushInput(analog.read(0));
  digitalWrite(REDLEDPin, LOW);
  LMS.pushNoise(analog.read(0));
  analog.write(0, LMS.pullOutput());
}