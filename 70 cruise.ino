#include "GyverPID.h"

/*
t = 0.5

P = tgt - speed
I += 0.5 * (tgt - speed) * t
D = 0.20 * (prev-speed - speed) / t 
P + I + D

*/



GyverPID pid;

void InitCruise() {
  pid.setLimits(MIN_ACC, MAX_ACC);
  pid.setDt(500);
//              //overspeed downhill//less swing//swing// +
  pid.Kp = 0.7; //0.7;              //0.6;      //0.5; //0.7; //1;
  pid.Ki = 0.7; //0.7;              //0.7;      //1;   //0.7; //0.5;
  pid.Kd = 0.5; //0.7;              //0.7;      //1;   //0.5; //0.2;
}


void StartCruise(int tgt) {
  if (speed >= MIN_SPEED) {
    if (tgt == 0) tgt = speed;
    mode = MODE_CRUISE;
    tgt_speed = tgt;
  }
}

void StartLimiter(int tgt) {
  if (speed >= MIN_SPEED) {
    if (tgt == 0) tgt = speed;
    mode = MODE_LIMITER;
    tgt_speed = tgt;
  }
}

void Cruise() {
  if (mode == MODE_CRUISE) {
    pid.setpoint = tgt_speed;
  } else {
    pid.setpoint = speed;
    pid.integral = acc100;
  }
    
  pid.input = speed;
  acc100calc = pid.getResultTimer();

  if (mode == MODE_CRUISE && acc100 > acc100calc) {
    SetAcc100(acc100);
    submode = SUBMODE_BLINK;
  } else {
    SetAcc100(acc100calc);
    submode = SUBMODE_OFF;
  }
}
