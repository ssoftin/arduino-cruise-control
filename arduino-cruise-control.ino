#include <EEPROM.h>

// wires:  
// black  - gnd
// red    - +12 ign
// white  - speed
// yellow - brake
// orange - keys
// green  - led
// blue   - led
// violet - led

// wires2:
// black  - 
// red    - +5 out
// white  - SDA
// yellow - SCL
// orange - miles
// green  - backlight
// blue   - 
// violet - 











void setup() {
  Serial.begin(9600);
  Serial.println(F("start"));

  Load();
  InitSpeed();
  InitDisp();
  InitMode();
  InitKeys();
  InitOutput();
  InitAcc();
}




void print() {
  static unsigned long prev = 0;
  unsigned long t = millis();
  if (t - prev < 500) return;
  prev = t;


  Serial.println(GetSpeedInterval(500));
  
}




void loop() {
  Led();
  Reley();
  PrintSpeed();
  Mode();

//  print();

  delay(1);

  
}
