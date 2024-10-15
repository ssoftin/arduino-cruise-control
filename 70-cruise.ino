#include "GyverPID.h"

/*
t = 0.5

P = Pk * (tgt - speed)
I += Ik * (tgt - speed) * t
D = Dk * (prev_err - cur_err) / t 
P + I + D

// lack of impact//total swing//overspd downhil//less swing//swing// +
//Kp= 0.5;       //0.7;       //0.7;           //0.6;      //0.5; //0.7; //1;
//Ki= 0.5;       //0.7;       //0.7;           //0.7;      //1;   //0.7; //0.5;
//Kd= 0.5;       //1;         //0.7;           //0.7;      //1;   //0.5; //0.5;

// used long//
Kpu 1         1
Kpd           1.5 
Kiu 0.5       0.5
Kid 1         1
Kd  0.5       0.7


*/



#define PID_KP 1
#define PID_KP_DOWN 1.5
#define PID_KI 0.5
#define PID_KI_DOWN 1
#define PID_KD 0.7


GyverPID pid;

void InitCruise() {
  pid.setLimits(MIN_ACC, MAX_ACC);
  pid.setDt(500);
  pid.Kp = PID_KP;
  pid.Ki = PID_KI;
  pid.Kd = PID_KD;
}


void StartCruise(int tgt) {
  if (speed >= MIN_SPEED) {
    if (tgt == 0) tgt = speed;
    mode = MODE_CRUISE;
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

  if (speed <= tgt_speed) {
    pid.Kp = PID_KP;
    pid.Ki = PID_KI;
  } else {
    pid.Kp = PID_KP_DOWN;
    pid.Ki = PID_KI_DOWN;
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
