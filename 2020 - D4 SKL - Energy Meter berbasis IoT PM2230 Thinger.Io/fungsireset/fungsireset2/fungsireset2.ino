#include <avr/wdt.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("WORK");
  delay(3000);
  wdt_enable(WDTO_8S);
}

void loop() {
  // put your main code here, to run repeatedly:
  wdt_reset();
  delay(10000);
  wdt_disable();
}
