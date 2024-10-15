#include<Wire.h>
#include <Adafruit_MCP4725.h>

//DAC0 mapped to A0

#define DAC0ADDR 0x62
#define DAC1ADDR 0x63
#define PIN0 A0
#define PIN1 A1

Adafruit_MCP4725 dac0;
Adafruit_MCP4725 dac1;

void InitAcc() {
  dac0.begin(DAC0ADDR);
  dac1.begin(DAC1ADDR);
}


int GetAcc100() {
  long acc0 = analogRead(PIN0);
  long acc1 = analogRead(PIN1);

  if (save.acc0max - save.acc0min > save.acc1max - save.acc1min)
    return 100 * (acc0 - save.acc0min) / (save.acc0max - save.acc0min);

  return 100 * (acc1 - save.acc1min) / (save.acc1max - save.acc1min);
}

void SetAcc100(int acc100) {
  long r0 = save.acc0max - save.acc0min;
  long r1 = save.acc1max - save.acc1min;
  
  dac0.setVoltage((save.acc0min + r0 * acc100 / 100) << 2 , false);
  dac1.setVoltage((save.acc1min + r1 * acc100 / 100) << 2 , false);
}


void StartCalibrate() {
  save.acc0max = 0;
  save.acc0min = 1024;
  save.acc1max = 0;
  save.acc1min = 1024;

  mode = MODE_CALIBRATE;
}

void Calibrate() {
  int acc0 = analogRead(PIN0);
  int acc1 = analogRead(PIN1);

  if (acc0 > save.acc0max) save.acc0max = acc0;
  if (acc0 < save.acc0min) save.acc0min = acc0;
  
  if (acc1 > save.acc1max) save.acc1max = acc1;
  if (acc1 < save.acc1min) save.acc1min = acc1;

  Serial.print(acc0);
  Serial.print(" ");
  Serial.print(acc1);
  Serial.print(" ");
  Serial.print(save.acc0max);
  Serial.print("-");
  Serial.print(save.acc0min);
  Serial.print(" ");
  Serial.print(save.acc1max);
  Serial.print("-");
  Serial.println(save.acc1min);
}