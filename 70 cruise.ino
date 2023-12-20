#include "GyverPID.h"

/*

err = tgt - speed
t = 0.5

P = err
I += err * t
D = 0.20 * (speed - prev-speed) / t 
P + I + D


start:
tgt   30

speed 30
P 0  
I 6
D 0
acc 6

speed 29
P 1
I 6.5
D -0.4
acc 7



*/



GyverPID pid;

void InitCruise() {
  pid.setLimits(MIN_ACC, MAX_ACC);
  pid.setDt(500);
  pid.Kp = 1;
  pid.Ki = 0.5;
  pid.Kd = 0.2;
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
