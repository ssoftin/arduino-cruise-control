#define PIN_KEYS A2 // orange wire

#define KEY_VALID_READ 1
#define KEY_LONG_TIME 500
#define KEY_EXLONG_TIME 3000
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


void InitKeys() {
  pinMode(PIN_KEYS, INPUT_PULLUP);
}

// int PrintKeySignal() {
//   int val = analogRead(PIN_KEYS);
//   Serial.println(val);
// }

int DetectKey() {
  int val = analogRead(PIN_KEYS);
// on can set res  none
// 15  22  34  58  122
//   18  28  46  90
  if (val < 18) return KEY_ON_OFF;
  if (val < 28) return KEY_CANCEL;
  if (val < 46) return KEY_COAST_SET;
  if (val < 90) return KEY_ACC_RES;
  return KEY_NONE;
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
