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

#define MIN_SPEED 20
#define MIN_ACC 2
#define MAX_ACC 35

float speed;
float tgt_speed;
int acc100;
int acc100calc;





void setup() {
  Serial.begin(9600);
  Serial.println(F("start"));

  Load();
  InitSpeed();
  InitMenu();
  InitDisp();
  InitMode();
  InitKeys();
  InitOutput();
  InitAcc();
  InitCruise();
}




void loop() {
//  speed = GetSpeedInterval(100);
  speed = GetSpeedPulses(5);
  acc100 = GetAcc100();

  Led();
  Reley();
//  PrintSpeed();
  PrintDebug();
  Mode();
  Cruise();

  delay(1);
}
