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
#define PID_KI 0.5
#define PID_KD 0.7
#define PID_DT 500


GyverPID pid;

void InitCruise() {
  pid.setLimits(MIN_ACC, MAX_ACC);
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

  pid.setDt(save.pid_dt);
  pid.Kp = save.pid_kp;
  pid.Ki = save.pid_ki;
  pid.Kd = save.pid_kd;

  pid.input = speed;
  acc100calc = pid.getResultTimer();

  if (mode == MODE_CRUISE && acc100 > acc100calc) {
    SetAcc100(acc100);
    led_mode = LED_CRUISE_BLINK;
  } else {
    SetAcc100(acc100calc);
    led_mode = LED_NORMAL;
  }
}
