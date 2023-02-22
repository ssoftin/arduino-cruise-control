
#define INTERRUPT_SPEED 0
#define PIN_SPEED 2
#define PIN_BRAKE
#define PIN_RELEY
#define PIN_LED_CRUISE
#define PIN_LED_LIMITER
#define PIN_LED_SET
#define PIN_KEYS

enum {
  MODE_OFF,
  MODE_ON,
  MODE_CALIBRATE,
  MODE_CRUISE,
  MODE_LIMITER  
};

#define KEY_VALID_READ 3
#define KEY_LONG_TIME 500
#define KEY_EXLONG_TIME 5000
#define KEY_LONG_OFFSET 10
#define KEY_EXLONG_OFFSET 20
enum {
  KEY_NONE,
  KEY_ON_OFF,
  KEY_CANCEL,
  KEY_COAST_SET,
  KEY_ACC_RES,
  KEY_ON_OFF_LONG = KEY_ON_OFF + KEY_LONG_OFFSET,
  KEY_CANCEL_LONG,
  KEY_COAST_SET_LONG,
  KEY_ACC_RES_LONG,
  KEY_ON_OFF_EXLONG = KEY_ON_OFF + KEY_EXLONG_OFFSET,
  KEY_CANCEL_EXLONG,
  KEY_COAST_SET_EXLONG,
  KEY_ACC_RES_EXLONG
};



int mode = MODE_OFF;
float speed = 0;
unsigned long speed_time_prev = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_SPEED, INPUT_PULLUP);
  attachInterrupt(INTERRUPT_SPEED, InterruptSpeed, FALLING);

}

// 10hz = 30km/h
// millis 1000 = 1sec
// 10hz: d = 100
// 3000 / d = km/h
void InterruptSpeed() {
  unsigned long cur = millis();
  float speed_new = 3000.0 / (cur - speed_time_prev);
  speed_time_prev = cur;
  speed += (speed_new - speed) * 0.1;
}

void ControlSpeed() {
  if (speed < 10 || millis() - speed_time_prev > 300) speed = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  ControlSpeed();
  Mode();
  Led();
  Control();
  Reley();
}

void Reley() {
  switch (mode) {
    case MODE_OFF:
    case MODE_ON:
    case MODE_CALIBRATE:
      digitalWrite(PIN_RELEY, 0);
      return;  
    case MODE_CRUISE:
    case MODE_LIMITER:
      digitalWrite(PIN_RELEY, 1);
      return;  
  }
}

void Led() {
  switch (mode) {
    case MODE_OFF:
      digitalWrite(PIN_LED_CRUISE, 0);
      digitalWrite(PIN_LED_LIMITER, 0);
      digitalWrite(PIN_LED_SET, 0);
      return;  
    case MODE_ON:
      digitalWrite(PIN_LED_CRUISE, 1);
      digitalWrite(PIN_LED_LIMITER, 0);
      digitalWrite(PIN_LED_SET, 0);
      return;
    case MODE_CALIBRATE:
      digitalWrite(PIN_LED_CRUISE, 1 & (millis() >> 8));
      digitalWrite(PIN_LED_LIMITER, 0);
      digitalWrite(PIN_LED_SET, 0);
      return;
    case MODE_CRUISE:
      digitalWrite(PIN_LED_CRUISE, 1);
      digitalWrite(PIN_LED_LIMITER, 0);
      digitalWrite(PIN_LED_SET, 1);
      return;
    case MODE_LIMITER:
      digitalWrite(PIN_LED_CRUISE, 1);
      digitalWrite(PIN_LED_LIMITER, 1);
      digitalWrite(PIN_LED_SET, 0);
      return;  
  }
}

void Mode() {
  switch (mode) {
    case MODE_OFF:
      ModeOff();
      return;  
    case MODE_ON:
      ModeOn();
      return;  
    case MODE_CALIBRATE:
      ModeCalibrate();
      return;  
    case MODE_CRUISE:
      ModeCruise();
      return;  
    case MODE_LIMITER:
      ModeLimiter();
      return;  
  }
}

void ModeOff() {
  int key = GetKey();
  switch (key) {
    case KEY_ON_OFF:
      mode = MODE_ON;
      return;
    case KEY_ON_OFF_EXLONG:
      if (speed == 0) mode = MODE_CALIBRATE;
      return;
  }
}

void ModeCalibrate() {
  int key = GetKey();
  switch (key) {
    case KEY_ON_OFF_EXLONG:
      SaveCalibration();
    case KEY_ON_OFF:
    case KEY_CANCEL:
      mode = MODE_OFF;
      return;
  }  
}

void ModeON() {
  int key = GetKey();
  if (key == KEY_ON_OFF) {
      mode = MODE_OFF;
      return;
  }
  if (digitalRead(PIN_BRAKE) != 0) {
    switch (key) {
      case KEY_COAST_SET:
        mode = MODE_CRUISE;
        speed_tgt = speed_cur;
        return;
      case KEY_ACC_RES:
        mode = MODE_CRUISE;
        return;
      case KEY_CANCEL_LONG:
        mode = MODE_LIMITER;
        speed_limit = speed_cur;
        return;
    }
  }
}

void ModeCruise() {
  if (digitalRead(PIN_BRAKE) == 0) {
    mode = MODE_ON;
    return;
  }
  int key = GetKey();
  switch (key) {
    case KEY_ON_OFF:
      mode = MODE_OFF;
      return;
    case KEY_CANCEL:
      mode = MODE_ON;
      return;
    case KEY_COAST_SET:
      speed_tgt -= 1;
      return;
    case KEY_ACC_RES:
      speed_tgt += 1;
      return;
  }
}

void ModeLimiter() {
  int key = GetKey();
  switch (key) {
    case KEY_ON_OFF:
      mode = MODE_OFF;
      return;
    case KEY_CANCEL:
      mode = MODE_ON;
      return;
  }
}


int DetectKey() {
  int val = analogRead(PIN_KEYS);
// on can set res  none
// 15  22  33  54  113
//   18  27  43  83
  if (val < 18) return KEY_ON_OFF;
  if (val < 27) return KEY_CANCEL;
  if (val < 43) return KEY_COAST_SET;
  if (val < 83) return KEY_ACC_RES;
  return NONE;
}

int GetKey() {
  static int prev = KEY_NONE;
  static int cnt = 0;
  static unsigned long ts = 0;
  int val = DetectKey();
  if (val == KEY_NONE) {
    int res = KEY_NONE;
    if (cnt >= KEY_VALID_READ) res = prev;
    if (res != KEY_NONE) {
      if (millis() - ts >= KEY_EXLONG_TIME) res += KEY_EXLONG_OFFSET;
      else if (millis() - ts >= KEY_LONG_TIME) res += KEY_LONG_OFFSET;
    }
    prev = KEY_NONE;
    cnt = 0;
    ts = 0;
    return res;
  }
  if (prev == val) {
    cnt++;
  } else {
    prev = val;
    cnt = 1;
    ts = millis(); 
  }
  return KEY_NONE;
}
