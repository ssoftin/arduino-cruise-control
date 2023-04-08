#define PIN_RELEY 4
#define PIN_LED_CRUISE  6 // green wire
#define PIN_LED_LIMITER 7 // violet wire
#define PIN_LED_SET     5 // blue wire


void InitOutput() {
  pinMode(PIN_RELEY, OUTPUT);
  digitalWrite(PIN_RELEY, 1);

  pinMode(PIN_LED_CRUISE, OUTPUT);
  pinMode(PIN_LED_LIMITER, OUTPUT);
  pinMode(PIN_LED_SET, OUTPUT);

  int b = 1;
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_LED_CRUISE, 1 & b);
    digitalWrite(PIN_LED_LIMITER, 2 & b);
    digitalWrite(PIN_LED_SET, 4 & b);
    b |= b << 1;
    delay(500);
  }    
  for (int i = 0; i < 3; i++) {
    b = b << 1;
    digitalWrite(PIN_LED_CRUISE, 1 & b);
    digitalWrite(PIN_LED_LIMITER, 2 & b);
    digitalWrite(PIN_LED_SET, 4 & b);
    delay(500);
  }  
}

void LedTest() {
  int t = millis() / 1000;
  t %= 3;
  int b = 1 << t;
  digitalWrite(PIN_LED_CRUISE, 1 & b);
  digitalWrite(PIN_LED_LIMITER, 2 & b);
  digitalWrite(PIN_LED_SET, 4 & b);
}



void Reley() {
  switch (mode) {
    case MODE_OFF:
    case MODE_ON:
    case MODE_CALIBRATE:
      digitalWrite(PIN_RELEY, 1);
      return;  
    case MODE_CRUISE:
    case MODE_LIMITER:
      digitalWrite(PIN_RELEY, 0);
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
      digitalWrite(PIN_LED_CRUISE, 1);
      digitalWrite(PIN_LED_LIMITER, 1 ^ (1 & (millis() >> 8)));
      digitalWrite(PIN_LED_SET, 1 & (millis() >> 8));
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
